/*
 * CANcomm.h
 *
 *  Created on: 2013-11-4
 *      Author: cotto
 */

#ifndef CANCOMM_H_
#define CANCOMM_H_

#define CANSENDNOWAIT   //CAN ���Ͳ��ȴ�������� TA ��־ (Ĭ�ϲ��Գ���ѡ��)
//#define CANNOWWAIT     //CAN ���͵ȴ���ǰ���䷢����� TA ��־
//#define CANLASTWAIT	// CAN ���͵ȴ���һ��ID������� TA ��־���Ƽ���ʽ

#define CANRxLEN 20

typedef struct CAN_Rev{
	union
	  {
	    Uint16 All;
	    struct{
	            Uint16  RevFlag                  :1;  //���յ����ݱ�־
	            Uint16  HWOVFlag                 :1;  //DSPӲ���������������־
	            Uint16  rFifoDataflag            :1;  //�����ڴ�ǿ�
	            Uint16  rFifoFullFlag             :1;  //�����ڴ����

	    		}Status_Bits;
	  	}Mark_Para;
	Uint16 rxData[CANRxLEN][4];				     //���ջ���, [0]�Žϵ�λ����
	Uint16 rxReadIndex;                         //����FIFOд������
	Uint16 rxWriteIndex;                        //����FIFO��������
}RCAN_Msg;

extern RCAN_Msg CANRxMsg[];
extern RCAN_Msg *PtrCanRx;



typedef struct CAN_Tx{
	Uint16 DA;
	Uint16 DB;
	Uint16 DC;
	Uint16 DD;

	Uint16 MBXnbr;                      //ָ�����͵�����
	Uint16 RevMBXnID ;                  //ָ�����͵�ID��,ȡֵ��Χ 0x00 �� 0x3FF , 11bit ��׼֡ID
}TCAN_Msg;

extern TCAN_Msg  CANTxMsg[];
extern TCAN_Msg *PtrCanTx;


void CAN_Init_Interrupt(void);
void canMailboxInit(void);
void InitECana_1(void);

void CANInit(void);
void CAN_Send(TCAN_Msg *CANTx);
void handleCANRxFIFO(void);


#endif /* CANCOMM_H_ */
