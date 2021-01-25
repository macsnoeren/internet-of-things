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

// FSM first implementation
// RAM:   [===       ]  33.7% (used 27600 bytes from 81920 bytes)
// Flash: [===       ]  25.3% (used 263940 bytes from 1044464 bytes)

// FSM second implementation
// RAM:   [===       ]  33.3% (used 27312 bytes from 81920 bytes)
// Flash: [===       ]  25.2% (used 263604 bytes from 1044464 bytes)