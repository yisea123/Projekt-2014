/*****************************************************************************
 * University of Southern Denmark
 * Embedded C Programming (ECP)
 *
 * MODULENAME.: communication_task.c
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
 * 1404XX  G3    Communication task added.
 * 1405XX  G3    Communication task modified.
 *
 *****************************************************************************/

/***************************** Include files *******************************/
#include "communication/communication_task.h"
#include "RTCS/rtcs.h"
#include "inc\lm3s6965.h"
#include "debug/debug.h"

/*****************************    Defines    *******************************/
#define TICK_INTERUPT_MS 500

#define NORMAL 0
#define FIXEDROTATION 1

// decide runmode
#define RUNMODE NORMAL

// for fixedrotation
#define ROTATIONS 20 // 0 = infinity
#define STARTVALUE 0x80000000
#define TICKSPERROTATION 360
#define FIXEDSPEED 298
#define DIRECTION 1 // 1 and 0

// for normal
#define REPEAT_PWM_START 10 //* � ms (500�s) // the time it starts at
#define REPEAT_PWM_INC 10 //* � ms (500�s) // the time it is inc with
#define REPEAT_PWM_TIME 2000 //* � ms (500�s) // the time it takes before inc.

/*
 *  100 = 50ms
 * 1000 = 500ms
 * 2000 = 1s
 */



extern INT32U encoderTicks;



/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
INT8U array[1000] = {83,34,10,37,18,51,62,25,52,55,95,92,5,48,12,4,36,85,63,77,60,41,79,27,83,35,59,13,49,43,5,81,31,48,24,13,28,13,75,24,78,32,12,93,96,36,5,89,28,42,39,73,29,69,23,59,80,0,88,7,86,74,38,31,28,15,23,29,69,14,83,23,20,79,3,37,52,54,79,84,68,27,30,19,7,74,43,8,31,57,53,85,55,83,77,97,90,71,98,11,42,77,54,6,22,32,40,100,65,70,64,98,77,59,62,46,78,36,27,92,29,22,86,18,83,95,32,44,99,86,54,61,86,48,5,45,0,78,47,63,76,99,6,61,65,0,46,27,69,3,24,39,43,100,41,36,36,39,21,58,84,14,18,16,60,10,40,99,66,19,88,21,1,62,66,14,1,2,67,73,16,92,48,1,74,53,92,8,100,48,98,57,27,61,56,74,0,16,14,4,32,4,22,21,37,11,83,5,45,32,71,63,98,24,5,67,75,97,46,53,54,10,60,28,53,16,85,2,24,37,53,95,27,5,33,4,70,79,9,44,89,70,89,94,31,37,66,36,0,18,92,58,97,32,33,29,83,33,6,61,37,3,12,89,25,93,23,37,86,63,41,44,70,44,37,49,23,64,14,71,68,71,2,74,90,88,58,47,79,66,81,41,90,65,82,89,10,28,76,99,89,97,95,63,51,70,3,24,90,48,9,20,12,71,83,23,12,52,75,57,62,11,60,16,89,19,0,56,1,44,20,25,79,99,94,42,53,99,79,40,5,28,36,20,24,24,83,12,98,74,97,24,41,41,28,27,27,33,93,83,8,23,92,37,68,79,83,2,87,36,84,69,37,85,35,54,44,0,92,27,15,32,92,36,3,0,33,74,43,55,9,50,1,4,24,26,55,68,83,67,46,76,24,67,65,79,23,64,40,30,37,17,93,51,37,1,58,58,19,60,12,87,21,62,2,48,21,63,56,4,47,75,30,99,73,92,30,64,4,54,86,89,76,5,52,57,36,65,32,71,64,6,43,3,30,5,67,80,14,82,72,28,82,70,31,17,8,43,66,67,77,44,8,93,56,31,55,29,56,73,2,100,11,24,17,37,22,91,86,64,5,12,49,76,32,100,79,63,82,83,51,31,67,36,11,28,92,15,26,2,17,4,59,33,88,43,9,37,85,50,71,68,27,95,78,58,13,20,18,2,61,22,19,24,30,63,25,57,17,40,5,39,11,10,82,43,36,15,12,53,55,44,19,61,46,31,96,70,65,18,12,36,66,90,11,24,39,70,84,95,93,30,86,40,77,85,42,74,51,8,6,70,96,46,28,10,40,9,45,71,73,53,3,50,58,9,3,11,22,64,83,40,70,44,70,87,50,65,26,62,57,71,58,56,18,52,39,51,68,57,69,8,86,45,92,64,96,76,74,39,21,2,20,54,73,7,34,2,72,8,97,9,94,89,98,63,17,43,69,30,77,72,30,83,33,11,27,90,30,63,57,0,23,77,8,11,83,14,82,20,12,10,88,81,82,41,24,98,41,43,28,11,89,56,44,80,6,0,73,82,59,38,34,71,21,50,93,91,16,7,70,78,99,6,38,28,34,17,25,49,53,35,83,28,3,12,100,82,97,40,20,43,85,99,35,56,2,40,12,84,48,72,55,73,38,63,53,17,37,47,61,58,62,85,97,67,27,78,57,59,39,36,0,37,94,38,22,12,39,46,22,67,79,94,95,92,58,39,69,50,70,49,44,52,63,10,17,99,32,41,26,13,64,20,11,9,90,76,76,60,51,0,23,10,7,51,50,75,32,54,5,7,76,75,5,53,23,90,50,4,78,10,75,30,74,84,81,10,5,87,3,29,59,46,69,27,75,95,28,19,88,23,40,19,82,42,96,57,94,99,100,97,7,33,54,29,62,27,19,87,57,51,69,96,2,99,60,51,36,75,36,32,22,44,65,16,13,47,78,50,26,51,93,13,72,88,90,2,76,0,61,49,88,75,84,99,54,6,74,98,35,74,94,27,18,55,8,19,97,36,81,89,49,47,49,38,64,8,37,37,33,79,45,61,88,53,29,87,49,45,90,24,11,70,0,32,9,95,85,86,39,62,71,55,4,96,86,95,3,59,5,97,63,16,49,40,74,99,30,10,2,43,9,51,16,79,15,9,83,40,33,6,93,4,53,73,31,28,26,80,8,99,30,83,29,79,28,95,83,55,9,95,21,18,48,41,48,19,44,73,77,10,100,75,75,33,93,25,30,53};

INT8U duration[1000] = {0,1,1,0,1,1,0,1,0,1,1,1,0,1,0,0,1,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,0,1,0,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0,0,1,1,1,1,0,1,1,1,0,1,0,0,0,1,0,0,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,0,1,1,1,0,0,1,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,1,1,0,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1,0,0,1,1,1,0,1,1,1,1,1,0,0,0,1,0,1,1,0,1,0,1,1,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,0,0,1,0,1,1,0,0,0,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,0,1,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,1,1,1,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,1,0,0,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,1,1,1,1,1,0,1,0,0,0,0,0,1,1,0,1,1,1,0,1,1,1,0,0,0,1,1,0,0,0,0,1,0,1,1,0,0,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,0,0,1,1,0,1,1,0,0,1,0,0,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,1,0,0,1,0,0,1,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,1,1,0,1,1,0,1,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,0,1,1,0,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,1,0,1,0,0,0,0,1,1,1,0,1,1,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,1,0,0,0,1,1,0,1,1,0,1,1,0,1,1,1,1,0,0,1,0,1,0,0,1,0,0,0,0,1,0,1,1,1,1,1,0,0,1,1,0,0,1,1,1,0,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0};
/*****************************   Functions   *******************************/
void init_communication_task(){
  // init pwm
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM; // Enable the PWM clock

  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // Enable the clock to PORTB

  GPIO_PORTB_AFSEL_R |= 0x3; // Enable the PB0 alternate function
  GPIO_PORTB_DEN_R |= 0x3;

  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; //Enable PWM divide
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;
  SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_8;

  //Magic!
  PWM_1_CTL_R &= ~PWM_X_CTL_ENABLE;
  PWM_1_GENA_R |= 0x8C;
  PWM_1_GENB_R |= 0x80C;

  PWM_INVERT_R |= 0x0C;

  PWM_1_LOAD_R = 299;

  PWM_1_CMPA_R = 0;
  PWM_1_CMPB_R = 0;

  PWM_1_CTL_R |= PWM_X_CTL_ENABLE; // Start the timers

  PWM_ENABLE_R |= (PWM_ENABLE_PWM3EN | PWM_ENABLE_PWM2EN);
  //UARTprintf("Starting Communication Task\r\n");

  _start(COMMUNICATION_TASK, MILLI_SEC(0));
}
void communication_task()
/*****************************************************************************
 *   Input    : 	-
 *   Output   : 	-
 *   Function :
 *   Run @	 :
 *****************************************************************************/
{
  debug_pin_red(OFF);
  static INT16S i = 0;
  static INT16S n = 0;
  static INT16S j = 0;
  static INT16U time = REPEAT_PWM_START;
  static INT16U countTime = 0;

  // update time for the pwm length
  ++countTime;
  if (countTime == REPEAT_PWM_TIME) {
    time += REPEAT_PWM_INC;
    countTime = 0;
  }

  // send ticks and pwm
  UARTCharPut(0, encoderTicks >> 24);
  UARTCharPut(0, encoderTicks >> 16);
  UARTCharPut(0, encoderTicks >> 8);
  UARTCharPut(0, encoderTicks);
  UARTCharPut(0, array[n]);

#if RUNMODE == NORMAL
  if (i == 0)
    {
    debug_pin_red(TOGGLE);
    // update n
    if(duration[(j-1)%1000] != duration[j])
      {
      // inc n
      ++n;
      n %= 1000;
      }

    // set pwm
    if(array[n]>50)
      {
      PWM_1_CMPB_R = 0;
      PWM_1_CMPA_R = (array[n] - 50)*2.98;
      }
    else
      {
      PWM_1_CMPA_R = 0;
      PWM_1_CMPB_R = (50 - array[n])*2.98;
      }

    // inc j (bit array)
    ++j;
    j %= 1000;
  }

  ++i;
  if (i > time) {
    i = 0;
  }
#endif



#if RUNMODE == FIXEDROTATION
  if (ROTATIONS == 0) {
#if DIRECTION == 0
    PWM_1_CMPB_R = 0;
    PWM_1_CMPA_R = FIXEDSPEED;
#endif
#if DIRECTION == 1
    PWM_1_CMPB_R = FIXEDSPEED;
    PWM_1_CMPA_R = 0;
#endif

  }
  else {
    if (encoderTicks >= (STARTVALUE + ROTATIONS*TICKSPERROTATION)) {
      PWM_1_CMPA_R = 0;
      PWM_1_CMPB_R = 0;
    }
    else {
#if DIRECTION == 0
    PWM_1_CMPB_R = 0;
    PWM_1_CMPA_R = FIXEDSPEED;
#endif
#if DIRECTION == 1
    PWM_1_CMPB_R = FIXEDSPEED;
    PWM_1_CMPA_R = 0;
#endif
    }
  }

#endif
}

/****************************** End Of Module *******************************/
