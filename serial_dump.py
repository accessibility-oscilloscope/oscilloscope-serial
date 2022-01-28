#!/usr/bin/env python3

from serial import Serial
from struct import unpack
import time

INIT_SEQ: list[list[int]] = [
    [0, 20],  #Set board id to 0
    [135, 0, 100],  #serialdelaytimerwait of 100
    [122, 0, 10],  #number of samples 10
    [123, 0],  #send increment
    [124, 3],  #downsample 3
    [125, 1],  #tickstowait 1
    [136, 2, 32, 0, 0, 255, 200],  # io expanders on (!)
    [136, 2, 32, 1, 0, 255, 200],  # io expanders on (!)
    [136, 2, 33, 0, 0, 255, 200],  #, io expanders on (!)
    [136, 2, 33, 1, 0, 255, 200],  # io expanders on (!)
    [136, 2, 32, 18, 240, 255, 200],  # init
    [136, 2, 32, 19, 15, 255, 200],  # init (and turn on ADCs!)
    [136, 2, 33, 18, 0, 255, 200],  # init
    [136, 2, 33, 19, 0, 255, 200],  # init
    [131, 8, 0],  # adc offset
    [131, 6, 16],  #offset binary output
    # [131, 6, 80], #test pattern output
    [131, 4, 36],  #300 Ohm termination A
    [131, 5, 36],  #300 Ohm termination B
    [131, 1, 0],  #not multiplexed
    [136, 3, 96, 80, 136, 22, 0],  # channel 0 , board 0 calib
    [136, 3, 96, 82, 136, 22, 0],  # channel 1 , board 0 calib
    [136, 3, 96, 84, 136, 22, 0],  # channel 2 , board 0 calib
    [136, 3, 96, 86, 136, 22, 0],  # channel 3 , board 0 calib
]

if __name__ == "__main__":
    ser = Serial("/dev/ttyUSB1", 1500000, timeout=1.0)

    for command in INIT_SEQ:
        ser.write(bytearray(command))
        time.sleep(.1)  # pessimistic init sequence delay

    print("wrote init sequence")
    while True:
        ser.write(bytearray([100, 10]))  # arm trigger and get an event
        result = ser.read(40)
        p = unpack('%dB' % len(result), result)
        for i in range(0, 4):
            print(p[10 * i:10 * i + 10])  # print out the 4 channels * 10 bytes

    ser.close()
