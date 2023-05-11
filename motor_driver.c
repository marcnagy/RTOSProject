#include "motor_driver.h"


void motor_up() {
  // Set PA2 high and PA3 low
    GPIO_PORTA_DATA_R |= (1 << 2);
    GPIO_PORTA_DATA_R &= ~(1 << 3);
}

void motor_down() {
    // Set PA2 low and PA3 high
    GPIO_PORTA_DATA_R &= ~(1 << 2);
    GPIO_PORTA_DATA_R |= (1 << 3);

}

void motor_stop() {
    // Set PA2 low and PA3 low
    GPIO_PORTA_DATA_R &= ~(1 << 2);
    GPIO_PORTA_DATA_R &= ~(1 << 3);
}





