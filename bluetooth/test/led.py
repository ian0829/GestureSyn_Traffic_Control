from gpiozero import LED

RED_LIGHT = LED(17)
GREEN_LIGHT = LED(27)

stop = 0
while(stop != 1):
    signal = input('Which light? ')
    if(signal == 'r'):
        RED_LIGHT.on()
        GREEN_LIGHT.off()
    elif(signal == 'g'):
        RED_LIGHT.off()
        GREEN_LIGHT.on()
    else:
        RED_LIGHT.off()
        GREEN_LIGHT.off()
        stop = 1