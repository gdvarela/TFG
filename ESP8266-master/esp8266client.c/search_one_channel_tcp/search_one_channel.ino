#include <ESP8266WiFi.h>

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
  #include "c_types.h"
  #include "osapi.h"
}
#endif

const char* ssid     = "RaspWiFi";
const char* password = "raspberry";
const int channel = 1;
const char* host = "192.168.0.1";
const int tcpPort = 7891;
bool connected_to_wifi = false;
String data = "message=green\n";

void setup() {
  Serial.begin(115200);
}

static  void  ICACHE_FLASH_ATTR scan_done(void *arg, STATUS  status){
  // method call after scanning is finished
    struct  bss_info  *bss_link = (struct bss_info  *)arg;
    
    if(bss_link!=NULL){
      Serial.println();
      Serial.print("Connecting to ");
      Serial.println((char*)bss_link->ssid);

      wifi_set_opmode(STATIONAP_MODE);//Set  softAP  + station mode

      struct station_config stationConf;
      stationConf.bssid_set  = 0;            //need  not check MAC address of  AP
    
      os_memcpy(&stationConf.ssid, ssid, 32);  
      os_memcpy(&stationConf.password, password, 64);  
      
      wifi_station_set_config(&stationConf); 

      wifi_station_disconnect();
      connected_to_wifi = wifi_station_connect();
    }
}

void loop() {
  // Call Espressif SDK functionality - wrapped in ifdef so that it still
  // compiles on other platforms
  
  if(!connected_to_wifi){
    #ifdef ESP8266
      scan_config scan = {
          (uint8 *)ssid,            //here goes the SSID
          NULL,                         //here goes the bssid
          channel,                           //Here goes the channel we want
          false                         //hide channels
      };
   
      wifi_station_scan(&scan, scan_done);

      Serial.println("NET STATUS:");
      Serial.print("STATION_IDLE:");
      Serial.print(STATION_IDLE);
      Serial.println("");
      Serial.print("STATION_CONNECTING:");
      Serial.print(STATION_CONNECTING);
      Serial.println("");
      Serial.print("STATION_WRONG_PASSWORD:");
      Serial.print(STATION_WRONG_PASSWORD);
      Serial.println("");
      Serial.print("STATION_NO_AP_FOUND:");
      Serial.print(STATION_NO_AP_FOUND);
      Serial.println("");
      Serial.print("STATION_WRONG_PASSWORD:");
      Serial.print(STATION_WRONG_PASSWORD);
      Serial.println("");
      Serial.print("STATION_GOT_IP:");
      Serial.print(STATION_GOT_IP);
      Serial.println("");
      
    #endif
  }else{
    enum Status {
        STATION_IDLE  = 0,
        STATION_CONNECTING,
        STATION_WRONG_PASSWORD,
        STATION_NO_AP_FOUND,
        STATION_CONNECT_FAIL,
        STATION_GOT_IP
    };
    uint8 netStatus = wifi_station_get_connect_status();
    Serial.print("CONNECTION STATUS:");
    Serial.print(netStatus);
    Serial.println("");

    // send to server
    if(netStatus==STATION_GOT_IP){
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
      
       Serial.print("Sending message...: ");
       // Send request to the server:
       client.print(data);
         
       if(data=="message=green\n"){
          data = "message=yellow\n";
       }else if(data=="message=yellow\n"){
          data = "message=red\n";
       }else  if(data=="message=red\n"){
          data = "message=green\n";
       }
       
       if (client.connected()) { 
          client.stop();  // DISCONNECT FROM THE SERVER
       }
       Serial.println();
       Serial.println("closing connection");
   }
    
  }
  delay(1000);
}
