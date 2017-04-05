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
	//GPIO寄存器受保护
	//SPI SCK SDI SDO CS --- GPIO21 GPIO20 GPIO23 GPIO24
	//SDI
	GpioCtrlRegs.GPAMUX2.bit.GPIO20  = 0;	//GPIO20作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO20   = 1;		//GPIO20方向为输出
	GpioDataRegs.GPASET.bit.GPIO20   = 1;		//GPIO20输出高电平
	//SCK
	GpioCtrlRegs.GPAMUX2.bit.GPIO21  = 0;	//GPIO21作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO21   = 1;		//GPIO21方向为输出
	GpioDataRegs.GPASET.bit.GPIO21   = 1;		//GPIO21输出高电平
	//SDO
	GpioCtrlRegs.GPAMUX2.bit.GPIO23  = 0;	//GPIO23作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO23   = 0;		//GPIO23方向为输入
	GpioCtrlRegs.GPAPUD.bit.GPIO23   = 1;   //上拉23
	//CS
	GpioCtrlRegs.GPAMUX2.bit.GPIO24  = 0;	//GPIO24作为普通IO
	GpioCtrlRegs.GPADIR.bit.GPIO24   = 1;		//GPIO24方向为输出
	GpioDataRegs.GPASET.bit.GPIO24   = 1;		//GPIO24输出高电平
	EDIS;
}
void SPI_INit(void)
{
	SPIGpio_Init();//初始化GPIO
	//初始化FLASH
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

//***************判BUSY********************************
unsigned char DataFlash_SPI_BUSY_OrNot (void) //在读和写之前得先判断FLASH是否BUSY
{                        //BUSY的原因是擦除，或是连续读写
    unsigned char k;          //如果没有以上方式，不必判定可以写读
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
//0xEF13,表示芯片型号为w25q80
//0xEF14,表示芯片型号为w25q16
//0xEF15,表示芯片型号为w25q32
//0xEF16,表示芯片型号为w25q64
//0xEF17,表示芯片型号为w25q128
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

    Temp = ((unsigned long)Temp0 << 16) | (Temp1 << 8) | Temp2;//针对内存为16位cpu需要强制转换避免出错

    return Temp;
}


////每次写一页面 地址低8位应为0
//void SPI_Flash_Page_Write(uint32_t pageNum, unsigned char* data)
//{
//    uint8_t *rxBuffer = data;
//    while(1)
//    {
//        if(SPI_BUSY_OrNot() == W25X_NotBUSY) break;
//    }
//
//    WriteEnable();   //写允许
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
//    WriteDisable();//写禁止
//}
//
////**************片擦除****************** OK
//void SPI_EraseDevice(void)
//{
//    while(1)
//    {
//        if(SPI_BUSY_OrNot() == W25X_NotBUSY) break;
//    }
//    WriteEnable();   //写允许
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


