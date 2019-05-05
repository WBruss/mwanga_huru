/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 


#include <ESP8266WiFi.h>

char ssid[] = ".";           
char pass[] = "jamii2yote";  

WiFiServer server(9999);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

WiFiClient *ESPClients;
String *MACaddress;
int devices = 0;
int dev = 0;
bool match = false;
String response;

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_AP_STA);

  Serial.print("Connecting to: "); Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  WiFi.softAP("ESPServer", "espserver");
  WiFi.softAPConfig(IP, IP, mask);
  server.begin();
  Serial.println();
  Serial.println("Server started.");
  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("MAC:");     Serial.println(WiFi.softAPmacAddress());

  ESPClients = new WiFiClient[1];
  ESPClients = {};

  MACaddress = new String[1];
  MACaddress = {};
}

void loop() {
  devices = WiFi.softAPgetStationNum();
  
  WiFiClient client = server.available();
  if (!client) {
    return;
    }else{
      if(dev != devices){
        Serial.print("Number of devices: "); Serial.println(devices);
        delete ESPClients; 
        delete MACaddress;    
        if(dev > devices){
          Serial.println("One device went offline.");
          dev = devices;
        }else if(dev < devices){
          Serial.println("New device connected.");
          dev = devices;
        }
        ESPClients = new WiFiClient[dev];
        MACaddress = new String[dev];
        for(int i = 0; i < dev; i++){
          MACaddress[i] = " ";
        }
      } 
        String request = client.readStringUntil('\r');
        response = "Connected";
      
        Serial.println("\n");
        Serial.println("********************************");
        Serial.println("From the ESPClient: " + request);
        client.flush();     
    }
 

  for(int i = 0; i < dev; i++){
    if(MACaddress[i] == response){
      match = true;
      break;
    }
    if(!match){
      if(MACaddress[i] == " "){
        MACaddress[i] = response;
        ESPClients[i] = client;
        Serial.print("Byte sent to the ESPClient: ");
        Serial.println(ESPClients[i].println(response + "\r"));
        Serial.println("********************************");
        break;
      }      
    }
  }  

  
}
