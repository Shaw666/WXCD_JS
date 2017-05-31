#include <stdio.h>
#include <file.h>
#include "Module_Project.h"

__interrupt void Module_SciaRxFIFO(void);

union Module_Fault_REG ModuleFault;

void SetupSCI(Uint32 buad)
{
	Uint16 brr_reg = (1875000 / buad) - 1;	//15000000/8 = 1875000
	//Allow write to protected registers
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;		// Enable pull-up for GPIO22 (LIN TX)
	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;		// Enable pull-up for GPIO23 (LIN RX)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO23 (LINRXA)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO19 for LIN TX operation	 (3-Enable,0-Disable)
	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO23 for LIN RX operati

	LinaRegs.SCIGCR0.bit.RESET = 0; //Into reset
	LinaRegs.SCIGCR0.bit.RESET = 1; //Out of reset

	LinaRegs.SCIGCR1.bit.SWnRST = 0; //Into software reset

	//SCI Configurations
	LinaRegs.SCIGCR1.bit.COMMMODE = 0;   //Idle-Line Mode
	LinaRegs.SCIGCR1.bit.TIMINGMODE = 1; //Asynchronous Timing
	LinaRegs.SCIGCR1.bit.PARITYENA = 0;  //No Parity Check
	LinaRegs.SCIGCR1.bit.PARITY = 0;	 //Odd Parity
	LinaRegs.SCIGCR1.bit.STOP = 0;		 //One Stop Bit
	LinaRegs.SCIGCR1.bit.CLK_MASTER = 1; //Enable SCI Clock
	LinaRegs.SCIGCR1.bit.LINMODE = 0;	 //SCI Mode
	LinaRegs.SCIGCR1.bit.SLEEP = 0;      //Ensure Out of Sleep
	LinaRegs.SCIGCR1.bit.MBUFMODE = 0;	 //No Buffers Mode
	LinaRegs.SCIGCR1.bit.LOOPBACK = 0;   //External Loopback
	LinaRegs.SCIGCR1.bit.CONT = 1;		 //Continue on Suspend
	LinaRegs.SCIGCR1.bit.RXENA = 1;		 //Enable RX
	LinaRegs.SCIGCR1.bit.TXENA = 1;		 //Enable TX

	//Ensure IODFT is disabled
    LinaRegs.IODFTCTRL.bit.IODFTENA = 0x0;

    //Set transmission length
    LinaRegs.SCIFORMAT.bit.CHAR = 7;	 //Eight bits
    LinaRegs.SCIFORMAT.bit.LENGTH = 0;   //One byte

	//Set baudrate
    LinaRegs.BRSR.bit.SCI_LIN_PSL =  brr_reg & 0x00FF; //0XC2-->9600 ; 97--> 19200 ;0x30-->38400;14-->128000
    LinaRegs.BRSR.bit.SCI_LIN_PSH = (brr_reg >> 8) & 0x00FF;
    // baud = LSPCLK/8/((BRR+1)

    LinaRegs.BRSR.bit.M = 5;

    LinaRegs.SCIGCR1.bit.SWnRST = 1;  //bring out of software reset

	//Disable write to protected registers
	EDIS;
	//等待准备完毕
	while(LinaRegs.SCIFLR.bit.IDLE == 1);
//	//Wait for a charachter to by typed
//	if(LinaRegs.SCIFLR.bit.RXRDY == 1)
//	{
//		ReceivedChar = LinaRegs.SCIRD;
//
//		//Wait for the module to be ready to transmit
//		while(LinaRegs.SCIFLR.bit.TXRDY == 0);
//		//Begin transmission
//		LinaRegs.SCITD = ReceivedChar;
//	}
}


void SCI_Init(Uint32 buad) {
	Uint16 brr_reg = (1875000 / buad) - 1;	//15000000/8 = 1875000
	InitSciaGpio();
	SciaRegs.SCICTL1.bit.SWRESET = 0;

	SciaRegs.SCICCR.all = 0x0007;   // 1 stop bit,  No loopback
									// No parity,8 char bits,
									// async mode, idle-line protocol

// baud = LSPCLK/8/((BRR+1) 
// baud @LSPCLK = 15MHz (60 MHz SYSCLK)
	SciaRegs.SCIHBAUD = (brr_reg >> 8) & 0x00FF;
	SciaRegs.SCILBAUD = brr_reg & 0x00FF; //0XC2-->9600 ; 97--> 19200 ;0x30-->38400;14-->128000

	SciaRegs.SCICTL1.bit.SWRESET = 1;     // Relinquish SCI from Reset
	SciaRegs.SCIFFTX.bit.SCIRST = 1;

	SciaRegs.SCIFFRX.bit.RXFFIL = 1;  //设置FIFO深度
	SciaRegs.SCIFFTX.bit.TXFFIL = 1;  //设置FIFO深度
	SciaRegs.SCICTL1.bit.TXENA = 1;       //使能发送
	SciaRegs.SCICTL1.bit.RXENA = 1;       //使能接收

//	SciaRegs.SCICTL2.bit.TXINTENA =1;
//	SciaRegs.SCICTL2.bit.RXBKINTENA =1;
//  SciaRegs.SCIFFTX.bit.TXFFIENA = 0; //禁止发送中断使能
	//中断配置步骤-----1
	SciaRegs.SCIFFTX.bit.SCIFFENA = 1; //使能FIFO中断
	SciaRegs.SCIFFRX.bit.RXFFIENA = 1;
	EALLOW;
	PieVectTable.SCIRXINTA = &Module_SciaRxFIFO; //中断配置步骤-----2
	EDIS;
	PieCtrlRegs.PIEIER9.bit.INTx1 = 1;    //中断配置步骤-----3
	IER |= M_INT9;						  //中断配置步骤-----4
	SciaRegs.SCIFFCT.all = 0x00;
	SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
	SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
}

Uint16 SciaReceiveCount = 0;
Uint16 SciaReceiveBuff[5];
Uint16 SciaRecTimeoutCount = 0;
Uint16 DataBuff[10];
void Module_SciaRxFIFO(void)  //串口接收中断
{
	Uint16 data, rankCount;

	while (SciaRegs.SCIFFRX.bit.RXFFST > 0) {
		if (SciaReceiveCount < DealRxLenth) {
			data = SciaRegs.SCIRXBUF.all;
			SciaReceiveBuff[SciaReceiveCount++] = data;
//			LinaRegs.SCITD = data;
		}
	}

	if (SciaReceiveCount >= DealRxLenth) {
		switch (SciaReceiveBuff[0]) {
		case 0x51:
//PFC后电压电流
			break;
		case 0x55:
//前级状态帧
			DataBuff[0] = ((SciaReceiveBuff[1] & 0x00ff) << 8)
					+ ((SciaReceiveBuff[2] & 0x00FF));
			DataBuff[1] = ((SciaReceiveBuff[3] & 0x00ff) << 8)
					+ ((SciaReceiveBuff[4] & 0x00FF));

			break;
		case 0xFA:
			for (rankCount = 1; rankCount < 5; rankCount++) {
				if (SciaReceiveBuff[rankCount] == 0x0055) {
				} else {
					//握手失败重发握手信息
					SendRequestSCI(0xAF);
					SciaRecTimeoutCount++;
					if(SciaRecTimeoutCount>=10){
						//已经重新握手十次不成功，取消本次充电请求，关闭接收端主机。
					}
					break;
				}
			}
			break;
		default:
			SciaReceiveCount = 0;
			break;
		}
		SciaReceiveCount = 0;
	}
	if (SciaRegs.SCIRXST.bit.RXERROR == 1) {
		SciaRegs.SCICTL1.bit.SWRESET = 0; //复位串口
		SciaRegs.SCICTL1.bit.SWRESET = 1;
	}
	SciaRegs.SCIFFRX.bit.RXFFOVRCLR = 1;  // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;  // Clear Interrupt flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR = 1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.bit.ACK9 = 1;
	//Begin transmission

}

Uint16 SciaResponseBuff[5];
void ResponseSCI(Uint16* tmp) {
	Uint16 SciaResponseCount = 0;
	Uint16 SciaResponTimeoutCount = 0;
	for (SciaResponseCount = 0; SciaResponseCount < 5; SciaResponseCount++) {
		while (SciaRegs.SCIFFTX.bit.TXFFST != 0) {
			SciaResponTimeoutCount++;
			if (SciaResponTimeoutCount >= 200)
				break;
		}
		//Begin transmission
		SciaRegs.SCITXBUF = *tmp++;
	}

}
//Uint16 key=0,flag=0,counttt=0;
Uint16 SciaSendBuff[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
Uint16 SendRequestSCI(Uint16 tmp) {
	Uint16 SciaSendTimeoutCount = 0;
	Uint16 SciaSendCount = 0;
	SciaSendBuff[0] = tmp;
	switch (SciaSendBuff[0]) {
	case 0xA1:
		//发送交流输入电压电流
	{
		SciaSendBuff[1] = ((Sample.JSOutVoltReal & 0xff00) >> 8);
		SciaSendBuff[2] = (Sample.JSOutVoltReal & 0x00ff);
		SciaSendBuff[3] = ((Sample.JSOutCurrentReal & 0xff00) >> 8);
		SciaSendBuff[4] = ((Sample.JSOutCurrentReal & 0x00ff));
	}

		break;
	case 0xA2:
		//发送直流输出电压电流
	{

		SciaSendBuff[1] = ((Sample.JSOutVoltProtectReal & 0xff00) >> 8);
		SciaSendBuff[2] = (Sample.JSOutVoltProtectReal & 0x00ff);
		SciaSendBuff[3] = ((Sample.JSOutCurrentReal & 0xff00) >> 8);
		SciaSendBuff[4] = ((Sample.JSOutCurrentReal & 0x00ff));
//		counttt++;
//		key = key^0x01;
//		if(flag==1) SciaSendBuff[0]=0xff;
//			if(key==0x01){
//			GpioDataRegs.GPASET.bit.GPIO5 = 1;
//			}
//			else {GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;}
//			if((Sample.JSOutVoltProtectReal<=0x0639)&&(counttt>=5000)) {GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
//			flag = 1;
//			}
	}
		break;
	case 0x55:
		//状态
	{
		SciaSendBuff[1] = ((ModuleFault.all & 0xff00) >> 8);
		SciaSendBuff[2] = ((ModuleFault.all & 0x00ff));
	}
		break;
	case 0x11:
		//开机
	{
		SciaSendBuff[0] = 0xAA;
		SciaSendBuff[1] = 0xFF;
		SciaSendBuff[2] = 0xFF;
		SciaSendBuff[3] = 0xFF;
		SciaSendBuff[4] = 0xFF;
	}
		break;
	case 0x22:
		//关机
	{
		SciaSendBuff[0] = 0xAA;
		SciaSendBuff[1] = 0x00;
		SciaSendBuff[2] = 0x00;
		SciaSendBuff[3] = 0x00;
		SciaSendBuff[4] = 0x00;
	}
		break;
	case 0xFF:
	//设定环路电压值
	{
		SciaSendBuff[1] = ((ModuleCtlReg.OutputVoltSet&0xff00)>>8);
		SciaSendBuff[2] = ((ModuleCtlReg.OutputVoltSet&0x00ff));
	}
		break;
	case 0xAF:
	{
		SciaSendBuff[1] = 0xAA;
		SciaSendBuff[2] = 0xAA;
		SciaSendBuff[3] = 0xAA;
		SciaSendBuff[4] = 0xAA;
	}
		break;
	default:
		break;
	}
	if (SciaSendBuff[0] != 0x00) {
		for (SciaSendCount = 0; SciaSendCount < 5; SciaSendCount++) {
			while (SciaRegs.SCIFFTX.bit.TXFFST != 0){
				SciaSendTimeoutCount++;
				if (SciaSendTimeoutCount >= 200)
					break;   //发送超时
			}
			//Begin transmission
			SciaRegs.SCITXBUF = SciaSendBuff[SciaSendCount];
		}
	}
	return 0x00;   //发送成功
}















//-------------------------------以下都为 标准 printf函数连接--------------------------------------------------------------
//#################################################
//-----------------------------------------------
//Printf 函数连接
//-----------------------------------------------
void scia_xmit(Uint16 a) {
	while (SciaRegs.SCIFFTX.bit.TXFFST != 0);
	SciaRegs.SCITXBUF = a;
//	while(LinaRegs.SCIFLR.bit.TXRDY == 0);
//	LinaRegs.SCITD = a;
}

void open_uart_debug(void) {
	int status;
	status = add_device("uart", _MSA, my_open, my_close, my_read, my_write,
			my_lseek, my_unlink, my_rename);
	if (status == 0) {
		freopen("uart:", "w", stdout);	// open uart and redirect stdout to UART
				setvbuf(stdout, NULL, _IONBF, 0);// disable buffering for stdout
			}
		}

int my_open(const char *path, unsigned flags, int fno) {
	//scia_fifo_init();
	//scia_echoback_init();
	path = path;
	flags = flags;
	fno = fno;
	return 0;
}

int my_close(int fno) {
	fno = fno;
	return 0;
}

int my_read(int fno, char *buffer, unsigned count) {
	fno = fno;
	buffer = buffer;
	count = count;
	return 0;
}

int my_write(int fno, const char *buffer, unsigned count) {
	int i = 0;
	fno = fno;

	while (count-- > 0) {
		scia_xmit(buffer[i++]);
	}
	return count;
}

off_t my_lseek(int fno, off_t offset, int origin) {
	fno = fno;
	offset = offset;
	origin = origin;
	return 0;
}

int my_unlink(const char *path) {
	path = path;
	return 0;
}

int my_rename(const char *old_name, const char *new_name) {
	old_name = old_name;
	new_name = new_name;
	return 0;
}

