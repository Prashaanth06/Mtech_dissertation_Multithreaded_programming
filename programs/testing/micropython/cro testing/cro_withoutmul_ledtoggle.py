import machine
from machine import Pin
import time
from time import sleep

led = Pin(2,Pin.OUT)
manual_button = machine.Pin(15, machine.Pin.IN)   # Manual Press button

while True:
    if manual_button.value() == 1:
        led.value(1)
        #sleep(0.1)
    else:
        led.value(0)
        #sleep(0.3)