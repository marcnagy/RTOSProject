#include "types.h"
#include "inc/hw_types.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#define Set_Bit(Register,Bit)    Register |= ( 1 << Bit )
#define Get_Bit(Register,Bit)    ((Register >> Bit )&1)
#define Clear_Bit(Register,Bit)  Register &=!( 1 << Bit )
#ifndef DIO_h
#define DIO_h

void DIO_Init (void);
#endif