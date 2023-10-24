#include "utility.h"
extern UART_HandleTypeDef huart3;

float mappingValue(float val,float preMin,float preMax,float postMin,float postMax){
	  return (val-preMin)/(preMax-preMin)*(postMax-postMin)+postMin;
  }
int clamp(int min,int value,int max){
	if(min>value)
		return min;
	else if(max<value)
		return max;
	else
		return value;
}
bool inBetween(int min,int value,int max){
	return (min<=value)&&(value<=max);
}
int abs(int a){
	return a<0?-a:a;
}
int min(int a,int b){
	return a<=b?a:b;
}
int max(int a,int b){
	return a>=b?a:b;
}
int threasHold(float input,int threasholdCnt,float min,float max){
	float threasholdRange = (max-min )/(float)threasholdCnt;
	int returnValue = (int)((input-min)/threasholdRange);
	if(returnValue >= threasholdCnt)
		returnValue--;
	return returnValue;
}
SampleData createSample(int size){
	SampleData sample;
	sample.size = size;
	sample.datas = malloc(4*size);
	sample.total = 0;
	sample.average = 0;
	sample.index = 0;
	sample.count = 0;
	for(int i=0;i<size;i++){
		*(sample.datas+i) = 0;
	}
	return sample;
}
void AppendSample(float input,SampleData* st){
	if(st->count<st->size)
		st->count++;
	st->total +=input-st->datas[st->index];
	st->datas[st->index]=input;
	st->average = st->total/st->count;
	st->index=st->index<st->size?st->index+1:0;
}
void setDebounceValue(Debounce* debounce,bool val){
	if(debounce->currentValue !=val){
		debounce->currentValue = val;
		debounce->startTime = HAL_TICK();
	}
}
bool getDebounceValue(Debounce* debounce){
	if(debounce->currentValue != debounce->debounceValue&&HAL_TICK()-debounce->startTime>100){
		debounce->debounceValue = debounce->currentValue;
	}
	return debounce->debounceValue;
}
void Toggle(bool* b){
	*b=!*b;
}
bool digitalRead(GPIO_TypeDef* GPIOx, uint16_t pin){
	return HAL_GPIO__ReadPin(GPIOx,pin) == GPIO_PIN_SET;
}
void digitalWrite(GPIO_TypeDef* GPIOx, uint16_t pin,bool b){
	//Example GPIOB,GPIO_PIN_0 ,GPIO_PIN_RESET/SET
	HAL_GPIO_WritePin( GPIOx,pin ,b);
}
uint32_t ADCRead(ADC_HandleTypeDef* hadc){
	while(HAL_ADC_PollForConversion(hadc,100)!=HAL_OK){}
	return HAL_ADC_GetValue(hadc);
}
char getChar(){
	char ch1;
	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_RXNE)== RESET){}
	HAL_UART_Receive(&huart3, (uint8_t*) &ch1, 1, 1000);
	return ch1;
}
void printOut(char* str){

	while(__HAL_UART_GET_FLAG(&huart3,UART_FLAG_TC)==RESET){}
		HAL_UART_Transmit(&huart3, (uint8_t*) str,strlen(str),1000);

}
void setRed(TIM_HandleTypeDef* tim,float dutyCycle){
	tim->Instance -> CCR1 = (1000-1) * dutyCycle;
}
void setGreen(TIM_HandleTypeDef* tim,float dutyCycle){
	tim->Instance -> CCR2 = (1000-1) * dutyCycle;
}
void setBlue(TIM_HandleTypeDef* tim,float dutyCycle){
	tim->Instance -> CCR3 = (1000-1) * dutyCycle;
}
QueueElement* QueueElement_init(void* value){
	QueueElement* element = malloc(sizeof(QueueElement));
	element->value = value;
	element->previous = NULL;
	element->next = NULL;
	return element;
}
Queue* Queue_init(){
	Queue* q = malloc(sizeof(Queue));
	q->front = NULL;
	q->back = NULL;
	q->size = 0;
	return q;
}
void DeleteQueueElement(Queue* q,QueueElement* element){
	if(q==NULL||element==NULL)
		return;
	q->size--;
	if(element->next == NULL){
		q->back = element->previous;
	}
	else{
		element->next->previous = element->previous;
	}
	if(element->previous == NULL){
		q->front = element->next;
	}
	else{
		element->previous->next = element->next;
	}
	free(element->value);
	free(element);
}
QueueElement* enqueue(Queue* q,void* ptr){
	if(q == NULL&&ptr == NULL)
		return NULL;
	QueueElement* element = QueueElement_init(ptr);
	if(q->size){
		q->back->next =element;
		element->previous = q->back;
	}
	else{
		q->front = element;
		element->previous = NULL;
		element->next = NULL;
	}
	q->size++;
	q->back=element;
	return element;
}
void dequeue(Queue* q){
	if(q != NULL)
		DeleteQueueElement(q,q->front);

}
/*
 * utiliity.c
 *
 *  Created on: Oct 7, 2023
 *      Author: vorapolpromwan
 */


