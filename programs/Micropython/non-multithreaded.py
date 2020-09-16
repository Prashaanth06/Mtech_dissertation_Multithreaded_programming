import machine, time, gc, micropython, esp, os
from machine import Pin,ADC
from time import sleep

"""N = 200000
t0 = time.ticks_us()"""
start = 0


def df():
    s = os.statvfs('//')
    return ('{0} MB'.format((s[0]*s[3])/1327936))

def free(full=False):
    F = gc.mem_free()
    print(gc.mem_free())
    A = gc.mem_alloc()
    print(gc.mem_alloc())
    T = F+A
    P = '{0:.2f}%'.format(F/T*100)
    if not full: return P
    else : return ('Total:{0} Free:{1} ({2})'.format(T,F,P))

print('disk free: ', df())
print('memory free: ', free())

gc.collect()
micropython.mem_info()
print('-----------------------------')

smoke = ADC(Pin(35))          # MQ-2 sensor
manual_button = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)   # Manual Press button
flame = machine.Pin(27,machine.Pin.IN)   # Flame sensor
led = machine.Pin(2,machine.Pin.OUT)      # LED
buzzer = machine.Pin(4,machine.Pin.OUT)    # Buzzer 
exhaust = machine.Pin(5,machine.Pin.OUT)    # Exhaust fan motor
water = ADC(Pin(34))                   # water level sensor
watopen = machine.Pin(12,machine.Pin.OUT)   # water opening

buzzer_value = buzzer.value()     # buzzer reading
led_state = led.value()           # LED reading the pin state
exhaust_fan = exhaust.value()    # reading exhaust fan
water_opening = watopen.value()   # reading water opening


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
    if manual_button.value() == 1 or smoke.read() >=700 and flame.value()  == 0:
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

        # Water opening
        watopen.value(1)

        print('Fire has occured stay careful get out of here')

    else:
        led.value(0)
        buzzer.value(0)
        exhaust.value(0)
        watopen.value(0)
    
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
    break


"""t1 = time.ticks_us()
dt =  time.ticks_diff(t1,t0)
fmt = '{:5.3f} sec, {:6.3f} sec : {:8.2f} hz'
print(fmt.format(dt * 1e-6, dt / N, N / dt * 1e3))"""


end = time.time()
total = end-start
print(total)


