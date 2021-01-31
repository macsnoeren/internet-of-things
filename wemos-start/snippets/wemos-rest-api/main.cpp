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

void loop() {
  FSMLoop();
  //pollRESTApiEvent();
  //sentRESTApiStatus("testing");
}
// FSM first implementation
// RAM:   [===       ]  33.7% (used 27600 bytes from 81920 bytes)
// Flash: [===       ]  25.3% (used 263940 bytes from 1044464 bytes)

// FSM second implementation
// RAM:   [===       ]  33.3% (used 27312 bytes from 81920 bytes)
// Flash: [===       ]  25.2% (used 263604 bytes from 1044464 bytes)