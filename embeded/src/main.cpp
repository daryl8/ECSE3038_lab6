#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <env.h>

#define endpoint "https://specialcase-lab6-api.onrender.com"

#define fan_Pin 22
#define light_Pin 23

float getTemp(){

  return random(21.0,33.0);
}

void setup() {
 Serial.begin(9600);

	pinMode(fan_Pin,OUTPUT);
  pinMode(light_Pin,OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println("");

Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
   if(WiFi.status()== WL_CONNECTED){
    Serial.println("");
    Serial.println("");
    HTTPClient http;

    String url =  String(endpoint) + "/api/temperature";
    http.begin(url);
    http.addHeader("Content-type", "application/json");
    http.addHeader("Content-length", "23");

    StaticJsonDocument<1024> docput;
    String httpRequestData;

      docput["temperature"] = getTemp();

       serializeJson(docput, httpRequestData);
        
    int httpResponseCode = http.PUT(httpRequestData);
    String http_response;
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("HTTP Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);
    }
     else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();    
    url =  String(endpoint) + "/api/state";
    http.begin(url);
    httpResponseCode = http.GET();

    Serial.println("");
    Serial.println("");

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }

    StaticJsonDocument<1024> docget;

    DeserializationError error = deserializeJson(docget, http_response);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    
    bool temp = docget["fan"]; 
    bool light= docget["light"]; 

    digitalWrite(fan_Pin,temp);
    digitalWrite(light_Pin,temp);
    
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}
