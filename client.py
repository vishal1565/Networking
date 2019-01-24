import socket
import sys
import os
from PIL import Image, ImageOps

sd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
serverAdd = (input("Enter Server Address: "),1555)

sd.connect(serverAdd)
fileName = input("ENter filename to send: ")
sd.send(fileName.encode())

# inverting image

img  = Image.open(fileName)
img = ImageOps.invert(img)
s = 'inverted.jpg'
img.save(s)


with open(s, "rb") as fp:
    print("\nSending File:",fileName)
    sd.send((str(os.stat(s).st_size)).encode())
    data = fp.read()
    sd.sendall(data)

    print("File Sent Successfully!!!\nClosing Connection...")
    sd.close()