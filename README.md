
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


#### Functions we need

- socket - create an endpoint for communication
- bind - bind a name to a socket
- poll - wait for some event on a file descriptor
- htonl, htons, ntohl, ntohs - convert values between host and network byte order

#### Youtube videos with concept explanation
[Web Server Concepts and Examples](https://www.youtube.com/watch?v=9J1nJOivdyw)


#### Webserver Info

1. listens
2. on a port
3. for a request
4. transport protocol
5. response
6. resource