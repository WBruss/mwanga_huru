/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/station-class.rst
 *  Works with: accesspoint_bare_01.ino
 */

#include <ESP8266WiFi.h>

char ssid[] = "ESP8266";           // SSID of your AP
char pass[] = "jamii2yote";         // password of your AP

IPAddress server(192,168,4,1);     // IP address of the AP
WiFiClient clients;

const int pin = 2;
String answer = "NULL";


void setup() {
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
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
  Serial.print("LocalIP:"); Serial.println(WiFi.localIP());
  Serial.println("MAC:" + WiFi.macAddress());
  Serial.print("Gateway:"); Serial.println(WiFi.gatewayIP());
  Serial.print("AP MAC:"); Serial.println(WiFi.BSSIDstr());
}

void loop() {
  Serial.println("");
  Serial.println((clients.connect(server, 80))? "Connected" : "Server not found");  
  
    Serial.print("Byte sent to the ESPServer: ");
    Serial.println(clients.print(WiFi.macAddress() + "."));
    
    while (clients.connected() || clients.available())
    {
      if (clients.available())
      {
        answer = clients.readStringUntil('.');
        Serial.println("From the ESPServer: " + answer);
        clients.flush();

        if( answer == "on" ){
          digitalWrite( pin, HIGH );
          Serial.println("on");
        }
        else if(answer == "off" ){
          digitalWrite( pin, LOW ); 
          Serial.println("off");
        } 
      }
    }  
    clients.stop();  
}
