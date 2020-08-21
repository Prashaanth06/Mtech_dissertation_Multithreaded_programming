import machine
from machine import Pin,ADC
import esp32
from time import sleep
import dht

sensor = dht.DHT11(Pin(25))   # temperature sensor
smoke = ADC(Pin(35))          # MQ-2 sensor
manual_button = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)   # Manual Press button
flame = machine.Pin(27,machine.Pin.IN)   # Flame sensor
water = ADC(Pin(34))                   # water level sensor

water_sensor = water.read()       # reading water level
smoke_sensor = smoke.read()       # reading MQ2 sensor
flame_value = flame.value()       # reading flame sensor
man_press = manual_button.value()  # reading a manual interrupt


def water_level():
    sleep(0.2)
    print('Water level=%d' %water_sensor)
    if water_sensor >= 2000:
        print('Full tank water')
    elif water_sensor<= 1600 and water_sensor >= 550:
        print('Water is in medium level')
    elif water_sensor <= 500:
        print('Water is low level turn on the motor')


def smoke_sens():
    if man_press == 0:
        sleep(0.2)
        print('Smoke sensor value=%d ' % smoke_sensor)

def flame_sensor():
    if man_press == 0:
        sleep(0.2)
        print('Flame sensor value=%d ' % flame_value)   # Printing flame sensor value

def manual_press(): 
        sleep(0.2)
        print('Manual Press button state=%d ' % man_press)   # Printing flame sensor value
        print('Actuate things')

def temp_sensor():   
    if man_press == 0: 
        sleep(0.3)
        sensor.measure()
        temp = sensor.temperature()
        print('Temperature: %3.1f C' %temp)     # printing temperature value  
        
    
while True:
        manual_press()
        flame_sensor()
        temp_sensor()
        smoke_sens()
        water_level()
        print('')
    