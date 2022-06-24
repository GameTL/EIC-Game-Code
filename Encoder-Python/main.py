
import serial
import time
import random

arduino = serial.Serial(port='/dev/cu.usbserial-1420',
                        baudrate=115200, timeout=.001)


""" def write_read():
    arduino.write(bytes(x, 'utf-8'))
    # time.sleep(0.05)
    data = arduino.readline()
    return data """


def GernerateVelocity():
    return random.randint(-100, 100)


while True:
    arduino.write(bytes(str(GernerateVelocity()) + "\n", 'ascii'))
    # x = arduino.readline().decode("utf-8")
    """ if x != "":

        print(x.strip() + "π rad/s") """
    time.sleep(0.2)
    # ""
