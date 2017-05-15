/******************************
 * zm5168.h
 *临时参数配置协议   命令帧 格式          |  应答返回帧
 * |3字节（协议标志)|1字节  |N字节 | 3字节（协议标志)|1字节  |1字节
 * |DE DF EF      |功能码|参数   | DE DF EF      |功能码|返回值
 ****************************/
//ssssssssssssss
/******************************
 * 永久参数配置协议   命令帧 格式
 * |3字节（协议标志)|1字节  |N字节       |1字节      |
 * |AB BC CD      |功能码|命令实体  |字节校验 |
 *
 *  Created on: 2017-3-24
 *      Author: Shaw
 *******************************/

#ifndef ZM5168_H_
#define ZM5168_H_
//校验 永久配置协议标志-----------------------------------------------
#define Agreement_1		0xAB
#define Agreement_2		0xBC
#define Agreement_3		0xCD        //AB+BC+CD=0x234
//#define checksum(x)		x&0x0ff   	//x是取低一字节除校验位之外的和
//应答帧响应状态
#define OK				0x00
#define LENGTH_FAUSE	0x01
#define ADDRESS_FAUSE	0x02
#define CHECK_FAUSE		0x03
#define WRITE_FAUSE		0x04
#define OTHER_FAUSE		0x05
//功能码
#define RLocalConf		0xD1		//读取本地配置
#define SChannelNum		0xD2		//设置通道号 掉电不保存
#define Search			0xD4		//搜索
#define RRemoteConf		0xD5		//获取远程配置信息
#define WConf			0xD6		//修改配置（设置成功需复位）
#define Reset			0xD9		//复位模块
#define Restore			0xDA		//恢复出厂设置（设置成功需复位）

//临时配置协议标志-----------------------------------------------------
#define tmpAgreement_1		0xDE
#define tmpAgreement_2		0xDF
#define tmpAgreement_3		0xEF        //AB+BC+CD=0x234
//功能码
#define tmpSChannelNum		0xD1     //修改通道号
#define tmpSDstAddress      0xD2     //修改目的网络地址
#define tmphibernate        0xD8     //进入休眠
#define tmpSMode			0xD9     //设置通讯模式
#define tmpsignintensity    0xDA     //查询节点信号强度


//串口处理状态
extern Uint16 SerDealSta;

//65字节 DEV_INFO 结构信息
typedef struct DEV_INFO_TYPE{
	Uint16 DevName[8];				//设备名称16
	Uint16 DevPwd[8];				//设备密码16
	Uint16 DevMode_Chan;			//设备类型+通道号1+1
	Uint16 PanID;					//网络ID2
	Uint16 MyAddr;					//本地网络地址2
	Uint16 MyIEEE[4];				//本地物理地址（MAC）8
	Uint16 DstAddr;					//目标网络地址2
	Uint16 DstIEEE[4];				//目标物理地址保留8
	Uint16 Reserve_PowerLevel;		//保留+发射功率1+1
	Uint16 RetryNum_TranTimeout;	//发送数据重试次数+发送数据重试时间间隔（/10ms）1+1
	Uint16 Serial_Rate_Serial_DataB;//波特率+数据位1+1
	Uint16 Serial_StopB_Serial_ParityB;
									//停止位+校验位1+1
	Uint16 SendMode;			    //发送模式1

}DEV_INFO;

extern DEV_INFO zm5168_p2;



u8 SetLocalConf();
u8 SetChanelNum(u8 chanelnum);
void LocalConfDeal();
u8 ReadLocalConf();


void ZM5168_INit(void);
void SetupSCI(Uint32 buad);

#endif /* ZM5168_H_ */
