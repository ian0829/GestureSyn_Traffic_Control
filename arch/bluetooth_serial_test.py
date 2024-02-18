#! /usr/bin/python
import serial
from time import sleep
bluetoothSerial = serial.Serial( "/dev/rfcomm0", baudrate=9600 )

#while True:
#	bluetoothSerial.write("\r\nSay something: ")
#	rcv = port.read(10)
#	bluetoothSerial.write("\r\n ECHO: " + repr(rcv))

count = None
while count == None:
    try:
        count = int(input( "Please enter the number of times to blink the LED: "))
    except:
        pass    # Ignore any errors that may occur and try again


bluetoothSerial.write(b'hello')
print(count)
print(bluetoothSerial.readline())


#https://www.uugear.com/portfolio/bluetooth-communication-between-raspberry-pi-and-arduino/
"""
sudo apt-get update
sudo apt-get install bluetooth bluez-utils blueman

hciconfig

hcitool scan

sudo bluez-simple-agent hci# xx:xx:xx:xx:xx:xx



sudo nano /etc/bluetooth/rfcomm.conf

Append to file:
rfcomm1 {
    bind yes;
    device xx:xx:xx:xx:xx:xx;
    channel 1;
    comment "Connection to Bluetooth serial module";
}

sudo rfcomm bind all


sudo apt-get install python-serial
python bluetooth_serial_test.py
"""
