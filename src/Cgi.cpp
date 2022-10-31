#include "../inc/Cgi.hpp"
#include <iostream>
#include <unistd.h>

void			Cgi::executeScript(void)
{
	int	fds[2];
	int	stat;
	pipe(fds); //3,4 

	//std::cout << fds[0] << " is 0, " << fds[1] << " is 1." << std::endl;
	pid_t	pid = fork(); //child inherits parent stdin, stdout, etc
	if (pid == 0)
	{	
		close(fds[0]); //read end
		close(STDIN_FILENO);
		dup2(fds[1], STDOUT_FILENO); //closes stdout, now fds1 is stdout
		//if (access(//name of executable), (F_OK && X_OK) == 0) //f_ok is existence, x_ok is execute permission
		//{
			//execve() //first will be the path as built from location, second the arguments after, third is envp variables
			//write(fds[1], "HELLO FROM CHILD", 17); //could write straight to STDOUT_FILENO (1)
		//}
		//else
		//{
		//we couldnt execute it, exitcode 1 or so to check if it was executable or not?
		//}
		write(fds[1], "HELLO FROM CHILD", 17); //could write straight to STDOUT_FILENO (1)
		close(fds[1]);
	}
	else
	{
		close(fds[1]); //no use for write end of the pipe
		waitpid(-1, &stat, 0); //wait for any child
		dup2(fds[0], STDIN_FILENO); //CLOSES STDIN, NOW 0 IS in
		if (WIFEXITED(stat) == true) //did it exit without problem?
		{
			//closing stdin
			static char buff[1024];
			int ret = read(fds[0], buff, sizeof(buff));
			std::cout << buff << " is whats inside the buffer, " << ret << " is ret." << std::endl;
			//std::cout << "INSIDE PARENT" << std::endl;
			//while (1){
				//close(STDIN_FILENO); //this is fds[0] now
			//};
		}
		close(fds[0]); //why is this not being seen?
	}
	return ;
}

Cgi::Cgi() 
{
	std::cout << "inside constructor for cgi" << std::endl;
	return ;
}

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
