import urllib

re_url = "http://challengeland.co/Challenges/Programming/0b17c079deRequest"
print(dir(urllib))
f = urllib.urlopen(re_url)
print(f.read())