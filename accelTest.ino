#include <Wire.h>
#include <SPI.h>
#include <SigFox.h>
#include <ArduinoLowPower.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

double averageX, averageY, averageZ;
String message = "Move";
int trigger = 0;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup(void) {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);
  
  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
  sensors_event_t event;
  lis.getEvent(&event);
  int measurementRuns = 50; 
  
  for(int i = 0; i < measurementRuns; i++)
  {
      averageX = averageX + event.acceleration.x;
      Serial.print(i);Serial.print("       ");Serial.println(averageX);
      averageY = averageY + event.acceleration.y;
      Serial.print(i);Serial.print("       ");Serial.println(averageY);
      averageZ = averageZ + event.acceleration.z;
      Serial.print(i);Serial.print("       ");Serial.println(averageZ);
      delay(200);
  }
  
  averageX = averageX/measurementRuns;
  averageY = averageY/measurementRuns;
  averageZ = averageZ/measurementRuns;
  }


void loop() 
{
  sensors_event_t event;
  lis.getEvent(&event);

  if(event.acceleration.x > (averageX + 5) || event.acceleration.x < (averageX - 5) || event.acceleration.y > (averageY + 5) || event.acceleration.y < (averageY - 5) || event.acceleration.z > (averageZ + 10) || event.acceleration.z < (averageZ - 10))
    {
      if(trigger == 0)
        {
          sendString();
        }
    }
}

void loop1()
{
  
}

void sendString()
{
  if(trigger == 0)
  {
    SigFox.begin();
    Serial.println("sent");
    delay(100);
    SigFox.beginPacket();
    SigFox.print(message);

    int ret = SigFox.endPacket();
    
    if (ret > 0) {
      Serial.println("No transmission");
    } else {
      Serial.println("Transmission ok");
    }
    
    SigFox.end();
    trigger++;
    loop1();
  }
}

