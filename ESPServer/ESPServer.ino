/*  Accesspoint - station communication without router
 *  see: https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-class.rst
 *       https://github.com/esp8266/Arduino/blob/master/doc/esp8266wifi/soft-access-point-examples.rst
 *       https://github.com/esp8266/Arduino/issues/504
 *  Works with: station_bare_01.ino
 */ 


#include <ESP8266WiFi.h>

#define MAX_CLIEBTS 8

char ssid[] = ".";           
char pass[] = "jamii2yote";  

WiFiServer server(9999);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);

WiFiClient clients;
WiFiClient ESPClients[MAX_CLIEBTS] = {};
String MACaddress[MAX_CLIEBTS] = {"NULL"};
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
}

void loop() {
  devices = 0;
  devices = WiFi.softAPgetStationNum();
  Serial.print("Devices: "); Serial.println(devices);
  delay(3000);
  
  if(dev != devices){
    Serial.print("Number of devices: "); Serial.println(devices);
    if(dev > devices){
          Serial.println("One device went offline.");
          dev = devices;
        }else if(dev < devices){
          Serial.println("New device connected.");
          dev = devices;
        }
    
    Serial.print("Number of dev: "); Serial.println(dev);
    Serial.println("MAC Addresses");
    for(int i = 0; i < dev; i++){
      Serial.print("MAC ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(MACaddress[i]);
    }

    Serial.println("Waiting for connections");
    
      while (!clients) {
        WiFiClient client = server.available();
        clients = client;
        dev = 0;
        delay(1);
        }
        if(clients){   
          Serial.println("Client Found");  
          if(clients.available(){
              String request = clients.readStringUntil('\r');
              delay(1);
          }            
            clients.print("Received");
            response = "Connected";      
            Serial.println("\n");
            Serial.println("********************************");
            Serial.println("From the ESPClient: " + request);
            clients.flush();    
    
             for(int i = 0; i < dev; i++){
                if(MACaddress[i] == request){
                  Serial.println("Match found");
                  match = true;
                  break;
                }
             }
             
             for(int i = 0; i < dev; i++){
                if(match == false){
                  Serial.println("Updating Clients");
                  if(MACaddress[i] == "null"){
                    MACaddress[i] = request;
                    ESPClients[i] = clients;
                    Serial.print("Byte sent to the ESPClient: ");
                    Serial.println(ESPClients[i].println(response + "\r"));
                    Serial.println("********************************");
                    match = false;
                    break;
                  }
                  break;      
                }
              }
        }
  }
  
    Serial.print("#");
    
    delay(3000);
 
  Serial.println("Stored MAC Addresses");
  for(int i = 0; i < dev; i++){
    Serial.print("MAC ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(MACaddress[i]);
    Serial.print(" Client: ");
    Serial.println(ESPClients[i]);
  }
  Serial.println("");
  for(int i = 0; i < dev; i++){
    Serial.print("Sent to client "); Serial.println(i);
    Serial.println(ESPClients[i].print("Stay tuned\r"));
  }
  Serial.println("*************END****************");
  
}
