#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <maurice.h>

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

//FSM fsm(STATES_TOTAL, EVENTS_TOTAL);
Maurice maurice(STATES_TOTAL, EVENTS_TOTAL);

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

// External Events!
void callbackMQTT(char* topic, byte* pl, unsigned int length) {
  String payload = "";
  for (unsigned int i=0;i<length;i++) {
    payload += (char)pl[i];
  }
  Serial.println("MQTT EVENT '" + String(topic) + "': " + payload);
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
  maurice.addState(STATE_START,          preStart, loopStart, postStart);
  maurice.addState(STATE_LIGHT_1_GREEN,  preLight1Green, loopLight1Green, postLight1Green);
  maurice.addState(STATE_LIGTH_1_ORANGE, preLight1Orange, loopLight1Orange, postLight1Orange);
  maurice.addState(STATE_LIGTH_1_RED,    preLight1Red, loopLight1Red, postLight1Red);
  maurice.addState(STATE_LIGHT_1_GREEN,  preLight2Green, loopLight2Green, postLight2Green);
  maurice.addState(STATE_LIGTH_1_ORANGE, preLight2Orange, loopLight2Orange, postLight2Orange);
  maurice.addState(STATE_LIGTH_1_RED,    preLight2Red, loopLight2Red, postLight2Red);
  maurice.addState(STATE_OUT_OF_ORDER,   preOutOfOrder, loopOutOfOrder, postOutOfOrder);

  // Add the events to the FSM
  maurice.addTransition(STATE_START, EVENT_STATE_EXECUTED, STATE_LIGHT_1_GREEN);
  maurice.addTransition(STATE_LIGHT_1_GREEN, EVENT_TIMER, STATE_LIGTH_1_ORANGE);
  maurice.addTransition(STATE_LIGTH_1_ORANGE, EVENT_TIMER, STATE_LIGTH_1_RED);
  maurice.addTransition(STATE_LIGTH_1_RED, EVENT_TIMER, STATE_LIGHT_2_GREEN);

  // Connect to the Wi-Fi (if not known use WifiManager from tzapu!)
  WiFi.begin("MaCMaN_GUEST", "GUEST@MACMAN"); // Connect with the Wi-Fi
  Serial.print("Setup Wi-Fi:");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected:");
  Serial.println(WiFi.localIP());

  // Setup the MQTT client
  mqtt.setClient(client);
  mqtt.setBufferSize(2048); // override MQTT_MAX_PACKET_SIZE
  mqtt.setCallback(callbackMQTT);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.connect("wemos-client-1234");
  mqtt.subscribe("wemos-event-1234");

  if ( mqtt.connected() ) {
    Serial.println("MQTT CONNECTED!");
  } else {
    Serial.println("MQTT NOT CONNECTED!");
  }

  maurice.setup(STATE_START, EVENT_STATE_EXECUTED);
}

void loop() {
  mqtt.loop();
  //fsm.loop();
  maurice.loop();
}

void preStart() {
  Serial.println("Pre Start");
  digitalWrite(LIGHT_1_RED,    LOW); // Ligth 1 and 2 turn to red
  digitalWrite(LIGHT_2_RED,    LOW);
}

void loopStart() {
  Serial.println("Loop start");
}

void postStart() {
  Serial.println("Post start");
}

void preLight1Green() {
  Serial.println("Pre Ligt1Green");
  digitalWrite(LIGHT_1_RED,    HIGH);
  digitalWrite(LIGHT_1_ORANGE, HIGH);
  digitalWrite(LIGHT_1_GREEN,  LOW);
  digitalWrite(LIGHT_2_RED,    LOW);
  digitalWrite(LIGHT_2_ORANGE, HIGH);
  digitalWrite(LIGHT_2_GREEN,  HIGH);
  timer = millis();
}

void loopLight1Green() {
  Serial.println("Loop Ligt1Green");
  if ( millis() - timer > 5000 ) {
    maurice.raiseEvent(EVENT_TIMER);
  }
}

void postLight1Green() {
  Serial.println("Post Ligt1Green");
}

void preLight1Orange() {
  timer = millis();
}

void loopLight1Orange() {
  if ( millis() - timer > 5000 ) {
    maurice.raiseEvent(EVENT_TIMER);
  }
}

void postLight1Orange() {

}

void preLight1Red() {
    
}

void loopLight1Red() {

}

void postLight1Red() {

}

void preLight2Green() {

}

void loopLight2Green() {

}

void postLight2Green() {

}

void preLight2Orange() {

}

void loopLight2Orange() {

}

void postLight2Orange() {

}

void preLight2Red() {

}

void loopLight2Red() {

}

void postLight2Red() {

}

void preOutOfOrder() {

}

void loopOutOfOrder() {

}

void postOutOfOrder() {

}
