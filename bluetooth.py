import serial

# Transmission Config
port = '/dev/ttyAMA0'
bps  = 57600

# Serial Configuration
blue = serial.Serial(port, baudrate=bps, timeout=1)
blue.close()
blue.open()
blue.flushInput()

print("Begin...")

while True:
    line = blue.readline()                # read bytes until line-ending
    line = line.decode(encoding='UTF-8')  # convert to string
    line = line.rstrip('\r\n')            # remove line-ending characters
    print(line)


