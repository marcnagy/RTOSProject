#include "types.h"
#include "DIO.h"
#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
#include "task.h"
#include <math.h>
#include "semphr.h"
#include "queue.h"
#include "motor_driver.h"

void Task1();
void Task2();
void Task3();
void UP();
void DOWN();
void Emergency();

TaskHandle_t task2;
xSemaphoreHandle XMUTEX;
xSemaphoreHandle EmergencyMutex;
xSemaphoreHandle EmergencySemaphore;
xQueueHandle xQueueUP;
xQueueHandle xQueueDOWN;


//Driver Controll of the switch
void Task1(){
	// Initialise the xLastWakeTime variable with the current time.
  TickType_t xLastWakeTime;   
	xLastWakeTime = xTaskGetTickCount(); 
	 for( ; ; )
		{
			int i=1;
			xSemaphoreTake(XMUTEX,portMAX_DELAY);
			//down button
if(((GPIO_PORTB_DATA_R >>6) & 1) == 0){
	portBASE_TYPE xstatus;
while(((GPIO_PORTB_DATA_R >>6) & 1) == 0){
	xstatus=xQueueSendToBack(xQueueDOWN,&i,100);
	i++;
}
//button not being pressed anymore
int x=0;
xstatus=xQueueSendToBack(xQueueDOWN,&x,0);
}
				
//up button
if(((GPIO_PORTB_DATA_R >>7) & 1) == 0){
	portBASE_TYPE xstatus;
while(((GPIO_PORTB_DATA_R >>7) & 1) == 0){
	xstatus=xQueueSendToBack(xQueueUP,&i,100);
	i++;
}
//button not being pressed anymore
int x=0;
xstatus=xQueueSendToBack(xQueueUP,&x,0);
}
			xSemaphoreGive(XMUTEX);
			vTaskDelayUntil(&xLastWakeTime,100);		
		}			
		}




//Passenger Controll of the switch
void Task2(){
	// Initialise the xLastWakeTime variable with the current time.
  TickType_t xLastWakeTime;   
	xLastWakeTime = xTaskGetTickCount(); 
	 for( ; ; )
		{
			int i=1;
		xSemaphoreTake(XMUTEX,portMAX_DELAY);
			//down button
if(((GPIO_PORTC_DATA_R >>4) & 1) == 0){
	portBASE_TYPE xstatus;
while(((GPIO_PORTC_DATA_R >>4) & 1) == 0){
	xstatus=xQueueSendToBack(xQueueDOWN,&i,100);
	i++;
}
//button not being pressed anymore
int x=0;
xstatus=xQueueSendToBack(xQueueDOWN,&x,0);
}
				
//up button
if(((GPIO_PORTC_DATA_R >>5) & 1) == 0){
	portBASE_TYPE xstatus;
while(((GPIO_PORTC_DATA_R >>5) & 1) == 0){
	xstatus=xQueueSendToBack(xQueueUP,&i,100);
	i++;
}
//button not being pressed anymore
int x=0;
xstatus=xQueueSendToBack(xQueueUP,&x,0);
}
				
		xSemaphoreGive(XMUTEX);
		vTaskDelayUntil(&xLastWakeTime,100);
		}			
}

//Driver Lock
void Task3(){
		// Initialise the xLastWakeTime variable with the current time.
  TickType_t xLastWakeTime;   
	xLastWakeTime = xTaskGetTickCount(); 

	 for( ; ; )
		{
if(((GPIO_PORTE_DATA_R >>1) & 1) == 0)
	vTaskSuspend(task2);
if(((GPIO_PORTE_DATA_R >>1) & 1) == 1)
vTaskResume(task2);
vTaskDelayUntil(&xLastWakeTime,100);
		}
}
//EMERGENCY DOWN
void Emergency(){	
		// Initialise the xLastWakeTime variable with the current time.
  TickType_t xLastWakeTime;   
	xLastWakeTime = xTaskGetTickCount(); 
	xSemaphoreTake(EmergencySemaphore,0);
for(;;){
	
	xSemaphoreTake(EmergencySemaphore,portMAX_DELAY);
	xSemaphoreTake(EmergencyMutex,portMAX_DELAY);
	motor_down();
	vTaskDelay(500/portTICK_RATE_MS);
	motor_stop();
	xSemaphoreGive(EmergencyMutex);
}
}
//UP
void UP(){
		// Initialise the xLastWakeTime variable with the current time.
	portBASE_TYPE xstatus; 
	int newvalue,prevValue=0;
	for( ; ; )
		{
	if(newvalue !=NULL)
	prevValue=newvalue;
	xstatus=xQueueReceive(xQueueUP,&newvalue,100);
	if(xstatus==pdPASS){
if(xSemaphoreGetMutexHolder(EmergencyMutex) == NULL ){
				//sensor sensed obstacle
	if(((GPIO_PORTE_DATA_R >>2) & 1) == 0){
			xSemaphoreGive(EmergencySemaphore);
			}
				//automatic up
	else if(newvalue==0 && prevValue>10000 && prevValue<20000){
			motor_up();
		//limit switch
		while(((GPIO_PORTF_DATA_R >>4) & 1) == 1 &&((GPIO_PORTC_DATA_R >>4) & 1) == 1 &&((GPIO_PORTB_DATA_R >>6) & 1) == 1){
			//sensor sensed obstacle
	if(((GPIO_PORTE_DATA_R >>2) & 1) == 0){
			xSemaphoreGive(EmergencySemaphore);
		while(xSemaphoreGetMutexHolder(EmergencyMutex) != NULL);
		break;
			}
		}
			motor_stop();
	}
	
	else if(newvalue==0||((GPIO_PORTF_DATA_R >>4) & 1) == 0){
		motor_stop();
		}
	else if(newvalue==1){
			motor_up();
		}
		}
		}
		}
		
}

//DOWN
void DOWN(){
		// Initialise the xLastWakeTime variable with the current time.
	portBASE_TYPE xstatus; 
	int newvalue,prevValue=0;
	for( ; ; )
		{
	if(newvalue !=NULL)
	prevValue=newvalue;
	xstatus=xQueueReceive(xQueueDOWN,&newvalue,100);
	if(xstatus==pdPASS){
				//automatic DOWN
		if(newvalue==0 && prevValue>10000 && prevValue<20000){
		motor_down();
		 //limit switch
		while(((GPIO_PORTF_DATA_R >>0) & 1) == 1 && ((GPIO_PORTB_DATA_R >>7) & 1) == 1 &&((GPIO_PORTC_DATA_R >>5) & 1) == 1);
			motor_stop();
	}
		
		else if(newvalue==0 ||((GPIO_PORTF_DATA_R >>0) & 1) == 0){			
			motor_stop();
		}
		else if(newvalue==1){
			motor_down();
		}

		}

		}
		}


		int main()
{
	xQueueUP=xQueueCreate(1,sizeof(int));
	xQueueDOWN=xQueueCreate(1,sizeof(int));
	vSemaphoreCreateBinary(EmergencySemaphore);
	XMUTEX= xSemaphoreCreateMutex();
	EmergencyMutex= xSemaphoreCreateMutex();
	DIO_Init();
	xTaskCreate(Task1, "DriverControll", 128, NULL, 2, NULL);
	xTaskCreate(Task2, "PassengerControll", 128, NULL, 2, &task2);
	xTaskCreate(Task3, "DriverLock", 128, NULL, 2, NULL);
	xTaskCreate(UP, "UP", 128, NULL, 3, NULL);
	xTaskCreate(DOWN, "DOWN", 128, NULL, 3, NULL);
	xTaskCreate(Emergency, "Emergency Down", 128, NULL, 4, NULL);
	if(xQueueUP !=NULL)
	vTaskStartScheduler();
	while(1){
	}
}
