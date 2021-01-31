# First example: Blinking the built-in led!
# Copy the code to your project.
# Author: Maurice Snoeren

#include <Arduino.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Serial.println("Setup completed!");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);

}