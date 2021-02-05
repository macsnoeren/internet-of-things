#pragma once

#include <Arduino.h>

class Button {
  private:
    int buttonPin;
    int buttonState;
    int lastButtonState;   // the previous reading from the input pin
    void (*callback)(int buttonPin, int state);

    unsigned long lastDebounceTime;  // the last time the output pin was toggled
    unsigned long debounceDelay;    // the debounce time; increase if the output flickers

  public:
    Button(int buttonPin, void (*callback)(int buttonPin, int state));
    void setup();
    void loop();
};