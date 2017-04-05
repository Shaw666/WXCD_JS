/*
 * sim_spi.c
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "HK_all_include.h"
void SPIGpio_Init(void)
{
	EALLOW;
	//GPIO�Ĵ����ܱ���
	//SPI SCK SDI SDO CS --- GPIO21 GPIO20 GPIO23 GPIO24
	//SDI
	GpioCtrlRegs.GPAMUX2.bit.GPIO20  = 0;	//GPIO20��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO20   = 1;		//GPIO20����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO20   = 1;		//GPIO20����ߵ�ƽ
	//SCK
	GpioCtrlRegs.GPAMUX2.bit.GPIO21  = 0;	//GPIO21��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO21   = 1;		//GPIO21����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO21   = 1;		//GPIO21����ߵ�ƽ
	//SDO
	GpioCtrlRegs.GPAMUX2.bit.GPIO23  = 0;	//GPIO23��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO23   = 0;		//GPIO23����Ϊ����
	GpioCtrlRegs.GPAPUD.bit.GPIO23   = 1;   //����23
	//CS
	GpioCtrlRegs.GPAMUX2.bit.GPIO24  = 0;	//GPIO24��Ϊ��ͨIO
	GpioCtrlRegs.GPADIR.bit.GPIO24   = 1;		//GPIO24����Ϊ���
	GpioDataRegs.GPASET.bit.GPIO24   = 1;		//GPIO24����ߵ�ƽ
	EDIS;
}
void SPI_INit(void)
{
	SPIGpio_Init();//��ʼ��GPIO
	//��ʼ��FLASH
	//...................
}

//*************** Write enable ****************************  OK
void DataFlash_WriteEnable  (void)
{
    SPI_CS = 0;
    DELAY_US(2);
    DataFlash_Write_Byte(W25X_WriteEnable);
    DELAY_US(2);
    SPI_CS = 1;
}

//*************** Write disable ****************************    OK
void DataFlash_WriteDisable (void)
{
    SPI_CS = 0;
    DELAY_US(2);
    DataFlash_Write_Byte(W25X_WriteDisable);
    DELAY_US(2);
    SPI_CS = 1;
}

//***************��BUSY********************************
unsigned char DataFlash_SPI_BUSY_OrNot (void) //�ڶ���д֮ǰ�����ж�FLASH�Ƿ�BUSY
{                        //BUSY��ԭ���ǲ���������������д
    unsigned char k;          //���û�����Ϸ�ʽ�������ж�����д��
    SPI_CS = 0;
    DELAY_US(2);
    DataFlash_Write_Byte(W25X_ReadStatusReg1);
    k = DataFlash_Write_Byte(Dummy_Byte1);
    DELAY_US(2);
    SPI_CS = 1;
    if(k&0x01)return (W25X_BUSY);
    else return (W25X_NotBUSY);
}

void SPI_Flash_Read(uint32_t ReadAddr, uint32_t numBytes,void *data)
{
    uint8_t *rxBuffer = (uint8_t *) data;

    /* Enable chip select */
    SPI_CS = 0;
    DELAY_US(2);

    DataFlash_Write_Byte(W25X_ReadData);
    DataFlash_Write_Byte(ReadAddr >> 16);
    DataFlash_Write_Byte(ReadAddr >> 8);
    DataFlash_Write_Byte(ReadAddr);
    while (numBytes--)
    {
        *rxBuffer = DataFlash_Write_Byte(Dummy_Byte1); //Read one byte
        rxBuffer++;
    }

    /* Disable chip select */
    DELAY_US(2);
    SPI_CS = 1;
}
//0xEF13,��ʾоƬ�ͺ�Ϊw25q80
//0xEF14,��ʾоƬ�ͺ�Ϊw25q16
//0xEF15,��ʾоƬ�ͺ�Ϊw25q32
//0xEF16,��ʾоƬ�ͺ�Ϊw25q64
//0xEF17,��ʾоƬ�ͺ�Ϊw25q128
/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH identification.
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/

unsigned long SPI_Flash_ReadID(void)
{
    unsigned int Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

   /* Enable chip select */
    SPI_CS = 0;
    DELAY_US(2);

    /* Send "RDID " instruction */
    DataFlash_Write_Byte(0x9F);
    /* Read a byte from the FLASH */
    Temp0 = DataFlash_Write_Byte(Dummy_Byte1);
    /* Read a byte from the FLASH */
    Temp1 = DataFlash_Write_Byte(Dummy_Byte1);
    /* Read a byte from the FLASH */
    Temp2 = DataFlash_Write_Byte(Dummy_Byte1);

    /* Disable chip select */
    DELAY_US(2);
    SPI_CS = 1;

    Temp = ((unsigned long)Temp0 << 16) | (Temp1 << 8) | Temp2;//����ڴ�Ϊ16λcpu��Ҫǿ��ת���������

    return Temp;
}


////ÿ��дһҳ�� ��ַ��8λӦΪ0
//void SPI_Flash_Page_Write(uint32_t pageNum, unsigned char* data)
//{
//    uint8_t *rxBuffer = data;
//    while(1)
//    {
//        if(SPI_BUSY_OrNot() == W25X_NotBUSY) break;
//    }
//
//    WriteEnable();   //д����
//
//    /* Enable chip select */
//    GPIO_PinOutClear(Flash_SPI_CS_PORT, Flash_SPI_CS_BIT);
//
//    SPI_FlashWrite(W25X_PageProgram);
//
//    unsigned int WriteAddr = pageNum<<8;
//
//    SPI_FlashWrite((WriteAddr >> 16));
//    SPI_FlashWrite((uint32_t)(WriteAddr >> 8));
//    SPI_FlashWrite((uint32_t) WriteAddr);
//
//    for(int i=0;i<SPI_FLASH_PAGE_SIZE;i++)
//    {
//        SPI_FlashWrite(*rxBuffer);
//        rxBuffer++;
//    }
//    /* Disable chip select */
//    GPIO_PinOutSet(Flash_SPI_CS_PORT, Flash_SPI_CS_BIT);
//
//    WriteDisable();//д��ֹ
//}
//
////**************Ƭ����****************** OK
//void SPI_EraseDevice(void)
//{
//    while(1)
//    {
//        if(SPI_BUSY_OrNot() == W25X_NotBUSY) break;
//    }
//    WriteEnable();   //д����
//    GPIO_PinOutClear(Flash_SPI_CS_PORT, Flash_SPI_CS_BIT);
//    SPI_FlashWrite(W25X_ChipErase);
//    GPIO_PinOutSet(Flash_SPI_CS_PORT, Flash_SPI_CS_BIT);
//}




unsigned char DataFlash_Write_Byte(unsigned char Data)
{
    unsigned char Out = 0;
    unsigned char i = 0;

    SPI_CLK = 0;
    for(i = 0; i < 8; i++)
    {
        if(Data & 0x80)SPI_MOSI = 1;
        else SPI_MOSI = 0;
        Data = Data << 1;
        DELAY_US(3);
        SPI_CLK = 1;
        if(SPI_MISO)Out |= 0x80 >> i;
        DELAY_US(3);
        SPI_CLK = 0;
    }
    SPI_CLK = 0;

    return Out;
}


