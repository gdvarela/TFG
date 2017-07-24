#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

const char* ssid     = "RaspWiFi";
const char* password = "raspberry";

const char* host = "192.168.0.1";

void setup() {
  Serial.begin(115200);  

  WiFi.mode(WIFI_STA);
}

void loop() {

  WiFi.disconnect();

  Serial.println("scan start");
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0){
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      if (WiFi.SSID(i) == ssid){
         // We start by connecting to a WiFi network
          Serial.println();             
          Serial.println();
          Serial.print("Connecting to ");
          Serial.println(ssid);
        
          WiFi.begin(ssid, password);
          
          while (WiFi.status() != WL_CONNECTED) {
            delay(50);
            Serial.print(".");
          }
        
          Serial.println("");
          Serial.println("WiFi connected");
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP()); 
        
          WiFiClient client;
          const int tcpPort = 7891;
          if (!client.connect(host, tcpPort)) {
            Serial.println("connection failed");
            return;
          }
        
         String data = "message=ok";
        
         Serial.print("Sending message...: ");
         // Send request to the server:
         client.print(data);
           
         if (client.connected()) { 
            client.stop();  // DISCONNECT FROM THE SERVER
         }
         Serial.println();
         Serial.println("closing connection");
         return;
      }
      
    }
  }
}
