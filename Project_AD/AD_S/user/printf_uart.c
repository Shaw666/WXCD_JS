#include <stdio.h>
#include <file.h>
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File  
#include "HK_all_include.h"


/*

	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  UartRevFlag           :1;  //���յ����ݱ�־
	            Uint16  HWOVFlag          :1;  //DSPӲ���������������־

	            Uint16  rFifoDataflag            :1;  //�����ڴ�ǿ�
	            Uint16  rFifoFullflag            :1;  //�����ڴ����
	    		}Status_Bits;
	  	}Mark_Para;

	char rxData[UartRxLEN];						//���ջ���
	Uint16 rxReadIndex;                         //����FIFO��������
	Uint16 rxWriteIndex;                        //����FIFOд������

	Uint16 timerOut;                            //��ʱ�ж�
}
*/

Uart_Msg SCI_Msg={0, {0},0,0, 0};


//#################################################
//���ڽ��ջ��洦����
//-----------------------------------------------
void handleRxFIFO()
{
	Uint16 wrIndex =0;

	if(SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag == 1 )
	{
		SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag =0;
		while(SCI_Msg.rxReadIndex != SCI_Msg.rxWriteIndex)
		{
			//���ﴦ����յ�������
			wrIndex = (SCI_Msg.rxWriteIndex!=0)?(SCI_Msg.rxWriteIndex-1):(UartRxLEN-1); //��ȡд��FIFOָ�룬�ϴ���PC
			printf("\r\nrxData[%d] = %c ; wr:%d",SCI_Msg.rxReadIndex,SCI_Msg.rxData[SCI_Msg.rxReadIndex],wrIndex);
		   //���ڵ���LED
			switch(SCI_Msg.rxData[SCI_Msg.rxReadIndex])
			{
			   case 0x01:  D401TOGGLE();break;
			   case 0x02:  D402TOGGLE();break;
			   default:break;
			}
			SCI_Msg.rxReadIndex=(++SCI_Msg.rxReadIndex)%(UartRxLEN);
		}

	}

	//�����������������
	if(SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag ==1)
	{
		SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag = 0;
		printf("\r\n---------------FIFO overflow-------------------------------------");

		SCI_Msg.rxReadIndex  = 0;
		SCI_Msg.rxWriteIndex  = 0;//SCI_Msg.rxReadIndex;
	}

	//������Ӳ�����ڽ����������
	if(SCI_Msg.Mark_Para.Status_Bits.HWOVFlag == 1)
	{
		SCI_Msg.Mark_Para.Status_Bits.HWOVFlag = 0;
		printf("\r\n---------------SCI HW Overflow ISR Clear-------------------------");
	}

	//ɨ��Ӳ��FIFO�������
	if(SciaRegs.SCIFFRX.bit.RXFFOVF == 1)
	{
		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
		SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
		printf("\r\n---------------Mannul HW Overflow  Clear------------------------");

	}
}

//#################################################
//���ڽ����жϺ���
//����FIFO���ƣ����棩
//SCI_FIFO_LEN ����Ϊ 1�����Ϊ4
//-----------------------------------------------
interrupt void uartRx_isr(void)
{
//	if(SciaRegs.SCIFFRX.bit.RXFFOVF == 0)//����FIFOδ���
//	{
//		SCI_Msg.Mark_Para.Status_Bits.rFifoDataflag = 1;
//
//		if((SCI_Msg.rxWriteIndex + SCI_FIFO_LEN) != SCI_Msg.rxReadIndex )
//		{
//			//��������
//			while(SciaRegs.SCIFFRX.bit.RXFFST)
//			{
//				SCI_Msg.rxData[SCI_Msg.rxWriteIndex] = SciaRegs.SCIRXBUF.all;
//				SCI_Msg.rxWriteIndex=(++SCI_Msg.rxWriteIndex)%(UartRxLEN);
//			}
//		}
//		else//���ջ�����
//		{
//			//�û��������������Ĵ���,
//			SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
//			SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
//
//			SCI_Msg.Mark_Para.Status_Bits.rFifoFullflag = 1;
//		}
//	}
//	else
//	{
//		//�û�����������Ӳ������Ĵ���,������ȫ��ȡ��FIFO������ݻ������FIFO
//		//�������FIFO����
//		SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear HW Overflow flag
//		SciaRegs.SCIFFRX.bit.RXFIFORESET = 0;  //Write 0 to reset the FIFO pointer to zero, and hold in reset.
//		SciaRegs.SCIFFRX.bit.RXFIFORESET = 1 ; //Re-enable receive FIFO operation
//		SCI_Msg.Mark_Para.Status_Bits.HWOVFlag = 1;
//
//	}

	SCI_Msg.rxData[SCI_Msg.rxWriteIndex] = SciaRegs.SCIRXBUF.all;
    if(SCI_Msg.rxWriteIndex < UartRxLEN)
	{
    	SCI_Msg.rxWriteIndex ++;
	}else{
		SCI_Msg.rxWriteIndex = 0;
	}
    SciaRegs.SCITXBUF = SCI_Msg.rxData[SCI_Msg.rxWriteIndex];

    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP9;
}




//#################################################
//-----------------------------------------------
//���ڳ�ʼ��
//128000  8N1
// P21 ��ͼ�����ж�
//-----------------------------------------------
void SCI_Init(Uint32 buad)
{
	Uint16 brr_reg = (1875000/buad) - 1;	//15000000/8 = 1875000
	InitSciaGpio();
	SciaRegs.SCICTL1.bit.SWRESET = 0;

 	SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol

// baud = LSPCLK/8/((BRR+1) 
// baud @LSPCLK = 15MHz (60 MHz SYSCLK)
	SciaRegs.SCIHBAUD = (brr_reg>>8)&0x00FF;
    SciaRegs.SCILBAUD = brr_reg&0x00FF;  //0XC2-->9600 ; 97--> 19200 ;0x30-->38400;14-->128000

    SciaRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset
    SciaRegs.SCIFFTX.bit.SCIRST=1;

	SciaRegs.SCIFFRX.bit.RXFFIL  = SCI_FIFO_LEN;  //����FIFO���
	SciaRegs.SCICTL1.bit.TXENA = 1;       //ʹ�ܷ���
	SciaRegs.SCICTL1.bit.RXENA = 1;       //ʹ�ܽ���

//	SciaRegs.SCICTL2.bit.TXINTENA =1;
//	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
//  SciaRegs.SCIFFTX.bit.TXFFIENA = 0; //��ֹ�����ж�ʹ��
	//�ж����ò���-----1
	SciaRegs.SCIFFTX.bit.SCIFFENA = 1; //ʹ��FIFO�ж�
	SciaRegs.SCIFFRX.bit.RXFFIENA=1;
	EALLOW;
	PieVectTable.SCIRXINTA = &uartRx_isr; //�ж����ò���-----2
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;    //�ж����ò���-----3
	IER |= M_INT9;						  //�ж����ò���-----4

	SciaRegs.SCIFFCT.all=0x00;

	SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET=1;
}


//#################################################
//-----------------------------------------------
//����һ���ֽ�,��ʱ����,SYSCLK = 60MHz
//-----------------------------------------------
void scia_xmit(int a)
{
	Uint32 WaitTimer = 0;

	while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
	//while(SciaRegs.SCICTL2.bit.TXEMPTY != 1)
	{
		WaitTimer++;
		if(WaitTimer > TIMEROUTSCI)break;
	}
	if(WaitTimer <= TIMEROUTSCI)
		SciaRegs.SCITXBUF=a;
}






//-------------------------------���¶�Ϊ ��׼ printf��������--------------------------------------------------------------
//#################################################
//-----------------------------------------------
//Printf ��������
//-----------------------------------------------
void open_uart_debug (void)
{
	int status;
	status = add_device("uart", _MSA, my_open, my_close, my_read, my_write, my_lseek, my_unlink, my_rename);
	if (status == 0) {
		freopen("uart:", "w", stdout);		// open uart and redirect stdout to UART
		setvbuf(stdout, NULL, _IONBF, 0); 	// disable buffering for stdout
	}
}

int my_open(const char *path, unsigned flags, int fno)
{
	//scia_fifo_init();
	//scia_echoback_init();
	path = path;
	flags = flags;
	fno = fno;
	return 0;
}

int my_close(int fno)
{
	fno =fno;
	return 0;
}

int my_read(int fno, char *buffer, unsigned count)
{
	fno = fno;
	buffer = buffer;
	count = count;
	return 0;
}

int my_write(int fno, const char *buffer, unsigned count)
{	
	int i=0;
	fno = fno;
	
	while(count-- > 0) {
		scia_xmit(buffer[i++]);
	}
	return count;
}

off_t my_lseek(int fno, off_t offset, int origin)
{
	fno = fno; offset = offset; origin = origin;
	return 0;
}

int my_unlink(const char *path)
{
	path = path;
	return 0;
}

int my_rename(const char *old_name, const char *new_name)
{
	old_name = old_name;
	new_name = new_name;
	return 0;
}

