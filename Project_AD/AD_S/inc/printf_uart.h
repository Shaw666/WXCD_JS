#ifndef  __PRINTF_UART_H
#define  __PRINTF_UART_H

#include <stdio.h>
#include <file.h>

#define SCIBaudRate   128000L
#define TIMEROUTSCI (Uint32)10*(SYSCLK/SCIBaudRate) //����ĵȴ���ʱʱ�䣬�����ʵ���޸�

#define RTU_TIMEROUT 5 //ms

#define SCI_FIFO_LEN  3 //����DSP����FIFO���


#define UartRxLEN 20  //���ջ��泤��
#define UartTxLEN 20  //���ͻ��泤��


typedef struct Uart_Type{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  UartRevFlag           :1;  //���յ����ݱ�־
	            Uint16  HWOVFlag               :1;  //DSPӲ���������������־

	            Uint16  rFifoDataflag            :1;  //�����ڴ�ǿ�
	            Uint16  rFifoFullflag            :1;  //�����ڴ����

	            Uint16  DISRevflag                 :1;  //���չر�

	    		}Status_Bits;
	  	}Mark_Para;

	char rxData[UartRxLEN];						//���ջ���
	Uint16 rxReadIndex;                         //����FIFOд������
	Uint16 rxWriteIndex;                        //����FIFO��������

	Uint16 timerOut;                            //��ʱ�ж�
}
Uart_Msg; 

extern Uart_Msg SCI_Msg;

//--------------------------------------------------------------------
void handleRxFIFO(void);
void SCI_Init(Uint32 buad);
void scia_xmit(int a);
void open_uart_debug (void);
int printf(const char* str, ...); 


//---------------------------------------------------------------------
int my_open(const char *path, unsigned flags, int fno);
int my_close(int fno);
int my_read(int fno, char *buffer, unsigned count);
int my_write(int fno, const char *buffer, unsigned count);
off_t my_lseek(int fno, off_t offset, int origin);
int my_unlink(const char *path);
int my_rename(const char *old_name, const char *new_name);


#endif//  __PRINTF_UART_H


