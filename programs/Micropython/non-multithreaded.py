import machine
from machine import Pin,ADC
from time import sleep

smoke = ADC(Pin(35))          # MQ-2 sensor
manual_button = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)   # Manual Press button
flame = machine.Pin(27,machine.Pin.IN)   # Flame sensor
led = machine.Pin(2,machine.Pin.OUT)      # LED
buzzer = machine.Pin(4,machine.Pin.OUT)    # Buzzer 
exhaust = machine.Pin(5,machine.Pin.OUT)    # Exhaust fan motor
water = ADC(Pin(34))                   # water level sensor

buzzer_value = buzzer.value()     # buzzer reading
led_state = led.value()           # LED reading the pin state
exhaust_fan = exhaust.value()    # reading exhaust fan


def water_level():                     # function for water level sensor
    sleep(0.1)
    water_sensor = water.read()       # reading water level
    print('Water level=%d' %water_sensor)
    if water_sensor >= 2000:
        print('Full tank water')
    elif water_sensor<= 1600 and water_sensor >= 550:
        print('Water is in medium level')
    elif water_sensor <= 500:
        print('Water is low level turn on the motor')


def smoke_sens():                    # function for smoke sensor
        sleep(0.1)
        smoke_sensor = smoke.read()       # reading MQ2 sensor
        print('Smoke sensor value=%d ' % smoke_sensor)

def flame_sensor():                  # function for flame sensor
        sleep(0.1)
        flame_value = flame.value()       # reading flame sensor
        print('Flame sensor value=%d ' % flame_value)   # Printing flame sensor value


def manual_press():                  # function for manual press
        man_press = manual_button.value()  # reading a manual interrupt
        print('Manual button state=%d ' % man_press)   # Printing flame sensor value
        print('Actuate things hardware interrupt is given')


def actuation():                     # function for actuation unit
        if manual_button.value() == 1 or smoke.read() >=1400 and flame.value()  == 0:
            # LED on indication
            led.value(1)
            sleep(0.1)
            led.value(0)
            sleep(0.1)

            # Buzzer indication
            buzzer.value(1) 
            sleep(0.1)
            buzzer.value(0)
            sleep(0.1)

            # Exhaust fan
            exhaust.value(1)

            print('Fire has occured stay careful get out of here')

        else:
            led.value(0)
            buzzer.value(0)
            exhaust.value(0)
    
while True:
    actuation()
    water_level()

    if manual_button.value() == 0: 
        flame_sensor()
        smoke_sens()
        print('')
    else:
        manual_press()
        print('')
