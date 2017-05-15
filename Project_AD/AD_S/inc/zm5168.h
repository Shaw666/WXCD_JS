/******************************
 * zm5168.h
 *��ʱ��������Э��   ����֡ ��ʽ          |  Ӧ�𷵻�֡
 * |3�ֽڣ�Э���־)|1�ֽ�  |N�ֽ� | 3�ֽڣ�Э���־)|1�ֽ�  |1�ֽ�
 * |DE DF EF      |������|����   | DE DF EF      |������|����ֵ
 ****************************/
//ssssssssssssss
/******************************
 * ���ò�������Э��   ����֡ ��ʽ
 * |3�ֽڣ�Э���־)|1�ֽ�  |N�ֽ�       |1�ֽ�      |
 * |AB BC CD      |������|����ʵ��  |�ֽ�У�� |
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 *******************************/

#ifndef ZM5168_H_
#define ZM5168_H_
//У�� ��������Э���־-----------------------------------------------
#define Agreement_1		0xAB
#define Agreement_2		0xBC
#define Agreement_3		0xCD        //AB+BC+CD=0x234
//#define checksum(x)		x&0x0ff   	//x��ȡ��һ�ֽڳ�У��λ֮��ĺ�
//Ӧ��֡��Ӧ״̬
#define OK				0x00
#define LENGTH_FAUSE	0x01
#define ADDRESS_FAUSE	0x02
#define CHECK_FAUSE		0x03
#define WRITE_FAUSE		0x04
#define OTHER_FAUSE		0x05
//������
#define RLocalConf		0xD1		//��ȡ��������
#define SChannelNum		0xD2		//����ͨ���� ���粻����
#define Search			0xD4		//����
#define RRemoteConf		0xD5		//��ȡԶ��������Ϣ
#define WConf			0xD6		//�޸����ã����óɹ��踴λ��
#define Reset			0xD9		//��λģ��
#define Restore			0xDA		//�ָ��������ã����óɹ��踴λ��

//��ʱ����Э���־-----------------------------------------------------
#define tmpAgreement_1		0xDE
#define tmpAgreement_2		0xDF
#define tmpAgreement_3		0xEF        //AB+BC+CD=0x234
//������
#define tmpSChannelNum		0xD1     //�޸�ͨ����
#define tmpSDstAddress      0xD2     //�޸�Ŀ�������ַ
#define tmphibernate        0xD8     //��������
#define tmpSMode			0xD9     //����ͨѶģʽ
#define tmpsignintensity    0xDA     //��ѯ�ڵ��ź�ǿ��


//���ڴ���״̬
extern Uint16 SerDealSta;

//65�ֽ� DEV_INFO �ṹ��Ϣ
typedef struct DEV_INFO_TYPE{
	Uint16 DevName[8];				//�豸����16
	Uint16 DevPwd[8];				//�豸����16
	Uint16 DevMode_Chan;			//�豸����+ͨ����1+1
	Uint16 PanID;					//����ID2
	Uint16 MyAddr;					//���������ַ2
	Uint16 MyIEEE[4];				//���������ַ��MAC��8
	Uint16 DstAddr;					//Ŀ�������ַ2
	Uint16 DstIEEE[4];				//Ŀ�������ַ����8
	Uint16 Reserve_PowerLevel;		//����+���书��1+1
	Uint16 RetryNum_TranTimeout;	//�����������Դ���+������������ʱ������/10ms��1+1
	Uint16 Serial_Rate_Serial_DataB;//������+����λ1+1
	Uint16 Serial_StopB_Serial_ParityB;
									//ֹͣλ+У��λ1+1
	Uint16 SendMode;			    //����ģʽ1

}DEV_INFO;

extern DEV_INFO zm5168_p2;



u8 SetLocalConf();
u8 SetChanelNum(u8 chanelnum);
void LocalConfDeal();
u8 ReadLocalConf();


void ZM5168_INit(void);
void SetupSCI(Uint32 buad);

#endif /* ZM5168_H_ */
