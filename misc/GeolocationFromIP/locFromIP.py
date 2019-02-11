import geoip2
import geoip2.database
import webbrowser
import os
import sys

readDB = geoip2.database.Reader('./Geolite2-City.mmdb')
add = input("Enter IP Address: ")
print(add)
a = input()
getLoc = readDB.city(add)
print("Country :",getLoc.country.name)
print("City :",getLoc.city.name)
print("Postal Code :",getLoc.postal.code)
print("Latitude :",getLoc.location.latitude)
print("Longitude :",getLoc.location.longitude)
lat, lon = getLoc.location.latitude,getLoc.location.longitude
mapUrl = 'http://www.google.com/maps/place/49.46800006494457,17.11514008755796'+str(lat)+','+str(lon)
webbrowser.open(mapUrl)
