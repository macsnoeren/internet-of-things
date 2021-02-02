#include <Arduino.h>
#include <fsm.h>
#include <fsmstates.h>

FSM fsm(STATES_TOTAL, EVENTS_TOTAL);
StateStart stateStart();
StateLight1Green stateLight1Green();
StateLight1Orange stateLight1Orange();
StateLight1Red stateLight1Red();
StateLight2Green stateLight2Green();
StateLight2Orange stateLight2Orange();
StateLight2Red stateLight2Red();

FSMState* test = &stateStart;

void setup() {
  
  fsm.addState(STATE_START, (FSMState*) &stateStart);
  fsm.addState(STATE_LIGHT_1_GREEN, &stateLight1Green);
  fsm.addState(STATE_LIGTH_1_ORANGE, &stateLight1Orange);
  fsm.addState(STATE_LIGTH_1_RED, &stateLight1Red);
  fsm.addState(STATE_LIGHT_2_GREEN, &stateLight2Green);
  fsm.addState(STATE_LIGTH_2_ORANGE, &stateLight2Orange);
  fsm.addState(STATE_LIGTH_2_RED, &stateLight2Red);

  fsm.addTransition(STATE_START, EVENT_STATE_EXECUTED, STATE_LIGHT_1_GREEN);
  fsm.addTransition(STATE_LIGHT_1_GREEN, EVENT_BUTTON_2, STATE_LIGTH_1_ORANGE);
  fsm.addTransition(STATE_LIGTH_1_ORANGE, EVENT_TIMER, STATE_LIGTH_1_RED);
  fsm.addTransition(STATE_LIGTH_1_RED, EVENT_TIMER, STATE_LIGHT_2_GREEN);
  fsm.addTransition(STATE_LIGHT_2_GREEN, EVENT_BUTTON_1, STATE_LIGTH_2_ORANGE);
  fsm.addTransition(STATE_LIGTH_2_ORANGE, EVENT_TIMER, STATE_LIGTH_2_RED);
  fsm.addTransition(STATE_LIGTH_2_RED, EVENT_TIMER, STATE_LIGHT_1_GREEN);

  fsm.setup(STATE_START);
}

void loop() {
  fsm.loop();
}