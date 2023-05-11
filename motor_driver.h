#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tm4c123gh6pm.h"

#ifndef motor_driver
#define motor_driver


void motor_up();
void motor_down();
void motor_stop();
#endif