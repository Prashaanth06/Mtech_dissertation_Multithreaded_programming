import machine
from machine import Pin
import _thread
import time
from time import sleep

#mutex = _thread.allocate_lock()

def toggleThread():
    sleep(0.1)
    led=Pin(2,Pin.OUT)
    manual_button = machine.Pin(15, machine.Pin.IN)   # Manual Press button
    while True:
        #mutex.acquire()
        if manual_button.value() == 1:
            led.value(1)
        else:
            led.value(0)
        #mutex.release()


_thread.start_new_thread(toggleThread, ())