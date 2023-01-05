#include <WiFi.h>
#include <ESPAsync.h>
#include <SPIFFS.h>

const char* ssid = "VRMalan";
const char* password = "wifiathome";

int pin = 0;
String ledState;

String processor(const String& var){
  //Serial.println(var);
  if(var == "GPIO_STATE"){
    if(digitalRead(LED_BUILTIN)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    //Serial.print(ledState);
    return ledState;
  }
  return String();
}

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  AsyncWebServer server(80);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/main.html", String(), false, processor);
  });

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, HIGH);
    request->send(SPIFFS, "/main.html", String(), false, processor);
  });

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED_BUILTIN, LOW);
    request->send(SPIFFS, "/main.html", String(), false, processor);
  });

  server.begin();
}

void loop(){
  
}