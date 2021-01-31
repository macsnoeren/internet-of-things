#include <rest.h>

#include <Arduino.h>
#include <ESP8266WiFi.h> // Connect to Wi-Fi
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <fsm.h>

WiFiClient client;
WiFiClientSecure clients;
HTTPClient http;

void RESTInit () {
    clients.setInsecure(); // We will not create a certification store here to validate sites!
}

void RESTGetStatus (String status) {
  // HTTPS
  //clients.connect("https://vmacman.jmnl.nl/wemos-test/status.php", 443);
  //http.begin(clients, "https://vmacman.jmnl.nl/wemos-test/status.php");
  
  // HTTP
  http.begin(client, "http://vmacman.jmnl.nl/wemos-test/status.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "status=" + status;

  if (http.POST(httpRequestData) == HTTP_CODE_OK) {   
      String payload = http.getString();
      Serial.println("POST: '" + payload + "'");
  }
}

void RESTPollEvent () {
  // HTTPS
  //clients.connect("https://vmacman.jmnl.nl/wemos-test/event.php", 443);
  //http.begin(clients, "https://vmacman.jmnl.nl/wemos-test/event.php");
  
  // HTTP
  http.begin(client, "http://vmacman.jmnl.nl/wemos-test/event.php");

  if (http.GET() == HTTP_CODE_OK) {   
      String payload = http.getString();
      Serial.println("'" + payload + "'");
      if ( payload.equals("EVENT_OUTOFORDER") ) {
        FSMRaiseEvent(EVENT_OUTOFORDER);
      }
      
      if ( payload.equals("EVENT_GREEN") ) {
        FSMRaiseEvent(EVENT_GREEN);      
      }
      
      if ( payload.equals("EVENT_RED") ) {
        FSMRaiseEvent(EVENT_RED);      
      }
  }
}