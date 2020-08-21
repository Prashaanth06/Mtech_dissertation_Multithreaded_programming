
#define BLYNK_PRINT Serial
//#include <SimpleTimer.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "3UDwhXDPMCvbNpNDTlntGjLgD2B82GNg";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Ajay";
char pass[] = "prashaanth";

int data;

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(15,INPUT);

  Blynk.begin(auth, ssid, pass);
  //timer.setInterval(1000L, getSendData);
}

void loop()
{
  //timer.run(); // Initiates SimpleTimer
  Blynk.run();
  Serial.println(analogRead(15));
}

void getSendData()
{
data = analogRead(15); 
  Blynk.virtualWrite(V4, data);
 
  if (data > 700 )
  {
    Blynk.notify("Smoke Detected!"); 
  }
 
}
