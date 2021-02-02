#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Create the wifi-client to connect to the Internet
WiFiClient client;

// Create the MQTT client to connect to a MQTT Broker (server)
PubSubClient mqtt;

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
constexpr int    MQTT_PORT   = 1883;

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
  Serial.println();

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


}

void loop() {
  mqtt.loop();
}