#include <SoftwareSerial.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


//software serial hanya dapat digunakan pada pin D0-D5
SoftwareSerial SerialX(D3, D4);

#ifndef STASSID
#define STASSID "qinthil"
#define STAPSK  "qinthil"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
WiFiClient client;

void setup() {
  Serial.begin(9600);
  SerialX.begin(9600);
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  sendRequest(listenRequest());
}

void sendRequest(JsonObject& data) {
  
  Serial.flush();
  bool fail = false;
  String replies;
  if (data != JsonObject::invalid()) {   
    String port = data["port"];
    String host = data["host"];
    String request = data["dataReq"];
    Serial.println("Host    : " + host);
    Serial.println("Port    : " + port);
    Serial.println("Request : " + request);
    Serial.println();
    Serial.println("Requesting...");

    if (client.connect(host, port.toInt())) {
      Serial.println("Connected!");
      client.println(request);
      client.println("Host: " + host);
      client.println("Connection: close");
      client.println();
      replies = client.readString();
      client.stop();
    } else {
      fail = true;
      Serial.println("Failed!");
    }
    if (fail) {
      SerialX.println("Failed!");
    } else {
      replies = replies.substring(replies.indexOf("{"));
      replies.remove(replies.indexOf("}") + 1);

      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(replies);
      root.printTo(SerialX);
      Serial.println(replies);
    }
  }

}

JsonObject& listenRequest() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(SerialX);
  return root;
}
