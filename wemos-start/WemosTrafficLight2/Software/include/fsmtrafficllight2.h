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
  STATE_OUT_OF_ORDER,
  STATES_TOTAL
};

enum FSM_EVENTS {
  EVENT_BUTTON_1,
  EVENT_BUTTON_2,
  EVENT_TIMER,
  EVENT_STATE_EXECUTED, // Add this one always!
  EVENTS_TOTAL
};

class FSMTrafficLight2 : public FSM {

  private:

  public:
    FSMTrafficLight2();
    ~FSMTrafficLight2();

    static void preStart();
    static void loopStartloop();
    static void postStart();

    static void preLight1Green();
    static void loopLight1Green();
    static void postLight1Green();

    static void preLight1Orange();
    static void loopLight1Orange();
    static void postLight1Orange();

    static void preLight1Red();
    static void loopLight1Red();
    static void postLight1Red();

    static void preLight2Green();
    static void loopLight2Green();
    static void postLight2Green();

    static void preLight2Orange();
    static void loopLight2Orange();
    static void postLight2Orange();

    static void preLight2Red();
    static void loopLight2Red();
    static void postLight2Red();

    static void preOutOfOrder();
    static void loopOutOfOrder();
    static void postOutOfOrder();
};

