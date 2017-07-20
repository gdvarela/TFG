#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "TRAFFIC_1_RED";

ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
	server.send(200, "text/html", "<h1>You are connected</h1>");
}

void setup() {
	delay(1000);
	Serial.begin(115200);
  
  WiFi.softAP(ssid);
}

void loop() {  
  delay(5000);
  WiFi.disconnect();
  WiFi.softAP("TRAFFIC_1_AMBER");
  delay(1000);
  WiFi.disconnect();
  WiFi.softAP("TRAFFIC_1_GREEN");
  delay(5000);
  WiFi.disconnect();
  WiFi.softAP("TRAFFIC_1_AMBER");
  delay(1000);
  WiFi.disconnect();
  WiFi.softAP("TRAFFIC_1_RED");

}
