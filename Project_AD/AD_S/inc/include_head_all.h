#ifndef __MOTOR_HEAD_ALL_H
#define __MOTOR_HEAD_ALL_H

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "printf_uart.h" 

#define TASK_NUM 2

typedef enum {FWD=0,FCUWINCOUNT=1,CAN_DEAL=2}TaskStatus;

typedef struct nodeTask  TaskList;
struct nodeTask
{
	TaskStatus TaskType;   //ָʾ��������
	
	Uint16 taskTick;      //���ʱ������
	Uint16 taskPeriod;    //�������� ��λ ms

	
	Uint16 (*TaskFun)(void*);
	
	struct nodeTask  *next;
};

extern TaskList task[];
extern TaskList *PtrTask;
void mainTask(void);
void mainTaskInit(void);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif //__MOTOR_HEAD_ALL_H







