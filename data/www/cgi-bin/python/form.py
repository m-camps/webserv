
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

# print("<!DOCTYPE html>")
# print("<html>")
# print("<head>")
# print('<link rel="stylesheet" href="default.css" id="theme-color">')
# print('</head>')
# print("<body>")
# print("<p>It seems to be working inside CGI.")
# print("</p>")
# print('<section class="py-7 py-md-0 bg-hero" id="home">')
# print('<div class="container">')
# print('<div class="row vh-md-100">')
# print('<div class="col-md-8 col-sm-10 col-12 mx-auto my-auto text-center">')
# print('<h1 class="heading-black text-capitalize">Quickly build landing pages with Knight</h1>')
# print('<p class="lead py-3">Knight is a platform that helps freelancers and companies build beautiful landing pages in minutes. Sign up for free.</p>')
# print('<button class="btn btn-primary d-inline-flex flex-row align-items-center">')
# print('Get started now')
# print('<em class="ml-2" data-feather="arrow-right"></em>')
# print('</button>')
# print('</div>')
# print('</div>')
# print('</div>')
# print('</section>')
# print("<p>")
# print("Value of env variable 'PATH_INFO': {} ".format(path_info))
# print("Value of env variable 'SCRIPT_NAME': {} ".format(script_name))
# print("</p>")
# print("<p>")
# print("IF THE VALUES ABOVE WERE PRINTED CORRECTLY, PASSING ENV VARIABLES VIA CGI IS SUFFICIENTLY FUNCTIONING.\n")
# print("</p>")
# print("</body>")
# print("</html>")


import os

# #print(cgi.test())
path_info = os.getenv("PATH_INFO", default=None)
script_name = os.getenv("SCRIPT_NAME", default=None)
server_name = os.getenv("SERVER_NAME", default=None)


print('<!DOCTYPE html>')
print('<html lang="en">')
print('<head>')
print('<meta charset="UTF-8">')
print('<meta http-equiv="X-UA-Compatible" content="IE=edge">')
print('<meta name="viewport" content="width=device-width, initial-scale=1.0">')
print('<link rel="stylesheet" href="menu.css">')
print('<title>Product Landing Page</title>')
print('</head>')
print('<body>')
print('<nav>')
print('<div class="logo">Brand</div>')
print('<span class="menubtn" onclick="openNav()">&#9776;</span>')
print('<div class="navLinks">')
print('<ul>')
print('<li><a href="http://ns.nl">Home</a></li>')
print('<li><a href="#">')
print(server_name)
print('</a></li>')
print('<li><a href="#">Services</a></li>')
print('<li><a href="#">Contact</a></li>')
print('<button type="button">Login</button>')
print('</ul>')
print('</div>')
print('</nav>')

print('<div class="sideNav" id="sidenav">')
print('<a href="#" class="closeBtn" onclick="closeNav()"> &#10006;</a>')
print('<a href="#">Home</a>')
print('<a href="#">About</a>')
print('<a href="#">Services</a>')
print('<a href="#">Contact</a>')
print('<a href="#"><button type="button">Login</button> </a>')
print('</div>')

print('<div id="centered-body">') #begin body div
print('<div>')
print('<h1 class="displayedBody">This page is being displayed by calling a python script via CGI</h1>')
print('</div>')
print('<div>')
print('<p class="padded">server name passed from cgi:' + server_name + '</p>')
print('</div>')

print('<div>')
print('<p class="padded">script name passed from cgi:' + script_name + '</p>')
print('</p>')
print('</div>')

print('<div>')
print('<p class="padded">path info passed from cgi:' + path_info + '</p>')
print('</p>')
print('</div>')

print('</div>')

# print('<div id="centered-body">')
# print('<h2 class="displayedBody">SERVER NAME : </h1>')
# print('</div>')

# print('<div id="centered-body">') #begin body div
# print('<p>server name passed from cgi: {}\n</p>'.format(server_name))
# print("</div>")
print("</div>")
#print(server_name)

print('<p>hi there</p>')
print('<p>hi there</p>')

print('</body>')
print('</html>')

#        <li><a href="#">Home</a></li>
#        <li><a href="#">About</a></li>
#        <li><a href="#">Services</a></li>
#        <li><a href="#">Contact</a></li>
#        <button type="button">Login</button>
#      </ul>
#    </div>
#  </nav>
#    <!-- responsive side navbar -->
#    <div class="sideNav" id="sidenav">
#      <a href="#" class="closeBtn" onclick="closeNav()"> &#10006;</a>
#      <a href="#">Home</a>
#      <a href="#">About</a>
#      <a href="#">Services</a>
#      <a href="#">Contact</a>
#      <a href="#"><button type="button">Login</button> </a>
#    </div>
   
#    <!-- Header content with banner image -->
#    <div class="row">
#      <div class="column1">
#        <h1>Responsive Design</h1>
#        <p>Contact us for responsive web design to grow your business!</p>
#        <button>PURCHASE</button>
#      </div>
#      <div class="column2">
#        <img src="https://www.hmablogs.com/wp-content/uploads/2022/06/banner-img.png" alt="banner" width="500px">
#      </div>
#    </div>
   
#    <!-- javascript to make side menu appear -->
#  <script>
#    function openNav() {
#      document.getElementById("sidenav").style.width = "50%";
#    }
#    function closeNav() {
#      document.getElementById("sidenav").style.width = "0%";
#    }
#  </script>
 
# </body>
# </html>




# import os

# #print(cgi.test())
# path_info = os.getenv("PATH_INFO", default=None)
# script_name = os.getenv("SCRIPT_NAME", default=None)
# print("<!DOCTYPE html>")
# print("<html>")
# print("<head>")
# print('<link rel="stylesheet" href="default.css" id="theme-color">')
# print('</head>')
# print("<body>")
# print("<p>It seems to be working inside CGI.")
# print("</p>")
# print('<section class="py-7 py-md-0 bg-hero" id="home">')
# print('<div class="container">')
# print('<div class="row vh-md-100">')
# print('<div class="col-md-8 col-sm-10 col-12 mx-auto my-auto text-center">')
# print('<h1 class="heading-black text-capitalize">Quickly build landing pages with Knight</h1>')
# print('<p class="lead py-3">Knight is a platform that helps freelancers and companies build beautiful landing pages in minutes. Sign up for free.</p>')
# print('<button class="btn btn-primary d-inline-flex flex-row align-items-center">')
# print('Get started now')
# print('<em class="ml-2" data-feather="arrow-right"></em>')
# print('</button>')
# print('</div>')
# print('</div>')
# print('</div>')
# print('</section>')
# print("<p>")
# print("Value of env variable 'PATH_INFO': {} ".format(path_info))
# print("Value of env variable 'SCRIPT_NAME': {} ".format(script_name))
# print("</p>")
# print("<p>")
# print("IF THE VALUES ABOVE WERE PRINTED CORRECTLY, PASSING ENV VARIABLES VIA CGI IS SUFFICIENTLY FUNCTIONING.\n")
# print("</p>")
# print("</body>")
# print("</html>")

