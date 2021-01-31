#include <button.h>

#include <Arduino.h>

Button::Button(int buttonPin, void (*callback)(void)) {
    this->lastButtonState  = LOW; // the previous reading from the input pin
    this->lastDebounceTime = 0;    // the last time the output pin was toggled
    this->debounceDelay    = 50;   // the debounce time; increase if the output flickers
    this->callback         = callback;
}

void Button::setup() {
  pinMode(this->buttonPin, INPUT);
}

void Button::loop() {
  int reading = digitalRead(buttonPin);

  if (reading != this->lastButtonState) {
    this->lastDebounceTime = millis();
  }

  if ((millis() - this->lastDebounceTime) > this->debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
          this->callback();
      }
    }
  }

  this->lastButtonState = reading;
}
