import socket
import sys
import random


def imgHeader(height,width,fp,fileName):         
    fp.write("P2\n")
    fp.write(("# "+fileName+"\n"))
    fp.write((str(height)+" "+str(width)+"\n"))
    fp.write("15")

sd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
serverAdd = ("127.0.0.1",1555)
sd.bind(serverAdd)
sd.listen(1)
print("Server is UP and RUNNING!!\n")

conn, addr = sd.accept()
print("Client Connected:",addr,"\n")
msg = conn.recv(1024).decode()
print("Client:",msg)

print("\nNow Generating a Random Image\n")
fileName = "generatedImage.pgm"
fp = open(fileName,"w")
height, width, img = 10, 10, []
imgHeader(height,width,fp,fileName)
for i in range(height):
    temp = []
    for j in range(width):
        temp.append(random.randint(0,15))
    temp1 = " ".join(str(k) for k in temp)
    fp.write(("\n"+temp1))
    img.append(temp)
fp.close()
print("Random Image generated Successfully..\nSaving File in 'generatedImage.pgm'")

# Sending dimensions of Image
conn.send(str(height).encode())
conn.send(str(width).encode())

# Now sending Image to Client
for i in img:
    conn.send((" ".join(str(j) for j in i)).encode())

# Receiving Inverted Image from Client
invImage = []
fileName = "invertedImage.pgm"
fp = open(fileName,"w")
imgHeader(height,width,fp,fileName)

print("\nReceiving Inverted Image from Client\n")
for _ in range(height):
    temp = conn.recv(1024).decode()
    fp.write(("\n"+temp))
    invImage.append(temp)
fp.close()
print("Successfully Received Inverted Image from Client\n")
sd.close()
