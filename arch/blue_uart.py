import machine
import utime

uart = machine.UART(0, baudrate=9600, tx=machine.Pin(8), rx=machine.Pin(10))