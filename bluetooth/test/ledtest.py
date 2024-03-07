from gpiozero import LED
import time

RED_LIGHT = LED(17)
YELLOW_LIGHT = LED(27)
GREEN_LIGHT = LED(22)


while True:
    RED_LIGHT.on()
    time.sleep(2)
    RED_LIGHT.off()
    time.sleep(2)
        