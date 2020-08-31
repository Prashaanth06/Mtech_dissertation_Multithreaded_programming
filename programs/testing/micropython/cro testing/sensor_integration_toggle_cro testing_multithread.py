import machine, time, _thread
from machine import Pin
from time import sleep

def toggleThread():
    led=Pin(2,Pin.OUT)
    flame = machine.Pin(27,machine.Pin.IN)   # Flame sensor
    while True:
        if flame.value() == 0:
            led.value(1)
        else:
            led.value(0)


_thread.start_new_thread(toggleThread, ())