#include <Arduino.h>
#include "fsm.h"

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);

  Serial.println("Setup completed!");

  FSMInit();
}

void loop() {
  FSMLoop();
}