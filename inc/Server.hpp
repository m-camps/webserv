#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <map>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>

#define TOTAL_METHODS 3
#define TOTAL_ACCEPTED_SOCKETS 4

class Server
{
	public:
		Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);
		~Server();

		/* accessors */
		std::vector<std::string>&	getPort(void);
		std::vector<std::string>&	getName(void);
		std::vector<std::string>&	getRoot(void);
		std::vector<std::string>&	getIndex(void);
		std::string&				getClientBodySize(void);
		struct sockaddr_in			getSockAddr(void) const;
		size_t    					getSockAddrLen(void) const;

		/* mutators */
		void	setPort(Server& server, std::string& portToAdd);
		void	setName(Server& server, std::string& serverNameToAdd);
		void	setRoot(Server& server, std::string& rootLocationToAdd);
		void	setIndex(Server& server, std::string& indexToAdd);
		void	setClientBody(Server& server, std::string& BodySize);


		/* rest */
		void	printServerAttributes(Server& server);

	private:
		std::vector<std::string>				_port; //listen
		std::vector<std::string>				_name; //can be searching for multiple names
		std::vector<std::string>				_root; //default = "/"
		std::vector<std::string>				_index; //default = "index.html"
		std::string								_clientBodySize;	//default = 10;
		std::list<std::string>					_methods[TOTAL_METHODS]; // default = ["GET", "POST", "DELETE"]
		int										_socket_fd;
		int										_sockets[TOTAL_ACCEPTED_SOCKETS];
		struct sockaddr_in						_address;
		size_t                                  _addressLen;
		//std::map<std::string, struct location>	locations<location_path, location>;
};

std::ostream& operator<<(std::ostream& stream, const Server& Server);

#endif

