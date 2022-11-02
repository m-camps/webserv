#include "../inc/Cgi.hpp"
#include <iostream>
#include <unistd.h>

const char*		buildCgiExecPath(Exchange& ExchangeRef, locIt& location)
{
	Server current = ExchangeRef.getServer();

	char absolutePath[300];
	getcwd(absolutePath, sizeof(absolutePath));
	
	std::string executablePath(absolutePath); //../data/www/cgi-bin/filename+extension
	executablePath += "/";
	executablePath += current.getRoot(); //for now, will depend on the root of the location/server block
	executablePath += "/cgi-bin";
	executablePath += location->second.getLocationName();
	executablePath += "/";
	executablePath += location->second.getLocationCgiName();

	//only if there is an fileextension format
	//executablePath += ".";
	//executablePath += location->second.getLocationCgiFileExtension();
	const char* execvePath = executablePath.c_str();

	return execvePath;
}

void			Cgi::childProcess(int *fds, Exchange& ExchangeRef, locIt& location)
{
	const char *path = buildCgiExecPath(ExchangeRef, location);
		
	close(fds[0]);
	close(STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	if (access(path, (X_OK | F_OK)) == 0) //access("/Users/bence/Desktop/tmpwebserv/webserv_current_oct30/webserv/data/www/cgi-bin/hello", (X_OK | F_OK)) == 0)   ////f_ok is existence, x_ok is execute permission
	{
		execve(path, NULL, NULL); //first will be the path as built from location, second the arguments after, third is envp variables
	}
	else
	{
		std::cout << "We could not execute the CGI script what you asked for. Exciting process." << std::endl;
		exit(1);
	}
	close(fds[1]); //prob dont need this one
	return ;
}


void			Cgi::parentProcess(int* fds, int& stat)
{
	close(fds[1]);
	dup2(fds[0], STDIN_FILENO);
	waitpid(-1, &stat, 0);
	if (WIFEXITED(stat) == true)
	{
		static char buff[1024];
		int ret = read(fds[0], buff, sizeof(buff));
		std::cout << buff << " " << ret << std::endl;
	}
	else
	{
		//maybe signaled quit? interruption ? 
	}
	close(fds[0]);
	return ;
}


void			Cgi::executeScript(Exchange& ExchangeRef, locIt& location)
{
	int	fds[2];
	int	stat;
	pipe(fds); //3,4

	pid_t	pid = fork();
	if (pid == -1)
	{
		std::cout << "Forking process for CGI failed." << std::endl;
	}
	else if (pid == 0)
	{	
		childProcess(fds, ExchangeRef, location);
	}
	else if (pid > 0)
	{
		parentProcess(fds, stat);
	}
	return ;
}

Cgi::Cgi() 
{
	std::cout << "inside constructor for cgi" << std::endl;
	return ;
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
