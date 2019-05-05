/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst
 *  Works with: accesspoint_bare_01.ino
 */

#include <ESP8266WiFi.h>

char ssid[] = "ESPServer";           // SSID of your AP
char pass[] = "espserver";         // password of your AP

IPAddress server(192,168,4,15);     // IP address of the AP
WiFiClient clients;


void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);           // connects to the WiFi AP
  Serial.println();
  Serial.println("Connection to the AP");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.println("station_bare_01.ino");
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
}

void loop() {
  clients.connect(server, 9999);
  Serial.println("\n");
  Serial.println("********************************");
  Serial.print("Byte sent to the ESPServer: ");
  Serial.println(clients.print(WiFi.macAddress() + "\r"));
  String answer = clients.readStringUntil('\r');
  Serial.println("From the ESPServer: " + answer);
  Serial.println("********************************");
  clients.flush();
  clients.stop();
  delay(2000);
  delay(2000); 
}
