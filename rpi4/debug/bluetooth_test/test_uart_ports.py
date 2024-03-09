#!/usr/bin/env python
# -*- coding: utf-8 -*-
import serial
test_string = "6".encode('utf-8')
port_list = ["/dev/ttyS0","/dev/ttyS0"]
for port in port_list:
  try:
    serialPort = serial.Serial(port, 9600, timeout = 2)
    print ("Serial port", port, " ready for test :")
    bytes_sent = serialPort.write(test_string)
    print ("Sended", bytes_sent, "byte")
    loopback = serialPort.read(bytes_sent)
    print(loopback)
    if loopback == test_string:
      print ("Received ",len(loopback), "bytes. Port", port,"is OK ! \n")
    else:
      print ("Received incorrect data:", loopback, "on serial part", port, "loopback \n")
    serialPort.close()
  except IOError:
    print ("Error on", port,"\n")