from gpiozero import LED
import time
import datetime
import random

RED_LIGHT = LED(17)
YELLOW_LIGHT = LED(27)
GREEN_LIGHT = LED(22)

start_time = datetime.datetime.now()
print(start_time)

green_start_time = datetime.datetime.now()

car_count = 0

# 1 red
# 2 green

prev_signal = 'r'
curr_signal = 'r'
while True:
    # green light to red light
    if(prev_signal == 'g' and curr_signal == 'r'):
        GREEN_LIGHT.off()
        YELLOW_LIGHT.on()
        time.sleep(2)
        YELLOW_LIGHT.off()
        RED_LIGHT.on()

        curr_signal = 'r'
        prev_signal = 'r'
        start_time = datetime.datetime.now()

    # red light to green light
    elif(prev_signal == 'r' and curr_signal == 'g'):
        RED_LIGHT.off()
        GREEN_LIGHT.on()
        
        if((datetime.datetime.now() - green_start_time).seconds > 10):
            prev_signal = 'g'
            curr_signal = 'r'

    # constant red light
    elif(prev_signal == 'r' and curr_signal == 'r'):
        RED_LIGHT.on()
        if((datetime.datetime.now() - start_time).seconds == 3):
            start_time = datetime.datetime.now()
            car_count = random.randint(0, 10)
            print(car_count)

            if(car_count > 7):
                prev_signal = 'r'
                curr_signal = 'g'
                green_start_time = datetime.datetime.now()
        