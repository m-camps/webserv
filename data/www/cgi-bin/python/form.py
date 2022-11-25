
# import sys
# from xml.etree import ElementTree as ET

# html = ET.Element('html')
# body = ET.Element('body')
# head = ET.Element('head') # print("<head>First CGI program</head>")

# html.append(body)
# html.append(head)
# div_head = ET.Element('div', attrig={'class': 'head_foo'})
# head.append(div_head)
# head.text = "First CGI program"
# div = ET.Element('div', attrib={'class': 'foo'})
# body.append(div)
# span = ET.Element('span', attrib={'class': 'bar'})
# div.append(span)
# span.text = "Hello World"

# if sys.version_info < (3, 0, 0):
#   # python 2
#   ET.ElementTree(html).write(sys.stdout, encoding='utf-8',
#                               method='html')
# else:
#   # python 3
#   ET.ElementTree(html).write(sys.stdout, encoding='unicode',
#                           method='html')

import os

path_info = os.getenv("PATH_INFO", default=None)
server_protocol = os.getenv("SERVER_PROTOCOL", default=None)
server_name = os.getenv("SERVER_NAME", default=None)

print('<!DOCTYPE html>')
print('<html lang="en">')
print('<head>')
print('<meta charset="UTF-8">')
print('<meta http-equiv="X-UA-Compatible" content="IE=edge">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print('<title>Product Landing Page</title>')
print('</head>')
print('<link rel="stylesheet" href="default.css">')
print('<body>')
print('<nav>')
print('<div class="logo">Team BMX</div>')
print('<span class="menubtn" onclick="openNav()">&#9776;</span>')
print('<div class="navLinks">')
print('<ul>')
print('<li><a href="http://ns.nl">NS.nl</a></li>')
print('<li><a href="#">' + server_name + '</a></li>')
print('<li><a href="#">' + server_protocol + '</a></li>')
print('<li><a href="#">' + path_info + '</a></li>')
print('<button type="button">CGI</button>')
print('</ul>')
print('</div>')
print('</nav>')
print('<div class="sideNav" id="sidenav">')
print('<a href="#" class="closeBtn" onclick="closeNav()"> &#10006;</a>')
print('<a href="#">Home</a>')
print('<a href="#">About</a>')
print('<a href="#">Services</a>')
print('<a href="#">Contact</a>')
print('<a href="#"><button type="button">CGI</button> </a>')
print('</div>')
print('<div id="centered-body">')
print('<div>')
print('<h1 class="displayedBody">This page is being displayed by calling a python script via CGI.</h1>')
print('</div>')
print('</body>')
print('</html>')
