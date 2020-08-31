#define button 15
#define led 2

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
pinMode(button,INPUT);
pinMode(led,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(button)== 1)
{
  digitalWrite(led,HIGH);
}
else
{
  digitalWrite(led,LOW);
}
}
