static int taskCore = 1;
static int lessCore = 0;

#define fire 27     // Flame sensor
#define gas 13      // smoke sensor
#define watlvl 14     // water level sensor
#define manpres 15    // Manual_Press for emergency
#define LED 2        // led for indication
#define Buzzer 4     // Buzzer  
#define Exhaust 5     // Exhaust fan.
#define watopen 12     // water opening

int freq = 1500;
int channel = 0;
int resolution = 7;
int dutyCycle = 128;

unsigned long start,stoppy;

TaskHandle_t xflameTaskHandle = NULL;
TaskHandle_t xmanpressTaskHandle = NULL;
TaskHandle_t xsmokeTaskHandle = NULL;
TaskHandle_t xactuationTaskHandle = NULL;

SemaphoreHandle_t xflameStartSemaphore;
SemaphoreHandle_t xmanpressSemaphore;
SemaphoreHandle_t xsmokeSemaphore;
SemaphoreHandle_t xactuationSemaphore;

void setup() {
        Serial.begin(115200);   // Serial monitor setting up baud rate
  
   pinMode(fire,INPUT);  
   pinMode(gas, INPUT);
   pinMode(watlvl, INPUT);
   pinMode(manpres,INPUT_PULLUP);
   pinMode(LED,OUTPUT);
   pinMode(Exhaust, OUTPUT);
   pinMode(Buzzer,OUTPUT);
   pinMode(watopen,OUTPUT);
   
   ledcSetup(channel, freq, resolution);
  ledcAttachPin(Buzzer, channel);
      
      xTaskCreatePinnedToCore(flame_watlvl,   /* Function to implement the task */
                    "flame sensor readings", /* Name of the task */
                    2000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    9,          /* Priority of the task */
                    &xflameTaskHandle,     /* Task handle. */
                    lessCore);                    /* Task Core*/
       
      xTaskCreatePinnedToCore(manual_press,   /* Function to implement the task */
                    "manual_press", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      9,          /* Priority of the task */
                     &xmanpressTaskHandle,    /* Task handle. */
                     taskCore);                /* Task Core*/

       xTaskCreatePinnedToCore(smoke,   /* Function to implement the task */
                    "Smoke sensor reading", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      8,          /* Priority of the task */
                     &xsmokeTaskHandle,     /* Task handle. */
                     lessCore);                   /* Task Core*/

      xTaskCreatePinnedToCore(actuation,   /* Function to implement the task */
                    "Actuation task", /* Name of the task */
                     2000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      10,          /* Priority of the task */
                     &xactuationTaskHandle,     /* Task handle. */
                     taskCore);                        /* Task Core*/

  
  /*Serial.print("Allocated heap=");
  Serial.println(ESP.getMaxAllocHeap());*/
                       
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
      vTaskPrioritySet(xflameStartSemaphore,9);
        //start = xTaskGetTickCount();
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
    /*stoppy= xTaskGetTickCount()- start;
    Serial.print("flame task=");
  Serial.println(stoppy);*/
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
            vTaskPrioritySet(xmanpressSemaphore,9);
     // start = xTaskGetTickCount();
    if(digitalRead(manpres) == 1)
    {
      Serial.println("Manual Press task given");
    Serial.println("open everything");
    }
    }
    Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(50);
    /* stoppy= xTaskGetTickCount()- start;
     Serial.print("Manual task=");
  Serial.println(stoppy);*/
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
     vTaskPrioritySet(xsmokeSemaphore,8);
      //start = xTaskGetTickCount();
    if(digitalRead(manpres) == 0)
    {
      Serial.println("smoke sensor task is taken");
      Serial.print("smoke=");
      Serial.println(analogRead(gas));
    }
    }
    Serial.println("----------");
  Serial.println("----------");
    vTaskDelay(100);
     /*stoppy= xTaskGetTickCount()- start;
     Serial.print("smoke task=");
  Serial.println(stoppy);*/
  xSemaphoreGive(xsmokeSemaphore) == pdTRUE;
}
}


void actuation(void * pvParameters)
{
  xactuationSemaphore = xSemaphoreCreateBinary();
    while(1)
    {
   if (xSemaphoreTake(xactuationSemaphore, 0) == pdTRUE)
    {
           vTaskPrioritySet(xactuationSemaphore,10);
       //start = xTaskGetTickCount();
      if((digitalRead(manpres) == 1) || (analogRead(gas)>=100) && (digitalRead(fire)==0))
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

                /*Water opening*/
                digitalWrite(watopen,HIGH);
                
                }
                else
                {
                digitalWrite(Exhaust,LOW);
                ledcWrite(channel, 0);
                digitalWrite(LED,LOW);
                digitalWrite(watopen,LOW);
                }
              
    }
  vTaskDelay(100);
  /*stoppy= xTaskGetTickCount()- start;
  Serial.print("Actuation task=");
  Serial.println(stoppy);*/
  xSemaphoreGive(xactuationSemaphore) == pdTRUE;
    }
}
