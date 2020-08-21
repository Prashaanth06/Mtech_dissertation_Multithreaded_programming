import machine
 
# Create a virtual timer with period 500ms
tim = machine.Timer(-1)
 
led = machine.Pin(2, machine.Pin.OUT)
cligno = machine.Pin(15, machine.Pin.IN, machine.Pin.PULL_UP)
on = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_UP)
 
# State of the LED
led_state = False
def toggle_led(t) :
  global led_state
 
  led_state = not led_state # invert the state of the LED
  led.value(led_state)
 
# When the left button is pressed, activate the timer to make the LED blink
def led_cligno(irq) :
  tim.init(period=500, callback = toggle_led)
 
# When the right button is pressed, deactivate the timer and switch the LED on
def led_on(irq) :
  tim.deinit()
  led.on()
 
# Setup the interrupts on the push buttons
cligno.irq(trigger = machine.Pin.IRQ_FALLING, handler = led_cligno)
on.irq(trigger = machine.Pin.IRQ_FALLING, handler = led_on)
