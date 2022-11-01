#include "../inc/Cgi.hpp"
#include <iostream>
#include <unistd.h>


void			Cgi::childProcess(int *fds)
{
	/* will have to build a proper path and call it as executable for now you can replace the path in faccessat and execve with your absolute path
	Server current = ExchangeRef.getServer();
	std::string executablePath; //../data/www/cgi-bin/filename+extension
	executablePath += "../";
	executablePath += current.getRoot();
	executablePath += "/cgi-bin"; //for now
	executablePath += "/hello"; //for now

	const char* execvePath = executablePath.c_str();
	*/
	close(fds[0]);
	close(STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO); //closes stdout, now fds1 is stdout
	if (access("/Users/bence/Desktop/tmpwebserv/webserv_current_oct30/webserv/data/www/cgi-bin/hello", (X_OK | F_OK)) == 0)   //faccessat(AT_FDCWD, "/Users/bence/Desktop/tmpwebserv/webserv_current_oct30/webserv/data/www/cgi-bin/hello", (X_OK | F_OK), 0) == 0) //f_ok is existence, x_ok is execute permission
	{
		std::cout << "RIGHT PLACE HERE" << std::endl;
		execve("/Users/bence/Desktop/tmpwebserv/webserv_current_oct30/webserv/data/www/cgi-bin/hello", NULL, NULL); //first will be the path as built from location, second the arguments after, third is envp variables
	}
	else
	{
		std::cout << "We could not execute the CGI script what you asked for. Exciting process." << std::endl;
		exit(1);
	}
	close(fds[1]); //prob dont need this one
	return ;
}

/*
void			Cgi::parentProcess(int* fds, int& stat)
{
	close(fds[1]); //no use for write end of the pipe
	dup2(fds[0], STDIN_FILENO); //closes STDIN, points at the read end of the pipe
	waitpid(-1, &stat, 0); //wait for any child
	if (WIFEXITED(stat) == true) //did it exit without problem?
	{
		static char buff[1024];
		int ret = read(fds[0], buff, sizeof(buff)); //reads from the read end
		//std::cout << buff << " is whats inside the buffer, " << ret << " is ret." << std::endl;
	}
	close(fds[0]); //closes last unused fd
	return ;
}
*/

void			Cgi::executeScript(Exchange& ExchangeRef)
{
	int	fds[2];
	int	stat;
	pipe(fds); //3,4 

	pid_t	pid = fork();
	if (pid == -1)
	{
		std::cout << "FORK FAILED" << std::endl;
	}
	else if (pid == 0)
	{	
		childProcess(fds);
	}
	else if (pid > 0)
	{
		close(fds[1]); //no use for write end of the pipe
		dup2(fds[0], STDIN_FILENO); //closes STDIN, points at the read end of the pipe
		waitpid(-1, &stat, 0); //wait for any child
		if (WIFEXITED(stat) == true) //did it exit without problem?
		{
			static char buff[1024];
			int ret = read(fds[0], buff, sizeof(buff)); //reads from the read end
			//std::cout << buff << " is whats inside the buffer, " << ret << " is ret." << std::endl;
		}
		close(fds[0]); //closes last unused fd
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
