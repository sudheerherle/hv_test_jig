#ifndef IIC_ROUTINE_PINS
#include "sfr_r82b.h"
#include "main.h"
#define		SDA							p2_2
#define		SCL							p2_1
#define		SDA_Direction				pd2_2 
#define		SCL_Direction				pd2_1
#define		WRITE_ENABLE				prc2
#define		HIGH						0x01
#define		LOW							0x00

//#define		SDA				p8_1
//#define		SCL				p8_0
//#define		SDA_Direction	pd8_1 
//#define		SCL_Direction	pd8_0 
//#define		WRITE_ENABLE	prc2
//#define		HIGH			0x01
//#define		LOW				0x00
#endif

signed char Write2i2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs);
signed char ReadFromi2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs);
void StartBit(void);
void StopBit(void);
void SlaveNoAck(void);
void IICClockDelay(void);
void Set_SDA_As_Output(void);
void Set_SDA_As_Input(void);
signed char ChkIICBus();
void Chk_IIC_BusFree();

unsigned char Send_IIC_Byte(unsigned char iic_data);
unsigned char Rec_IIC_Byte();
unsigned char Poll_For_WriteOver(unsigned char DeviceAddress);
