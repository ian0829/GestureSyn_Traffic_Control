from machine import Pin,UART
uart = UART(0, baudrate=9600, tx=Pin(8), rx=Pin(10))

while True:
    if uart.any():
        data = uart.readline()
        print(data)