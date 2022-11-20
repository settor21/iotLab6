import urllib.request
temp = "1"
humidity ="1"
url = "http://127.0.0.1/iotlab6/Lab6.php?Temperature="+str(temp)+"&Humidity="+str(humidity)
print ("started")
contents = urllib.request.urlopen(url).read()
print (contents)
print ("finshed")
