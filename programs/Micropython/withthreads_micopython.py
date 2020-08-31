import _thread
import machine, time, gc, micropython
from machine import Pin,ADC
from time import sleep

"""N = 200000
t0 = time.ticks_us()"""
start = 0

gc.collect()
micropython.mem_info()
print('-----------------------------')


led = machine.Pin(2,machine.Pin.OUT)      # LED
buzzer = machine.Pin(4,machine.Pin.OUT)    # Buzzer 
exhaust = machine.Pin(5,machine.Pin.OUT)    # Exhaust fan motor
manual_button = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)   # Manual Press button
smoke = ADC(Pin(35))                      # MQ-2 sensor
flame = machine.Pin(27,machine.Pin.IN)      # Flame sensor
watopen = machine.Pin(12,machine.Pin.OUT)   # water opening

mutex = _thread.allocate_lock()

buzzer_value = buzzer.value()     # buzzer reading
led_state = led.value()           # LED reading the pin state
exhaust_fan = exhaust.value()    # reading exhaust fan
water_opening = watopen.value()   # reading water opening


def water_levelThread():                     # function for water level sensor
    sleep(0.2)
    print('')
    water = ADC(Pin(34))                   # water level sensor
    while True:
        mutex.acquire()
        water_sensor = water.read()       # reading water level
        print('Water level=%d' %water_sensor)
        if water_sensor >= 2000:
            print('\nFull tank water')
        elif water_sensor<= 1600 and water_sensor >= 550:
            print('\nWater is in medium level')
        elif water_sensor <= 500:
            print('\nWater is low level turn on the motor')
        mutex.release()


def smoke_sensThread():                    # function for smoke sensor
    sleep(0.2)
    print('')
    smoke_sensor = smoke.read()       # reading MQ2 sensor
    while True:
        mutex.acquire()
        if manual_button.value() == 0:
            print('Smoke sensor value=%d ' % smoke_sensor)
        mutex.release()


def manual_pressThread():                  # function for manual press
    sleep(0.2)
    print('')
    #man_press = manual_button.value()  # reading a manual interrupt
    while True:
        mutex.acquire()
        if manual_button.value() == 1:
            #print('Manual button state=%d ' % man_press)   # Printing flame sensor value
            print('\nActuate things hardware interrupt is given\n')
        mutex.release()


def flame_sensorThread():                  # function for flame sensor
    sleep(0.2)
    print('')
    while True:
        mutex.acquire()
        if manual_button.value() == 0:
            flame_value = flame.value()       # reading flame sensor
            print('Flame sensor value=%d ' % flame_value)   # Printing flame sensor value
        mutex.release()


def actuationThread():                     # function for actuation unit
    sleep(0.2)
    print('')
    while True:
        mutex.acquire()
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

            # Water opening
            watopen.value(1)

            print('\nFire has occured stay careful get out of here')
            print('')

        else:
            led.value(0)
            buzzer.value(0)
            exhaust.value(0)
            watopen.value(0)
        mutex.release()


_thread.start_new_thread(water_levelThread, ())
_thread.start_new_thread(smoke_sensThread, ())
_thread.start_new_thread(manual_pressThread, ())
_thread.start_new_thread(flame_sensorThread, ())
_thread.start_new_thread(actuationThread, ())


"""t1 = time.ticks_us()
dt =  time.ticks_diff(t1,t0)
fmt = '{:5.3f} sec, {:6.3f} usec/blink : {:8.2f} kblinks/sec'
print(fmt.format(dt * 1e-6, dt / N, N / dt * 1e3))"""
end = time.time()
total = end-start
#print('execution time=')
print(total)


