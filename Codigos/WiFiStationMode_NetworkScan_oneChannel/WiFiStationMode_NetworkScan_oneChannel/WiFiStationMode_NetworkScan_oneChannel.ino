#include <Regexp.h>
#include <ESP8266WiFi.h>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
#include "c_types.h"
#include "osapi.h"
}
#endif

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  Serial.println("Setup done");
}

void scan_done (void *result, STATUS status) {
  struct  bss_info  *bss_info = (struct bss_info  *)result;

  if (status == OK) {

    while (bss_info) {

      String ssid = (char*) bss_info->ssid;

      if(ssid.substring(0,7) == "TRAFFIC") {
        Serial.print("SSID: ");
        Serial.print(ssid);
        Serial.println("");

        String code = ssid.substring(8,4);
        String data = ssid.substring(13);

        Serial.print("Code: ");
        Serial.print(code);
        Serial.println("");

        Serial.print("Data: ");
        Serial.print(data);
        Serial.println("");
      } else {
        Serial.println("Traffic Station not founded");
      }
    
      bss_info = STAILQ_NEXT(bss_info, next);
    }

  } else {
    Serial.println("Scan failed!\n");
  }
}

void loop() {

  struct scan_config conf;

  conf.ssid = NULL;
  conf.bssid = NULL;
  conf.channel = 1;
  conf.show_hidden = 0;

  wifi_station_scan(&conf, scan_done);
}
