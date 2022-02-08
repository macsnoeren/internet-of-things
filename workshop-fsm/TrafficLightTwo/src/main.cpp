// Avans University of Applied Sciences (Avans Hogeschool)
// Module: Intelligent Wireless Sensor Netwerken (IWSN)
// Author: Maurice Snoeren
// Date  : 05-02-2021
//
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include <fsm.h>

// Create the wifi-client to connect to the Internet
WiFiClient client;

// Create the MQTT client to connect to a MQTT Broker (server)
PubSubClient mqtt;

// Finite state machine
enum FSM_STATES {
  STATE_START,
  STATE_LIGHT_1_GREEN,
  STATE_LIGTH_1_ORANGE,
  STATE_LIGTH_1_RED,
  STATE_LIGHT_2_GREEN,
  STATE_LIGTH_2_ORANGE,
  STATE_LIGTH_2_RED,
  STATE_OUT_OF_ORDER,
  STATES_TOTAL
};

// Finite state machine events
enum FSM_EVENTS {
  EVENT_BUTTON_1,
  EVENT_BUTTON_2,
  EVENT_TIMER,
  EVENT_OUT_OF_ORDER,
  EVENT_START,
  EVENT_ERROR,
  EVENT_STATE_EXECUTED, // Add this one always!
  EVENTS_TOTAL
};

// Finite state machine callback methods forward declarations
void preStart();
void loopStart();
void postStart();
void preLight1Green();
void loopLight1Green();
void postLight1Green();
void preLight1Orange();
void loopLight1Orange();
void postLight1Orange();
void preLight1Red();
void loopLight1Red();
void postLight1Red();
void preLight2Green();
void loopLight2Green();
void postLight2Green();
void preLight2Orange();
void loopLight2Orange();
void postLight2Orange();
void preLight2Red();
void loopLight2Red();
void postLight2Red();
void preOutOfOrder();
void loopOutOfOrder();
void postOutOfOrder();

//FSM
FSM fsm(STATES_TOTAL, EVENTS_TOTAL, false);

// Output
constexpr int LIGHT_1_RED    = D0;
constexpr int LIGHT_1_ORANGE = D1;
constexpr int LIGHT_1_GREEN  = D2;
constexpr int LIGHT_2_RED    = D3;
constexpr int LIGHT_2_ORANGE = D4;
constexpr int LIGHT_2_GREEN  = D5;

// Input
constexpr int BUTTON_1       = D6;
constexpr int BUTTON_2       = D7;

constexpr char MQTT_SERVER[] = "test.mosquitto.org";
constexpr int  MQTT_PORT     = 1883;

// Timer method for timing purposes
unsigned long timer;
unsigned long timerMQTT;
String payload = "";
StaticJsonDocument<256> jsondoc;

// External Events!
void callbackMQTT(char* topic, byte* pl, unsigned int length) {
  payload = "";
  for (unsigned int i=0;i<length;i++) {
    payload += (char)pl[i];
  }

  // Enable also a JSON interface
  DeserializationError err = deserializeJson(jsondoc, payload.c_str());
  if (err) {
    Serial.println("NO JSON: " + String(err.c_str()));
    Serial.println("Parsing as plain text!");

  } else {
    String event = jsondoc["event"];
    Serial.println("JSON EVENT: " + event);
    payload = event;
  }

  if ( payload.equals("EVENT_OUT_OF_ORDER") ) {
    fsm.raiseEvent(EVENT_OUT_OF_ORDER);

  } else if ( payload.equals("EVENT_START") ) {
    fsm.raiseEvent(EVENT_START);

  } else if ( payload.equals("EVENT_BUTTON_1") ) {
    fsm.raiseEvent(EVENT_BUTTON_1);

  } else if ( payload.equals("EVENT_BUTTON_2") ) {
    fsm.raiseEvent(EVENT_BUTTON_2);
  }

  Serial.println("MQTT EVENT '" + String(topic) + "': " + payload);
}

void connectMQTT() {
  while (!mqtt.connected()) {
    mqtt.connect("iwsn-wemos-client-maurice");
    mqtt.subscribe("iwsn-wemos-event");
    delay(1000);
    if ( mqtt.connected() ) {
      Serial.println("MQTT CONNECTED!");

    } else {
      Serial.println("MQTT NOT CONNECTED!");
    }
  }
}

void setup() {
  // Get the mode of the leds
  pinMode(LIGHT_1_RED,    OUTPUT);
  pinMode(LIGHT_1_ORANGE, OUTPUT);
  pinMode(LIGHT_1_GREEN,  OUTPUT);
  pinMode(LIGHT_2_RED,    OUTPUT);
  pinMode(LIGHT_2_ORANGE, OUTPUT);
  pinMode(LIGHT_2_GREEN,  OUTPUT);

  // Switch leds off
  digitalWrite(LIGHT_1_RED,    HIGH);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    HIGH);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);

  // Set the buttons as input
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);

  // For debugging purposes
  Serial.begin(9600);
  Serial.printf("\n\nSdk version: %s\n",   ESP.getSdkVersion());
  Serial.printf("Core Version: %s\n",      ESP.getCoreVersion().c_str());
  Serial.printf("Boot Version: %u\n",      ESP.getBootVersion());
  Serial.printf("Boot Mode: %u\n",         ESP.getBootMode());
  Serial.printf("CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Reset reason: %s\n",      ESP.getResetReason().c_str());

  // Add the state method to the FSM
  fsm.addState(STATE_START,          preStart, loopStart, postStart);
  fsm.addState(STATE_LIGHT_1_GREEN,  preLight1Green, loopLight1Green, postLight1Green);
  fsm.addState(STATE_LIGTH_1_ORANGE, preLight1Orange, loopLight1Orange, postLight1Orange);
  fsm.addState(STATE_LIGTH_1_RED,    preLight1Red, loopLight1Red, postLight1Red);
  fsm.addState(STATE_LIGHT_2_GREEN,  preLight2Green, loopLight2Green, postLight2Green);
  fsm.addState(STATE_LIGTH_2_ORANGE, preLight2Orange, loopLight2Orange, postLight2Orange);
  fsm.addState(STATE_LIGTH_2_RED,    preLight2Red, loopLight2Red, postLight2Red);
  fsm.addState(STATE_OUT_OF_ORDER,   preOutOfOrder, loopOutOfOrder, postOutOfOrder);

  // Add the events to the FSM
  fsm.addTransition(STATE_START, EVENT_START, STATE_LIGHT_1_GREEN);
  fsm.addTransition(STATE_LIGHT_1_GREEN, EVENT_TIMER, STATE_LIGTH_1_ORANGE); // Demo, auto to ligth 2
  fsm.addTransition(STATE_LIGHT_1_GREEN, EVENT_BUTTON_2, STATE_LIGTH_1_ORANGE);
  fsm.addTransition(STATE_LIGTH_1_ORANGE, EVENT_TIMER, STATE_LIGTH_1_RED);
  fsm.addTransition(STATE_LIGTH_1_RED, EVENT_TIMER, STATE_LIGHT_2_GREEN);
  fsm.addTransition(STATE_LIGHT_2_GREEN, EVENT_TIMER, STATE_LIGTH_2_ORANGE); // Demo, auto to light 1
  fsm.addTransition(STATE_LIGHT_2_GREEN, EVENT_BUTTON_1, STATE_LIGTH_2_ORANGE);
  fsm.addTransition(STATE_LIGTH_2_ORANGE, EVENT_TIMER, STATE_LIGTH_2_RED);
  fsm.addTransition(STATE_LIGTH_2_RED, EVENT_TIMER, STATE_LIGHT_1_GREEN);

  fsm.addTransition(STATE_LIGHT_1_GREEN,  EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGHT_1_GREEN,  EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGTH_1_ORANGE, EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGTH_1_RED,    EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGHT_2_GREEN,  EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGHT_2_GREEN,  EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGTH_2_ORANGE, EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_LIGTH_2_RED,    EVENT_OUT_OF_ORDER, STATE_OUT_OF_ORDER);
  fsm.addTransition(STATE_OUT_OF_ORDER,   EVENT_START,        STATE_LIGHT_1_GREEN);

  fsm.addTransition(STATE_LIGHT_1_GREEN,  EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGHT_1_GREEN,  EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGTH_1_ORANGE, EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGTH_1_RED,    EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGHT_2_GREEN,  EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGHT_2_GREEN,  EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGTH_2_ORANGE, EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_LIGTH_2_RED,    EVENT_ERROR, STATE_START);
  fsm.addTransition(STATE_OUT_OF_ORDER,   EVENT_ERROR, STATE_START);

  WiFi.begin("MaCMaN_GUEST", "GUEST@MACMAN"); // Connect to the Wi-Fi (if not known use WifiManager from tzapu!)
  Serial.print("Setup Wi-Fi:");
  while ( WiFi.status() != WL_CONNECTED ) {
      delay(500);
      Serial.print(".");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.println();
  Serial.print("Connected:");
  Serial.println(WiFi.localIP());

  mqtt.setClient(client); // Setup the MQTT client
  mqtt.setBufferSize(256); // override MQTT_MAX_PACKET_SIZE
  mqtt.setCallback(callbackMQTT);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  connectMQTT();

  fsm.setup(STATE_START, EVENT_STATE_EXECUTED);
  timerMQTT = millis();
}

// Ardiuno loop
void loop() {   
  fsm.loop();

  if ( millis() - timerMQTT > 5000 ) { // Sent every five second the loop timing of the wemos device
    payload = "{ \"id\": \"maurice\", \"looptiming\": " + String(fsm.getLoopTime()) + "}";
    mqtt.publish("iwsn-wemos", payload.c_str());

    if ( WiFi.status() != WL_CONNECTED || !mqtt.connected() ) { // If we are not connected, raise an error
      fsm.raiseEvent(EVENT_ERROR);
    }

    timerMQTT = millis();
  }

  mqtt.loop();
}

void preStart() {
  digitalWrite(LIGHT_1_RED,    LOW);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
}

void loopStart() {
  if ( WiFi.status() == WL_CONNECTED ) {
    if ( !mqtt.connected() ) {
      connectMQTT();

    } else {
      payload = "{ \"id\": \"maurice\", \"mqtt\": \"iwsn-wemos-client-maurice\", \"ip\": \"" + WiFi.localIP().toString() + "\"}";
      mqtt.publish("iwsn-wemos", payload.c_str());
      fsm.raiseEvent(EVENT_START);
    }

  } else {
    Serial.println("WiFi reconnection");
    WiFi.begin("MaCMaN_GUEST", "GUEST@MACMAN"); // Connect to the Wi-Fi (if not known use WifiManager from tzapu!)
    //WiFi.reconnect();
    for ( unsigned int i=0; i < 20; ++i ) {
      if ( WiFi.status() != WL_CONNECTED ) {
        delay(1000);
        Serial.print(".");
          Serial.println();
          Serial.print("Connected:");
          Serial.println(WiFi.localIP());

      } else {
        i = 20;
      }
    }
  }
}

void postStart() {
}

void preLight1Green() {
  digitalWrite(LIGHT_1_RED,    HIGH);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  LOW);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"light1 green\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight1Green() {
  if ( millis() - timer > 15000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }

  if ( digitalRead(BUTTON_2) == LOW ) {
    fsm.raiseEvent(EVENT_BUTTON_2);
  }
}

void postLight1Green() {
}

void preLight1Orange() {
  digitalWrite(LIGHT_1_RED,    HIGH);
  digitalWrite(LIGHT_1_ORANGE, LOW);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"light1 orange\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight1Orange() {
  if ( millis() - timer > 2000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }
}

void postLight1Orange() {

}

void preLight1Red() {
  digitalWrite(LIGHT_1_RED,    LOW);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"light1 red\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight1Red() {
  if ( millis() - timer > 2000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }
}

void postLight1Red() {

}

void preLight2Green() {
  digitalWrite(LIGHT_1_RED,    LOW);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    HIGH);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  LOW);
  payload = "{ \"id\": \"maurice\", \"state\": \"light2 green\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight2Green() {
  if ( millis() - timer > 15000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }

  if ( digitalRead(BUTTON_1) == LOW ) {
    fsm.raiseEvent(EVENT_BUTTON_1);
  }
}

void postLight2Green() {

}

void preLight2Orange() {
  digitalWrite(LIGHT_1_RED,    LOW);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    HIGH);
  digitalWrite(LIGHT_2_ORANGE, LOW);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"light2 orange\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight2Orange() {
  if ( millis() - timer > 2000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }
}

void postLight2Orange() {

}

void preLight2Red() {
  digitalWrite(LIGHT_1_RED,    LOW);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"light2 red\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopLight2Red() {
  if ( millis() - timer > 2000 ) {
    fsm.raiseEvent(EVENT_TIMER);
  }
}

void postLight2Red() {

}

void preOutOfOrder() {
  digitalWrite(LIGHT_1_RED,    HIGH);
  digitalWrite(LIGHT_1_ORANGE, LOW);
  digitalWrite(LIGHT_1_GREEN,  HIGH);
  digitalWrite(LIGHT_2_RED,    HIGH);
  digitalWrite(LIGHT_2_ORANGE, LOW);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  payload = "{ \"id\": \"maurice\", \"state\": \"out of order\" }";
  mqtt.publish("iwsn-wemos", payload.c_str());
  timer = millis();
}

void loopOutOfOrder() {
  if ( millis() - timer > 1000 ) {
    digitalWrite(LIGHT_1_ORANGE, !digitalRead(LIGHT_1_ORANGE));
    digitalWrite(LIGHT_2_ORANGE, !digitalRead(LIGHT_2_ORANGE));
    timer = millis();
  }
}

void postOutOfOrder() {

}
