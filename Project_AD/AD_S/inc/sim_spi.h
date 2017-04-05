/*
 * sim_spi.h
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 */

#ifndef SIM_SPI_H_
#define SIM_SPI_H_

#define W25X_BUSY       0
#define W25X_NotBUSY    1
#define Dummy_Byte1     0xA5


/*********************************************
 - W25X16的操作指令表，MCU通过向W25X16
  发送以下指令就可以对W25X16进行以下的操作
*********************************************/
#define W25X_WriteEnable            0x06    //Write Enable
#define W25X_WriteEnableVSR         0x50    //Write Enable for Volatile Status Register
#define W25X_WriteDisable           0x04    //Write Disable
#define W25X_ReadStatusReg1         0x05    //读状态寄存器1：S7~S0
#define W25X_ReadStatusReg2         0x35    //读状态寄存器2：S15~S8
#define W25X_WriteStatusReg         0x01    //写读状态寄存器：BYTE1:S7~S0  BYTE2：S15~S8
#define W25X_PageProgram            0x02    //单页编程：BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:D7~D0
#define W25X_SectorErase            0x20    //扇区擦除：4K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_BlockErase32K          0x52    //块擦除：32K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_BlockErase64K          0xD8    //块擦除：64K  BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0
#define W25X_ChipErase              0xC7    //芯片擦除
#define W25X_EraseSuspend           0x75    //暂停擦除
#define W25X_EraseResume            0x7A    //恢复擦除
#define W25X_PowerDown              0xB9    //掉电
#define W25X_ContinuousReadMode     0xFF    //连续读模式

#define W25X_ReadData               0x03    //读数据：BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:D7~D0
#define W25X_FastReadData           0x0B    //快速读取：BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:dummy  BYTE5:D7~D0
#define W25X_FastReadDual           0x3B    //快速双读取：BYTE1:A23~A16  BYTE2:A15~A8  BYTE3:A7~A0  BYTE4:dummy  BYTE5:D7~D0

#define W25X_ReleasePowerDown       0xAB
#define W25X_DeviceID               0xAB
#define W25X_ManufactDeviceID       0x90
#define W25X_JedecDeviceID          0x9F



#define SPI_CS     GpioDataRegs.GPASET.bit.GPIO24
#define SPI_CLK    GpioDataRegs.GPASET.bit.GPIO21
#define SPI_MOSI   GpioDataRegs.GPASET.bit.GPIO20
#define SPI_MISO   GpioDataRegs.GPADAT.bit.GPIO23

unsigned char DataFlash_Write_Byte(unsigned char Data);

void SPIGpio_Init(void);
void SPI_INit(void);

#endif /* SIM_SPI_H_ */
