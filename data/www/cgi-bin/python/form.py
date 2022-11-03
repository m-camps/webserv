#/usr/bin/python3
#for this one we would need to set up the meta variables in parent process

import cgi
form = cgi.FieldStorage()

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print('<meta charset="UTF-8">')
print("<title>Index</title>")
print(form["username"])
print("</head>")
print("<body>")

