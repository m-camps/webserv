#include "../inc/Cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <stdlib.h> //check if we need this one

const char*		buildCgiExecPath(Respond& ResponderRef)
{
	Location current = ResponderRef.getLocation();
	char absolutePath[300]; //calloc, or static?

	if (getcwd(absolutePath, sizeof(absolutePath)) == NULL)
	{
		std::cout << "Could not enter to current working directory, closing program" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string executablePath(absolutePath); //../data/www/cgi-bin/filename+extension
	executablePath += "/";
	executablePath += current.getRoot(); //for now, will depend on the root of the location/server block
	executablePath += "/cgi-bin";
	executablePath += current.getName(); //what happens if this isnt / but just like python
	executablePath += "/";
	executablePath += current.getCgiName();
	if (current.getCgiFileExtension() != "" && current.getCgiName() != "")
	{
		executablePath += ".";
		executablePath += current.getCgiFileExtension();
	}
	const char* execvePath = executablePath.c_str();
	return execvePath;
}

char**			Cgi::createEnvVariables(Respond& ResponderRef)
{
	std::string	currentVal;
	std::string currentKey;

	char** envp = (char **)calloc(2, sizeof(char *));
	envp[0] = (char*)calloc(23, sizeof(char));

	//assigning servername to envp
	// currentVal = "SERVER_NAME=";
	// currentVal += ExchangeRef.getServer().getNames().at(0);
	//currentKey =  //what happens with more server blocks?
	
	memcpy(envp[0],  currentVal.c_str(), currentVal.length());
	return envp;
}

void			Cgi::childProcess(int *fds, Respond& ResponderRef)
{
	const char *path = buildCgiExecPath(ResponderRef);
	//char **envp = createEnvVariables(ResponderRef, location);
	//int res = putenv(envp[0]); //to check if it worked
	close(fds[0]);
	close(STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);

	//if I pass it here it works, if I create it above it doesnt for some reason
	char** envp = (char**)malloc(2 * sizeof(char**));
	envp[1] = NULL;
	envp[0] = (char*)malloc(sizeof(char) * 22);
	envp[0][21] = '\0';
	envp[0] = "SERVER_NAME=LOCALHOST";
	if (access(path, (X_OK | F_OK)) == 0)
	{
		execve(path, NULL, envp);
	}
	else
	{
		std::cout << "We could not execute the CGI script what you asked for. Exciting process." << std::endl;
		exit(1);
	}
}


void			Cgi::parentProcess(Respond& ResponderRef, int* fds, int& stat)
{
	close(fds[1]);
	dup2(fds[0], STDIN_FILENO);
	waitpid(-1, &stat, 0);
	if (WIFEXITED(stat) == true)
	{
		static char buff[1024];
		int ret = read(fds[0], buff, sizeof(buff));
		//if ret <= 0 ? 
		//if (ret == 0) //EOF
		std::string cgiBody(buff);
		ResponderRef.setBody(cgiBody); //exhangerefst body?
	}
	else
	{
		//maybe signaled quit? interruption ? 
	}
	close(fds[0]);
	return ;
}


std::string			Cgi::executeScript(Respond& ResponderRef)
{
	int	fds[2];
	int	stat;

	pipe(fds);
	pid_t	pid = fork();
	if (pid == -1)
	{
		std::cout << "Forking process for CGI failed." << std::endl;
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

Cgi::Cgi() 
{
	;
}


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
