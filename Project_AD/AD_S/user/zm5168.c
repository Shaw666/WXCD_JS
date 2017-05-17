/*
 * zm5168.c
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */
#include "Module_Project.h"

Uint16 DealRxLenth = 0;
Uint16 DealTxLenth = 0;



/*
 * ������չ���zm5168����zigbee��������
 * GPIO 2 6
 * DSPTX---- RX13
 * DSPRX---- TX12
 */
void ZM5168_INit(void)
{
	//��ʼ��linaΪuartģʽ
//	SetupSCI();
	EALLOW;
	//GPIO�Ĵ����ܱ���  reset WAKE RX TX ----- GPIO2 GPIO6 LINTX LINRX
	GpioCtrlRegs.GPAMUX1.bit.GPIO9  = 0;	//GPIO2��Ϊ��ͨIO  1ms�͵�ƽ��Ч
	GpioCtrlRegs.GPAMUX1.bit.GPIO6  = 0;	//GPIO6��Ϊ��ͨIO

	GpioCtrlRegs.GPADIR.bit.GPIO9  = 1;		//GPIO2����Ϊ���
	GpioCtrlRegs.GPADIR.bit.GPIO6  = 1;		//GPIO6����Ϊ���

	GpioDataRegs.GPASET.bit.GPIO9 = 1;		//GPIO2����ߵ�ƽ
	GpioDataRegs.GPASET.bit.GPIO6 = 1;  	//GPIO6����ߵ�ƽ
	EDIS;
	GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;  	//��λ������
	DELAY_US(1024);                        //��ʱ1024us
	GpioDataRegs.GPASET.bit.GPIO9 = 1;  	//����
//	ReadLocalConf();
}


//����Э���־ 0--��ʱ  1--Y����
u8 SendAgreementFlag(Uint16 op)
{
	u8 res=0;
	if(op!=0){
	scia_xmit(Agreement_1);
	scia_xmit(Agreement_2);
	scia_xmit(Agreement_3);
	}else{
	scia_xmit(tmpAgreement_1);
	scia_xmit(tmpAgreement_2);
	scia_xmit(tmpAgreement_3);
	}
	return res;
}

DEV_INFO zm5168_p2={0};

void LocalConfDeal(Uint16 temp[69])
{
	Uint16 i,j=4;

	for(i=0,j=4;i<8;i++,j++){
		zm5168_p2.DevName[i] = 0xff00&(temp[j]<<8);
	    zm5168_p2.DevName[i] |= 0x00ff&temp[++j];
	}
	for(i=0;i<8;i++,j++){
		zm5168_p2.DevPwd[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.DevPwd[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.DevMode_Chan = temp[36]<<8|temp[37];
	zm5168_p2.PanID = temp[38]<<8|temp[39];
	zm5168_p2.MyAddr = temp[40]<<8|temp[41];
	for(i=0,j=42;i<4;i++,j++){
		zm5168_p2.MyIEEE[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.MyIEEE[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.DstAddr = temp[50]<<8|temp[51];
	for(i=0,j=52;i<4;i++,j++){
		zm5168_p2.DstIEEE[i] = 0xff00&(temp[j]<<8);
		zm5168_p2.DstIEEE[i] |= 0x00ff&temp[++j];
	}
	zm5168_p2.Reserve_PowerLevel = temp[60]<<8|temp[61];
	zm5168_p2.RetryNum_TranTimeout = temp[62]<<8|temp[63];
	zm5168_p2.Serial_Rate_Serial_DataB = temp[64]<<8|temp[65];
	zm5168_p2.Serial_StopB_Serial_ParityB = temp[66]<<8|temp[67];
	zm5168_p2.SendMode = temp[68];
	for(i=0;i<1000;i++){
	DELAY_US(1024);
	}
	WriteConf(zm5168_p2.MyAddr,temp);
}

//��ȡ��������
u8 ReadLocalConf()
{
	u8 res=0x00;
	DealRxLenth = 74;//�ı䴮�ڽ���
	SendAgreementFlag(1);//������������Э���־
	scia_xmit(RLocalConf); //���͹�����
	res=0x0ff&(Agreement_1+Agreement_2+Agreement_3+RLocalConf);
	scia_xmit(res);
	res=0x00;
	return res;
}
//����ͨ����
u8 SetChanelNum(u8 chanelnum)
{
	u8 res=0;
	return res;
}
//����
u8 SearchDevice()
{
	u8 res=0;
	DealRxLenth = 13;//�ı䴮�ڽ���
	SendAgreementFlag(1);//������������Э���־
	scia_xmit(Search); //���͹�����
	res=0x0ff&(Agreement_1+Agreement_2+Agreement_3+Search);
	scia_xmit(res);
	DealRxLenth = 13;
	return res;
}
//��ȡԶ��������Ϣ
u8 ReadRemoteConf(u16 remoteadress)
{
	u8 res=0;
	return res;
}
//�޸���������
//���Ƽ�ʹ��
Uint16 WriteConf(Uint16 deviceadress, Uint16 setTemp[74])
{
	u8 res=0,i;
	Uint16 checkSum;
	DealRxLenth = 7;
	deviceadress= 0x2001;
	SendAgreementFlag(1);//3

	scia_xmit(WConf);//1
	scia_xmit(((0xff00&deviceadress)>>8));
	scia_xmit((0x00ff&deviceadress));//2
	for(i=4;i<69;i++){
		scia_xmit(setTemp[i]);
		checkSum += setTemp[i];
	}
	res = (0x0ff&(checkSum+Agreement_1+Agreement_2+Agreement_3+WConf+((0xff00&deviceadress)>>8)+(0x00ff&deviceadress)));
	scia_xmit(res);

	return res;
}
//��λ�豸
Uint16 ResetDevice(Uint16 deviceTypeh, Uint16 deviceTypel)
{
	u8 res=0;
//	zm5168_p2.MyAddr=0x2001;
	SendAgreementFlag(1);//3
	scia_xmit(Reset); //1
	scia_xmit(((0xff00&zm5168_p2.MyAddr)>>8));
	scia_xmit((0x00ff&zm5168_p2.MyAddr));//2
	scia_xmit(deviceTypeh);
	scia_xmit(deviceTypel);
	res = (0x00ff&(0x20+0x01+Agreement_1+Agreement_2+Agreement_3+Reset+deviceTypeh+deviceTypel));
	scia_xmit(res);
	return res;
}
//�ظ���������
u8 RestoreDevice(u16 deviceadress, u16 devicetype)
{
	u8 res=0;
	return res;
}

