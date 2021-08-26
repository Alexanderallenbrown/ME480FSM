/*! \file ME480FSM.cpp */

#include "Arduino.h"
#include "ME480FSM.h"

/*!
   @brief   This function runs when you "construct" a rising edge counter
  
   At construction:
   - preset will be set to the given value<br>
   - The saved values of in_Up and in_Down are initialized to false<br>
   - The count is set to 0<br>
   - The counter is set to the waiting state
   .
   @return  RisingEdgeCounter object.
   @param   _preset (int) Sets the preset count when the state of the counter changes
 */
RisingEdgeCounter::RisingEdgeCounter(long _preset)
{
    //when we create our counter, the only unique thing is the preset.
    //set initial values for all other variables we need as necessary here as well.
    preset = _preset;
    state_Waiting = true;
    state_Up = false;
    state_Down = false;
    state_RST = false;
    in_Up_Old = false;
    in_Down_Old = false;
    count = 0;
}

/*!
   @brief   Deallocates the RisingEdgeCounter object
 */
RisingEdgeCounter::~RisingEdgeCounter(){}

/*!
   @brief   This function updates the counter based on a change in the rising edge of the input variables or the value of the RST variable.

   If the up or down input sense a change from false to true (rising edge) the counter will increment or 
   decrement. 
   - The counter value is limited to positive values. 
   - Decrementing when the counter is at 0 will not change the value of the counter
   - When the count is greater than, or equal to, the preset CNT will be set to true
   .
   If RST is true the counter will reset to 0. This does not need a transition change

   @return  nothing

   @param   in_Up (bool) a false to true transition in this variable will increment the counter
   @param   in_Down (bool) a false to true transition in this variable will decrement the counter
   @param   RST (bool) true will reset the counter to 0, does not need a false to true transition

   @note    a reset value overrides an up or down transition.<br> 
   If up and down are both rising edges during the same call to the function, the counter does 
   not change the count and waits for the next false-true transition
 */
void RisingEdgeCounter::update(bool in_Up,bool in_Down,bool RST){
    //Block 1: unique presses on up, down, reset
    bool upp = in_Up && !in_Up_Old;
    bool dnp = in_Down && !in_Down_Old;


    //Block 2: State Transition Logic.
    //note all of these are local, since they don't need to be "remembered"
    bool waitToUp = state_Waiting && upp && !(dnp || RST);
    bool waitToDown = state_Waiting && dnp && !(upp || RST);
    bool waitToRST = state_Waiting && RST;
    bool waitToWait = state_Waiting && !(waitToUp || waitToDown || waitToRST);
    bool upToWait = state_Up;
    bool upToRST = state_Up && RST;
    bool downToWait = state_Down;
    bool downToRST = state_Up && RST;
    bool RSTToWait = state_RST&&!(RST);
    bool RSTToRST = state_RST && !RSTToWait;

    //Block 3: Update States
    state_Waiting = upToWait||downToWait||RSTToWait||waitToWait;
    state_Up = waitToUp;
    state_Down = waitToDown;
    state_RST = waitToRST||upToRST||downToRST||RSTToRST;

    //Block 4: outputs and old variables
    if(state_Up){
      count++;
    }
    if(state_Down){
      if(count>0) count--;
    }
    if (state_RST) {
      count = 0;
    }
    CNT = (count >= preset);

    in_Up_Old = in_Up;
    in_Down_Old = in_Down;
}

/*!
   @brief   This function runs when you "construct" a timer

   At construction:
   - the duration of the timer will be set to the given value of milliseconds<br>
   - the timer is in the waiting (non-timing) state
   .

   @return  FSMTimer object.
   @param   _duration (unsigned long) sets the duration of the timer in milliseconds
 */
FSMTimer::FSMTimer(unsigned long _duration)
{
    //when we create our timer, the only unique thing is the duration.
    //set initial values for other variables we need as necessary here as well.
    duration = _duration;
    state_Waiting = true;
    state_Timing = false;
    startTime = millis();//initialize the starttime here.
    elapsed = 0;
}


/*!
   @brief   Deallocate the FSMTimer system object
 */
FSMTimer::~FSMTimer() {}

/*!
   @brief   This function updates the timer based on the enable input.

   If enable is true the timer checks to see if the elapsed time is greater than the preset duration and sets the 
   TMR value<br>
   If enable is false the timer resets 

   @return  nothing

   @param   enable (bool) a turns the timer on or off
 */
void FSMTimer::update(bool enable){
    //Block 1: nothing needed, since enable has been passed from 
    //main program!!

    //Block 2: State Transition Logic.
    //note all of these are local, since they don't need to be "remembered"
    bool waitToTime = state_Waiting && enable;
    bool waitToWait = state_Waiting &&! enable;
    bool timeToWait = state_Timing &&! enable;
    bool timeToTime = state_Timing && enable;

    //Block 3: Update States
    state_Waiting = timeToWait||waitToWait;
    state_Timing = waitToTime||timeToTime;

    //Block 4: outputs and old variables
    unsigned long curTime = millis();
    if(state_Waiting){
        startTime = curTime;
    }
    elapsed = curTime-startTime;
    TMR = (elapsed>=duration);
}

/*!
   @brief   This function runs when you "construct" a fast timer

   At construction:
   - the duration of the timer will be set to the given value in microseconds<br>
   - the timer is in the waiting (non-timing) state
   .

   @return  FSMFastTimer object.
   @param   _duration (unsigned long) sets the duration of the timer in microseconds
 */
FSMFastTimer::FSMFastTimer(unsigned long _duration)
{
  //when we create our timer, the only unique thing is the duration.
  //set initial values for other variables we need as necessary here as well.
  duration = _duration;
  state_Waiting = true;
  state_Timing = false;
  startTime = micros();//initialize the starttime here.
  elapsed = 0;
}


/*!
   @brief   Deallocate the FSMTimer system object
 */
FSMFastTimer::~FSMFastTimer() {}

/*!
   @brief   This function updates the timer based on the enable input.

   If enable is true the timer checks to see if the elapsed time is greater than the preset duration and sets the
   TMR value<br>
   If enable is false the timer resets

   @return  nothing

   @param   enable (bool) a turns the timer on or off
 */
void FSMFastTimer::update(bool enable) {
  //Block 1: nothing needed, since enable has been passed from 
  //main program!!

  //Block 2: State Transition Logic.
  //note all of these are local, since they don't need to be "remembered"
  bool waitToTime = state_Waiting && enable;
  bool waitToWait = state_Waiting && !enable;
  bool timeToWait = state_Timing && !enable;
  bool timeToTime = state_Timing && enable;

  //Block 3: Update States
  state_Waiting = timeToWait || waitToWait;
  state_Timing = waitToTime || timeToTime;

  //Block 4: outputs and old variables
  unsigned long curTime = micros();
  if (state_Waiting) {
    startTime = curTime;
  }
  elapsed = curTime - startTime;
  TMR = (elapsed >= duration);
}