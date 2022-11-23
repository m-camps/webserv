//
// Created by Xander Voorvaart on 11/1/22.
//

#include "Cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>

std::string		Cgi::createServerErrorBody(Respond& ResponderRef, int errorCode)
{
	ResponderRef.setStatusCode(errorCode);
	std::string defaultServerErrorBody = Generator::generateDefaulPage(ResponderRef);
	return defaultServerErrorBody;
}

void		Cgi::createFailedSysCallResponse(Respond& ResponderRef, int errorCode)
{
	ResponderRef.setStatusCode(errorCode);
	ResponderRef.setBody(createServerErrorBody(ResponderRef, errorCode));
	return ;
}

std::string		Cgi::buildCgiExecPath(Respond& ResponderRef)
{
	Location 	currentLoc = ResponderRef.getLocation();
	std::string PATH = ResponderRef.getEntryFromMap("Path");
	std::string locationRoot = currentLoc.getRoot();
	std::string locationName = currentLoc.getName();
	size_t		index = PATH.find(locationName);
	char 		absolutePath[500]; //do we need this big container?

	PATH.replace(index, locationName.length(), locationRoot);

	if (getcwd(absolutePath, sizeof(absolutePath)) == NULL)
	{
		std::cout << "Could not enter to current working directory, closing program" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string executablePath(absolutePath);
	executablePath += '/' + PATH; //end of path should be the same as the cgi_name + ext

	return executablePath;
}

char**			Cgi::createArgv(Respond& ResponderRef)
{
	std::string 				requestedFilePath = buildCgiExecPath(ResponderRef);
//	std::string					interpreterPath = "/opt/homebrew/opt/python@3.10/bin/python3.10"; //on m2
	std::string				interpreterPath = "/usr/local/bin/python3"; //could we make this less error prone? in case it changes
	std::vector<std::string>	argvString;
	char						**argv;

	argvString.push_back(interpreterPath);
	argvString.push_back(requestedFilePath);

	try
	{
		argv = new char*[3];
	}
	catch(std::bad_alloc& e)
	{
		std::exit(INTERNAL_SERVER_ERROR_CODE);
	}
	for (int i = 0; i < 2; i++)
	{
		argv[i] = strdup(static_cast<const char*>(argvString[i].data()));
		if (!argv[i])
		{
			std::exit(INTERNAL_SERVER_ERROR_CODE);
		}
	}
	argv[2] = NULL;
	return argv;
}

char**			Cgi::createEnvp(Respond& ResponderRef)
{
	std::vector<std::string>	envpStrig;
	std::string					pathInfo = "PATH_INFO=" + ResponderRef.getLocation().getName();
	std::string					scriptName = "SCRIPT_NAME=/cgi-bin";
	char**						envp;

	envpStrig.push_back(pathInfo);
	envpStrig.push_back(scriptName);

	try
	{
		envp = new char*[3];
	}
	catch(std::bad_alloc& e)
	{
		std::exit(INTERNAL_SERVER_ERROR_CODE);
	}
	for (int i = 0; i < 2; i++)
	{
		envp[i] = strdup(static_cast<const char*>(envpStrig[i].data()));
		if (!envp[i])
		{
			std::exit(INTERNAL_SERVER_ERROR_CODE);
		}
	}
	envp[2] = NULL;
	return envp;
}

void			Cgi::childProcess(int *fds, Respond& ResponderRef)
{
	struct stat	s;
	char**		argv = createArgv(ResponderRef);
	char**		envp = createEnvp(ResponderRef);
	Location 	currentLoc = ResponderRef.getLocation();
	std::string requestedFileName = currentLoc.getCgiName() + "." + currentLoc.getCgiFileExtension();

	if (close(fds[0]) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
	if (close(STDIN_FILENO) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
	if (dup2(fds[1], STDOUT_FILENO) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));

	if (stat(argv[1], &s) == 0)
	{
		char* cgiFileFoundInPath = strstr(argv[1], requestedFileName.c_str());
		if (s.st_mode & S_IFREG && access(argv[1], (X_OK)) == 0 && cgiFileFoundInPath != NULL) //small bug, it sees python// also an executable path
		{
			execve(argv[0], argv, envp); //no need to check if its < 0
		}
	}
	std::exit(INTERNAL_SERVER_ERROR_CODE);
}

void		Cgi::parentProcess(Respond& ResponderRef, int* fds, int& stat)
{
	if (close(fds[1]) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
	if (dup2(fds[0], STDIN_FILENO) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
	if (waitpid(-1, &stat, 0) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));

	if (WIFEXITED(stat) == true)
	{
		static char buff[1024];
		int exit_status = WEXITSTATUS(stat);
		if (exit_status == INTERNAL_SERVER_ERROR_CODE)
		{	
			close(fds[0]);
			return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
		}
		int ret = read(fds[0], buff, sizeof(buff));
		if (ret == -1)
		{
			return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
		}
		std::string cgiBody(buff);
		ResponderRef.setBody(cgiBody);
	}
	if (close(fds[0]) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));
}

std::string			Cgi::executeScript(Respond& ResponderRef)
{
	int	fds[2];
	int	stat;

	if (pipe(fds) < 0)
	{
		return (createServerErrorBody(ResponderRef, e_InternalServerError));
	}
	pid_t	pid = fork();
	if (pid == -1)
	{
		return (createServerErrorBody(ResponderRef, e_InternalServerError));
	}
	else if (pid == 0)
	{	
		childProcess(fds, ResponderRef);
	}
	else if (pid > 0)
	{
		parentProcess(ResponderRef, fds, stat);
	}
	return (ResponderRef.getBody());
}

Cgi::Cgi() {}


/*
if there is a root in the locationblock then we will have to find 
https://serverfault.com/questions/853559/nginx-place-a-prefix-in-my-url

	//if there is no root in the location block then the "name" of location will be put to the server (root) where we have to look for stuff
	//1, get the name of location
	//2, check if there is root
	//3, if there is no root in location, root will be the server's root + location(name) eg => data/www/locationname
	//4, check for extension and name to know if the location block will be a cgi
	//5, save the executable name somewhere?
	//6, once it was executed and the output passed back to the parent process, create the respond headers

	location /bar {
		return 301 https://$host/foo$request_uri;
	}

	location /foo {
		alias /var/www/foo; //aka root => will search for /var/www/foo/foo
	}
*/


// std::string* Cgi::getEnvTable(void)
// {
// 	return _ENV;
// }

// void	Cgi::printEnv(void)
// {
// 	std::string* env = getEnvTable();
// 	for (int i = 0; i < 24; i++)
// 	{
// 		std::cout << env[i] << " ";
// 	}
// 	std::cout << std::endl;
// 	return ;
// }
	// const char *envp[] =
	// {
	// 	"HOME=/",
	// 	"PATH=/bin:/usr/bin",
	// 	"TZ=UTC0",
	// 	"USER=beelzebub",
	// 	"LOGNAME=tarzan",
	// 	0
	// };
// void	Cgi::createCgiResponse(void)
// {
// 	std::cout << "Content-type:text/html\r\n\r\n";
// 	std::cout << "<html>\n";
// 	std::cout << "<head>\n";
// 	std::cout << "<title>CGI Environment Variables</title>\n";
// 	std::cout << "</head>\n";
// 	std::cout << "<body>\n";
// 	std::cout << "<table border = \"0\" cellspacing = \"2\">";
// 	for (int i = 0; i < NR_OF_CGI_ENV_VARS; i++)
// 	{
// 		std::cout << "<tr><td>" << _ENV[i] << "</td><td>";
// 		//we have to create ENV based on the location details to pass it in a char ** to the execved program
// 		//concat env[i] + the info gained from the config file
		
// 		char *value = getenv(_ENV[i].c_str());
// 		if (value != NULL)
// 		{
// 			std::cout << value;
// 		}
// 		else
// 		{
// 			std::cout << "ENV var doesn't exist.";
// 		}
// 		std::cout << "</td></tr>\n";
// 	}
// 	std::cout << "</table>\n";
// 	std::cout << "</body>\n";
// 	std::cout << "</html>\n";
// 	return ;
// }
