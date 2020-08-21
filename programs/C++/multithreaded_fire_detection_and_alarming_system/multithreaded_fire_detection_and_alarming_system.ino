#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

int freq = 1500;
int channel = 0;
int resolution = 7;
int dutyCycle = 128;

#define DHTPIN 25     // Temperature sensor 
#define fire 27     // Flame sensor
#define gas 13      // smoke sensor
#define watlvl 14     // water level sensor
#define manpres 15    // Manual_Press for emergency
#define LED 2        // led for indication
#define Buzzer 4     // Buzzer  
#define Exhaust 5     // Exhaust fan.

#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

TaskHandle_t xflameTaskHandle = NULL;
TaskHandle_t xmanpressTaskHandle = NULL;
TaskHandle_t xsmokeTaskHandle = NULL;
TaskHandle_t xtempTaskHandle = NULL;
TaskHandle_t xactuationTaskHandle = NULL;


SemaphoreHandle_t xflameStartSemaphore;
SemaphoreHandle_t xmanpressSemaphore;
SemaphoreHandle_t xsmokeSemaphore;
SemaphoreHandle_t xtempSemaphore;
SemaphoreHandle_t xactuationSemaphore;

void setup() {
        Serial.begin(115200);   // Serial monitor setting up baud rate
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
      
      xTaskCreate(flame_watlvl,   /* Function to implement the task */
                    "flame sensor readings", /* Name of the task */
                    1000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    9,          /* Priority of the task */
                    &xflameTaskHandle);     /* Task handle. */
       
      xTaskCreate(manual_press,   /* Function to implement the task */
                    "manual_press", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      9,          /* Priority of the task */
                     &xmanpressTaskHandle);     /* Task handle. */

       xTaskCreate(smoke,   /* Function to implement the task */
                    "Smoke sensor reading", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      8,          /* Priority of the task */
                     &xsmokeTaskHandle);     /* Task handle. */

       xTaskCreate(temperature,   /* Function to implement the task */
                    "temperature sensor reading", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      7,          /* Priority of the task */
                     &xtempTaskHandle);     /* Task handle. */

      xTaskCreate(actuation,   /* Function to implement the task */
                    "Actuation task", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      10,          /* Priority of the task */
                     &xactuationTaskHandle);     /* Task handle. */
                     

  unsigned long timeEnd = micros();
  unsigned long duration = timeEnd - timeBegin;
  double averageDuration = (double)duration / 1000.0;
  Serial.println(averageDuration);
}


void loop() 
{

}


void flame_watlvl(void * pvParameters)
{
  xflameStartSemaphore = xSemaphoreCreateBinary();
  while(1)
  {
    if (xSemaphoreTake(xflameStartSemaphore, 0) == pdTRUE)
    {
    if(digitalRead(manpres) == 0)
    {
        Serial.println("Flame sensor task is taken");
     Serial.print("flame=");
   Serial.println(digitalRead(fire));
        if(digitalRead(fire)==0)
         {
          Serial.println("Fire has occured stay careful get out of here");
         }
    }
  }
    
     Serial.print("water level=");
   Serial.println(analogRead(watlvl));
   Serial.println("water level checking task");

   if(analogRead(watlvl)>= 2000)
   {
    Serial.println("Full tank water");
   }
   else if((analogRead(watlvl)<= 1600)&& (analogRead(watlvl)== 550))
   {
    Serial.println("Medium water level");
   }
   else if((analogRead(watlvl)<= 500))
   {
    Serial.println("Water level is low, turn on the motor");
   }
   Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(100);
   xSemaphoreGive( xflameStartSemaphore ) == pdTRUE; 
  }
}

void manual_press(void * pvParameters)
{
 xmanpressSemaphore = xSemaphoreCreateBinary();
    while(1)
    {
      if (xSemaphoreTake(xmanpressSemaphore, 0) == pdTRUE)
    {
    if(digitalRead(manpres) == 1)
    {
      Serial.println("Manual Press task given");
    Serial.println("open everything");
    }
    }
    Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(100);
  xSemaphoreGive(xmanpressSemaphore) == pdTRUE;
}
}

void smoke(void * pvParameters)
{
 xsmokeSemaphore = xSemaphoreCreateBinary();
    while(1)
    {
      if (xSemaphoreTake(xsmokeSemaphore, 0) == pdTRUE)
    {
    if(digitalRead(manpres) == 0)
    {
      Serial.println("smoke sensor task is taken");
      Serial.print("smoke=");
      Serial.println(analogRead(gas));
      if((analogRead(gas)>=700) && (digitalRead(fire)==1))
      {
        Serial.println("Fire has occured stay careful get out of here");
      }
    }
    }
    Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(100);
  xSemaphoreGive(xsmokeSemaphore) == pdTRUE;
}
}

void temperature(void * pvParameters)
{
 xtempSemaphore = xSemaphoreCreateBinary();
    while(1)
    {
   if (xSemaphoreTake(xtempSemaphore, 0) == pdTRUE)
    {
    if(digitalRead(manpres) == 0)
    {
      Serial.println("temperature sensor task has taken");
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

  if((event.temperature)>= 80)
  {
    Serial.println("Fire has occured stay careful get out of here");
  }
    }
    }
    Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(100);
  xSemaphoreGive(xtempSemaphore) == pdTRUE;
}
}

void actuation(void * pvParameters)
{
  xactuationSemaphore = xSemaphoreCreateBinary();
    while(1)
    {
   if (xSemaphoreTake(xactuationSemaphore, 0) == pdTRUE)
    {
      if((digitalRead(manpres) == 1) || (analogRead(gas)>=700) && (digitalRead(fire)==0))
                {
                  Serial.println("Actuation task started");
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
                }
                else
                {
                digitalWrite(Exhaust,LOW);
                ledcWrite(channel, 0);
                digitalWrite(LED,LOW);
                }
              
    }
  vTaskDelay(100);
  xSemaphoreGive(xactuationSemaphore) == pdTRUE;
    }
  
}