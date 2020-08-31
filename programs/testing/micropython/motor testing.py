import time
from machine import Pin


exhaust = machine.Pin(2,machine.Pin.OUT)    # Exhaust fan motor

exhaust.value(1)
