/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: main.c
 *
 * PROJECT....: G3 - Tracking system utilizing a pan/tilt system
 *
 * DESCRIPTION: See module specification file (.h-file).
 *
 * Change Log:
 ******************************************************************************
 * Date    Id    Change
 * YYMMDD
 * --------------------
 * 140501  G3   Module created
 * 1405XX  G3   Module modified
 *
 *****************************************************************************/

/***************************** Include files ********************************/
#include "control_tasks/control_speed_tilt.h"
#include "RTCS/rtcs.h"
#include "debug/debug.h"
/*****************************    Defines    ********************************/
#define NORMAL 0
#define DEBUGINFO 1
#define PLOTSPEED 2
#define INTENSEDEBUG 4
#define DEBUGTIME 5

#define RUN_MODE NORMAL //

#define PID_RUN_INTERVAL 40 // ticks // 100 = 50Hz // 40 = 125Hz

//#define Kp 0.236157
#define Kp 0.0785284
#define Ki 0
#define Kd 0.0004

#define IDT (1000000/(PID_RUN_INTERVAL*T_TICK))

#define TICKS_PER_FRAME_ROTATION 1080
#define INTEGRATORLIMIT 400
#define MAXSPEED_LIMIT 1500 // ticks/s
#define MAXPWMVALUE 255
#define MAXPWM_MASK 0xFF
#define MOTOR_DIRECTION_FORWARDS 0b10
#define MOTOR_DIRECTION_BACKWARDS 0b01

// fixedspeed plot
#define CHANGE_SPEED_PLOTPOSITION 50000// 1/5 ms

/*****************************   Constants   ********************************/
/*****************************   Variables   ********************************/
/*****************************   Functions   ********************************/
void init_tilt_speed_task(){
  //UARTprintf("Starting Speed Tilt Task\r\n");
  _start(TILT_SPEED_TASK);
}

void tilt_speed_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :  PID_RUN_INTERVAL
 *****************************************************************************/
{
#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. speed tilt: Start.\r\n");
#endif
  INT16U current_position = 0;

  // PID controller vars
  static INT16U last_error = 0;
  INT16S error, Derror, set_pwm;
  static INT16S Ierror = 0;

  static INT16U last_position = 0;

  static int i = 1;

  INT16S set_speed_tilt = 0;

  INT8U var;

  static INT16U pid_interval_counter = PID_RUN_INTERVAL;
  static INT16S last_pwm = 0;
  INT8U direction = 0;
  INT32S current_speed = 0;

  // PID control loop
  if(!(--pid_interval_counter)){
#if RUN_MODE == DEBUGTIME
    debug_pin(ON);
#endif
    pid_interval_counter = PID_RUN_INTERVAL;

#if RUN_MODE == INTENSEDEBUG
    UARTprintf("C. speed tilt: Loop entered.\r\n");
#endif
    //get speed
    QueuePeek(&QueueTiltSpeed, &set_speed_tilt);

    // debug info
#if (RUN_MODE == DEBUGINFO)
    UARTprintf("ss: %d\r\n", set_speed_tilt);
#endif


    // get current position
    QueuePeek(&QueuePositionTilt, &current_position);

#if RUN_MODE == INTENSEDEBUG
    UARTprintf("C. speed tilt: Peeks made.\r\n");
#endif

    // calc current speed
    current_speed = (current_position - last_position)*IDT;

    // save last position
    last_position = current_position;

    // adjust current speed for overflow due to direction reset (1079 -> 0 and 0 -> 1079)
    if(current_speed < -MAXSPEED_LIMIT){ // CW
      current_speed += TICKS_PER_FRAME_ROTATION*IDT;
    }
    else if (current_speed > MAXSPEED_LIMIT) { // CCW
      current_speed -=  TICKS_PER_FRAME_ROTATION*IDT;
    }

    // error calc
    error = set_speed_tilt - current_speed;
    Derror = (error - last_error)*IDT;
    Ierror += error;

    // limit integral
    if(Ierror > INTEGRATORLIMIT*IDT){
      Ierror = INTEGRATORLIMIT*IDT;
    }
    else if(Ierror < -INTEGRATORLIMIT*IDT){
      Ierror = -INTEGRATORLIMIT*IDT;
    }
    // calculate PID
    set_pwm = (error*Kp + Derror*Kd + ((Ierror*Ki)/IDT));
    set_pwm += last_pwm;

    // limit pwm
    if (set_pwm > MAXPWMVALUE) {
      set_pwm = MAXPWMVALUE;
    }
    else if (set_pwm < -MAXPWMVALUE) {
      set_pwm = -MAXPWMVALUE;
    }

    // save last pwm and error for next calc
    last_pwm = set_pwm;
    last_error = error;

    // debuginfo
#if (RUN_MODE == DEBUGINFO)
    UARTprintf("cs: %d, pwmO: %d\r\n", current_speed, set_pwm);
#endif

    //
    if(set_pwm > 0 ){
      direction = MOTOR_DIRECTION_FORWARDS;
    } else {
      direction = MOTOR_DIRECTION_BACKWARDS;
      set_pwm = -set_pwm;
    }

    // ready data to send and send it.
    set_pwm = (direction << 8) | (set_pwm & MAXPWM_MASK);
    QueueSend(&QueuePWMOutTilt, &set_pwm);

#if RUN_MODE == INTENSEDEBUG
    UARTprintf("C. speed tilt: PWM send.\r\n");
#endif

    // plotspeed
#if (RUN_MODE == PLOTSPEED)
    if(--i == 0){
      i = 5;
      current_speed+=32768;
      UARTCharPut(0,'|');
      UARTCharPut(0, current_speed>> 8);
      UARTCharPut(0, current_speed);
    }
#endif
#if RUN_MODE == DEBUGTIME
    debug_pin(OFF);
#endif
  }
#if RUN_MODE == INTENSEDEBUG
  UARTprintf("C. speed tilt: Exit.\r\n");
#endif
}
/****************************** End Of Module *******************************/
