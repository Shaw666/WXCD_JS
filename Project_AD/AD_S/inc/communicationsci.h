#ifndef  __COMMUNICATIONSCI_H
#define  __COMMUNICATIONSCI_H

#include <stdio.h>
#include <file.h>

#define SCIBaudRate   128000L
#define TIMEROUTSCI (Uint32)10*(SYSCLK/SCIBaudRate) //估算的等待超时时间，请根据实际修改

#define RTU_TIMEROUT 5 //ms

extern Uint16 DealRxLenth;
extern Uint16 DealTxLenth;

//--------------------------------------------------------------------
void handleRxFIFO(void);
void SCI_Init(Uint32 buad);
void SetupSCI(Uint32 buad);
Uint16 SendRequestSCI(Uint16 tmp);
void ResponseSCI(Uint16* tmp);


void open_uart_debug (void);
int printf(const char* str, ...); 
interrupt void uartRx_isr(void);

void scia_xmit(Uint16 a);
//---------------------------------------------------------------------
int my_open(const char *path, unsigned flags, int fno);
int my_close(int fno);
int my_read(int fno, char *buffer, unsigned count);
int my_write(int fno, const char *buffer, unsigned count);
off_t my_lseek(int fno, off_t offset, int origin);
int my_unlink(const char *path);
int my_rename(const char *old_name, const char *new_name);


#endif//  __PRINTF_UART_H


