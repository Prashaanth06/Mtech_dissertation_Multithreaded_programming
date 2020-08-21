#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

int freq = 1500;
int channel = 0;
int resolution = 7;
int dutyCycle = 128;

#define DHTPIN 32     // Temperature sensor 
#define fire 27     // Flame sensor
#define gas 13      // smoke sensor
#define watlvl 14     // water level sensor
#define manpres 15    // Manual_Press for emergency
#define LED 2        // led for indication
#define Buzzer 4     // Buzzer. 
#define Exhaust 5     // Exhaust fan.

#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(115200);
    unsigned long timeBegin = micros();
    

  pinMode(fire,INPUT);  
  pinMode(gas, INPUT);
  pinMode(watlvl, INPUT);
  pinMode(manpres,INPUT_PULLUP);
  pinMode(LED,OUTPUT);
  pinMode(Exhaust, OUTPUT);
  pinMode(Buzzer,OUTPUT);
  pinMode(DHTPIN,INPUT);

  // Initialize device.
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  // Print humidity sensor details.
  delayMS = sensor.min_delay / 1000;

  ledcSetup(channel, freq, resolution);
  ledcAttachPin(Buzzer, channel);

  unsigned long timeEnd = micros();
  unsigned long duration = timeEnd - timeBegin;
  double averageDuration = (double)duration / 1000.0;
  Serial.println(averageDuration);

}

void loop() {
  
  // put your main code here, to run repeatedly:
  delay(200);
  Serial.println("----------");
  Serial.println("----------");
  actuation();
  water_level();
  
  if(digitalRead(manpres) == 0)
  {
   temperature();
   flame();
   smoke();
  }
  else
  {
  manual_press();
  }
}

void temperature()
{

delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  
  if((event.temperature)>= 70)
  {
    Serial.println("Fire has occured stay careful get out of here");
  }
}

void flame()
{
   Serial.print("flame=");
   Serial.println(digitalRead(fire));
}

void smoke()
{
  Serial.print("smoke=");
   Serial.println(analogRead(gas));
   if((analogRead(gas)>=700) && (digitalRead(fire) == 0))
   {
    Serial.println("Fire has occured stay careful get out of here");
   }
}

void water_level()
{
  Serial.print("water level=");
   Serial.println(analogRead(watlvl));

   if(analogRead(watlvl)>= 2000)
   {
    Serial.println("Full tank water");
   }
   else if((analogRead(watlvl)<= 1600)&& (analogRead(watlvl)>= 550))
   {
    Serial.println("Water is in medium level");
   }
   else if((analogRead(watlvl)<= 500))
   {
    Serial.println("Water is low level turn on the motor");
   }
}

void manual_press()
{
    //actuation();
    Serial.println("open everything");
}

void actuation()
{
   if((digitalRead(manpres) == 1) || (analogRead(gas)>=700) && (digitalRead(fire)==0))
                {
                  /* LED indication */
                digitalWrite(LED,HIGH);
                delay(100);
                digitalWrite(LED,LOW);
                delay(100);

                /* Exhaust fan */
                digitalWrite(Exhaust,HIGH);

                /* Alarm indication*/
                ledcWrite(channel, dutyCycle);
                vTaskDelay(100);
                ledcWrite(channel, 0);
                vTaskDelay(100);

                  /*Water opening*/
                }
                else
                {
                digitalWrite(Exhaust,LOW);
                ledcWrite(channel, 0);
                digitalWrite(LED,LOW);
                }
    
}


               
  
