/*! \file ME480FSM.h */
/*!
 * @file ME480FSM.h
 *
 * @mainpage ME480 Library to help with the implimentation of Finite State Machines
 *
 * @section intro_sec Introduction
 *
 * This is the documentation for the ME480FSM library developed for ME480L controls laboratory. 
 * The library contains routines that help with the implimentation of finite state machines 
 * programmed on Arduino processors. The functions in the classes are built in the 
 * four block format adopted by the course. The code is available in the 
 * Arduino/Libraries/ME480FSM/src directory. The *.cpp files contain the actual code and the 
 * *.h files declare the class structures.
 *
 * @section author Author
 *
 * Writen by Alex Brown.
 *
 * @section license License
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef ME480FSM_h
#define ME480FSM_h

 /*!
  @brief  This class impliments rising edge up-down counters

  The RisingEdgeCounter class creates a counter that will increment or decrement each time the
  increment or decrement boolean value in the update function changes from false to true. This 
  counter is particularly suited to counting state changes and mimics the action of the PLC counter.
  The counter can be reset by setting the reset boolean value to true.
  The user can read the current preset, count and state of counting (CNT)
 */
class RisingEdgeCounter
{   //public functions and variables that can be accessed by user
    public:
        // Constructor/destructor:
        //must declare the class itself as public
        RisingEdgeCounter(long _preset); //add leading underscore to avoid confusing this for the class's "stored" duration
        ~RisingEdgeCounter(void);       //releases any resources used by the class when destroyed.
                             
        //function that runs the state machine
        void update(bool in_Up, bool in_Down, bool RST);//function that runs the state machine.
        //variables that can be queried by main program:
        long preset;  ///<Sets/returns the number of counts that will trip the state of counting variable
        int count;    ///<Returns the current number of counts
        bool CNT;     ///<State of counting, true if the number of counts is greater than, or equal to, the preset value


    //private variables are ones that can't be accessed by main program
        //note that these variables only exists in the RisingEdgeCounter class so
        //duplicate names in other classes do not create a conflict
    private:
        //no real need for the states to be known by main program
        bool state_Waiting; 
        bool state_Up;
        bool state_Down;
        bool state_RST;
        bool in_Up_Old;
        bool in_Down_Old;
};

/*!
 @brief  This class impliments a millisecond timer

 The FSMTimer class impliments a millisecond timer. The duration of the timer is set when the timer object 
 is constructed. If the enable input of the update function is true, the function checks if the elapsed time
 is equal to, or greater than, the set duration. If the enable input is false, the startTime is updated to 
 the current time, reseting the elapsed time to 0. The TMR value will return a true if the elapsed time is 
 greater than, or equal to, the set duration. 
*/

class FSMTimer
{   //public functions and variables that can be accessed by user
    public:
        // Constructor/destructor:
        //must declare the class itself as public
        FSMTimer(unsigned long _duration); //add leading underscore to avoid confusing this for the class's "stored" duration
        ~FSMTimer(void);

        //function that runs the state machine
        void update(bool enable);//function that runs the state machine.

        //variables that can be queried by main program:
        unsigned long duration; ///<Duration value of the timer in milliseconds
        unsigned long elapsed;  ///<Elapsed time that the timer has been counting
        bool TMR;               ///<State of timing, true if elapsed time is greater than, or equal to, the duration value

    //private variables are ones that can't be accessed by main program
        //note that these variables only exists in the FSMTimer class so
        //duplicate names in other classes do not create a conflict
    private:
        //starttime not needed by main program
        unsigned long startTime;
        //no real need for the states to be known by main program
        bool state_Waiting;
        bool state_Timing;
};


/*!
 @brief  This class impliments a microsecond timer

 The FSMFastTimer class impliments a microsecond timer. The duration of the timer is set when the timer object
 is constructed. If the enable input of the update function is true, the function checks if the elapsed time
 is equal to, or greater than, the set duration. If the enable input is false, the startTime is updated to
 the current time, reseting the elapsed time to 0. The TMR value will return a true if the elapsed time is
 greater than, or equal to, the set duration.
*/

class FSMFastTimer
{   //public functions and variables that can be accessed by user
public:
  // Constructor/destructor:
  //must declare the class itself as public
  FSMFastTimer(unsigned long _duration); //add leading underscore to avoid confusing this for the class's "stored" duration
  ~FSMFastTimer(void);

  //function that runs the state machine
  void update(bool enable);//function that runs the state machine.

  //variables that can be queried by main program:
  unsigned long duration; ///<Duration value of the timer in milliseconds
  unsigned long elapsed;  ///<Elapsed time that the timer has been counting
  bool TMR;               ///<State of timing, true if elapsed time is greater than, or equal to, the duration value

//private variables are ones that can't be accessed by main program
    //note that these variables only exists in the FSMFastTimer class so
    //duplicate names in other classes do not create a conflict
private:
  //starttime not needed by main program
  unsigned long startTime;
  //no real need for the states to be known by main program
  bool state_Waiting;
  bool state_Timing;
};
#endif