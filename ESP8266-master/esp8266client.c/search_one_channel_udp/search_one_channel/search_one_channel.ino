#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <string.h>
#include <stdio.h>

struct Packet
{
  double Time;
  char ID[10];
  char Lon[50];
  char Lat[50];
  char Course[50];
  char Message[50];
};


typedef struct Packet Packet_t;
char packet_char[sizeof(struct Packet)];

/* Payload */
char payload[64];
int payload_size;

static int id = 0;

void buildPayload(Packet_t &data, char *payload, int *payload_size){

  /* | UDP Header | ID | Lon | Lat | Course | Message */

  itoa(id,data.ID,2);
  id = id + 1;
  itoa(0,data.Lon,10);          /* dummy */
  itoa(0,data.Lat,10);          /* dummy */
  strcpy(data.Course,"NE");     /* dummy */
  strcpy(data.Message,"STOP");   /* dummy */

  strcpy(payload,data.ID);
  strcat(payload," | ");
  strcat(payload,data.Lon);
  strcat(payload," | ");
  strcat(payload,data.Lat);
  strcat(payload," | ");
  strcat(payload,data.Course);
  strcat(payload," | ");
  strcat(payload,data.Message);

  *payload_size = strlen(payload)+1;
}

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
  #include "c_types.h"
  #include "osapi.h"
}
#endif

const char* ssid = "netRider";
const int channel = 1;
bool connected_to_wifi = false;

Packet_t data;

void setup() {
  Serial.begin(115200);
}

static  void  ICACHE_FLASH_ATTR scan_done(void *arg, STATUS  status){
  // method call after scanning is finished
    struct  bss_info  *bss_link = (struct bss_info  *)arg;
    
    if(bss_link!=NULL){

       while (bss_link != NULL) {
          if(strstr((char*)bss_link->ssid, ssid)){
            Serial.println((char*)bss_link->ssid);

            Serial.println();
            Serial.print("Connecting to ");
            Serial.println((char*)bss_link->ssid);
      
            wifi_set_opmode(STATIONAP_MODE);  //Set softAP  + station mode
      
            struct station_config stationConf;
            stationConf.bssid_set  = 0;            //need not check MAC address of AP
          
            os_memcpy(&stationConf.ssid, (char*)bss_link->ssid, 32);  
            
            wifi_station_set_config(&stationConf); 
      
            wifi_station_disconnect();
            connected_to_wifi = wifi_station_connect();
            
            break;
          }
    
          bss_link = bss_link->next.stqe_next;
       } 
    }
}

void loop() {
  // Call Espressif SDK functionality - wrapped in ifdef so that it still
  // compiles on other platforms
  
  if(!connected_to_wifi){
    #ifdef ESP8266
      scan_config scan = {
          NULL,                         //here goes the SSID
          NULL,                         //here goes the bssid
          channel,                         //Here goes the channel we want
          false                         //hide channels
      };
   
      wifi_station_scan(&scan, scan_done);

      uint8 netStatus = wifi_station_get_connect_status();
      Serial.print("CONNECTION STATUS:");
      Serial.print(netStatus);
      Serial.println("");
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

        WiFiUDP Udp;
        const int udpPort = 13000;
        
        buildPayload(data, payload, &payload_size);
        
        /* copy to char to send */
        memcpy(packet_char, data.ID, sizeof data.ID);
        memcpy(packet_char + sizeof data.ID, data.Lon, sizeof data.Lon);
        memcpy(packet_char + sizeof data.ID + sizeof data.Lon, data.Lat, sizeof data.Lat);
        memcpy(packet_char + sizeof data.ID + sizeof data.Lon + sizeof data.Lat, data.Course, sizeof data.Course);
        memcpy(packet_char + sizeof data.ID + sizeof data.Lon + sizeof data.Lat + sizeof data.Course, data.Message, sizeof data.Message);  

        Serial.println("");
        Serial.println("WiFi gateway");
        Serial.println(WiFi.gatewayIP());
        Udp.beginPacket(WiFi.gatewayIP(), udpPort);
        Udp.write(packet_char);
        Udp.endPacket();
        Serial.println("sent");
   }
    
  }
  delay(1000);
}
