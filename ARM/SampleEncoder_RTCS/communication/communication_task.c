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
#define RUNMODE FIXEDROTATION

// for fixedrotation
#define ROTATIONS 0 // 0 = infinity
#define STARTVALUE 0x80000000
#define TICKSPERROTATION 360
#define FIXEDSPEED 74

// for normal
#define REPEAT_PWM 1000 //* � ms (500�s)
/*
 *  100 = 50ms
 * 1000 = 500ms
 * 2000 = 1s
 */



extern INT32U encoderTicks;



/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
INT8U array[1000] =
                {14, 78, 50, 1, 21, 53, 46, 99, 63, 66, 82, 6, 46, 93, 72, 58, 17,
                75, 2, 1, 57, 96, 2, 3, 71, 8, 94, 74, 77, 89, 97, 69, 79, 44, 99,
                89, 72, 88, 8, 21, 0, 92, 70, 7, 37, 8, 54, 0, 77, 13, 29, 70, 23,
                34, 18, 94, 17, 41, 54, 82, 88, 6, 48, 79, 57, 67, 68, 1, 1, 42, 23,
                40, 44, 37, 86, 44, 52, 99, 39, 37, 47, 68, 77, 82, 66, 58, 15, 50,
                49, 2, 90, 85, 94, 37, 2, 2, 90, 22, 16, 37, 10, 43, 78, 42, 58, 35,
                16, 42, 68, 30, 14, 83, 50, 83, 93, 99, 62, 70, 81, 7, 67, 91, 83,
                65, 38, 97, 39, 69, 45, 32, 55, 54, 15, 40, 24, 66, 11, 99, 82, 33,
                25, 19, 70, 49, 38, 92, 16, 74, 93, 12, 39, 50, 22, 0, 21, 6, 97, 50,
                84, 61, 63, 21, 64, 9, 42, 56, 85, 25, 77, 73, 36, 51, 96, 20, 43,
                89, 85, 29, 27, 97, 73, 100, 95, 1, 68, 57, 38, 0, 18, 80, 5, 40, 75,
                23, 51, 70, 79, 0, 16, 25, 73, 74, 83, 76, 6, 98, 92, 34, 26, 37, 36,
                68, 83, 85, 97, 88, 83, 96, 53, 33, 81, 86, 68, 99, 3, 53, 58, 3, 39,
                91, 85, 3, 49, 31, 73, 76, 12, 25, 17, 5, 3, 84, 77, 96, 79, 59, 28,
                74, 51, 39, 22, 94, 61, 18, 56, 91, 84, 98, 32, 7, 97, 24, 68, 94,
                36, 44, 38, 13, 95, 37, 73, 100, 67, 8, 10, 32, 48, 83, 45, 84, 3,
                37, 74, 70, 84, 76, 79, 96, 75, 45, 52, 30, 73, 22, 9, 48, 46, 41,
                82, 22, 32, 72, 18, 61, 95, 80, 24, 55, 53, 8, 7, 38, 36, 81, 83, 59,
                91, 61, 70, 3, 1, 36, 65, 8, 79, 87, 56, 37, 73, 84, 70, 3, 34, 51,
                44, 60, 28, 88, 52, 97, 96, 37, 7, 52, 30, 28, 21, 73, 61, 32, 72,
                10, 93, 64, 82, 41, 59, 1, 17, 2, 60, 77, 14, 57, 97, 56, 37, 32, 60,
                10, 99, 99, 37, 54, 65, 25, 26, 23, 51, 17, 75, 17, 100, 98, 55, 56,
                84, 18, 20, 52, 72, 99, 95, 76, 80, 69, 46, 62, 25, 88, 5, 91, 24,
                51, 72, 87, 25, 55, 58, 14, 70, 77, 2, 41, 18, 89, 49, 55, 20, 80,
                60, 14, 60, 56, 54, 37, 62, 76, 8, 27, 6, 46, 25, 60, 81, 38, 73, 54,
                4, 11, 57, 9, 13, 69, 10, 0, 5, 81, 75, 50, 40, 20, 53, 86, 70, 34,
                43, 78, 3, 65, 42, 75, 47, 25, 91, 52, 29, 39, 70, 54, 38, 45, 22,
                76, 51, 19, 35, 79, 25, 92, 68, 90, 43, 27, 2, 2, 73, 60, 44, 54, 8,
                78, 65, 82, 60, 71, 80, 58, 76, 55, 94, 45, 8, 96, 11, 66, 57, 35,
                98, 74, 36, 98, 62, 1, 23, 72, 85, 89, 55, 74, 1, 15, 73, 43, 97, 7,
                33, 97, 15, 31, 92, 25, 39, 73, 0, 25, 78, 66, 57, 87, 3, 89, 73, 40,
                6, 19, 84, 36, 47, 47, 67, 64, 72, 48, 43, 14, 53, 84, 51, 52, 79,
                44, 5, 15, 15, 49, 12, 24, 54, 5, 100, 86, 20, 56, 51, 66, 48, 77,
                29, 12, 34, 33, 46, 19, 39, 41, 49, 21, 44, 28, 56, 36, 11, 26, 73,
                74, 68, 9, 39, 60, 26, 4, 10, 10, 14, 51, 26, 54, 36, 51, 0, 45, 100,
                22, 15, 50, 9, 55, 73, 7, 12, 53, 46, 29, 4, 90, 4, 47, 65, 37, 23,
                16, 38, 48, 21, 55, 6, 57, 16, 34, 98, 24, 66, 57, 55, 62, 0, 21, 85,
                76, 8, 92, 50, 30, 95, 34, 86, 88, 50, 45, 55, 75, 18, 43, 47, 12,
                48, 38, 30, 58, 56, 99, 96, 39, 58, 35, 93, 69, 85, 25, 58, 20, 84,
                63, 88, 23, 27, 1, 96, 85, 5, 27, 13, 56, 7, 52, 27, 11, 99, 54, 63,
                32, 57, 59, 10, 82, 14, 100, 71, 37, 68, 96, 6, 47, 81, 10, 44, 72,
                24, 23, 50, 64, 97, 55, 80, 37, 16, 35, 66, 99, 0, 35, 46, 59, 39,
                56, 24, 41, 23, 50, 39, 16, 17, 50, 62, 83, 0, 83, 91, 1, 15, 91, 77,
                30, 92, 88, 73, 45, 46, 10, 33, 33, 40, 21, 20, 66, 4, 70, 62, 57,
                11, 65, 100, 85, 6, 23, 62, 58, 68, 49, 11, 90, 68, 69, 85, 30, 18,
                4, 69, 3, 84, 94, 53, 25, 55, 26, 41, 1, 51, 78, 58, 8, 51, 39, 55,
                1, 64, 21, 72, 60, 25, 60, 76, 72, 32, 9, 76, 13, 23, 70, 16, 27, 9,
                35, 5, 52, 99, 39, 89, 72, 45, 83, 97, 79, 59, 43, 83, 70, 55, 3, 73,
                12, 21, 49, 24, 23, 9, 58, 90, 79, 66, 71, 34, 24, 2, 57, 30, 2, 84,
                51, 14, 51, 48, 59, 39, 46, 6, 29, 47, 8, 11, 76, 45, 52, 58, 17, 20,
                57, 1, 17, 19, 51, 24, 90, 61, 50, 1, 70, 29, 100, 31, 96, 98, 51,
                73, 5, 19, 45, 6, 8, 33, 39, 1, 85, 69, 6, 80, 48, 77, 10, 68, 85,
                49, 48, 27, 6, 32, 75, 67, 58, 91, 6, 50, 55, 80, 90, 18, 42, 31, 6,
                85, 23, 63, 54, 20, 88, 25, 69, 79, 26, 65, 48, 48, 15, 77, 66, 88,
                91, 44, 21, 34, 38, 99, 78, 32, 98, 39, 51, 43, 15, 55, 59, 74, 80,
                78, 60, 59, 31, 72, 21, 37, 71, 28, 75, 4, 83, 28, 72, 25, 59, 69, 87,
                14, 84, 16, 59, 99, 24, 72, 22, 19, 28, 48, 23, 92, 90, 95, 57, 9,59};

INT8U duration[1000] =
                {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0,
                1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0,
                1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1,
                1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0,
                0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1,
                1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1,
                0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0,
                1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1,
                0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1,
                1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1,
                0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
                0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1,
                0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0,
                1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0,
                1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1,
                0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0,
                0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1,
                1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0,
                0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1,
                0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0,
                1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1,
                1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0,
                0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1,
                0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0,
                0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
                1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1,
                0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1,
                1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1,
                1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1,
                1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
                0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1,
                1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
                1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
                1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1,
                0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1,
                0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1,
                1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
                1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1,
                0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0,
                1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0};
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
  static INT16S i = 0;
  static INT16S n = 0;
  static INT16S j = 0;

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

    // update n
    if(duration[(j-1)%1000] != duration[j])
      {
      // inc n
      ++n;
      n %= 1000;
      }

    // inc j (bit array)
    ++j;
    j %= 1000;
  }

  ++i;
  i %= REPEAT_PWM;
#endif



#if RUNMODE == FIXEDROTATION
  if (ROTATIONS == 0) {
    PWM_1_CMPB_R = 0;
    PWM_1_CMPA_R = FIXEDSPEED;
  }
  else {
    if (encoderTicks >= (STARTVALUE + ROTATIONS*TICKSPERROTATION)) {
      PWM_1_CMPA_R = 0;
    }
    else {
      PWM_1_CMPB_R = 0;
      PWM_1_CMPA_R = FIXEDSPEED;
    }
  }

#endif
}

/****************************** End Of Module *******************************/
