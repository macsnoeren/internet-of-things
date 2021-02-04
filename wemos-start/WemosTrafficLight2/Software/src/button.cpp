#include <button.h>

#include <Arduino.h>

Button::Button(int pin, std::function<void(BUTTON_EVENT)> callback) : pin(pin), callback(callback), pressed(LOW), timeLongDown(5000), timeDoubleClick(500) {
}

void Button::setup() {
    if ( digitalRead(this->pin) == this->pressed ) { // Set the start state
        this->state = BUTTON_STATE_DOWN;
        this->buttonState = this->pressed;

    } else {
        this->state = BUTTON_STATE_UP;
        this->buttonState = !this->pressed;
    }
    this->timer = millis();
}

void Button::loop() {
    switch (this->state)
    {
    case BUTTON_STATE_DOWN:
        this->stateDown();
        break;
    case BUTTON_STATE_UP:
        this->stateUp();
        break;
    case BUTTON_STATE_LONG_DOWN:
        this->stateLongDown();
        break;
    case BUTTON_STATE_DOUBLE_CLICK:
        this->stateDoubleClick();
        break;
    
    default:
        if ( digitalRead(this->pin) == this->pressed ) { // Set the start state
           this->state = BUTTON_STATE_DOWN;

        } else {
            this->state = BUTTON_STATE_UP;
        }
        this->timer = millis();
    }

    // get the state of the button debounce it.
    // todo
}

void Button::setPressedState(int pressed) {
    this->pressed = pressed;
}

void Button::setTimeLongDown(int time) {
    this->timeLongDown = time;
}

void Button::setTimeDoubleClick(int time) {
    this->timeDoubleClick = time;
}
        
void Button::stateDown() {
    if ( millis() - this->timer > this->timeLongDown ) {
        this->callback(BUTTON_LONG_DOWN); // callback for long press
        this->state = BUTTON_STATE_LONG_DOWN;
        this->timer = millis();
    }

    if ( this->buttonState == !this->pressed ) { // button up
        this->callback(BUTTON_UP);
        this->state = BUTTON_STATE_UP;
        this->timer = millis();
    }
}

void Button::stateUp() {
    if ( this->buttonState == this->pressed && (millis() - this->timer) >= this->timeDoubleClick ) { // button down after double click timer
        this->callback(BUTTON_DOWN);
        this->state = BUTTON_STATE_DOWN;
        this->timer = millis();
    }

    if ( this->buttonState == this->pressed && (millis() - this->timer) < this->timeDoubleClick ) { // button down before double click timer
        this->callback(BUTTON_DOUBLE_CLICK);
        this->state = BUTTON_STATE_DOUBLE_CLICK;
        this->timer = millis();
    }
}

void Button::stateLongDown() {
    if ( this->buttonState == !this->pressed ) {
        this->callback(BUTTON_UP);
        this->state = BUTTON_STATE_UP;
        this->timer = millis();
    }
}

void Button::stateDoubleClick() {
    if ( this->buttonState == !this->pressed ) {
        this->callback(BUTTON_UP);
        this->state = BUTTON_STATE_UP;
        this->timer = millis();
    }
}
