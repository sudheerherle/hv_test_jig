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

struct driverInfo{
	unsigned long int displayID;    // response of 0x04h
	unsigned long int displayStatus; //response of 0x09h
	unsigned char displayPowerMode; // response of 0x0ah
	unsigned char displayPixelFormat; // response of 0x0ch;
	unsigned char displayImageMode; // response of 0x0dh;
	unsigned char displaySignalMode; // response of 0x0eh
	unsigned char displaySelfDiagnosticResult; // response of 0x0fh
};

unsigned long int byteToDWord(unsigned char lsb1, unsigned char lsb2, unsigned char msb1, unsigned char msb2);

struct driverInfo driverStatus;

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
	unsigned char data;
	TFT_Set_Port_Read();
	RD = 1;
	DelayMs(1);
	RD = 0;
	DelayMs(1);
	RD = 1;
	
	//data = TFT_DATA_PORTHI;
//	data = data << 8;
	data = TFT_DATA_PORTLO;	
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

void TFT_ST7789_New_Init(void)
{
	TFT_Init_IO();
	RESET = 1;
	RS = 1;
	WR = 1;
	RD = 1;
    // Reset controller
	RESET = 0;
    DelayMs(200);
	RESET = 1; // start
    DelayMs(200);
	
	//TFT_reset();
	
    /* Reset the display controller */
   // RESET = 0;
  //  DelayMs(100);
   // RESET = 1;
   // DelayMs(50);

    Send_TFT_Command(ST7789V_CMD_DISPOFF);
    Send_TFT_Command(ST7789V_CMD_SLPOUT);    /* Exit Sleep mode */

    DelayMs(100);

    Send_TFT_Command(ST7789V_CMD_MADCTL);
    Send_TFT_Data(0xa8);    /* MADCTL: memory data access control */

    Send_TFT_Command(ST7789V_CMD_COLMOD);
    Send_TFT_Data(0x55);    /* COLMOD: Interface Pixel format */

    Send_TFT_Command(ST7789V_CMD_PORCTRL);
    Send_TFT_Data(0x0C);
    Send_TFT_Data(0x0C);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x33);
    Send_TFT_Data(0x33);    /* PORCTRK: Porch setting */

    Send_TFT_Command(ST7789V_CMD_GCTRL);
    Send_TFT_Data(0x35);    /* GCTRL: Gate Control */

    Send_TFT_Command(ST7789V_CMD_VCOMS);
    Send_TFT_Data(0x2B);    /* VCOMS: VCOM setting */

    Send_TFT_Command(ST7789V_CMD_LCMCTRL);
    Send_TFT_Data(0x2C);    /* LCMCTRL: LCM Control */

    Send_TFT_Command(ST7789V_CMD_VDVVRHEN);
    Send_TFT_Data(0x01);
    Send_TFT_Data(0xFF);    /* VDVVRHEN: VDV and VRH Command Enable */

    Send_TFT_Command(ST7789V_CMD_VRHS);
    Send_TFT_Data(0x11);    /* VRHS: VRH Set */

    Send_TFT_Command(ST7789V_CMD_VDVS);
    Send_TFT_Data(0x20);    /* VDVS: VDV Set */

    Send_TFT_Command(ST7789V_CMD_FRCTRL2);
    Send_TFT_Data(0x0F);    /* FRCTRL2: Frame Rate control in normal mode */

    Send_TFT_Command(ST7789V_CMD_PWCTRL1);
    Send_TFT_Data(0xA4);
    Send_TFT_Data(0xA1);    /* PWCTRL1: Power Control 1 */

    Send_TFT_Command(ST7789V_CMD_PVGAMCTRL);
    Send_TFT_Data(0xD0);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x05);
    Send_TFT_Data(0x0E);
    Send_TFT_Data(0x15);
    Send_TFT_Data(0x0D);
    Send_TFT_Data(0x37);
    Send_TFT_Data(0x43);
    Send_TFT_Data(0x47);
    Send_TFT_Data(0x09);
    Send_TFT_Data(0x15);
    Send_TFT_Data(0x12);
    Send_TFT_Data(0x16);
    Send_TFT_Data(0x19);    /* PVGAMCTRL: Positive Voltage Gamma control */

    Send_TFT_Command(ST7789V_CMD_NVGAMCTRL);
    Send_TFT_Data(0xD0);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x05);
    Send_TFT_Data(0x0D);
    Send_TFT_Data(0x0C);
    Send_TFT_Data(0x06);
    Send_TFT_Data(0x2D);
    Send_TFT_Data(0x44);
    Send_TFT_Data(0x40);
    Send_TFT_Data(0x0E);
    Send_TFT_Data(0x1C);
    Send_TFT_Data(0x18);
    Send_TFT_Data(0x16);
    Send_TFT_Data(0x19);    /* NVGAMCTRL: Negative Voltage Gamma control */

    Send_TFT_Command(ST7789V_CMD_RASET);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0xEF);    /* Y address set */

    Send_TFT_Command(ST7789V_CMD_CASET);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x00);
    Send_TFT_Data(0x01);
    Send_TFT_Data(0x3F);    /* X address set */

    DelayMs(10);
	Clear_Device_ST7789(YELLOW);
    Send_TFT_Command(ST7789V_CMD_DISPON);
	Clear_Device_ST7789(BLUE);
}

void Read_Driver_Info(){
	
	unsigned char data1,data2,data3,data4,data5;
	Send_TFT_Command(0x04);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	data3 = Read_TFT_Data1();
	data4 = Read_TFT_Data1();
	
	driverStatus.displayID = byteToDWord (data1, data2, data3, data4);
	
	Send_TFT_Command(0x09);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	data3 = Read_TFT_Data1();
	data4 = Read_TFT_Data1();
	data5 = Read_TFT_Data1();
	driverStatus.displayStatus = byteToDWord (data2, data3, data4, data5);
	
	
	Send_TFT_Command(0x0A);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	
	driverStatus.displayPowerMode = data2;
	
	Send_TFT_Command(0x0C);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	driverStatus.displayPixelFormat = data2;
	
	
	Send_TFT_Command(0x0D);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	driverStatus.displayImageMode = data2;
	
	Send_TFT_Command(0x0E);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	driverStatus.displaySignalMode = data2;
	
	Send_TFT_Command(0x0f);
	data1 = Read_TFT_Data1();
	data2 = Read_TFT_Data1();
	driverStatus.displaySelfDiagnosticResult = data2;
	
	
}

unsigned long int byteToDWord(unsigned char lsb1, unsigned char lsb2, unsigned char msb1, unsigned char msb2){

	unsigned long int Dword = 0;
	Dword = msb2;
	Dword = Dword << 8 ;
	Dword |= msb1;
	Dword = Dword << 8;
	Dword |= lsb2;
	Dword = Dword << 8;
	Dword |= lsb1;	
	return Dword;	
}

void Set_Window_Add(signed int start_x, signed int end_x, signed int start_y, signed int end_y)
{
/* Specify the X direction operation area*/
    Send_TFT_Command(ST7789V_CMD_CASET);
    Send_TFT_Data(start_x);
    Send_TFT_Data(end_x);
    //Send_TFT_CData(start_x >> 8);
    //Send_TFT_CData(start_x);
    //Send_TFT_CData(end_x >> 8);
    //Send_TFT_CData(end_x);

   /* Specify the Y direction operation area*/
    Send_TFT_Command(ST7789V_CMD_RASET);
    Send_TFT_Data(start_y);
    Send_TFT_Data(end_y);
    //Send_TFT_CData(start_y >> 8);
    //Send_TFT_CData(start_y);
    //Send_TFT_CData(end_y >> 8);
    //Send_TFT_CData(end_y);

   /* Send this command, LCD starts to wait for receiving video memory data*/
    Send_TFT_Command(ST7789V_CMD_RAMWR);
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

    Send_TFT_Command(ST7789V_CMD_CASET);
    Send_TFT_Data(0);
    Send_TFT_Data(TFT_SIZE_X);
    //Send_TFT_CData(start_x >> 8);
    //Send_TFT_CData(start_x);
    //Send_TFT_CData(end_x >> 8);
    //Send_TFT_CData(end_x);

   /* Specify the Y direction operation area*/
    Send_TFT_Command(ST7789V_CMD_RASET);
    Send_TFT_Data(0);
    Send_TFT_Data(TFT_SIZE_Y);
	
}

void Clear_Device_ST7789(unsigned int clear_color)
{
	unsigned long counter;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO =  ST7789V_CMD_CASET; //GRAM_HOR_AD; 
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = ST7789V_CMD_RASET; //GRAM_VER_AD
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = 0;
	WR = 0;
	WR = 1;
	//TFT_DATA_PORTHI = 0;
	TFT_DATA_PORTLO = ST7789V_CMD_RAMWR; //RW_GRAM
	TFT_CONTROL_PORT &= 0xf9;
	TFT_CONTROL_PORT |= 0x06;
    for(counter = 0;counter < (unsigned long)FULL_SCREEN_SIZE;counter++)
	{
		TFT_DATA_PORTHI = clear_color >> 8;
		WR = 0;
		WR = 1;
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
Set_Window_Add(0,0xef,0,0x13f);
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
			WR = 0;
			WR = 1;
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
					WR = 0;
					WR = 1;
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


