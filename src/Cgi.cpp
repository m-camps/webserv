//
// Created by Xander Voorvaart on 11/1/22.
//

#include "../inc/Cgi.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>

std::string     Cgi::createServerErrorBody(Respond& ResponderRef, int errorCode)
{
	ResponderRef.setStatusCode(errorCode);
	std::string defaultServerErrorBody = Generator::generateDefaulPage(ResponderRef);
	return defaultServerErrorBody;
}

void        Cgi::createFailedSysCallResponse(Respond& ResponderRef, int errorCode)
{
	ResponderRef.setStatusCode(errorCode);
	ResponderRef.setBody(createServerErrorBody(ResponderRef, errorCode));
	return ;
}

std::string     Cgi::buildCgiExecPath(Respond& ResponderRef)
{
	Location    currentLoc = ResponderRef.getLocation();
	std::string PATH = ResponderRef.getEntryFromMap("Path");
	std::string locationRoot = currentLoc.getRoot();
	std::string locationName = currentLoc.getName();
	size_t      index = PATH.find(locationName);
	char        currentWorkingDir[500];

	PATH.replace(index, locationName.length() + 1, locationRoot);
	if (getcwd(currentWorkingDir, sizeof(currentWorkingDir)) == NULL)
	{
		std::cout << "Could not enter to current working directory, closing program" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string relativePath(currentWorkingDir);
	relativePath += '/' + PATH;
	return relativePath;
}

char**          Cgi::createArgv(Respond& ResponderRef)
{
	std::string                 requestedFilePath = buildCgiExecPath(ResponderRef);
	// std::string                 interpreterPath = "/opt/homebrew/opt/python@3.10/bin/python3.10"; //on m2
	std::string               interpreterPath = "/usr/local/bin/python3";
	std::vector<std::string>    argvString;
	char                        **argv;

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

char**          Cgi::createEnvp(Respond& ResponderRef)
{
	std::vector<std::string>    envpStrig;
	std::string                 pathInfo = "PATH_INFO=/usr/local/bin/python3";
	std::string                 serverProtocol = "SERVER_PROTOCOL=HTTP/1.1";
	std::string                	serverName = "SERVER_NAME=" + ResponderRef.getServer().getNames().front();
	char**                      envp;

	envpStrig.push_back(pathInfo);
	envpStrig.push_back(serverProtocol);
	envpStrig.push_back(serverName);

	try
	{
		envp = new char*[4];
	}
	catch(std::bad_alloc& e)
	{
		std::exit(INTERNAL_SERVER_ERROR_CODE);
	}
	for (int i = 0; i < 3; i++)
	{
		envp[i] = strdup(static_cast<const char*>(envpStrig[i].data()));
		if (!envp[i])
		{
			std::exit(INTERNAL_SERVER_ERROR_CODE);
		}
	}
	envp[3] = NULL;
	return envp;
}

void            Cgi::childProcess(int *fds, Respond& ResponderRef)
{
	struct stat s;
	char**      argv = createArgv(ResponderRef);
	char**      envp = createEnvp(ResponderRef);
	Location    currentLoc = ResponderRef.getLocation();

	if (close(fds[0]) < 0 || close(STDIN_FILENO) < 0 || dup2(fds[1], STDOUT_FILENO) < 0)
		return (createFailedSysCallResponse(ResponderRef, e_InternalServerError));

	if (stat(argv[1], &s) == 0)
	{
		if (s.st_mode & S_IFREG && access(argv[1], (X_OK)) == 0)
		{
			execve(argv[0], argv, envp);
		}
	}
	std::exit(INTERNAL_SERVER_ERROR_CODE);
}

void        Cgi::parentProcess(Respond& ResponderRef, int* fds, int& stat)
{
	if (close(fds[1]) < 0 || dup2(fds[0], STDIN_FILENO) < 0 || waitpid(-1, &stat, 0) < 0)
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

std::string         Cgi::executeScript(Respond& ResponderRef)
{
	int fds[2];
	int stat;

	if (pipe(fds) < 0)
	{
		return (createServerErrorBody(ResponderRef, e_InternalServerError));
	}
	pid_t   pid = fork();
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
