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
#include "inc\emp_type.h"
#include "inc\lm3s6965.h"

/*****************************    Defines    *******************************/
extern INT32U encoderTicks;
/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
/*****************************   Functions   *******************************/
void init_encoder_task(){
  //UARTprintf("Starting Communication Task\r\n");
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;

  //Set direction as input
  GPIO_PORTB_DIR_R &= ~0xC;

  //enable digital function
  GPIO_PORTB_DEN_R |= 0xC;

  _start(ENCODER_TASK, MILLI_SEC(0));
}
void encoder_task()
/*****************************************************************************
 *   Input    :         -
 *   Output   :         -
 *   Function :
 *   Run @       :
 *****************************************************************************/
{

  INT8U input = GPIO_PORTB_DATA_R & 0xC;
  input = input >> 2;
  static INT8U encoderState = 00;

  switch (encoderState) {
  case 0b00:
  if (input == 0b10) {
    encoderState = 0b10;
    --encoderTicks;
  }
  else if (input == 0b01) {
    encoderState = 0b01;
    ++encoderTicks;
  }
  break;
  case 0b01:
  if (input == 0b11) {
    encoderState = 0b11;
    ++encoderTicks;
  }
  else if (input == 0b00) {
    encoderState = 0b00;
    --encoderTicks;
  }
  break;
  case 0b11:
  if (input == 0b10) {
    encoderState = 0b10;
    ++encoderTicks;
  }
  else if (input == 0b01) {
    encoderState = 0b01;
    --encoderTicks;
  }
  break;
  case 0b10:
  if (input == 0b11) {
    encoderState = 0b11;
    --encoderTicks;
  }
  else if (input == 0b00) {
    encoderState = 0b00;
    ++encoderTicks;
  }
  break;
  default:
  break;
  }

}
/****************************** End Of Module *******************************/
