import geoip2
import geoip2.database
import webbrowser
import os
import sys

readDB = geoip2.database.Reader('./GeoLite2-City/Geolite2-City.mmdb')
add = input("Enter IP Address: ")
getLoc = readDB.city(add)
print("Country :",getLoc.country.name)
print("City :",getLoc.city.name)
print("Postal Code :",getLoc.postal.code)
print("Latitude :",getLoc.location.latitude)
print("Longitude :",getLoc.location.longitude)
lat, lon = getLoc.location.latitude,getLoc.location.longitude
mapUrl = 'http://www.google.com/maps/place/'+str(lat)+','+str(lon)
a = input("Press any key to navigate to Google Map...\n")
webbrowser.open(mapUrl)
