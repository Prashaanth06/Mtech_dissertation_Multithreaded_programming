#define LM 32 

int RawValue= 0;
double Voltage = 0;
double tempC = 0;
double tempF = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LM,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
    // Delay between measurements.
   RawValue = analogRead(LM);
  Voltage = (RawValue * 3300)/ 4095; // 5000 to get millivots.
  Serial.println(RawValue);
  tempC = Voltage / 10; 
  Serial.print("\t Temperature in C = ");
Serial.println(tempC,1);

}
