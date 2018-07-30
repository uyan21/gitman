#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#define LED D1
#define PORT 0/*수정*/
const char* ssid  = "SSID";/*수정*/
const char* password = "PASSWORD";/*수정*/
const char* server = "SERVER_URL";/*수정*/
int val;/*킬지 끌지 받는 변수 */
String jsondata="";/*전송용 데이터지만 무슨 이유인지 아두이노가 받기만 해도 형식상 보내 줘야 됩니다. */
String rcvbuf;/*서버로 부터 받은 json을 받는 변수*/
char c;/*한 글자 한 글자 읽어서 rcvbuf에 쌓는 변수*/
int parsing_run=0;/*{(중괄호) 부터 }(중괄호)까지 데이터에 쌓게 만들기위해서 토글 형식으로 사용하려고 할당
(제가 처음 ArduinoJson.h 다룰때 만든 사람이 파싱하는거 완성을 못 해놔서 제가 이런 식으로 만들어서 쓰고있습니다.)*/

StaticJsonBuffer<200> jsonBuffer;/*Json 클래스 할당*/
void setup() {
  pinMode(LED, OUTPUT);
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
act(1000);/*매개변수가 딜레이*/
}

void act(int del){
  WiFiClient client;
  delay(del);
  client.stop();
if(client.connect(server,PORT)){
  /*형식
    POST / HTTP 1.1
    Host: URL
    Connection: close
    */
  String s="Host: "+String(server);
  client.println("POST /act HTTP/1.1");
  client.println(s);
  client.println("Connection: close");
  client.println();
  delay(400);
  while(client.available()){
    c=client.read();
    if(c=='{'){parsing_run=1;}/*토글 형식으로 켜지면 rcvbuf에 쌓게 만듦*/
    if(parsing_run==1){rcvbuf+=c;}
    if(c=='}'){parsing_run=0;}  /*꺼지면 끝*/
      }
      JsonObject&root=jsonBuffer.parseObject(rcvbuf);/*코딩하기 쉽게 json파일로 변환*/
      Serial.println(rcvbuf);
      val=root["option"];/*option: 키에서 값을 받고 val에 할당 */
      if(val==1){digitalWrite(LED,HIGH);}
      else if(val==0){digitalWrite(LED,LOW);}
      rcvbuf="";/*받은 값 초기화*/
      Serial.println("Free Heap: "+String(ESP.getFreeHeap()));
    }
    else{Serial.println("NO");}
    }

