import socket
import struct
import os
import sys

# Unpack Ethernet frames
def ethernetFrame(data):
    dest, src, proto = struct.unpack('! 6s 6s H', data[:14])
    return macAddr(dest), macAddr(src), socket.htons(proto), data[14:]

# Fomatting Mac Address
def macAddr(byteAddr):
    byteStr = map('{:02x}'.format, byteAddr)
    macAddress = ':'.join(byteStr).upper()
    return macAddress

# Unpack IPv4 packet
def ipv4Packet(data):
    versionLength = data[0]
    version = versionLength >> 4
    headerLength = (versionLength & 15)*4
    ttl, proto, src, target = struct.unpack('! 8x B B 2x 4s 4s', data[:20])
    return version, ttl, proto, ipv4Format(src), ipv4Format(target), data[versionLength:]

# Format IPv4 address
def ipv4Format(addr):
    return '.'.join(map(str,addr))

# Unpack ICMP packet
def icmpPacket(data):
    icmpType, code, checkSum = struct.unpack('! B B H', data[:4])
    return icmpType, code, checkSum, data[4:]

# Unpack TCP Segment
def tcpSegment(data):
    srcPort, destPort, seq, ack, offsetFlag = struct.unpack('! H H L L H', data[:14])
    offset = (offsetFlag >> 12)*4
    flagUrg = (offsetFlag & 32) >> 5
    flagAck = (offsetFlag & 16) >> 5
    flagPsh = (offsetFlag & 8) >> 5
    flagRst = (offsetFlag & 4) >> 5
    flagSyn = (offsetFlag & 2) >> 5
    flagFin = (offsetFlag & 1)
    return srcPort, destPort, seq, ack, flagUrg, flagAck, flagPsh, flagRst, flagSyn, flagFin, data[offset:]

# Unpack UDP Segment
def udpSegment(data):
    srcPort, destPort, size = struct.unpack('! H H 2x H', data[:8])
    return srcPort, destPort, size, data[8:]


sd = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.ntohs(3))

while True:
    rawData, addr = sd.recvfrom(65536)
    destMac, srcMac, ethProto, data = ethernetFrame(rawData)
    print("\n\n************************************************************************************")
    print("Ethernet Frame :")
    print("\nProtocol:", ethProto)
    print("Destination:",destMac)
    print("Source:",srcMac,"\n")

    if ethProto == 8:
        version, ttl, proto, src, target, ipv4Data = ipv4Packet(rawData)
        print("\t-IPv4 Packet :")
        print("\t>>Version :",version)
        print("\t>>TTL :",ttl)
        print("\t>>Protocol : ",proto)
        print("\t>>Source : ",src)
        print("\t>>Destination : ",target)

        if proto == 1:
            icmpType, code, checkSum, icmpData = icmpPacket(data)
            print("\n\n\t-ICMP Packet :")
            print("\t>>Type : ",icmpType)
            print("\t>>Code : ",code)
            print("\t>>CheckSum : ",checkSum)
            print("\t>>Data : ",icmpData)

        elif proto == 6:
            srcPort, destPort, seq, ack, flagUrg, flagAck, flagPsh, flagRst, flagSyn, flagFin, tcpData = tcpSegment(data)
            print("\n\n\t-TCP Segment : ")
            print("\t>>Source Port : ",srcPort)
            print("\t>>Destination Port : ",destPort)
            print("\t>>Sequence : ",seq)
            print("\t>>Acknowledgement : ",ack)
            print("\t>>Flags: ")
            print("\t>>URG : ",flagUrg,"\tACK : ",flagAck,"\tPSH : ",flagPsh,"\tRST : ",flagRst,"\tSYN : ",flagSyn,"\tFIN : ",flagFin)
            print("\t>>Data : ",tcpData)
        
        elif proto == 17:
            srcPort, destPort, length, data = udpSegment(data)
            print("\n\n\t-UDP Segment : ")
            print(">>Source Port : ",srcPort)
            print(">>Destination Port : ",destPort)
            print(">>Length : ",length)
            print(">>Data : ",data)

        else:
            print(">>Data : ",data)
    else:
        print(">>Data : ",data)
