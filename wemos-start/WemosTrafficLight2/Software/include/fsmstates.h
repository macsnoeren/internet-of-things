#pragma once

#include <fsm.h>

enum FSM_STATES {
  STATE_START,
  STATE_LIGHT_1_GREEN,
  STATE_LIGTH_1_ORANGE,
  STATE_LIGTH_1_RED,
  STATE_LIGHT_2_GREEN,
  STATE_LIGTH_2_ORANGE,
  STATE_LIGTH_2_RED,
  STATES_TOTAL
};

enum FSM_EVENTS {
  EVENT_BUTTON_1,
  EVENT_BUTTON_2,
  EVENT_TIMER,
  EVENT_STATE_EXECUTED, // Add this one always!
  EVENTS_TOTAL
};

class StateStart : public FSMState {
    public:
        StateStart();
        void pre();
        void loop();
        void post();
};

class StateLight1Green : public FSMState {
    public:
        StateLight1Green();
        void pre();
        void loop();
        void post();
};

class StateLight1Orange : public FSMState {
    public:
        StateLight1Orange();
        void pre();
        void loop();
        void post();
};

class StateLight1Red : public FSMState {
    public:
        StateLight1Red();
        void pre();
        void loop();
        void post();
};

class StateLight2Green : public FSMState {
    public:
        StateLight2Green();
        void pre();
        void loop();
        void post();
};

class StateLight2Orange : public FSMState {
    public:
        StateLight2Orange();
        void pre();
        void loop();
        void post();
};

class StateLight2Red : public FSMState {
    public:
        StateLight2Red();
        void pre();
        void loop();
        void post();
};
