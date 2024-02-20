from gpiozero import LED
import time
import datetime
import random

RED_LIGHT = LED(17)
YELLOW_LIGHT = LED(27)
GREEN_LIGHT = LED(22)

start_time = datetime.datetime.now()
print(start_time)

car_count = 0

# 1 red
# 2 green

curr_state = 1
prev_state = 1 

while True:
    # green light to red light
    if(prev_state == 2 and curr_state == 1):
        GREEN_LIGHT.off()
        YELLOW_LIGHT.on()
        time.sleep(2)
        YELLOW_LIGHT.off()
        RED_LIGHT.on()
        prev_state == 1
        curr_state == 1
        start_time = datetime.datetime.now()

    # red light to green light
    elif(curr_state == 2):
        RED_LIGHT.off()
        GREEN_LIGHT.on()
        prev_state = 2
        curr_state = 2

    # constant red light
    elif(prev_state == 1 and curr_state == 1):
        RED_LIGHT.on()
        if((datetime.datetime.now() - start_time).seconds == 3):
            start_time = datetime.datetime.now()
            car_count = random.randint(0, 10)
            print(car_count)
            
    curr_state = int(input())

        