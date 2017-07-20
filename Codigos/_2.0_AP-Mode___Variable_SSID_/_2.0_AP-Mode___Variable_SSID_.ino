#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>

/* Initial credentials. */
char *ssid = "traffic_1_RED";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid);
}

void loop() {
  delay(10000);
  WiFi.softAPdisconnect(1);
  ssid = "traffic_1_AMBER";
  WiFi.softAP(ssid);
  delay(10000);
  WiFi.softAPdisconnect(1);
  ssid = "traffic_1_GREEN";
  WiFi.softAP(ssid);
  delay(10000);
  WiFi.softAPdisconnect(1);
  ssid = "traffic_1_RED";
  WiFi.softAP(ssid);
}
