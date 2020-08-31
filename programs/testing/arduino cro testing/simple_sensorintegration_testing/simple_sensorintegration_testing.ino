#define LED 2        // led for indication
#define fire 27     // Flame sensor


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);

pinMode(LED,OUTPUT);
pinMode(fire,INPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(fire)== 0)
{
  digitalWrite(LED,HIGH);
}
else
{
  digitalWrite(LED,LOW);
}
}
