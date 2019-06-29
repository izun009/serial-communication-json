#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial s(8, 10);
String server = "www.presensicl.online";
String port = "80";
String req;

void setup() {
  s.begin(9600);
  Serial.begin(9600);
  req = "GET /tes.php?id=13 HTTP/1.1";
}


void loop() {
  Serial.println("Sending....");
  SendtoNode(req);
  Serial.println("Waiting for replies....");

  int startTime = millis();
  int wait = 0;

  while (!printJson(listenReplies()) && wait < 10000) {
    wait = millis() - startTime;
  }

  //  if(s.available() > 0){
  //    Serial.println(s.readString());
  //  }

  Serial.flush();
  delay(1000);
}

JsonObject& listenReplies() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  return root;
}

boolean printJson(JsonObject& json) {
  if (json != JsonObject::invalid()) {
    String name = json["nama"];
    String id = json["id"];
    String state = json["status"];
    Serial.println();
    Serial.println("Nama  : " + name);
    Serial.println("Id    : " + id);
    Serial.println("Status: " + state);
    Serial.println();
    return true;
  } else {
    return false;
  }
}

void SendtoNode(String dataReq) {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["host"] = server;
  root["port"] = port;
  root["dataReq"] = dataReq;
  root.printTo(s);
}
