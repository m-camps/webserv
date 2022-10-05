
## Webserv 42
Webserver written in C++98
  

## Handy Documentation
[What is a socket](https://beej.us/guide/bgnet/html/index-wide.html#:~:text=2-,What%20is%20a%20socket%3F,-You%20hear%20talk)
[Dealing With and Getting Around Blocking Sockets](http://dwise1.net/pgm/sockets/blocking.html)


[HTTP Made Really Easy](https://www.jmarshall.com/easy/http/)

[Structure of HTTP transactions](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Structure%20of%20HTTP%20Transactions,-Like%20most%20network)

[Sample HTTP Exchange](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Sample%20HTTP%20Exchange,After%20sending%20the%20response%2C%20the%20server%20closes%20the%20socket.,-To%20familiarize%20yourself)

[Diagram for server](https://i.stack.imgur.com/YQ5ES.png)

[Lecture that explains sockets pretty well with examples (no clue how this ended up in my google search :grimacing:)](https://www.cs.cmu.edu/~srini/15-441/S10/lectures/r01-sockets.pdf)
[Struct used in lecture](https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html)

[Socket/Client applications](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)


### Features
- Parser for server config [link](https://www.nginx.com/resources/wiki/start/topics/examples/full/)

- HTTP Headers [link](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)

- HTTP Header Parser

- CGI (commen gateway interface)  [link](https://en.wikipedia.org/wiki/Common_Gateway_Interface)


#### Classes that might be needed

- Server

- Socket

- CGI?

- Request

- HTTP Parser

#### Youtube videos with concept explanation
[Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)


## Features
- [Parser for server config ](https://www.nginx.com/resources/wiki/start/topics/examples/full/)
- [HTTP Headers](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)
- [CGI (commen gateway interface) ](https://en.wikipedia.org/wiki/Common_Gateway_Interface)

#### Exchange class
The Exchange class will parse the incoming and outgoing data of the client & the server. You can get the parsed data by using `getHeader()`, which will return a `std::map<std::string, std::string>`.

A public function named: `PrintHeaderMap()`, which will print the key and the value of all nodes.

You can test it yourself by doing the following:
1. Run `webserv` <br>
2. Open a new terminal and run `telnet ADDRESS PORT`.
    `ADDRESS` is the URL of the webserv & `PORT` is the port given in the config file. <br>
3. `telnet` will connect to the webserv and ask what the next request   should be.
4. I recommend to use the following input: `GET /data/www/index.html HTTP/1.1`
5. Look at your webserv teminal and see how beautiful your line got parsed.

---

#### Functions we need

- socket - create an endpoint for communication
- bind - bind a name to a socket
- poll - wait for some event on a file descriptor
- htonl, htons, ntohl, ntohs - convert values between host and network byte order

#### Webserver Info

1. listens
2. on a port
3. for a request
4. transport protocol
5. response
6. resource

## Sources
- [What is a socket](https://beej.us/guide/bgnet/html/index-wide.html#:~:text=2-,What%20is%20a%20socket%3F,-You%20hear%20talk)
- [Dealing With and Getting Around Blocking Sockets](http://dwise1.net/pgm/sockets/blocking.html)
- [HTTP Made Really Easy](https://www.jmarshall.com/easy/http/)
[Structure of HTTP transactions](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Structure%20of%20HTTP%20Transactions,-Like%20most%20network)
- [Sample HTTP Exchange](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Sample%20HTTP%20Exchange,After%20sending%20the%20response%2C%20the%20server%20closes%20the%20socket.,-To%20familiarize%20yourself)
- [Diagram for server](https://i.stack.imgur.com/YQ5ES.png)
- [Lecture that explains sockets pretty well with examples (no clue how this ended up in my google search :grimacing:)](https://www.cs.cmu.edu/~srini/15-441/S10/lectures/r01-sockets.pdf)
- [Struct used in lecture](https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html)
- [Socket/Client applications](https://www.bogotobogo.com/cplusplus/sockets_server_client.php)
- [Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)
