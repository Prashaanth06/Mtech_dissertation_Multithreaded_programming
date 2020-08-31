#define fire 27
#define led 2

TaskHandle_t xmanpressTaskHandle = NULL;

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(fire,INPUT);
pinMode(led,OUTPUT);

xTaskCreate(check,   /* Function to implement the task */
                    "check", /* Name of the task */
                     1000,      /* Stack size in words */
                     NULL,       /* Task input parameter */
                      9,          /* Priority of the task */
                     &xmanpressTaskHandle);     /* Task handle. */

}

void loop() {
  // put your main code here, to run repeatedly:

}

void check(void * pvParameters)
{
  if(digitalRead(fire)== 0)
{
  digitalWrite(led,HIGH);
}
else
{
  digitalWrite(led,LOW);
}
}
