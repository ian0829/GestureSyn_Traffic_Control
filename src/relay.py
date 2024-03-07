import RPi.GPIO as GPIO
import time

# Set the GPIO pin numbers
PIN_RELAY_1 = 17 # GPIO12
PIN_RELAY_2 = 27 # GPIO16
PIN_RELAY_3 = 22 # GPIO20
PIN_RELAY_4 = 25 # GPIO25

# Set the GPIO mode to BCM
GPIO.setmode(GPIO.BCM)

# Setup the GPIO pins as outputs
GPIO.setup(PIN_RELAY_1, GPIO.OUT)
GPIO.setup(PIN_RELAY_2, GPIO.OUT)
GPIO.setup(PIN_RELAY_3, GPIO.OUT)
GPIO.setup(PIN_RELAY_4, GPIO.OUT)


try:
    while True:
        print("Turn on all 4 relays")
        GPIO.output(PIN_RELAY_1, GPIO.HIGH)
        # GPIO.output(PIN_RELAY_2, GPIO.HIGH)
        # GPIO.output(PIN_RELAY_3, GPIO.HIGH)
        # GPIO.output(PIN_RELAY_4, GPIO.HIGH)
        time.sleep(1)

        print("Turn off all 4 relays")
        GPIO.output(PIN_RELAY_1, GPIO.LOW)
        # GPIO.output(PIN_RELAY_2, GPIO.LOW)
        # GPIO.output(PIN_RELAY_3, GPIO.LOW)
        # GPIO.output(PIN_RELAY_4, GPIO.LOW)
        time.sleep(1)
        
except KeyboardInterrupt:
    # Cleanup GPIO on keyboard interrupt
    GPIO.cleanup()