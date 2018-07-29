#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#define PORT 0/*수정*/
const char* ssid  = "SSID";/*수정*/
const char* password = "PASSWORD";/*수정*/
const char* server = "SERVER_URL";/*수정*/
String jsondata="";
WiFiClient client;
StaticJsonBuffer<200> jsonBuffer;/*Json 클래스 할당*/
JsonObject&root=jsonBuffer.createObject();
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
  Serial.println("");
  Serial.println("MCU has been connected.");
  Serial.println("Router name : "+String(ssid));  
  Serial.print("Allocated IP address from Router is ");
  Serial.println(WiFi.localIP());
}
void loop() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();
act(1000,t,h);/*매개변수가 딜레이*/
}

void act(int del,int tem, int hum){
  delay(del);
  client.stop();
if(client.connect(server,PORT)){
  /*형식
    POST / HTTP 1.1
    Host: URL
    Connextion: close
    Content-Type: json
    Content-Length: sizeof(jsondata)
    jsondata
    */
  root["tem"]=tem;
  root["hum"]=hum;
  root.printTo(jsondata);
  String s="Host: "+String(server);
  client.println("POST /htparse HTTP/1.1");
  client.println(s);
  client.println("Connection: close");
  client.println("Content-Type: json");
  client.print("Content-Length: ");
  client.println(jsondata.length());
  client.println();
  client.println(jsondata);
  jsondata="";
  Serial.println("Done");
    }
    else{Serial.println("NO");}
    }

