
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//pinMode(15,INPUT_PULLUP);
pinMode(5,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    //Serial.println(digitalRead(15));
    digitalWrite(5,HIGH);
    delay(1000);
    digitalWrite(5,LOW);
}
