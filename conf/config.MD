## Config Options

Context Server:
```
- servername			name ...;
- listen				port;
- error_page			error_page(s) page_uri;
- client_body_size		size;
- root					path;
- allow_methods			method(s);
- index					default_file;
- return				statuscode uri;
- location 				uri (no regexp) { ... };
```

Context Location:
```
- autoindex				[on/off];
- root					path;
- allow_methods			method;
- index					default_file;
- cgi_file_extension	file_extenstion; (without .)
- cgi_name				name;
```
## Example

Server
```
server {
	servername			fred.com;
	listen				80;

	error_page			500 502 503 504 /404.html;
	client_body_size	100;
	root				/www/data;
	allow_methods		GET POST;
	index				index.html;

	location /php {
		root				/fpm;
		autoindex			off;
		index				index.php;
		allow_methods		GET;
		cgi_file_extension	php;
		cgi_name			php-fpm;
	}

	location /python {
		root				/;
		autoindex			on;
		index				python.html;
		allow_methods		GET DELETE;
		cgi_file_extension	py;
		cgi_name			python3;
	}
}
```
Redirect server

```
server {
	listen 				443;
	server_name			;
	return				301	http//localhost:80;
}
```