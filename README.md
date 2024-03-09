# rpi4-vehicle-det

## Table of Contents
- [Setup Bluetooth Connections](#setup-bluetooth-connection-with-the-hc-06-bluetooth-module-on-stm32)
- [Install Dependencies](#install-necessary-dependences-before-starting-the-car-detection-program)
- [Run the Program](#running-the-car-detection-program)

## Setup Bluetooth Connection with the HC-06 Bluetooth Module on STM32

In Raspberry Pi terminal, open the bluetooth control setting with the following command to pair the HC-06 bluetooth module with Raspberry Pi.

```
bluetoothctl
```
After opening the bluetooth control setting, use the following command to look for the HC-06 module from the Raspberry Pi side.

```
agent on
discoverable on
scan on
```

Find the bluetooth address of the HC-06 module and pair it with the following command (In this case, 00:21:06:BE:80:4D is our HC-06 bluetooth address, which should be replaced):
```
pair 00:21:06:BE:80:4D
trust 00:21:06:BE:80:4D
```
Exit bluetoothctl.
```
exit
```
Connect the Raspberry Pi with the HC-06 module (replace the bluetooth address):
```
sudo rfcomm connect 1 00:21:06:BE:80:4D
```

## Install necessary dependences before starting the car detection program

Install depenencies.
```
sudo apt update
sudo apt install build-essential
sudo apt install libatlas-base-dev
sudo apt install python3-pip
pip3 install tflite-runtime
pip3 install opencv-python==4.4.0.46
pip3 install pillow
pip3 install numpy
```

## Running the car detection program

Go into the source code directory:
```
cd rpi4/src
```
Option 1: Run the car detection program (with video stream displayed)
```
python3 inference_working.py --model ./pretrained/efficientdet_lite3.tflite --label coco_labels.txt
```
Option 2: Run the car detection program (with video stream off)
```
python3 inference.py --model ./pretrained/efficientdet_lite3.tflite --label coco_labels.txt
```