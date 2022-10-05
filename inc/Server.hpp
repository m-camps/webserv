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

#define ERROR 1

typedef struct	location //this could also be a class...
{
	std::vector<std::string>		_root; //default = "/"
	std::vector<std::string>		_index; //default = "index.html"
	std::vector<std::string>		_methods;//[TOTAL_METHODS]; // default = ["GET", "POST", "DELETE"]
	bool							_autoIndex;	//default = 10;
	std::string						_cgi_name;	// no-default //what is this?
	std::string						_cgi_file_extension;	// no-default what is extension?
}	t_location;

class Server
{
	public:
		Server();
		Server(const Server& src);
		Server& operator=(const Server& rhs);
		~Server();

		/* accessors for Server part */
		std::vector<std::string>&	getPort(void);
		std::vector<std::string>&	getName(void);
		std::vector<std::string>&	getRoot(void);
		std::vector<std::string>&	getIndex(void);
		std::vector<t_location>&	getLocations(void);
		//std::vector<t_location>&	getMethods(void);
		std::string&				getClientBodySize(void);
		struct sockaddr_in			getSockAddr(void) const;
		size_t    					getSockAddrLen(void) const;


		/* accessors for location part */
		//std::vector<std::string>&	getName(void);
		std::vector<std::string>&	getLocationRoot(void);
		std::vector<std::string>&	getLocationIndex(void);
		//std::vector<t_location>&	getMethods(void);
		bool&						getLocationAutoIndex(void);
		std::vector<std::string>&	getLocationAllowMethods(void);
		std::string&				getLocationCgiFileExtension(void);
		std::string&				getLocationCgiName(void);


		/* mutators for Server part*/
		void	setPort(Server& server, std::string& portToAdd);
		void	setName(Server& server, std::string& serverNameToAdd);
		void	setRoot(Server& server, std::string& rootLocationToAdd);
		void	setIndex(Server& server, std::string& indexToAdd);
		void	setClientBody(Server& server, std::string& BodySize);
		void	setMethods(Server& server, std::string& methodToAdd);

		/* mutators for Location part */
		//void	setLocationIndex(Server& server, std::string& LocationIndexToAdd);

		//void	parseAutoIndex

		/* rest */
		void	printServerAttributes(Server& server);

		// mcamps Functions
		void	setup();
		void	changePort(std::string newPort);
		int32_t	getSocketFd(void);
		int		acceptConnection();

	private:
		std::vector<std::string>				_port; //listen
		std::vector<std::string>				_name; //can be searching for multiple names
		std::vector<std::string>				_root; //default = "/"
		std::vector<std::string>				_index; //default = "index.html"
		std::string								_clientBodySize;	//default = 10;
		std::list<std::string>					_methods[TOTAL_METHODS]; // default = ["GET", "POST", "DELETE"] //this can be an array
		//int										_socket_fd;
		//int										_sockets[TOTAL_ACCEPTED_SOCKETS];
		struct sockaddr_in						_address;
		size_t                                  _addressLen;
		std::vector<t_location>					_locations; //listen

		//vector of locations?
		//std::map<std::string, struct location>	locations<location_path, location>;

		// mcamps variables
		struct sockaddr_in	*_address_in;
		int					_socket_fd;

		// mcamps functions
		void	getSocketAddr();
		void	createSocket();
		void	setupSocket();
};

void	setLocationIndex(Server& server, std::string& LocationIndexToAdd);
void	setCgiName(Server& server, std::string& cgiName);
void	setCgiFileExtension(Server& server, std::string& cgiFileExtension);
void	setLocationRoot(Server& server, std::string& locationRootToAdd);
void	setLocationAllowMethod(Server& server, std::string& locationAllowedMethods);
void	setLocationAutoindex(Server& server, std::string& AutiondexToSet);


std::ostream& operator<<(std::ostream& stream, const Server& Server);

#endif
