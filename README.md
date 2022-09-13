
## Webserv 42
Webserver written in C++98
  

## Handy Documentation
[What is a socket](https://beej.us/guide/bgnet/html/index-wide.html#:~:text=2-,What%20is%20a%20socket%3F,-You%20hear%20talk)
[Dealing With and Getting Around Blocking Sockets](http://dwise1.net/pgm/sockets/blocking.html)


[HTTP Made Really Easy](https://www.jmarshall.com/easy/http/)

Bottom 2 links middle mouse click for jumping to middle of page

[Structure of HTTP transactions](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Structure%20of%20HTTP%20Transactions,-Like%20most%20network)

[Sample HTTP Exchange](https://www.jmarshall.com/easy/http/#:~:text=Table%20of%20Contents-,Sample%20HTTP%20Exchange,After%20sending%20the%20response%2C%20the%20server%20closes%20the%20socket.,-To%20familiarize%20yourself)

  

### Features
- Parser for config [link](https://www.nginx.com/resources/wiki/start/topics/examples/full/)

- HTTP Headers [link](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers)

- CGI (commen gateway interface)  [link](https://en.wikipedia.org/wiki/Common_Gateway_Interface)


#### Classes that might be needed

Server
Socket?
CGI?
Request

  
  
  

### Link Dump (ripped from repos)
web socket

nice article https://www.bogotobogo.com/cplusplus/sockets_server_client.php

Beej's Guide to Network Programming http://beej.us/guide/bgnet/html/

"build http server from scratch in C" https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa

Les sockets en C https://broux.developpez.com/articles/c/sockets/

select() http://manpages.ubuntu.com/manpages/xenial/fr/man2/select_tut.2.html

select() example https://man.developpez.com/man2/select_tut/

select() in the details https://notes.shichao.io/unp/ch6/#readset-writeset-and-exceptset-as-value-result-

nice schema https://www.ibm.com/support/knowledgecenter/ssw_ibm_i_72/rzab6/xnonblock.htm

select() vs threads https://www.lowtek.com/sockets/select.html

TCP 3-way handshake schema ; Blocking vs non-blocking socket https://www.bogotobogo.com/cplusplus/sockets_server_client_2.php

more socket theory to waste time https://www.bogotobogo.com/cplusplus/sockets_server_client_3.php

http protocol

How the web works http://www.garshol.priv.no/download/text/http-tut.html

RFCs big picture, in french http://abcdrfc.online.fr/rfc-vf/pdf/rfc2616.pdf

RFCs 7230-7235 overview, in french https://www.bortzmeyer.org/http-11-reecrit.html

overview http://www.iprelax.fr/http/http_art.php

another overview https://www.commentcamarche.net/contents/520-le-protocole-http

Request and Response format https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages

another overview https://www.coozook.com/static/book-samples/B212934F6A-sample.pdf

http headers in details

MIME list https://developer.mozilla.org/fr/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Common_types

What HTTP response headers are required https://stackoverflow.com/questions/4726515/what-http-response-headers-are-required

response status code https://fr.wikipedia.org/wiki/Liste_des_codes_HTTP

auth theory https://developer.mozilla.org/en-US/docs/Web/HTTP/Authentication#Basic_authentication_scheme

auth practice https://stackoverflow.com/questions/180947/base64-decode-snippet-in-c

CGI programming

nice overview https://perso.liris.cnrs.fr/lionel.medini/enseignement/M1IF03/Tutoriels/Tutoriel_CGI_SSI.pdf

some details https://www.developpez.net/forums/d151285/php/langage/php-js-quoi-sert-php-cgi-exe-repertoire-php/

chunked encoding

wiki https://fr.wikipedia.org/wiki/Chunked_transfer_encoding

the idea https://www.geeksforgeeks.org/http-headers-transfer-encoding/

utils

https://superuser.com/questions/48505/how-to-find-virtual-memory-size-and-cache-size-of-a-linux-system

https://blog.seboss666.info/2015/08/reprenez-le-controle-du-cache-memoire-du-noyau-linux/