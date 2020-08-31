
void setup() {
  Serial.begin(115200);   // Serial monitor setting up baud rate
  Serial.println(ESP.getHeapSize());
  Serial.println(ESP.getFreeHeap());

}

void loop() {
  
}
