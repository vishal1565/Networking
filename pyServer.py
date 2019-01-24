import socket
import sys

sd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
serverAdd = (input("Enter Host: "),1555)
sd.bind(serverAdd)
sd.listen(1)
print("Server is UP and RUNNING!!\n")

conn, addr = sd.accept()
print("Client Connected:",addr,"\n")
fileName = conn.recv(1024).decode()
print("Client is sending file",fileName,"\n")

fp = open(fileName,"wb")
while True:
    filesize = conn.recv(1024).decode()
    print(filesize,type(filesize))
    data = conn.recv(int(filesize)+100)
    #data = conn.recv(100000)
    if not data:
        break
    fp.write(data)
    fp.close()

    print("File Received Successfully!!!\nClosing Connection....\n")
    sys.exit(0)