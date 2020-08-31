import machine, time, gc, micropython, esp
from machine import Pin,ADC
from time import sleep


flame = machine.Pin(27,machine.Pin.IN)   # Flame sensor
led=Pin(2,Pin.OUT)

while True:
        #mutex.acquire()
        if flame.value() == 0:
            led.value(1)
        else:
            led.value(0)