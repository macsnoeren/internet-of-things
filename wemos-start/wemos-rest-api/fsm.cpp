#include "fsm.h"
#include <Arduino.h>
#include <time.h>

#include <rest.h>

unsigned int const STATES_TOTAL = 6;
unsigned int const EVENTS_TOTAL = 5;

// Current state and last event that has been raised
ENUM_STATE state = STATE_START;
ENUM_EVENT event = EVENT_STATE_EXECUTED;

// Define the state methods
STATE_METHODS_STRUCT fsm_state_methods[STATES_TOTAL] = {
    {start_pre, start_loop, start_post},
    {red_pre, red_loop, red_post},
    {orange_pre, orange_loop, orange_post},
    {green_pre, green_loop, green_post},
    {outoforder_pre, outoforder_loop, outoforder_post}
};

// Define your transition table here. States given in the collomn and events in de row.
//
// ----------------------------------------------------------------------------------------------------
//      EVENTS| GREEN           | RED             | OUTOFORDER      | TIMER          | STATE_EXECUTED |
// ----------------------------------------------------------------------------------------------------
// START      | -               | -               | -               | -              | RED            |      
// RED        | ORANGE          | -               | OUTOFORDER      | -              | -              |
// ORANGE     | -               | -               | OUTOFORDER      | GREEN          | -              |
// GREEN      | -               | RED             | OUTOFORDER      | RED            | -              |
// OUTOFORDER | -               | RED             | -               | -              | -              |
// ----------------------------------------------------------------------------------------------------
STATE_TRANSITION_STRUCT fsm_state_transition[STATES_TOTAL][EVENTS_TOTAL] = {
  { // STATE_START
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION},
    {STATE_RED}
  },
  { // STATE_RED 
    {STATE_ORANGE},
    {STATE_INVALID_TRANSITION},
    {STATE_OUTOFORDER},
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION}
  }, 
  { // STATE_ORANGE
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION},
    {STATE_OUTOFORDER},
    {STATE_GREEN},
    {STATE_INVALID_TRANSITION}
  },
  { // STATE_GREEN
    {STATE_INVALID_TRANSITION},
    {STATE_RED},
    {STATE_OUTOFORDER},
    {STATE_RED},
    {STATE_INVALID_TRANSITION}
  },
  { // STATE_OUTOFORDER
    {STATE_INVALID_TRANSITION},
    {STATE_RED},
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION},
    {STATE_INVALID_TRANSITION}
  }
};

time_t timestamp1;
time_t timestamp2;

void FSMInit () {
  Serial.println("FSM Init");
  if( fsm_state_methods[state].pre != NULL) {
    fsm_state_methods[state].pre() ;
  } 
}

void FSMLoop () {
  if( fsm_state_methods[state].loop != NULL) {
    fsm_state_methods[state].loop() ;
  }

  FSMRaiseEvent(EVENT_STATE_EXECUTED);
}

String getEventName (ENUM_EVENT event) {
    switch (event) {
    case EVENT_OUTOFORDER:
        return "EVENT_OUTOFORDER";
    case EVENT_GREEN:
        return "EVENT_GREEN";
    case EVENT_RED:
        return "EVENT_RED";
    case EVENT_TIMER:
        return "EVENT_TIMER";
    case EVENT_STATE_EXECUTED:
        return "EVENT_STATE_EXECUTED";
    default:
        return "UNKNOWN";
    }
}

void FSMRaiseEvent (ENUM_EVENT new_event) {
    ENUM_STATE new_state = fsm_state_transition[state][new_event].nextState;
    if ( new_state != STATE_INVALID_TRANSITION ) {
        RESTGetStatus("ET: " + getEventName(new_event));

        // call event.post
        if( fsm_state_methods[state].post != NULL) {
            fsm_state_methods[state].post() ;
        } 
        
        // call newstate ev.pre
        if( fsm_state_methods[new_state].pre != NULL) {
            fsm_state_methods[new_state].pre() ;
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
    Serial.println("red_loop");

    if ( millis() - timestamp1 > 500 ) {
        digitalWrite(LED_BUILTIN, (digitalRead(LED_BUILTIN) ? LOW : HIGH));
        timestamp1 = millis();
    }

    if ( millis() - timestamp2 > 10000 ) { // simulate stimuli GREEN
        //FSMRaiseEvent(EVENT_OUTOFORDER);
        FSMRaiseEvent(EVENT_GREEN);
    }
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
    Serial.println("orange_loop");
    
    if ( millis() - timestamp1 > 2000 ) {
        digitalWrite(LED_BUILTIN, (digitalRead(LED_BUILTIN) ? LOW : HIGH));
        timestamp1 = millis();
    }

    if ( millis() - timestamp2 > 10000 ) {
        FSMRaiseEvent(EVENT_TIMER);
    }
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
    Serial.println("green_loop");
    
    if ( millis() - timestamp1 > 10000 ) {
        FSMRaiseEvent(EVENT_TIMER);
    }
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
    Serial.println("outoforder_loop");
    
    if ( millis() - timestamp1 > 1000 ) {
        digitalWrite(LED_BUILTIN, LOW);
    }

    if ( millis() - timestamp1 > 1100 ) {
        digitalWrite(LED_BUILTIN, HIGH);
        timestamp1 = millis();
    }
}

void outoforder_post() {
    Serial.println("outoforder_post");
}
