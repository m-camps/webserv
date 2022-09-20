## Config Options

Context Server:
- servername			name ...;
- listen				port;
- error_page			error_page(s) page_uri;
- client_body_size		size;
- root					path;
- allow_methods			method(s);
- index					default_file;
- return				uri redirect;
- location 				uri (no regexp) { ... };

Context Location:
- autoindex				[on/off];
- root					path;
- allow_methods			method;
- index					default_file;
- cgi_file_extension	file_extenstion;
- cgi_name				name;

## Example

```
server {
	servername			fred;
	listen				80;

	error_page			500 502 503 504 /404.html;
	client_body_size	100;
	
}
```