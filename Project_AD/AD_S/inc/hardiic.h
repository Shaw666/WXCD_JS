#ifndef HARDIIC_H_
#define HARDIIC_H_

#define IIC_NODE_NUM 2 //IIC�ڵ�����2�����ֱ��� TM1650/AT24C02

#define AT24CO2_TIMER_OUT  3 //msΪ��λ
#define IIC_RST_BIT        0X0020      //IIC ��λ

// Status Flags
#define I2C_AL_BIT          0x0001
#define I2C_NACK_BIT        0x0002
#define I2C_ARDY_BIT        0x0004
#define I2C_RRDY_BIT        0x0008
#define I2C_SCD_BIT         0x0020

// I2C State defines
#define IIC_IDLE               0
#define IIC_WRITE              1
#define IIC_READ_STEP1         2
#define IIC_READ_STEP2         3
#define IIC_READ_STEP3         4
#define IIC_BUS_BUSY           5
#define IIC_BUS_ERROR          6

#define RTC_SUCCESS 0

//��TM1650�йص������ַ����Ҫ���ƶ�1λ���б�׼��IIC��������׼IIC �� LSBΪ��дλ����7λΪ�����ַλ��
#define CMD_SEG  (0X48)>>1	//��ʾģʽ ADDR
#define CMD_KEY  (0X49)>>1	//����ģʽ ADDR

//������ұߵ�һ������ΪDIG1
//#define DIG1  (0X68)>>1	//DIG1 ADDR
//#define DIG2  (0X6A)>>1 //DIG2 ADDR
//#define DIG3  (0X6C)>>1 //DIG3 ADDR
//#define DIG4  (0X6E)>>1 //DIG4 ADDR

//�������ߵ�һ������ΪDIG1
#define DIG1  (0X6E)>>1 //DIG1 ADDR
#define DIG2  (0X6C)>>1 //DIG2 ADDR
#define DIG3  (0X6A)>>1 //DIG3 ADDR
#define DIG4  (0X68)>>1 //DIG4 ADDR

//---------------------------------------------------
//SDA����GPIO32
#define SDAGPIO GPIO32
#define SDAMUX GpioCtrlRegs.GPBMUX1.bit.SDAGPIO
#define SDADIR GpioCtrlRegs.GPBDIR.bit.SDAGPIO
#define SDA_H()  GpioDataRegs.GPBSET.bit.SDAGPIO=1
#define SDA_L() GpioDataRegs.GPBCLEAR.bit.SDAGPIO =1

//CLK����GPIO33
#define CLKGPIO GPIO33
#define CLKMUX GpioCtrlRegs.GPBMUX1.bit.CLKGPIO
#define CLKDIR GpioCtrlRegs.GPBDIR.bit.CLKGPIO
#define CLK_H()  GpioDataRegs.GPBSET.bit.CLKGPIO=1
#define CLK_L() GpioDataRegs.GPBCLEAR.bit.CLKGPIO =1

// I2C Message Structure
typedef struct I2CSlaveMSG{
  Uint16 IntSRC[10];            //�ж�Դ            ������
  Uint16 IntSRCCnt;             //�ж�Դ��¼  ������
  Uint16 MasterStatus;			//������������IIC״̬
  Uint16 SlavePHYAddress;	    //IIC�����ַ��Ӳ����ַ��
  Uint16 LogicAddr;             //������EEPROM����Ҫ�ṩ�߼���ַ
  Uint16 Len;			        //�������ݵĳ��ȣ����������ַ��
  Uint16 MsgInBuffer[4]; //�������飬���4�����
  Uint16 MsgOutBuffer[4];//�������飬���4�����
  Uint16 IIC_TimerOUT;   //�������ĳ�ʱ��������1ms��ʱ���м���
}I2CSlaveMSG;




extern char *IIC_ISR_String[];
extern uint8_t SEG7Table[];

void InitI2C_Gpio(void);
void   I2CA_Init(void);
Uint16 I2CA_Tx_STOP(struct I2CSlaveMSG *msg);
Uint16 I2CA_Rxdata_STOP(struct I2CSlaveMSG *msg);

u8 MCP79411_INit(void);
#endif /* TM1650_IIC_H_ */
