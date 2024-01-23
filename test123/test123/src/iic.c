#include "sfr_r82b.h"
#include "main.h"
#include "allvars.h"
#include "general.h"
#include "iic.h"
#include "init.h"
#include "hvtj.h"
#include "tft.h"
#include "timer.h"
#include "uart.h"
#include "tanktest.h"
#include "cctalk.h"
#include "transactions.h"
#include "adc.h"

signed char Write2i2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs)
{
	signed char return_value;
	signed char result;
	unsigned int usi;
	unsigned char temp_add;
	result = 0;//ChkIICBus();
	if (result == 0)
	{
		for (usi=0;usi<noofchrs;usi++)
		{
			StartBit();
			//device address should be 1-3 bits lsb
			temp_add = (DeviceAddress & 0xfe);
			result = Send_IIC_Byte(temp_add);
			if (result == 0)
			{
				if (AddressType == 2)
				{
					temp_add = (DataAddress >> 8);
					result = Send_IIC_Byte(temp_add);
					if (result == 0)
					{
						temp_add = (DataAddress & 0xff);
						result = Send_IIC_Byte(temp_add);
						if (result != 0)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					temp_add = (DataAddress & 0xff);
					result = Send_IIC_Byte(temp_add);
					if (result != 0)
					{
						break;
					}
				}
				result = Send_IIC_Byte(*DataPointer);
				if (result != 0)
				{
					break;
				}
				*DataPointer++;
				DataAddress++;
				StopBit();
				result = Poll_For_WriteOver(DeviceAddress);
				if (result != 0)
				{
					result = 2;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		//bus error
		result = 0;
		return (-1);
	}
	if (result == 1)
	{
		StopBit();			//Give stop bit
		return_value = -2;
	}
	else if (result == 2)
	{
		return_value = -2;
	}
	else
	{
		return_value = 0;
	}
	return (return_value);
}

signed char ReadFromi2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs)
{
	signed char return_value;
	signed char result;
	unsigned int usi;
	unsigned char temp_add;
	result = 0;//ChkIICBus();
	if (result == 0)
	{
		for (usi=0;usi<noofchrs;usi++)
		{
			StartBit();
			//device address should be 1-3 bits lsb
			temp_add = (DeviceAddress & 0xfe);
			result = Send_IIC_Byte(temp_add);
			if (result == 0)
			{
				if (AddressType == 2)
				{
					temp_add = (DataAddress >> 8);
					result = Send_IIC_Byte(temp_add);
					if (result == 0)
					{
						temp_add = (DataAddress & 0xff);
						result = Send_IIC_Byte(temp_add);
						if (result != 0)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					temp_add = (DataAddress & 0xff);
					result = Send_IIC_Byte(temp_add);
					if (result != 0)
					{
						break;
					}
				}
				StartBit();
				temp_add = (DeviceAddress | 0x01);
				result = Send_IIC_Byte(temp_add);
				if (result != 0)
				{
					break;
				}
				*DataPointer = Rec_IIC_Byte();
				SlaveNoAck();
				*DataPointer++;
				DataAddress++;
				StopBit();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		//bus error
		result = 0;
		return_value = -1;
	}
	if (result != 0)
	{
		StopBit();
		return_value = -2;
	}
	else
	{
		return_value = 0;
	}
	return (return_value);
}

void StartBit(void)
{
	Set_SDA_As_Output();
	SDA = HIGH;							// Set data line high
	asm("nop");
	SCL = HIGH;							// Set clock line high
	asm("nop");
	SDA = LOW;							// Set data line low (START SIGNAL)
	asm("nop");
	SCL = LOW;							// Set clock line low
	asm("nop");
}

void StopBit(void)
{
	Set_SDA_As_Output();
	SCL = LOW;							// Set clock line low
	asm("nop");
	SDA = LOW;							// Set data line low
	asm("nop");
	SCL = HIGH;							// Set clock line high
	asm("nop");
	SDA = HIGH;							// Set data line high (STOP SIGNAL)
	asm("nop");
	//IICClockDelay();
}

void SlaveNoAck(void)
{
	Set_SDA_As_Output();
	SDA	= 1;
	asm("nop");
	//IICClockDelay();
	SCL	= 1;
	asm("nop");
	//IICClockDelay();
	SCL = 0;
	asm("nop");
	//IICClockDelay();
}

void IICClockDelay(void)
{
	char a;
	//for(a=0;a<10;a++);
	for(a=0;a<3;a++);
}

void Set_SDA_As_Output(void)
{
	//prc2 = 1;		//port direction write enable off									//
	prcr = 0x04;
	SDA_Direction = 1;		//Set as output
	//prc2 = 0;		//port direction write enable off									//
	prcr = 0x00;
}

void Set_SDA_As_Input(void)
{
	//prc2 = 1;		//port direction write enable off									//
	prcr = 0x04;
	SDA_Direction = 0;		//Set as input
	//prc2 = 0;		//port direction write enable off									//
	prcr = 0x00;
}

void Set_SCL_As_Output(void)
{
	//prc2 = 1;		//port direction write enable off									//
	prcr = 0x04;
	SCL_Direction = 1;		//Set as output
	//prc2 = 0;		//port direction write enable off									//
	prcr = 0x00;
}

void Set_SCL_As_Input(void)
{
	//prc2 = 1;		//port direction write enable off									//
	prcr = 0x04;
	SCL_Direction = 0;		//Set as input
	//prc2 = 0;		//port direction write enable off									//
	prcr = 0x00;
}

signed char ChkIICBus()
{
	unsigned char a;
	signed char return_value;
	Set_SDA_As_Input();
	Set_SCL_As_Input();
	a = SDA;
	if (a == 1)
	{
		a = SCL;
		if (a == 1)
		{
			return_value = 0;
		}
		else
		{
			return_value = -1;
		}
	}
	else
	{
		return_value = -1;
	}
	Set_SDA_As_Output();
	Set_SCL_As_Output();
	return (return_value);
}

void Chk_IIC_BusFree()
{
	unsigned char usc;
	unsigned char a;
	if (ChkIICBus() == -1)
	{
		Set_SDA_As_Output();
		Set_SCL_As_Output();
		SDA = 1;
		SCL = 1;
		//asm("nop");
		IICClockDelay();
		for (usc=0;usc<9;usc++)
		{
			SCL = 0;
			//asm("nop");
			IICClockDelay();
			SCL = 1;
			//asm("nop");
			IICClockDelay();
			Set_SDA_As_Input();
			a = SDA;
			Set_SDA_As_Output();
			if (a == 1)
			{
				SDA = 0;
				//asm("nop");
				IICClockDelay();
				SDA = 1;
				//asm("nop");
				IICClockDelay();
				break;
			}
		}
		SCL = 1;
		SDA = 1;
	}
}

unsigned char Send_IIC_Byte(unsigned char iic_data)
{
	unsigned char usc;
	Set_SDA_As_Output();
	for(usc=0;usc<8;usc++)  	// Send 8 bits to the I2C Bus
	{
		//asm("nop");
		SDA = ((iic_data & 0x80) ? 1 : 0);
		//asm("nop");
      	iic_data  <<= 1;            		// Shift the byte by one bit
		SCL = HIGH;   		        		// Clock the data into the I2C Bus
		asm("nop");
		SCL = LOW;					
		//asm("nop");
	}
	Set_SDA_As_Input();
	usc = SDA;							// Put data pin into read mode
	Set_SDA_As_Output();
	SCL = HIGH; 	        			// Clock the ACK from the I2C Bus
	asm("nop");
	SCL = LOW;					
	return(usc);	
}

unsigned char Rec_IIC_Byte()
{
	unsigned char usc,input_data;
	Set_SDA_As_Output();
	SDA = HIGH;
	//asm("nop");
	Set_SDA_As_Input();
   	usc = SDA;					// Put data pin into read mode
	input_data = 0x00;
	for(usc=0;usc<8;usc++)  	// Send 8 bits to the I2C Bus
	{
		input_data <<= 1;		// Shift the byte by one bit
		SCL = HIGH;           	// Clock the data into the I2C Bus
		//asm("nop");
      	input_data |= SDA; 		// Input the data from the I2C Bus
		SCL = LOW;					
		//asm("nop");
	}
   return(input_data);
}

unsigned char Poll_For_WriteOver(unsigned char DeviceAddress)
{
	unsigned int usi;
	unsigned char temp_add;
	unsigned char result;
	//for (usi=0;usi<5;usi++)
	//{
	//	IICClockDelay();
	//}
	Set_SDA_As_Output();
	for (usi=0;usi<500;usi++)
	{
		StartBit();
		temp_add = (DeviceAddress & 0xfe);
		result = Send_IIC_Byte(temp_add);
		if (result == 0)
		{
			break;
		}
	}
	StopBit();
	return (result);
}

/*signed char Write2i2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs)
{
	signed char return_value;
	signed char result;
	unsigned int usi;
	unsigned char temp_add;
	result = ChkIICBus();
	if (result == 0)
	{
		result = -1;
		for (usi=0;usi<noofchrs;usi++)
		{
			StartBit();
			//device address should be 1-3 bits lsb
			temp_add = (DeviceAddress & 0xfe);
			result = Send_IIC_Byte(temp_add);
			if (result == 0)
			{
				if (AddressType == 2)
				{
					temp_add = (DataAddress >> 8);
					result = Send_IIC_Byte(temp_add);
					if (result == 0)
					{
						temp_add = (DataAddress & 0xff);
						result = Send_IIC_Byte(temp_add);
						if (result != 0)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					temp_add = (DataAddress & 0xff);
					result = Send_IIC_Byte(temp_add);
					if (result != 0)
					{
						break;
					}
				}
				result = Send_IIC_Byte(*DataPointer);
				if (result != 0)
				{
					break;
				}
				*DataPointer++;
				DataAddress++;
				StopBit();
				result = Poll_For_WriteOver(DeviceAddress);
				if (result != 0)
				{
					result = 2;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		//bus error
		result = 0;
		return_value = -1;
		return (-1);
	}
	if (result == 1)
	{
		StopBit();			//Give stop bit
		return_value = -2;
	}
	else if (result == 2)
	{
		return_value = -2;
	}
	else
	{
		return_value = 0;
	}
	return (return_value);
}

signed char ReadFromi2c(unsigned char DeviceAddress,unsigned char AddressType, unsigned int DataAddress, unsigned char *DataPointer,unsigned int noofchrs)
{
	signed char return_value;
	signed char result;
	unsigned int usi;
	unsigned char temp_add;
	result = ChkIICBus();
	if (result == 0)
	{
		for (usi=0;usi<noofchrs;usi++)
		{
			StartBit();
			//device address should be 1-3 bits lsb
			temp_add = (DeviceAddress & 0xfe);
			result = Send_IIC_Byte(temp_add);
			if (result == 0)
			{
				if (AddressType == 2)
				{
					temp_add = (DataAddress >> 8);
					result = Send_IIC_Byte(temp_add);
					if (result == 0)
					{
						temp_add = (DataAddress & 0xff);
						result = Send_IIC_Byte(temp_add);
						if (result != 0)
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				else
				{
					temp_add = (DataAddress & 0xff);
					result = Send_IIC_Byte(temp_add);
					if (result != 0)
					{
						break;
					}
				}
				StartBit();
				temp_add = (DeviceAddress | 0x01);
				result = Send_IIC_Byte(temp_add);
				if (result != 0)
				{
					break;
				}
				*DataPointer = Rec_IIC_Byte();
				SlaveNoAck();
				*DataPointer++;
				DataAddress++;
				StopBit();
			}
			else
			{
				break;
			}
		}
	}
	else
	{
		//bus error
		result = 0;
		return_value = -1;
	}
	if (result != 0)
	{
		StopBit();
		return_value = -2;
	}
	else
	{
		return_value = 0;
	}
	return (return_value);
}

void StartBit(void)
{
	Set_SDA_As_Output();
	SDA = HIGH;							// Set data line high
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	asm("nop");
	SCL = HIGH;							// Set clock line high
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	asm("nop");
	SDA = LOW;							// Set data line low (START SIGNAL)
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	asm("nop");
	SCL = LOW;							// Set clock line low
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	//asm("nop");
}

void StopBit(void)
{
	Set_SDA_As_Output();
	SCL = LOW;							// Set clock line low
	IICClockDelay();
	asm("nop");
	//asm("nop");
	//asm("nop");
	SDA = LOW;							// Set data line low
	IICClockDelay();
	asm("nop");
	//asm("nop");
	//asm("nop");
	SCL = HIGH;							// Set clock line high
	IICClockDelay();
	asm("nop");
	//asm("nop");
	//asm("nop");
	SDA = HIGH;							// Set data line high (STOP SIGNAL)
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	//asm("nop");
	IICClockDelay();
}

void SlaveNoAck(void)
{
	Set_SDA_As_Output();
	SDA	= 1;
	IICClockDelay();
	SCL	= 1;
	IICClockDelay();
	SCL = 0;
	IICClockDelay();
}

void IICClockDelay(void)
{
	char a;
	for(a=0;a<25;a++);
}

void Set_SDA_As_Output(void)
{
	prc2 = 1;		//port direction write enable off									//
	SDA_Direction = 1;		//Set as output
	prc2 = 0;		//port direction write enable off									//
}

void Set_SDA_As_Input(void)
{
	prc2 = 1;		//port direction write enable off									//
	SDA_Direction = 0;		//Set as input
	prc2 = 0;		//port direction write enable off									//
}

void Set_SCL_As_Output(void)
{
	prc2 = 1;		//port direction write enable off									//
	SCL_Direction = 1;		//Set as output
	prc2 = 0;		//port direction write enable off									//
}

void Set_SCL_As_Input(void)
{
	prc2 = 1;		//port direction write enable off									//
	SCL_Direction = 0;		//Set as input
	prc2 = 0;		//port direction write enable off									//
}

signed char ChkIICBus()
{
	unsigned char a;
	signed char return_value;
	Set_SDA_As_Input();
	Set_SCL_As_Input();
	a = SDA;
	if (a == 1)
	{
		a = SCL;
		if (a == 1)
		{
			return_value = 0;
		}
		else
		{
			return_value = -1;
		}
	}
	else
	{
		return_value = -1;
	}
	Set_SDA_As_Output();
	Set_SCL_As_Output();
	return (return_value);
}

void Chk_IIC_BusFree()
{
	unsigned char usc;
	unsigned char a;
	if (ChkIICBus() == -1)
	{
		Set_SDA_As_Output();
		Set_SCL_As_Output();
		SDA = 1;
		SCL = 1;
		IICClockDelay();
		for (usc=0;usc<9;usc++)
		{
			SCL = 0;
			IICClockDelay();
			SCL = 1;
			IICClockDelay();
			Set_SDA_As_Input();
			a = SDA;
			Set_SDA_As_Output();
			if (a == 1)
			{
				SDA = 0;
				IICClockDelay();
				SDA = 1;
				IICClockDelay();
				break;
			}
		}
		SCL = 1;
		SDA = 1;
	}
}

unsigned char Send_IIC_Byte(unsigned char iic_data)
{
	unsigned char usc;
	Set_SDA_As_Output();
	IICClockDelay();
	//asm("nop");
	for(usc=0;usc<8;usc++)  	// Send 8 bits to the I2C Bus
	{
        //asm("nop");                         			// Output the data bit to the I2C Bus
		IICClockDelay();
		asm("nop");
		SDA = ((iic_data & 0x80) ? 1 : 0);
		IICClockDelay();
		//asm("nop");
		//asm("nop");
		asm("nop");
      	iic_data  <<= 1;            		// Shift the byte by one bit
		SCL = HIGH;   		        		// Clock the data into the I2C Bus
		IICClockDelay();
		//asm("nop");
		//asm("nop");
		asm("nop");
		SCL = LOW;					
		IICClockDelay();
		//asm("nop");
		//asm("nop");
		//asm("nop");
	}
	Set_SDA_As_Input();
	usc = SDA;							// Put data pin into read mode
	Set_SDA_As_Output();
	SCL = HIGH; 	        			// Clock the ACK from the I2C Bus
	asm("nop");
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	SCL = LOW;					
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	return(usc);	
}

unsigned char Rec_IIC_Byte()
{
	unsigned char usc,input_data;
	Set_SDA_As_Output();
	SDA = HIGH;
	IICClockDelay();
	//asm("nop");
	//asm("nop");
	Set_SDA_As_Input();
   	usc = SDA;					// Put data pin into read mode
	input_data = 0x00;
	for(usc=0;usc<8;usc++)  	// Send 8 bits to the I2C Bus
	{
		input_data <<= 1;		// Shift the byte by one bit
		SCL = HIGH;           	// Clock the data into the I2C Bus
		IICClockDelay();
		asm("nop");
		//asm("nop");
		//asm("nop");
      	input_data |= SDA; 		// Input the data from the I2C Bus
		SCL = LOW;					
		IICClockDelay();
		asm("nop");
		//asm("nop");
		//asm("nop");
	}
   return(input_data);
}

unsigned char Poll_For_WriteOver(unsigned char DeviceAddress)
{
	unsigned int usi;
	unsigned char temp_add;
	unsigned char result;
	for (usi=0;usi<2000;usi++)
	{
		IICClockDelay();
	}
	Set_SDA_As_Output();
	for (usi=0;usi<1500;usi++)
	{
		StartBit();
		temp_add = (DeviceAddress & 0xfe);
		result = Send_IIC_Byte(temp_add);
		if (result == 0)
		{
			break;
		}
	}
	StopBit();
	return (result);
}*/

