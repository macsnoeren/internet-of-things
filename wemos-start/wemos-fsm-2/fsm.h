// Finite State Machine Implementation
// Based on SmartMeter source base of Diederich Kroeske
// Improvement of the FSM to go to a pure FSM implementation with a clean main loop
// Author: Maurice Snoeren
//
#pragma once

/* Define your FSM states here */
typedef enum {  STATE_START,
                STATE_RED,
                STATE_ORANGE,
                STATE_GREEN,
                STATE_OUTOFORDER,
                STATE_INVALID_TRANSITION, // Keep this: required to define an non existing state transition (do not count it as state!!)
} ENUM_STATE;

/* Define your FSM events here */
typedef enum {  EVENT_GREEN, 
                EVENT_RED, 
                EVENT_OUTOFORDER,
                EVENT_TIMER,
                EVENT_STATE_EXECUTED, // Keep this: required to define a state transition when the pre and loop method has been executed
} ENUM_EVENT;

/* Define FSM transition structure (advanced using pre and post methods) */
typedef struct {
   void (*pre)(void);  // When the state is active, this method is called once at start
   void (*loop)(void); // When the state is active, this method is continously called
   void (*post)(void); // When the state is deactivated, this method is called once
} STATE_METHODS_STRUCT;

/* Define FSM transition structure (advanced using pre and post methods) */
typedef struct {
   ENUM_STATE nextState;
} STATE_TRANSITION_STRUCT;

void FSMInit ();
void FSMLoop ();
void FSMRaiseEvent (ENUM_EVENT new_event);

// Forward declaration of the transition methods.
void start_pre();
void start_loop();
void start_post();
void red_pre();
void red_loop();
void red_post();
void orange_pre();
void orange_loop();
void orange_post();
void green_pre();
void green_loop();
void green_post();
void outoforder_pre();
void outoforder_loop();
void outoforder_post();
