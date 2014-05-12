/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: ECP
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140501  G3   Module created.
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_position_pan.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTPOSITION 2

#define RUN_MODE NORMAL //

#define STOP_BAND_START 205 // when the band were it can't be starts (221)
#define STOP_BAND_STOP 860 // ^... stops (840)

#define PID_RUN_INTERVAL 20 // ticks
#define  defaultPositionPan 0

#define Kp 0.5 //0.5
#define Ki 3 //1
#define Kd 0

#define IDT 1000/(PID_RUN_INTERVAL*T_TICK)
/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_pan_position_task(){
  _start(PAN_POSITION_TASK, MILLI_SEC(0));
}

void pan_position_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  > 5 times system frequency
 *****************************************************************************/
{
  INT16U current_position = 0;

  // PID controller vars
  static INT16S last_error = 0;
  INT16S error, Derror;
  static INT32S Ierror = 0;
  static INT16U pid_interval_counter = PID_RUN_INTERVAL;

  INT16U goToPosition = 0;
  static INT16U lastGoToPosition = 0;

  INT16S set_speed = 0;
  static INT8U i = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
    pid_interval_counter = PID_RUN_INTERVAL;

    // get position
    QueuePeek(QueuePositionPan, &current_position);
    if (!QueuePeek(QueueGoToPositionPan, &goToPosition)) {
      goToPosition = defaultPositionPan;
    }

    // test for valid position (rotation stopper)
    if (goToPosition > STOP_BAND_START && goToPosition < STOP_BAND_STOP) {
      if (goToPosition < (STOP_BAND_START + STOP_BAND_STOP)/2) {
        goToPosition = STOP_BAND_START;
      }
      else {
        goToPosition = STOP_BAND_STOP;
      }
    }

    /*
    // if new position
    if (lastGoToPosition != goToPosition) {
      lastGoToPosition = goToPosition;
    }
    */
    error = goToPosition - current_position;

    //shortest path correction(untested)
    if(error > 540){
    	error -= 1080;
    }
    else if (error < -540) {
		error += 1080;
	}
    Derror = (error - last_error)*IDT;
    Ierror+=error;

    if(Ierror > 200*IDT){
    	Ierror = 200*IDT;
    }
    // lukas limit 200*IDT
    else if(Ierror < -200*IDT){
    	Ierror = -200*IDT;
    }

    set_speed = error*Kp + (Ierror*Ki)/IDT + Derror*Kd;

    if(set_speed > 1500){
    	set_speed = 1500;
    }
    else if(set_speed < -1500){
    	set_speed = - 1500;
    }
    QueueOverwrite(QueuePanSpeed, &set_speed);

#if (RUN_MODE == PLOTPOSITION)
  if(--i == 0){
    i = 5;
    UARTCharPut(0,'|');
    UARTCharPut(0, current_position >> 8);
    UARTCharPut(0, current_position);
  }
#endif

  last_error = error;
  }
}
/****************************** End Of Module *******************************/
