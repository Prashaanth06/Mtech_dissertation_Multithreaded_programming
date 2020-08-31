import machine, time, gc, micropython, esp
from machine import Pin,ADC
from time import sleep
 
 
led = machine.Pin(2, machine.Pin.OUT)
man = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)
 
while True:
      if man == 1:
        led.value(1)
      else:
                
