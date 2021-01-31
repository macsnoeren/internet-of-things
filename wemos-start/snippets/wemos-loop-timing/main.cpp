#include <Arduino.h>
#include <ESP8266WiFi.h> // Connect to Wi-Fi
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include "fsm.h"
#include "rest.h"

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

  FSMInit();
  RESTInit();

  Serial.println("Setup completed!");
}

time_t loopTimer;
long loopTime;
long delayTime;

void loop() { 
  loopTimer = millis();
  FSMLoop();

  RESTPollEvent();
  loopTime = (int) (millis()-loopTimer);
  delayTime = 100 - loopTime;
  Serial.println("Loop duration: " + String(loopTime) + "ms - delay: " + String(delayTime) + "ms" + " - cpu load: " + String((int) (100-delayTime)) + "%");

  if ( delayTime > 0 ) {
    delay(delayTime);

  } else {
    Serial.println("Timing issues, cannot meet timing criteria of 100ms loop.");
  }
  // Hier iets uitleggen over determinstisch en dat je de loop 100 ms kan laten lopen. Dat als je hier overheen gaat dat je dan misschien een probleem hebt. Dat kan je dan in de gaten houden.
  // delay( 100 - (millis()-looptTime)); => controlling the loop time en checking it as well. => Hoe bepaal je CPU load? => Dat is dus gewoon op gevoel, wat vinden wij ervan, requirements?
  Serial.println("Final duration: " + String((int) (millis()-loopTimer)));
}
// FSM first implementation
// RAM:   [===       ]  33.7% (used 27600 bytes from 81920 bytes)
// Flash: [===       ]  25.3% (used 263940 bytes from 1044464 bytes)

// FSM second implementation
// RAM:   [===       ]  33.3% (used 27312 bytes from 81920 bytes)
// Flash: [===       ]  25.2% (used 263604 bytes from 1044464 bytes)