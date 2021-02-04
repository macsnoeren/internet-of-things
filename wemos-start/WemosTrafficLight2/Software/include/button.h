#pragma once

#include <functional>

// Finite state machine
enum BUTTON_FSM_STATE {
  BUTTON_STATE_DOWN,
  BUTTON_STATE_UP,
  BUTTON_STATE_LONG_DOWN,
  BUTTON_STATE_DOUBLE_CLICK,
  BUTTON_STATES_TOTAL
};

// Finite state machine events
enum FSM_EVENT {
  BUTTON_EVENT_DOWN,
  BUTTON_EVENT_UP,
  BUTTON_EVENT_TIMER,
  BUTTON_EVENTS_TOTAL
};

// Callback events
enum BUTTON_EVENT {
    BUTTON_DOWN,
    BUTTON_UP,
    BUTTON_CLICK,
    BUTTON_DOUBLE_CLICK,
    BUTTON_LONG_DOWN
};

class Button {
    private:
        int pin;
        std::function<void(BUTTON_EVENT)> callback;
        BUTTON_FSM_STATE state;
        int pressed;
        unsigned long timer;
        unsigned long timeLongDown;
        unsigned long timeDoubleClick;
        int buttonState;

    public:
        Button(int pin, std::function<void(BUTTON_EVENT)> callback);

        void setup();
        void loop();

        void setPressedState(int pressed);
        void setTimeLongDown(int time);
        void setTimeDoubleClick(int time);

        void stateDown();
        void stateUp();
        void stateLongDown();
        void stateDoubleClick();
};

