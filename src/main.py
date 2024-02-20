import concurrent.futures

# Function to control the LED
from enum import Enum
class LED_Color(Enum):
    RED = 0
    GREEN = 1
    BLUE = 2
def LED_main():
    cur_LED = '0' // RED
    print(f'Current traffic light is {LED_Color(cur_LED)}')

# Function to send the number of counts

# Function to run inference for detection

def main():
    with concurrent.futures.ProcessPoolExecutor() as executor:
        exe.submit(cube,2)

if __name__ == '__main__':
    main()