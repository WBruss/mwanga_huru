#include <ESP8266WiFi.h>

extern "C" {
#include <user_interface.h>
}

#define MAX_CLIENTS 8

const char* ssid = ".";
const char* pass = "jamii2yote";

WiFiServer server(80);
WiFiServer ESPServer(9999);
IPAddress IP(192,168,4,15);
IPAddress mask = (255, 255, 255, 0);
WiFiClient clients;

String header;
String Status = "NULL";
String request;
String MACaddress[MAX_CLIENTS];
bool match = false;
int devices = 0;
int count = 0;
String MACaddresses;


const int pin = 2; 

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  WiFi.mode(WIFI_AP_STA);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println(WiFi.localIP());
  
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);  

  //Set the Access point
  Serial.println("Setting Access Point");
  boolean result = WiFi.softAP("ESP8266", "jamii2yote");
//  WiFi.softAPConfig(IP, IP, mask);
  if(result == true){
    Serial.println("Ready");
  }else{
    Serial.println("Failed");
  }
  
  server.begin();  

  Serial.print("IP: ");     Serial.println(WiFi.softAPIP());
  Serial.print("IP: ");     Serial.println(WiFi.localIP());

   for (int i = 0; i < MAX_CLIENTS; i++){
    MACaddress[i] = "NULL";
  }
}


void loop() {
  int dev = 0;
  dev = wifi_softap_get_station_num();

  
    client_status();

  
  Serial.println(MACaddresses);
  
  //Check devices connected to access point
    
  WiFiClient client = server.available();
  if(!client){
    return;
  }  Serial.println("New Client");
  while(!client.available()){
    delay(1);
  }
  if(client.available()){
    request = client.readStringUntil('.');
    Serial.print("App: ");
    Serial.println(request);

    if(request == "on" || request == "off"){
      client.flush();
      Serial.print("Bytes sent: ");
      Serial.println(client.println(request));   

      if( request == "on" ){
          Status = "on.";
        }
        else if(request == "off" ){
          Status = "off.";          
        }    
      Serial.print("Status: "); Serial.println(Status); 
      client.stop();  
      } 
        Serial.print("Bytes sent to ESP: ");
        Serial.println(client.println(Status)); 
        Serial.print("Status Sent: "); Serial.println(Status);          
        client.flush();
        client.stop();  
    
  }
}


void client_status() {
  Serial.println("-------------Connected Clients List-----------------------");
  devices = wifi_softap_get_station_num();
  Serial.print(devices); Serial.println(" clients.");

  struct station_info *station_list = wifi_softap_get_station_info();

  while (station_list != NULL && count < MAX_CLIENTS) {
    char station_mac[18] = {0}; sprintf(station_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(station_list->bssid));
    Serial.print("MAC Address: "); Serial.println(station_mac);

    for( int j = 0; j < MAX_CLIENTS; j++){
      if(MACaddress[j] == station_mac){
        match = true;
        break;
      }else{
        match = false;
      }
    }    
    if(MACaddress[count] != "NULL"){
      continue;
      }
    else if(match == false){
      MACaddress[count] = station_mac;
      MACaddresses += (MACaddress[count] + "," );
      Serial.print("Storing client: "); Serial.println(count);
      count++;
      break;
      }
          
    station_list = STAILQ_NEXT(station_list, next);
  }

  wifi_softap_free_station_info();
  Serial.println();
}
