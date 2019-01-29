import socket
import sys
import os


def imgHeader(height,width,fp,fileName):         
    fp.write("P2\n")
    fp.write(("# "+fileName+"\n"))
    fp.write((str(height)+" "+str(width)+"\n"))
    fp.write("15")

sd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
serverAdd = ("127.0.0.1",1555)

sd.connect(serverAdd)
print("Connected to Server...\nNow requesting Image...")
msg = "Requesting Image"
sd.send(msg.encode())

# Now receiving dimensions of image
height = int(sd.recv(1024).decode())
width = sd.recv(1024).decode()
width = int(width)
print("Height: ",height)
print("Width: ",width)

# Now Receiving Image
print("\nNow Receiving Image from Server...")
img = []
fileName = "receivedImage.pgm"
fp = open(fileName,"w")
imgHeader(height,width,fp,fileName)
for _ in range(height):
    temp = sd.recv(1024).decode()
    fp.write(("\n"+temp))
    img.append(temp)
fp.close()
print("\nSuccessfully received the Image...")

# Now inverting Image
print("\nInverting the Image...")
fileName = "invertedImage.pgm"
fp = open(fileName,"w")
imgHeader(height,width,fp,fileName)
print("\nNow Inverting Image...")
invImage = []
for i in img:
    temp = [(15-int(j)) for j in i.split()]
    temp1 = " ".join(str(k) for k in temp)
    fp.write(("\n"+temp1))
    invImage.append(temp)
fp.close()
print("\nImage Inversion Successful...\n")

# Now Sending Inverted Image to Server
print("\nNow sending Inverted Image to Server")
for i in invImage:
    sd.send((" ".join(str(j) for j in i)).encode())
print("\nInverted Image Sent to Server...")
sd.close()