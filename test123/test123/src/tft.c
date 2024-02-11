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

void DelayMs(unsigned long delay)
{
	_delay_us(delay)
}

// Initialise port settings etc
void TFT_Init_IO(void)
{
	//p6 - d0-d7
	//p3 - d10-d17
	//p5 - cs,rs,wr,rd,reset
	prc2 = 1;		//port direction write enable off
	pd3 = 0xFF;
	pd5 = 0xFF;
	pd6 = 0xFF;
	prc2 = 0;		//port direction write enable off	
	//RD = 1;
	//WR = 1;   
	//RS = 1;   
	//RESET = 1;
	//CS = 0;
	TFT_CONTROL_PORT = 0x1e;
	TFT_Set_Port_Write();
}

// set direction to read 
static void TFT_Set_Port_Read(void)
{
	prc2 = 1;					//port direction write enable off
	TFT_PORTLO_DIR = 0x00;		//Set as input
	TFT_PORTHI_DIR = 0x00;		//Set as input
	prc2 = 0;					//port direction write enable off
}

// set direction to write 
static void TFT_Set_Port_Write(void)
{
	prc2 = 1;					//port direction write enable off
	TFT_PORTLO_DIR = 0xFF;		//Set as output
	TFT_PORTHI_DIR = 0xFF;		//Set as output
	prc2 = 0;					//port direction write enable off
}

/*********************************************************************
* Function:  void  Send_TFT_Data(BYTE index, WORD value)
* PreCondition: none
* Input: index - register number
*        value - value to be set
* Output: none
* Side Effects: none
* Overview: sets graphics controller register
* Note: none
********************************************************************/
void Send_TFT_RData(unsigned int index, unsigned int data)
{
	//Send_TFT_Command(index);
	//Send_TFT_Data(data);
	TFT_DATA_PORTHI = index >> 8;
	TFT_DATA_PORTLO = index;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = data >> 8;
	TFT_DATA_PORTLO = data;
	WR = 0;
	WR = 1;
}

unsigned int Read_TFT_RData(unsigned int index)
{
	unsigned int data;
	Send_TFT_Command(index);
	data =  Read_TFT_Data();
	return (data);
}

void Send_TFT_Command(unsigned int cmd)
{
	TFT_DATA_PORTHI = cmd >> 8;
	TFT_DATA_PORTLO = cmd;
	RS = 0;
	WR = 0;
	WR = 1;
	RS = 1;
}

void Send_TFT_Data(unsigned int data)
{
	TFT_DATA_PORTHI = data >> 8;
	TFT_DATA_PORTLO = data;
	WR = 0;
	WR = 1;
}

unsigned int Read_TFT_Data1()
{
	unsigned int data;
	TFT_Set_Port_Read();
	RD = 1;
	RD = 0;
	RD = 1;
	data = TFT_DATA_PORTHI;
	data = data << 8;
	data |= TFT_DATA_PORTLO;	
	TFT_Set_Port_Write();
	return (data);
}

unsigned int Read_TFT_Data()
{
	unsigned int data;
	TFT_Set_Port_Read();
	RD = 0;
	data = TFT_DATA_PORTHI;
	data = data << 8;
	data |= TFT_DATA_PORTLO;
	RD = 1;
	TFT_Set_Port_Write();
	return (data);
}

unsigned int Read_TFT_GData(unsigned int index)
{
	unsigned int data;
	Send_TFT_Command(index);
	data =  Read_TFT_Data();	//dummy read
	data =  Read_TFT_Data();
	return (data);
}
void TFT_reset()
{
	RESET = 1;
	DelayMs(50);
    RESET = 0;
    DelayMs(50);
    RESET = 1;
    DelayMs(50);
}

void TFT_9341New_Init(void)
{
	TFT_reset();
	
    Send_TFT_Command(ILI9341_RESET);
    DelayMs(200);
    Read_TFT_RData(ILI9341_READ_DISPLAY_IDENTIFICATION_INFORMATION);
    Send_TFT_Command(ILI9341_DISPLAY_OFF);

    Send_TFT_Command(ILI9341_POWERA);
    Send_TFT_Data(0x39);
    Send_TFT_Data(0x2C);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x34);
    Send_TFT_Data(0x02);
DelayMs(50);
    Send_TFT_Command(ILI9341_POWERB);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x81);
    Send_TFT_Data(0x30);
DelayMs(50);
    Send_TFT_Command(ILI9341_DTCA);
    Send_TFT_Data(0x84);
    Send_TFT_Data(0x11);
    Send_TFT_Data(0x7A);
DelayMs(50);
    Send_TFT_Command(ILI9341_DTCB);
    Send_TFT_Data(0x66);
    Send_TFT_Data(0x00);
DelayMs(50);
    Send_TFT_Command(ILI9341_POWER_SEQ);
    Send_TFT_Data(0x55);
    Send_TFT_Data(0x01);
    Send_TFT_Data(0x23);
    Send_TFT_Data(0x01);
DelayMs(50);
    Send_TFT_Command(ILI9341_PRC);
    Send_TFT_Data(0x10);
DelayMs(50);
    Send_TFT_Command(ILI9341_POWER1);
    Send_TFT_Data(0x21);
DelayMs(50);
    Send_TFT_Command(ILI9341_POWER2);
    Send_TFT_Data(0x10);
DelayMs(50);
    Send_TFT_Command(ILI9341_VCOM1);
    Send_TFT_Data(0x31);
    Send_TFT_Data(0x3C);
DelayMs(50);
    Send_TFT_Command(ILI9341_VCOM2);
    Send_TFT_Data(0xc0);
DelayMs(50);
    Send_TFT_Command(ILI9341_MAC);
    Send_TFT_Data(0x00);
DelayMs(50);
    Send_TFT_Command(ILI9341_PIXEL_FORMAT);
    Send_TFT_Data(0x55);
DelayMs(50);
    Send_TFT_Command(ILI9341_FRC);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x1b);
DelayMs(50);
    Send_TFT_Command(ILI9341_DFC);
    Send_TFT_Data(0x0a);
    Send_TFT_Data(0x82);
    Send_TFT_Data(0x27);
DelayMs(50);
    Send_TFT_Command(ILI9341_3GAMMA_EN);
    Send_TFT_Data(0x00);
DelayMs(50);
    Send_TFT_Command(ILI9341_COLUMN_ADDR);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0xEF);
DelayMs(50);
    Send_TFT_Command(ILI9341_PAGE_ADDR);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x01);
    Send_TFT_Data(0x3F);
DelayMs(50);
    Send_TFT_Command(ILI9341_GAMMA);
    Send_TFT_Data(0x01);
DelayMs(50);

    Send_TFT_Command(ILI9341_PGAMMA);
    Send_TFT_Data(0x0F);
    Send_TFT_Data(0x31);
    Send_TFT_Data(0x2B);
    Send_TFT_Data(0x0C);
    Send_TFT_Data(0x0E);
    Send_TFT_Data(0x08);
    Send_TFT_Data(0x4E);
    Send_TFT_Data(0xF1);
    Send_TFT_Data(0x37);
    Send_TFT_Data(0x07);
    Send_TFT_Data(0x10);
    Send_TFT_Data(0x03);
    Send_TFT_Data(0x0E);
    Send_TFT_Data(0x09);
    Send_TFT_Data(0x00);
DelayMs(200);
    Send_TFT_Command(ILI9341_NGAMMA);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x0E);
    Send_TFT_Data(0x14);
    Send_TFT_Data(0x03);
    Send_TFT_Data(0x11);
    Send_TFT_Data(0x07);
    Send_TFT_Data(0x31);
    Send_TFT_Data(0xC1);
    Send_TFT_Data(0x48);
    Send_TFT_Data(0x08);
    Send_TFT_Data(0x0F);
    Send_TFT_Data(0x0C);
    Send_TFT_Data(0x31);
    Send_TFT_Data(0x36);
    Send_TFT_Data(0x0F);
DelayMs(200);
	
	
    Send_TFT_Command(ILI9341_SLEEP_OUT);
    DelayMs(50);
	Send_TFT_Command(0x21);
    Send_TFT_Command(ILI9341_DISPLAY_ON);
//    Send_TFT_Command(ILI9341_GRAM);	
	Clear_Device_ILI9341(RED);
	DelayMs(50);
	Clear_Device_ILI9341(WHITE);

}

void Read_Driver_Info(){
	unsigned int data1,data2,data3,data4;
	Send_TFT_Command(0x04);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	data3 = Read_TFT_Data1();
	data4 = Read_TFT_Data1();
	
	Send_TFT_Command(0x09);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	data3 = Read_TFT_Data1();
	data4 = Read_TFT_Data1();
	
}

/*
void Send_TFT_RData(unsigned int index, unsigned int data)
{
	TFT_RS_PIN = 0;
	TFT_DATA_PORT = (unsigned char)index;
	TFT_WR_PIN = 0;
	TFT_WR_PIN = 1;
	TFT_RS_PIN = 1;
	//TFT_DATA_PORT = (unsigned char)data >> 8;
	//TFT_WR_PIN = 0;
	//TFT_WR_PIN = 1;
	TFT_DATA_PORT = (unsigned char)data;
	TFT_WR_PIN = 0;
	TFT_WR_PIN = 1;
}


void Send_TFT_Command(unsigned int cmd)
{
	//TFT_DATA_PORTHI = cmd >> 8;
	//TFT_DATA_PORTLO = cmd;
	TFT_RS_PIN = 0;
	TFT_DATA_PORT = (unsigned char)cmd;
	TFT_WR_PIN = 0;
	TFT_WR_PIN = 1;
	TFT_RS_PIN = 1;
}

void Send_TFT_CData(unsigned char data)
{
	TFT_DATA_PORT = data;
	TFT_WR_PIN = 0;
	TFT_WR_PIN = 1;
}

void TFT_ST7789_Init(void)
{
	TFT_Init_IO();
    //DelayMs(100);
	//Send_TFT_Command(0x01);//reset
    //DelayMs(250);
	Send_TFT_Command(0x11);			//SLPOUT exit SLEEP mode
    //DelayMs(500);
	Delay_In_250uSec(2);//
	Send_TFT_RData(0x36,0xa8);		//0x80//MADCTL: memory data access control
	//Send_TFT_RData(0x3A,0x66);	//COLMOD: Interface Pixel format *** I use 262K-colors in 18bit/pixel format when using 8-bit interface to allow 3-bytes per pixel
	Send_TFT_RData(0x3A,0x55);		//COLMOD: Interface Pixel format  *** I use 65K-colors in 16bit/pixel (5-6-5) format when using 16-bit interface to allow 1-byte per pixel
	Send_TFT_RData(0xB2,0x0C);		//PORCTRL
	Send_TFT_CData(0x0C);
	Send_TFT_CData(0x00);
	Send_TFT_CData(0x33);
	Send_TFT_CData(0x33);			//PORCTRL: Porch setting
	Send_TFT_RData(0xB7,0x35);		//GCTRL: Gate Control
	Send_TFT_RData(0xBB,0x2B);		//VCOMS: VCOM setting
	Send_TFT_RData(0xC0,0x2C);		//LCMCTRL: LCM Control
	Send_TFT_RData(0xC2,0x01);		//VDVVRHEN:
	Send_TFT_CData(0xFF);			//VDVVRHEN: VDV and VRH Command Enable
	Send_TFT_RData(0xC3,0x11);		//VRHS: VRH Set
	Send_TFT_RData(0xC4,0x20);		//VDVS: VDV Set
	Send_TFT_RData(0xC6,0x0F);		//FRCTRL2: Frame Rate control in normal mode
	Send_TFT_RData(0xD0,0xA4);		//PWCTRL1:
	Send_TFT_CData(0xA1);			//PWCTRL1: Power Control 1
	Send_TFT_RData(0xE0,0xD0);		//PVGAMCTRL:
	Send_TFT_CData(0x00);
	Send_TFT_CData(0x05);
	Send_TFT_CData(0x0E);
	Send_TFT_CData(0x15);
	Send_TFT_CData(0x0D);
	Send_TFT_CData(0x37);
	Send_TFT_CData(0x43);
	Send_TFT_CData(0x47);
	Send_TFT_CData(0x09);
	Send_TFT_CData(0x15);
	Send_TFT_CData(0x12);
	Send_TFT_CData(0x16);
	Send_TFT_CData(0x19);			//PVGAMCTRL: Positive Voltage Gamma control	
	Send_TFT_RData(0xE1,0xD0);		//NVGAMCTRL: 
	Send_TFT_CData(0x00);
	Send_TFT_CData(0x05);
	Send_TFT_CData(0x0D);
	Send_TFT_CData(0x0C);
	Send_TFT_CData(0x06);
	Send_TFT_CData(0x2D);
	Send_TFT_CData(0x44);
	Send_TFT_CData(0x40);
	Send_TFT_CData(0x0E);
	Send_TFT_CData(0x1C);
	Send_TFT_CData(0x18);
	Send_TFT_CData(0x16);
	Send_TFT_CData(0x19);			//NVGAMCTRL: Negative Voltage Gamma control
    Send_TFT_Command(0x29);			//DISPON
	//Clear_Device();
	Clear_Full_Window();
}
*/
void Set_GRAM_Add(signed int x, signed int y)
{
/*#ifdef TFT_ORN_PORTRAIT
	Send_TFT_RData(GRAM_HOR_AD,x);
	Send_TFT_RData(GRAM_VER_AD,y);
#else
	Send_TFT_RData(GRAM_HOR_AD,y);
	Send_TFT_RData(GRAM_VER_AD,x);
#endif
	Send_TFT_Command(RW_GRAM);*/

#ifdef TFT_ORN_PORTRAIT
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_HOR_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = x >> 8;
	TFT_DATA_PORTLO = x;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_VER_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = y >> 8;
	TFT_DATA_PORTLO = y;
	WR = 0;
	WR = 1;
#else
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_HOR_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = y >> 8;
	TFT_DATA_PORTLO = y;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_VER_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = x >> 8;
	TFT_DATA_PORTLO = x;
	WR = 0;
	WR = 1;
#endif
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = RW_GRAM;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
}

void Set_Window_Add(signed int start_x, signed int end_x, signed int start_y, signed int end_y)
{
	//Send_TFT_RData(START_ADX,start_x);
	//Send_TFT_RData(END_ADX,end_x);
	//Send_TFT_RData(GRAM_ADX,start_x);
	//Send_TFT_RData(START_ADY,start_y);
	//Send_TFT_RData(END_ADY,end_y);
	//Send_TFT_RData(GRAM_ADY,start_y);
	//Send_TFT_Command(RW_GRAM);

	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = START_ADX;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = start_x >> 8;
	TFT_DATA_PORTLO = start_x;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = END_ADX;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = end_x >> 8;
	TFT_DATA_PORTLO = end_x;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_ADX;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = start_x >> 8;
	TFT_DATA_PORTLO = start_x;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = START_ADY;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = start_y >> 8;
	TFT_DATA_PORTLO = start_y;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = END_ADY;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = end_y >> 8;
	TFT_DATA_PORTLO = end_y;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_ADY;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = start_y >> 8;
	TFT_DATA_PORTLO = start_y;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = RW_GRAM;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
}

/*********************************************************************
* Function: 		void Reset_Window_Add()
* PreCondition: 	none
* Input: 			none
* Output: 			none
* Side Effects: 	none
* Overview: 		resets the window horizontal and vertical address
* Note: 			none
********************************************************************/
void Reset_Window_Add()
{
    //Send_TFT_RData(HOR_START_AD,0x0000);
    //Send_TFT_RData(HOR_END_AD,0x00EF);
    //Send_TFT_RData(VER_START_AD,0x0000);
    //Send_TFT_RData(VER_END_AD,0x013F);

	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = HOR_START_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = HOR_END_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0xef;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = VER_START_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = VER_END_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	TFT_DATA_PORTHI = 0x01;
	TFT_DATA_PORTLO = 0x3f;
	WR = 0;
	WR = 1;
}

void Clear_Device_ILI9341(unsigned int clear_color)
{
	unsigned long counter;
	/*unsigned long coord;
	coord = (unsigned long)TFT_SIZE_X * (unsigned long)TFT_SIZE_Y;
	Set_GRAM_Add(0,0);
	//Send_TFT_Command(RW_GRAM);
    for(counter = 0;counter < coord;counter++)
	{
		Send_TFT_Data(clear_color);
    }*/

	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO =  ILI9341_COLUMN_ADDR; //GRAM_HOR_AD; 
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = ILI9341_PAGE_ADDR; //GRAM_VER_AD
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = ILI9341_GRAM; //RW_GRAM
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
    for(counter = 0;counter < (unsigned long)FULL_SCREEN_SIZE;counter++)
	{
		TFT_DATA_PORTHI = clear_color >> 8;
		TFT_DATA_PORTLO = clear_color;
		WR = 0;
		WR = 1;
    }
}
/*********************************************************************
* Function: 		void ClearDevice(void)
* PreCondition: 	none
* Input: 			none
* Output: 			none
* Side Effects: 	none
* Overview: 		clears screen with current color 
* Note: 			none
********************************************************************/
void Clear_Device(unsigned int clear_color)
{
	unsigned long counter;
	/*unsigned long coord;
	coord = (unsigned long)TFT_SIZE_X * (unsigned long)TFT_SIZE_Y;
	Set_GRAM_Add(0,0);
	//Send_TFT_Command(RW_GRAM);
    for(counter = 0;counter < coord;counter++)
	{
		Send_TFT_Data(clear_color);
    }*/

	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO =  GRAM_HOR_AD; 
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = GRAM_VER_AD;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = RW_GRAM;
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
    for(counter = 0;counter < (unsigned long)FULL_SCREEN_SIZE;counter++)
	{
		TFT_DATA_PORTHI = clear_color >> 8;
		TFT_DATA_PORTLO = clear_color;
		WR = 0;
		WR = 1;
    }
}

void Clear_Window(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int clear_color)
{
	signed int start_x,end_x,start_y,end_y;
	unsigned long coord,counter;
	start_x = x;
	start_y = y;
	end_x = start_x + w - 1;
	end_y = start_y + h - 1;
	w = end_x - start_x + 1;
	h = end_y - start_y + 1;
	coord = (unsigned long)w * (unsigned long)h;
	Set_Window_Add(start_x, end_x, start_y, end_y);
    for(counter = 0;counter < coord;counter++)
	{
		//Send_TFT_Data(clear_color);
		TFT_DATA_PORTHI = clear_color >> 8;
		TFT_DATA_PORTLO = clear_color;
		WR = 0;
		WR = 1;
    }
}

/*********************************************************************
* Function: 		void Draw_Straignt_Line(unsigned int start_x,unsigned int start_y,unsigned int end_x,unsigned int end_y,unsigned long line_color)
* PreCondition: 	none
* Input: 			start_x    StartCol
*  					start_y    StartRow
*  					end_x	   EndCol
*  					end_y      EndRow
*					line_color
* Output: 			draws a line with line colour
* Side Effects: 	none
* Overview: 		draws a line
* Note: 			none
********************************************************************/
void Draw_Straight_Line(int start_x,int start_y,int end_x,int end_y,unsigned int line_color)
{
	unsigned int i;
	unsigned int size;
	size = end_x - start_x;
	if (size == 0)
	{
		size = end_y - start_y;
	}
	Set_Window_Add(start_x,end_x,start_y,end_y);
	for (i=0;i<size;i++)
	{
		//Send_TFT_Data(line_color);
		TFT_DATA_PORTHI = line_color >> 8;
		TFT_DATA_PORTLO = line_color;
		WR = 0;
		WR = 1;
	}
}

/*********************************************************************
* Function: 		void Fill_Rect(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned char v )
* PreCondition: 	none
* Input: 			x1    StartCol
*  					y1    StartRow
*  					x2    EndCol
*  					y1    EndRow
*					state
* Output: 			draws a line with current colour
* Side Effects: 	none
* Overview: 		draws a line
* Note: 			none
********************************************************************/
void Fill_Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int fill_color, unsigned int change_color,unsigned char mode)
{
	signed int start_x,end_x,start_y,end_y;
	unsigned int data;
	unsigned long coord,counter;
	start_x = x;
	start_y = y;
	end_x = start_x + w - 1;
	end_y = start_y + h - 1;
	w = end_x - start_x + 1;
	h = end_y - start_y + 1;
	coord = (unsigned long)w * (unsigned long)h;
	Set_Window_Add(start_x, end_x, start_y, end_y);
	if (mode == TFT_MODE_NORMAL)
	{
	    for(counter = 0;counter < coord;counter++)
		{
			//Send_TFT_Data(fill_color);
			TFT_DATA_PORTHI = fill_color >> 8;
			TFT_DATA_PORTLO = fill_color;
			WR = 0;
			WR = 1;
	    }
	}
	else if (mode == TFT_MODE_FULL)
	{
	    for(counter = 0;counter < coord;counter++)
		{
			//data =  Read_TFT_Data();	//dummy read
			//data =  Read_TFT_Data();
			prc2 = 1;					//port direction write enable off
			TFT_PORTLO_DIR = 0x00;		//Set as input
			TFT_PORTHI_DIR = 0x00;		//Set as input
			prc2 = 0;					//port direction write enable off
			RD = 0;
			data = TFT_DATA_PORTHI;
			data = data << 8;
			data |= TFT_DATA_PORTLO;	//dummy read
			RD = 1;
			RD = 0;
			data = TFT_DATA_PORTHI;
			data = data << 8;
			data |= TFT_DATA_PORTLO;	//data read
			RD = 1;
			//TFT_Set_Port_Write();
			prc2 = 1;					//port direction write enable off
			TFT_PORTLO_DIR = 0xff;		//Set as input
			TFT_PORTHI_DIR = 0xff;		//Set as input
			prc2 = 0;					//port direction write enable off
			if (data == change_color)
			{
				//Send_TFT_Data(fill_color);
				TFT_DATA_PORTHI = fill_color >> 8;
				TFT_DATA_PORTLO = fill_color;
				WR = 0;
				WR = 1;
			}
			else
			{
				//Send_TFT_Data(data);
				TFT_DATA_PORTHI = data >> 8;
				TFT_DATA_PORTLO = data;
				WR = 0;
				WR = 1;
			}
	    }
	}
	else if (mode == TFT_MODE_INVERSE)
	{
	    for(counter = 0;counter < coord;counter++)
		{
			//data =  Read_TFT_Data();	//dummy read
			//data =  Read_TFT_Data();
			//Send_TFT_Data(~data);
			//TFT_Set_Port_Read();
			prc2 = 1;					//port direction write enable off
			TFT_PORTLO_DIR = 0x00;		//Set as input
			TFT_PORTHI_DIR = 0x00;		//Set as input
			prc2 = 0;					//port direction write enable off
			RD = 0;
			data = TFT_DATA_PORTHI;
			data = data << 8;
			data |= TFT_DATA_PORTLO;	//dummy read
			RD = 1;
			RD = 0;
			data = TFT_DATA_PORTHI;
			data = data << 8;
			data |= TFT_DATA_PORTLO;	//data read
			RD = 1;
			//TFT_Set_Port_Write();
			prc2 = 1;					//port direction write enable off
			TFT_PORTLO_DIR = 0xff;		//Set as input
			TFT_PORTHI_DIR = 0xff;		//Set as input
			prc2 = 0;					//port direction write enable off
			
			TFT_DATA_PORTHI = (~data )>> 8;
			TFT_DATA_PORTLO = (~data);
			WR = 0;
			WR = 1;
    	}
	}
}

/*********************************************************************
* Function: 		void Draw_Rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int line_color)
* PreCondition: 	none
* Input: 			left    StartCol
*  					top    	StartRow
*  					right   EndCol
*  					bottom  EndRow
*					line color
* Output: 			draws a rectangle with line colour
* Side Effects: 	none
* Overview: 		draws a rectangle
* Note: 			none
********************************************************************/
void Draw_Rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int line_color)
{
	Draw_Straight_Line(left,top,right,top,line_color);
	Draw_Straight_Line(left,bottom,right+1,bottom,line_color);
	Draw_Straight_Line(left,top,left,bottom,line_color);
	Draw_Straight_Line(right,top,right,bottom,line_color);
}

/*********************************************************************
* Function: 		void Draw_Window(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom,unsigned int dcolor,unsigned int lcolor)
* PreCondition: 	none
* Input: 			left    StartCol
*  					top    	StartRow
*  					right   EndCol
*  					bottom  EndRow
*					lcolor
*					dcolor
* Output: 			draws a window with lcolour and dcolor
* Side Effects: 	none
* Overview: 		draws a window
* Note: 			none
********************************************************************/
void Draw_Window(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom,unsigned int dcolor,unsigned int lcolor)
{
	//top hor lines (3)
	Draw_Straight_Line(left,top,right,top,WHITE);
	Draw_Straight_Line(left+1,top+1,right-1,top+1,lcolor);
	Draw_Straight_Line(left+2,top+2,right-2,top+2,GRAY2);
	Draw_Straight_Line(left+3,top+3,right-3,top+3,dcolor);
	//bottom hor lines (3)
	Draw_Straight_Line(left,bottom,right,bottom,dcolor);
	Draw_Straight_Line(left+1,bottom-1,right-1,bottom-1,GRAY2);
	Draw_Straight_Line(left+2,bottom-2,right-2,bottom-2,lcolor);
	Draw_Straight_Line(left+3,bottom-3,right-3,bottom-3,WHITE);
	//left ver lines (3)
	Draw_Straight_Line(left,top,left,bottom,WHITE);
	Draw_Straight_Line(left+1,top+1,left+1,bottom-1,lcolor);
	Draw_Straight_Line(left+2,top+2,left+2,bottom-2,GRAY2);
	Draw_Straight_Line(left+3,top+3,left+3,bottom-3,dcolor);
	//right ver lines (3)
	Draw_Straight_Line(right,top,right,bottom,dcolor);
	Draw_Straight_Line(right-1,top+1,right-1,bottom-1,GRAY2);
	Draw_Straight_Line(right-2,top+2,right-2,bottom-2,lcolor);
	Draw_Straight_Line(right-3,top+3,right-3,bottom-3,WHITE);
}

void Write_Center_Text(unsigned int x_start,unsigned int x_end,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode)
{
	unsigned int total_ch_width;
	signed int left_space;
	unsigned int i;
	unsigned int screen_width;
	total_ch_width = 0;
	i = 0;
	while(chr_array_ptr[i])
	{
		Get_Font_Size(chr_array_ptr[i],fnt);
		total_ch_width = total_ch_width + chr_bit_width;
		i++;
	}
	screen_width = x_end - x_start;
	left_space = screen_width - total_ch_width;
	if (left_space < 0)
	{
		left_space = 0;
	}
	left_space = (left_space / (unsigned int)2) + x_start;
	Write_Text(left_space,y,chr_array_ptr,fnt,font_color,back_color,mode);
}

void Write_Center_Text_Bar(unsigned int x_start,unsigned int x_end,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode)
{
	unsigned int total_ch_width;
	signed int left_space;
	unsigned int i;
	unsigned int screen_width;
	total_ch_width = 0;
	i = 0;
	while(chr_array_ptr[i])
	{
		Get_Font_Size(chr_array_ptr[i],fnt);
		total_ch_width = total_ch_width + chr_bit_width;
		i++;
	}
	screen_width = x_end - x_start;
	left_space = screen_width - total_ch_width;
	if (left_space < 0)
	{
		left_space = 0;
	}
	left_space = (left_space / (unsigned int)2) + x_start;
	Write_Text_Bar(x_start,y,left_space,x_end,chr_array_ptr,fnt,font_color,back_color,mode);
}

void Write_Right_Text(unsigned int rp,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode)
{
	unsigned int total_ch_width;
	unsigned int cur_x_pos;
	unsigned int i;
	total_ch_width = 0;
	i = 0;
	while(chr_array_ptr[i])
	{
		Get_Font_Size(chr_array_ptr[i],fnt);
		total_ch_width = total_ch_width + chr_bit_width;
		i++;
	}
	cur_x_pos = 0;
	if (rp > total_ch_width)
	{
		cur_x_pos = rp - total_ch_width;
	}
	Write_Text(cur_x_pos,y,chr_array_ptr,fnt,font_color,back_color,mode);
}

void Write_Right_Text_Bar(unsigned int rp,unsigned int y,unsigned int x_start,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode)
{
	unsigned int total_ch_width;
	unsigned int cur_x_pos;
	unsigned int i;
	total_ch_width = 0;
	i = 0;
	while(chr_array_ptr[i])
	{
		Get_Font_Size(chr_array_ptr[i],fnt);
		total_ch_width = total_ch_width + chr_bit_width;
		i++;
	}
	cur_x_pos = 0;
	if (rp > total_ch_width)
	{
		cur_x_pos = rp - total_ch_width;
	}
	Write_Text_Bar(x_start,y,cur_x_pos,rp,chr_array_ptr,fnt,font_color,back_color,mode);
}

void Write_Text(unsigned int x, unsigned int y, unsigned far char * chr_array_ptr, unsigned far char * fnt, unsigned int font_color,unsigned int back_color, unsigned char mode)
{
	unsigned int i;
	unsigned int temp_value;
	unsigned int data_bytes;
	unsigned char chr_offset;
	unsigned far char * cur_fnt;
	i = 0;
	buf_x = x;
	buf_y = y;
	while(chr_array_ptr[i])
	{
		data_bytes = Get_Font_Shape(chr_array_ptr[i],fnt);
		cur_fnt = fnt + data_bytes;
		Write_Cur_Char(cur_fnt,font_color,back_color,mode);
		buf_y = y;
		i++;
	}
}

void Write_Text_Bar(unsigned int x, unsigned int y, unsigned int txt_start,unsigned int x_end,unsigned far char * chr_array_ptr, unsigned far char * fnt, unsigned int font_color,unsigned int back_color, unsigned char mode)
{
	unsigned int i;
	unsigned int temp_value;
	unsigned int data_bytes;
	unsigned char chr_offset;
	unsigned far char * cur_fnt;
	unsigned char text_height;
	i = 0;
	buf_x = x;
	buf_y = y;
	if (txt_start > x)
	{
		text_height = fnt[0];
		Fill_Rect(x,y,txt_start-x,text_height,back_color,font_color,TFT_MODE_NORMAL);
	}
	buf_x = txt_start;
	while(chr_array_ptr[i])
	{
		data_bytes = Get_Font_Shape(chr_array_ptr[i],fnt);
		cur_fnt = fnt + data_bytes;
		Write_Cur_Char(cur_fnt,font_color,back_color,mode);
		buf_y = y;
		i++;
	}
	if (x_end > buf_y)
	{
		Fill_Rect(buf_x,buf_y,x_end-buf_x,text_height,back_color,font_color,TFT_MODE_NORMAL);
	}
}

unsigned int Get_Font_Shape(unsigned char cur_char,unsigned far char * fnt)
{
	unsigned int temp_value;
	unsigned int data_bytes;
	if ((cur_char < 0x20) || (cur_char > 0x7e))
	{
		cur_char = 0x00;
	}
	else
	{
		cur_char = cur_char - 0x20;
	}
	chr_bit_height = fnt[0];
	temp_value = (unsigned int) cur_char * (unsigned int) 3;
	chr_bit_width = fnt[temp_value+1];
	data_bytes = fnt[temp_value+2];
	data_bytes = data_bytes << 8;
	data_bytes |= fnt[temp_value+3];
	return (data_bytes);
}

void Get_Font_Size(unsigned char cur_char,unsigned far char * fnt)
{
	unsigned int temp_value;
	if ((cur_char < 0x20) || (cur_char > 0x7e))
	{
		cur_char = 0x00;
	}
	else
	{
		cur_char = cur_char - 0x20;
	}
	chr_bit_height = fnt[0];
	temp_value = (unsigned int) cur_char * (unsigned int) 3;
	chr_bit_width = fnt[temp_value+1];
}

void Write_Cur_Char(unsigned far char * fnt, unsigned int font_color,unsigned int back_color,unsigned char mode)
{
	unsigned int x;
	unsigned char bitno;
	unsigned int y;
	unsigned char cur_byte;
	unsigned char temp_value;
	unsigned char bit_value;
	unsigned int data;
	cur_byte = 0;
	Set_Window_Add(buf_x, (buf_x + chr_bit_width-1), buf_y, (buf_y + chr_bit_height-1));
	temp_value = 0x01;
	bitno = 8;
	x = (unsigned int)chr_bit_height * (unsigned int)chr_bit_width;
	for (y=0;y<x;y++)
	{
		bit_value = fnt[cur_byte] & temp_value;
		if (bit_value)
		{
			//Send_TFT_Data(font_color);
			TFT_DATA_PORTHI = font_color >> 8;
			TFT_DATA_PORTLO = font_color;
			WR = 0;
			WR = 1;
		}
		else
		{
			switch (mode)
			{
				case TFT_MODE_NORMAL:
					//data =  Read_TFT_Data();	//dummy read
					//data =  Read_TFT_Data();
					//Send_TFT_Data(data);
					prc2 = 1;					//port direction write enable off
					TFT_PORTLO_DIR = 0x00;		//Set as input
					TFT_PORTHI_DIR = 0x00;		//Set as input
					prc2 = 0;					//port direction write enable off
					RD = 0;
					data = TFT_DATA_PORTHI;
					data = data << 8;
					data |= TFT_DATA_PORTLO;	//dummy read
					RD = 1;
					RD = 0;
					data = TFT_DATA_PORTHI;
					data = data << 8;
					data |= TFT_DATA_PORTLO;	//data read
					RD = 1;
					//TFT_Set_Port_Write();
					prc2 = 1;					//port direction write enable off
					TFT_PORTLO_DIR = 0xff;		//Set as input
					TFT_PORTHI_DIR = 0xff;		//Set as input
					prc2 = 0;					//port direction write enable off
					TFT_DATA_PORTHI = data >> 8;
					TFT_DATA_PORTLO = data;
					WR = 0;
					WR = 1;
					break;
				case TFT_MODE_INVERSE:
					//Send_TFT_Data(~font_color);
					TFT_DATA_PORTHI = (~font_color) >> 8;
					TFT_DATA_PORTLO = (~font_color);
					WR = 0;
					WR = 1;
					break;
				default:
					//Send_TFT_Data(back_color);
					TFT_DATA_PORTHI = back_color >> 8;
					TFT_DATA_PORTLO = back_color;
					WR = 0;
					WR = 1;
					break;
			}
		}
		temp_value <<= 1;
		bitno--;
		if (bitno == 0)
		{
			temp_value = 0x01;
			bitno = 8;
			cur_byte++;
		}
	}
	buf_x = buf_x + chr_bit_width;
}


