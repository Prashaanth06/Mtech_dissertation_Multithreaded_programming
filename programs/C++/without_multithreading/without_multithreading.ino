
#define DHTPIN 32     // Temperature sensor 
#define fire 27     // Flame sensor
#define gas 13      // smoke sensor
#define watlvl 14     // water level sensor
#define manpres 15    // Manual_Press for emergency
#define LED 2        // led for indication
#define Buzzer 4     // Buzzer. 
#define Exhaust 5     // Exhaust fan.
#define watopen 12     // water opening

int freq = 1500;
int channel = 0;
int resolution = 7;
int dutyCycle = 128;

void setup() {
  Serial.begin(115200);
    

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

}

void loop() {

      unsigned long timeBegin = micros();

  // put your main code here, to run repeatedly:
  //delay(200);
  Serial.println("----------");
  Serial.println("----------");
  actuation();
  water_level();
  
  if(digitalRead(manpres) == 0)
  {
   flame();
   smoke();
  }
  else
  {
  manual_press();
  }

   Serial.print("Allocated heap=");
  Serial.println(ESP.getMaxAllocHeap());
  
 unsigned long timeEnd = micros();
  unsigned long duration = timeEnd - timeBegin;
  double averageDuration = (double)duration / 1000000.0;
  Serial.print(averageDuration);
  Serial.println("s");
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


               
  
