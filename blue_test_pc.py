import serial

def echo_message():
    ser = serial.Serial('/dev/rfcomm1', 9600)
    # ser = serial.Serial('/dev/ttyS0', 9600)
    data = ''
    interm_data = ''
    
    print(ser.name)         # check which port was really used
    
    
    test_str = "hello im pi\n".encode('utf-8')
    bytes_sent = ser.write(test_str)
    print (f'Sended {test_str} ({bytes_sent} bytes)')
    
    while True:
        while ser.inWaiting() > 0:
            data = ser.readline().decode().strip()
            print(data)
    
    # ser.close()             # close port
    
            
if __name__ == "__main__":
    print(echo_message())