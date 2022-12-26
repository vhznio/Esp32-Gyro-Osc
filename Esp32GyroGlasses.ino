#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <L3G.h>
#include <Wire.h>

L3G gyro;
WiFiUDP Udp;
const char* DestIp = "";
const int DestPort = 12000;

const char* ssid = "";
const char* password = "";
#define alpha 0.05    

void setup()
{
    Serial.begin(115200);   
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi... ");

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);

    Wire.begin();

    if (!gyro.init())
    {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
    }
    gyro.enableDefault();   
}

void loop(){    
  
    gyro.read();
    int xAxis = gyro.g.x;
    int yAxis = gyro.g.y;
    int zAxis = gyro.g.z;

    int xF = (alpha*xAxis) + ((1-alpha)*xF);
    int yF = (alpha*yAxis) + ((1-alpha)*yF);
    int zF = (alpha*zAxis) + ((1-alpha)*zF);
        
    int myValues[] = {xF, yF, zF};

    OSCMessage msg("/GyroAxisValues");
    for(int i = 0; i < 3;i++){
      msg.add(myValues[i]);
    }
    Udp.beginPacket(DestIp,DestPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(100);

    Serial.print("X: ");
    Serial.print(myValues[0]);
    Serial.print(" Y: ");
    Serial.print(myValues[1]);
    Serial.print(" Z: ");
    Serial.println(myValues[2]);
   
    if(WiFi.status() != WL_CONNECTED) {
      Serial.print("Connection lost");
      delay(200);
    }
}
