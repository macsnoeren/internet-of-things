#include <Arduino.h>
#include <ESP8266WiFi.h> // Connect to Wi-Fi
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include "PubSubClient.h" // MAKE SURE: in PubSubClient.h change MQTT_MAX_PACKET_SIZE to 2048 !! //
// setBufferSize(2048);

#include "fsm.h"
#include "rest.h"

WiFiClient clientw;
PubSubClient mqtt;

void mqtt_callback(char* topic, byte* pl, unsigned int length) {
  String payload = "";
  for (unsigned int i=0;i<length;i++) {
    payload += (char)pl[i];
  }
  Serial.println("MQTT EVENT: " + payload);

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

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  WiFi.begin("MaCMaN_N", "MaCMaN78@heksenenzo123"); // Connect with the Wi-Fi
  Serial.print("Connecting with Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  mqtt.setClient(clientw);
  mqtt.setBufferSize(2048); // override MQTT_MAX_PACKET_SIZE
  mqtt.setCallback(mqtt_callback);
  mqtt.setServer("test.mosquitto.org", 1883);
  mqtt.connect("wemos-client-1234");
  if ( mqtt.connected() ) {
    Serial.println("MQTT CONNECTED!");
  } else {
    Serial.println("MQTT NOT CONNECTED!");
  }

  mqtt.subscribe("wemos-event-1234");

  FSMInit();
  RESTInit();

  Serial.println("Setup completed!");
}

time_t loopTimer;
long loopTime;
long delayTime;
long diff = 0;

void loop() { 
  loopTimer = millis();
  FSMLoop();

  //RESTPollEvent();
  loopTime = (int) (millis()-loopTimer);
  delayTime = 100 - loopTime;
  Serial.println("Loop duration: " + String(loopTime) + "ms - delay: " + String(delayTime) + "ms" + " - cpu load: " + String((int) (100-delayTime)) + "%");

  String mes = "cpu load: " + String((int) (100-delayTime)) + "%";
  mqtt.publish("maurice123", mes.c_str());

  if ( !mqtt.loop() ) {
    Serial.println("MQTT ERROR");
    delay(1000);
  }

  if ( delayTime > 0 ) {
    delay(delayTime + diff);

  } else {
    Serial.println("Timing issues, cannot meet timing criteria of 100ms loop.");
  }
  // Hier iets uitleggen over determinstisch en dat je de loop 100 ms kan laten lopen. Dat als je hier overheen gaat dat je dan misschien een probleem hebt. Dat kan je dan in de gaten houden.
  // delay( 100 - (millis()-looptTime)); => controlling the loop time en checking it as well. => Hoe bepaal je CPU load? => Dat is dus gewoon op gevoel, wat vinden wij ervan, requirements?
  diff = 100 - (millis()-loopTimer);
  Serial.println("Final duration: " + String((int) (millis()-loopTimer)));
}
// FSM first implementation
// RAM:   [===       ]  33.7% (used 27600 bytes from 81920 bytes)
// Flash: [===       ]  25.3% (used 263940 bytes from 1044464 bytes)

// FSM second implementation
// RAM:   [===       ]  33.3% (used 27312 bytes from 81920 bytes)
// Flash: [===       ]  25.2% (used 263604 bytes from 1044464 bytes)