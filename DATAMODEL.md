
```
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};

struct	location {
	std::string					path;
	std::string					root;  // default = "/"
	std::string					index; // default = "index.html"
	std::list<std::string>		methods[TOTAL_METHODS]; // default = ["GET", "POST", "DELETE"]
	std::string					cgi_ext;	// no-default
	std::string					cgi_name;	// no-default
	int							autoindex; // default = 0
}

Class Server
	int										port;
	std::string								name;
	std::string								root; default = "/"
	std::string								index; default = "index.html"
	std::string								client_body_size;	default = 10;
	std::list<std::string>					methods[TOTAL_METHODS]; // default = ["GET", "POST", "DELETE"]
	int										socket_fd;
	int										sockets[TOTAL_ACCEPTED_SOCKETS];
	struct sockaddr_in						sockaddr_in;
	std::map<std::string, struct location>	locations<location_path, location>;

Main
	Server servers[TOTAL_SERVERS_BASED_ON_CONFIG];

```


parseNetwork
	parseServer (std::string from server { to the next } that closes that block)
		if (location) 
			parseLocation (std::string from location <name> { to the next } that closes that block)
			parseLocation (std::string from location <name> { to the next } that closes that block)
			parseLocation (std::string from location <name> { to the next } that closes that block)
		else
			parseDirective (std::string till end of line (; in our case))
			parseDirective (std::string till end of line (; in our case))
	parseServer  (std::vector<std::string> from server { to the next } that closes that block)
		if (location)
			parseLocation (std::string from location <name> { to the next } that closes that block)
				parseLocationDirective (std::string till end of line (; in our case)) //lines of the location eg root etc
				parseLocationDirective (std::string till end of line (; in our case))
				parseLocationDirective (std::string till end of line (; in our case))
			parseLocation (std::string from location <name> { to the next } that closes that block)
				parseLocationDirective (std::string till end of line (; in our case))
			parseLocation (std::string from location <name> { to the next } that closes that block)
				parseLocationDirective (std::string till end of line (; in our case))
		else
			parseDirective (std::string till end of line (; in our case))
			parseDirective (std::string till end of line (; in our case))
