#ifndef  __PRINTF_UART_H
#define  __PRINTF_UART_H

#include <stdio.h>
#include <file.h>

#define SCIBaudRate   128000L
#define TIMEROUTSCI (Uint32)10*(SYSCLK/SCIBaudRate) //估算的等待超时时间，请根据实际修改

#define RTU_TIMEROUT 5 //ms

#define SCI_FIFO_LEN  4 //定义DSP串口FIFO深度


#define UartRxLEN 20  //接收缓存长度
#define UartTxLEN 20  //发送缓存长度


typedef struct Uart_Type{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  UartRevFlag           :1;  //接收到数据标志
	            Uint16  HWOVFlag               :1;  //DSP硬件接收数据溢出标志

	            Uint16  rFifoDataflag            :1;  //接收内存非空
	            Uint16  rFifoFullflag            :1;  //接收内存溢出

	            Uint16  DISRevflag                 :1;  //接收关闭

	    		}Status_Bits;
	  	}Mark_Para;

	char rxData[UartRxLEN];						//接收缓存
	Uint16 rxReadIndex;                         //接收FIFO写入索引
	Uint16 rxWriteIndex;                        //接收FIFO读出索引

	Uint16 timerOut;                            //超时判断
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


