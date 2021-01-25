#include "fsm.h"
#include <Arduino.h>
#include <time.h>

unsigned int const STATES_TOTAL = 6;
unsigned int const EVENTS_TOTAL = 5;

// Current state and last event that has been raised
ENUM_STATE state = STATE_START;
ENUM_EVENT event = EVENT_STATE_EXECUTED;

// Define your transition table here. States given in the collomn and events in de row.
//
// ----------------------------------------------------------------------------------------------------
//      EVENTS| GREEN           | RED             | OUTOFORDER      | TIMER          | STATE_EXECUTED |
// ----------------------------------------------------------------------------------------------------
// START      | -               | -               | -               | -              | RED            |      
// RED        | ORANGE          | -               | OUTOFORDER      | -              | -              |
// ORANGE     | -               | -               | -               | GREEN          | -              |
// GREEN      | -               | RED             | -               | RED            | -              |
// OUTOFORDER | -               | RED             | -               | -              | -              |
// ----------------------------------------------------------------------------------------------------
STATE_TRANSITION_STRUCT fsm[STATES_TOTAL][EVENTS_TOTAL] = { // TODO: Dit kan misschien beter, struct voor de state functies en een struct state transitions.
  { // STATE_START
    {start_pre, start_loop, start_post, STATE_INVALID_TRANSITION},
    {start_pre, start_loop, start_post, STATE_INVALID_TRANSITION},
    {start_pre, start_loop, start_post, STATE_INVALID_TRANSITION},
    {start_pre, start_loop, start_post, STATE_INVALID_TRANSITION},
    {start_pre, start_loop, start_post, STATE_RED}
  },
  { // STATE_RED 
    {red_pre, red_loop, red_post, STATE_ORANGE},
    {red_pre, red_loop, red_post, STATE_INVALID_TRANSITION},
    {red_pre, red_loop, red_post, STATE_OUTOFORDER},
    {red_pre, red_loop, red_post, STATE_INVALID_TRANSITION},
    {red_pre, red_loop, red_post, STATE_INVALID_TRANSITION}
  }, 
  { // STATE_ORANGE
    {orange_pre, orange_loop, orange_post, STATE_INVALID_TRANSITION},
    {orange_pre, orange_loop, orange_post, STATE_INVALID_TRANSITION},
    {orange_pre, orange_loop, orange_post, STATE_INVALID_TRANSITION},
    {orange_pre, orange_loop, orange_post, STATE_GREEN},
    {orange_pre, orange_loop, orange_post, STATE_INVALID_TRANSITION}
  },
  { // STATE_GREEN
    {green_pre, green_loop, green_post, STATE_INVALID_TRANSITION},
    {green_pre, green_loop, green_post, STATE_RED},
    {green_pre, green_loop, green_post, STATE_INVALID_TRANSITION},
    {green_pre, green_loop, green_post, STATE_RED},
    {green_pre, green_loop, green_post, STATE_INVALID_TRANSITION}
  },
  { // STATE_OUTOFORDER
    {outoforder_pre, outoforder_loop, outoforder_post, STATE_INVALID_TRANSITION},
    {outoforder_pre, outoforder_loop, outoforder_post, STATE_RED},
    {outoforder_pre, outoforder_loop, outoforder_post, STATE_INVALID_TRANSITION},
    {outoforder_pre, outoforder_loop, outoforder_post, STATE_INVALID_TRANSITION},
    {outoforder_pre, outoforder_loop, outoforder_post, STATE_INVALID_TRANSITION}
  }
};

time_t timestamp1;
time_t timestamp2;

void FSMInit () {
  Serial.println("FSM Init");
  if( fsm[state][event].pre != NULL) {
    fsm[state][event].pre() ;
  } 
}

void FSMLoop () {
  if( fsm[state][event].loop != NULL) {
    fsm[state][event].loop() ;
  }

  FSMRaiseEvent(EVENT_STATE_EXECUTED);
}

void FSMRaiseEvent (ENUM_EVENT new_event) {
    ENUM_STATE new_state = fsm[state][new_event].nextState;
    if ( new_state != STATE_INVALID_TRANSITION ) {
        // call event.post
        if( fsm[state][event].post != NULL) {
            fsm[state][event].post() ;
        } 
        
        // Set new state
        ENUM_STATE new_state = fsm[state][new_event].nextState;
        
        // call newstate ev.pre
        if( fsm[new_state][new_event].pre != NULL) {
            fsm[new_state][new_event].pre() ;
        } 
        
        // Set new state and event
        state = new_state;
        event = new_event;
    }
}

void start_pre() {
    Serial.println("start_pre");
}

void start_loop() {
    Serial.println("start_loop");
}

void start_post() {
    Serial.println("start_post");
}

void red_pre() {
    Serial.println("red_pre");
    timestamp1 = millis(); // initialize the timestamp
    timestamp2 = millis(); // initialize the timestamp
}


void red_loop() {
    if ( millis() - timestamp1 > 500 ) {
        digitalWrite(LED_BUILTIN, (digitalRead(LED_BUILTIN) ? LOW : HIGH));
        timestamp1 = millis();
    }

    if ( millis() - timestamp2 > 10000 ) { // simulate stimuli GREEN
        //FSMRaiseEvent(EVENT_OUTOFORDER);
        FSMRaiseEvent(EVENT_GREEN);
    }

    Serial.println("red_loop");
}

void red_post() {
    Serial.println("red_post");
}

void orange_pre() {
    Serial.println("orange_pre");
    timestamp1 = millis(); // initialize the timestamp
    timestamp2 = millis(); // initialize the timestamp
}

void orange_loop() {
    if ( millis() - timestamp1 > 2000 ) {
        digitalWrite(LED_BUILTIN, (digitalRead(LED_BUILTIN) ? LOW : HIGH));
        timestamp1 = millis();
    }

    if ( millis() - timestamp2 > 10000 ) {
        FSMRaiseEvent(EVENT_TIMER);
    }

    Serial.println("orange_loop");
}

void orange_post() {
    Serial.println("orange_post");
}

void green_pre() {
    Serial.println("green_pre");
    timestamp1 = millis(); // initialize the timestamp
    timestamp2 = millis(); // initialize the timestamp
    digitalWrite(LED_BUILTIN, LOW);
}

void green_loop() {
    if ( millis() - timestamp1 > 10000 ) {
        FSMRaiseEvent(EVENT_TIMER);
    }

    Serial.println("green_loop");
}

void green_post() {
    Serial.println("green_post");
}

void outoforder_pre() {
    Serial.println("outoforder_pre");
    timestamp1 = millis(); // initialize the timestamp
    timestamp2 = millis(); // initialize the timestamp
}

void outoforder_loop() {
    if ( millis() - timestamp1 > 500 ) {
        digitalWrite(LED_BUILTIN, LOW);
    }

    if ( millis() - timestamp1 > 700 ) {
        digitalWrite(LED_BUILTIN, HIGH);
        timestamp1 = millis();
    }

    Serial.println("outoforder_loop");
}

void outoforder_post() {
    Serial.println("outoforder_post");
}
