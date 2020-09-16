# define wm 5
# define ef 12
# define button 15

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
pinMode(button,INPUT_PULLUP);
pinMode(wm,OUTPUT);
pinMode(ef,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   if(digitalRead(button) == 1)
    {
    digitalWrite(wm,HIGH);
    digitalWrite(ef,HIGH);
    }
    else
    {
   digitalWrite(wm,LOW);
    digitalWrite(ef,LOW);
    }
}
