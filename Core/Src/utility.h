#pragma once
#include <stdbool.h>
#include "main.h"

UART_HandleTypeDef* getUSART();

float mappingValue(float value,float preMin,float pres,float postMin,float postMax);
int clamp(int min,int value,int max);
int threasHold(float input,int threasholdCnt,float min,float max);
typedef struct SampleDataStruct{
	int size;
	float* datas;
	float total ;
	float average ;
	int index ;
	int count;

}SampleData;
typedef struct DebounceStruct{
	bool debounceValue;
	int startTime;
	bool currentValue;
}Debounce;
void setDebounceValue(Debounce* debounce,bool val);
bool getDebounceValue(Debounce* debounce);
SampleData createSample(int size);
void AppendSample(float input,SampleData* st);
void Toggle(bool* b);
bool digitalRead(GPIO_TypeDef* GPIOx, uint16_t pin);
void digitalWrite(GPIO_TypeDef* GPIOx, uint16_t pin,bool b);
char getChar();
void printOut(char* str);
void setRed(TIM_HandleTypeDef* tim,float dutyCycle);
void setGreen(TIM_HandleTypeDef* tim,float dutyCycle);
void setBlue(TIM_HandleTypeDef* tim,float dutyCycle);
typedef struct QueueElementStruct{
	void* value;
	struct QueueElementStruct* next;
	struct QueueElementStruct* previous;
}QueueElement;
QueueElement* QueueElement_init(void* value);
typedef struct QueueStruct{
	QueueElement* front;
	QueueElement* back;
	int size;
}Queue;
void DeleteQueueElement(Queue* q,QueueElement* element);
QueueElement* enqueue(Queue* q,void* ptr);
void dequeue(Queue* q);
/*
 * utiliity.c
 *
 *  Created on: Oct 7, 2023
 *      Author: vorapolpromwan
 */


/*
 * utility.h
 *
 *  Created on: Oct 7, 2023
 *      Author: vorapolpromwan
 */

#ifndef SRC_UTILITY_H_
#define SRC_UTILITY_H_



#endif /* SRC_UTILITY_H_ */
