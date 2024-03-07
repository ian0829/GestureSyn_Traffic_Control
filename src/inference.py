# Model Inference for 2D Object Detection
# Author: Howard Her and Christine Wu
#
# Picamera2 on Raspberry Pi OS Bullseye
# Pi Camera Module V2
#
# python3 real_time_with_labels.py --model mobilenet_v2.tflite --label coco_labels.txt
# python3 real_time_with_labels.py --model ../src/pretrained/efficientdet_lite3.tflite --label coco_labels.txt
# python3 inference.py --model ./pretrained/efficientdet_lite3.tflite --label coco_labels.txt
# sudo rfcomm connect 1 00:21:06:BE:80:4D

import cv2
import time
import datetime
import serial
import argparse
import numpy as np
import tflite_runtime.interpreter as tflite
from picamera2 import MappedArray, Picamera2, Preview
from multiprocessing import Process

normal_size = (640, 480)
lowres_size = (320, 240)
rectangles = []

### MAIN CONTROLLER OF LED LIGHTS ###
from gpiozero import LED

RED_LIGHT = LED(17)
YELLOW_LIGHT = LED(27)
GREEN_LIGHT = LED(22)

def led_main(ser): # 1 - red # 2 - green
    print(f'\nStarting LED signal module....')

    signal_in = '2'
    signal = 2
    
    while True:
        # LISTEN for signal from STM
        if ser.inWaiting() > 0:
            signal_in = ser.readline().decode().strip('\0')
            print(f'\nLED: {signal}\n')
            # if signal_in != signal:
            #     signal = signal_in
            signal = int(signal_in)
        
        # UPDATE the LED lights
        if(signal == 1):
            GREEN_LIGHT.off()
            YELLOW_LIGHT.on()
            time.sleep(2)
            YELLOW_LIGHT.off()
            RED_LIGHT.on()
            signal = 3
        elif(signal == 2):
        # else:
            # print(f'\nLED in Green\n')
            RED_LIGHT.off()
            GREEN_LIGHT.on()
            
        # else:
        #     signal = '1'
        #     RED_LIGHT.off()
        #     GREEN_LIGHT.off()  
                # print(f'\nLED: {signal}\n')
        
  
        


### RELAY CAR count to STM module ###
def sent_car_count(car_count, ser):  
    msg_str = str(car_count)
    bytes_sent = ser.write( msg_str.encode('utf-8') )
    print(f'{car_count} : {bytes_sent}')

    # print (f'Sended {bytes_sent} bytes')
    

def read_labels(file_path):
    with open(file_path, 'r') as f:
        lines = f.readlines()
    labels = {}
    for line in lines:
        pair = line.strip().split(maxsplit=1)
        labels[int(pair[0])] = pair[1].strip()
    return labels


def DrawRectangles(request):
    with MappedArray(request, "main") as m:
        for rect in rectangles:
            rect_start = (int(rect[0] * 2) - 5, int(rect[1] * 2) - 5)
            rect_end = (int(rect[2] * 2) + 5, int(rect[3] * 2) + 5)
            cv2.rectangle(m.array, rect_start, rect_end, (0, 255, 0, 0))
            if len(rect) == 5:
                text = rect[4]
                font = cv2.FONT_HERSHEY_SIMPLEX
                cv2.putText(m.array, text, (int(rect[0] * 2) + 10, int(rect[1] * 2) + 10),
                            font, 1, (255, 255, 255), 2, cv2.LINE_AA)


def inference_model(image, model, output, label=None):
    global rectangles

    if label:
        labels = read_labels(label)
    else:
        labels = None

    interpreter = tflite.Interpreter(model_path=model, num_threads=4)
    interpreter.allocate_tensors()

    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]
    floating_model = False
    if input_details[0]['dtype'] == np.float32:
        floating_model = True

    rgb = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
    initial_h, initial_w, channels = rgb.shape

    picture = cv2.resize(rgb, (width, height))

    input_data = np.expand_dims(picture, axis=0)
    if floating_model:
        input_data = (np.float32(input_data) - 127.5) / 127.5

    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()

    detected_boxes = interpreter.get_tensor(output_details[0]['index'])
    detected_classes = interpreter.get_tensor(output_details[1]['index'])
    detected_scores = interpreter.get_tensor(output_details[2]['index'])
    num_boxes = interpreter.get_tensor(output_details[3]['index'])

    rectangles = []
    num_det_car = 0
    for i in range(int(num_boxes)):
        top, left, bottom, right = detected_boxes[0][i]
        classId = int(detected_classes[0][i])
        score = detected_scores[0][i]
        if score > 0.3:
            if labels[classId] == 'car':
                num_det_car += 1
            xmin = left * initial_w
            ymin = bottom * initial_h
            xmax = right * initial_w
            ymax = top * initial_h
            box = [xmin, ymin, xmax, ymax]
            rectangles.append(box)
            if labels:
                rectangles[-1].append(labels[classId])
    return num_det_car

def det_main(ser):
    print(f'Starting detection module....')
    
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', help='Path to pre-trained detection model', required=True)
    parser.add_argument('--label', help='Path to labels')
    parser.add_argument('--output', help='Path to save output')
    args = parser.parse_args()

    if (args.output):
        output_file = args.output
    else:
        output_file = 'out.jpg'

    if (args.label):
        label_file = args.label
    else:
        label_file = None

    picam2 = Picamera2()
    # picam2.start_preview(Preview.QTGL)
    picam2.start_preview(Preview.NULL)
    config = picam2.create_preview_configuration(main={"size": normal_size},
                                                 lores={"size": lowres_size, "format": "YUV420"})
    picam2.configure(config)

    stride = picam2.stream_configuration("lores")["stride"]
    # picam2.post_callback = DrawRectangles

    picam2.start()
    
    start_time = datetime.datetime.now()
    while True:
        buffer = picam2.capture_buffer("lores")
        grey = buffer[:stride * lowres_size[1]].reshape((lowres_size[1], stride))
        

        # print (f'\nCAR COUNT :    {num_det_car}')
        
        # SEND the number of car count every three seconds
        if (datetime.datetime.now() - start_time).seconds == 3 :
            # GET the current number of cars
            num_det_car = inference_model(grey, args.model, output_file, label_file)
            start_time = datetime.datetime.now()
            sent_car_count(num_det_car, ser)    

def main():
    ### ESTABLISH bluetooth serial port connection ###
    ser = serial.Serial('/dev/rfcomm1', 9600) 
    # ser = ''
    
    ### START Processes ###
    p1 = Process(target=led_main, args=(ser,))
    p1.start()
    p2 = Process(target=det_main, args=(ser,))
    p2.start()
    p1.join()
    p2.join()


if __name__ == '__main__':
    main()

    # det_main()
    # led_main()
