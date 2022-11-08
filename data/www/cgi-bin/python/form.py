#!/usr/bin/python3
#for this one we would need to set up the meta variables in parent process

'''
import os
import cgi, cgitb
cgitb.enable()
form = cgi.FieldStorage()

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print('<meta charset="UTF-8">')
print("<title>Index</title>")
#print(form["username"])
print(os.environ['HOME'])
print("</head>")
print("<body>")
'''

import cgi, cgitb
cgitb.enable()
input_data = cgi.FieldStorage()
# name = input_data.getvalue('fname')
# print("Name of the user is:",name)
name = input_data.getvalue('SERVER_NAME')

#print("name is :", name)
#print("name of the user is INSIDE FORM:", name)

print(cgi.test())
#print("the name is %s, ", name)
# print("<!DOCTYPE html>")
# print("<html>")
# print("<head>First CGI program</head>")
# print("<body>")
# print("<p>It seems to be working inside CGI.")
# print("<form>Fill me out</form>")
# print('<input type="checkbox" name="item" value="1" />')
# print('<input type="checkbox" name="item" value="2" />')
# print("</p>")
# print(cgi.print_environ_usage())
# print("</body>")
# print("</html>")
