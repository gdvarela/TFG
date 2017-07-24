#include <ESP8266WiFi.h>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
#include "c_types.h"
#include "osapi.h"
}
#endif

static  void AP_changeSSID(char* ssid) {

  struct softap_config conf;

  conf.channel = 1;
  conf.ssid_hidden = 0;
  conf.max_connection = 4;
  conf.beacon_interval = 100;

  strcpy(reinterpret_cast<char*>(conf.ssid), ssid);
  conf.ssid_len = strlen(ssid);

  if (!wifi_softap_set_config(&conf)) {
    // set AP conf failed
    Serial.println("Set AP conf failed!\n");
  } else {
    Serial.print("SSID: ");
    Serial.print(ssid);
    Serial.println("");
  }
}

void setup() {
  Serial.begin(115200);

  if (!WiFi.enableAP(true)) {
    // enable AP failed
    Serial.println("Enable AP failed!\n");
  }


}

void loop() {
  AP_changeSSID("TRAFFIC_0001_RED");
  delay(5000);
  AP_changeSSID("TRAFFIC_0001_AMBER");
  delay(1000);
  AP_changeSSID("TRAFFIC_0001_GREEN");
  delay(5000);
  AP_changeSSID("TRAFFIC_0001_AMBER");
  delay(1000);
}
