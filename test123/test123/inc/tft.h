#ifndef _TFTLCD_H_
#define _TFTLCD_H_

//  _____________________
// | _______________	 |
// ||<-- origin in software
// ||	---------	|[]	 |
// ||		/		|[]	 |
// || 	-------->	|[]	 |
// ||_______________|[]	 |
// |<-- LCD's origin (0x0000)
//
//define TFT_ORN_PORTRAIT when your LCD is installed in vertical
//  ________________
// |<-- LCD's origin (0x0000) = origin in software
// ||--------------||
// ||			   ||
// ||	------     ||
// ||		/	   ||
// ||	  /		   ||
// ||	/		   ||
// ||	------>	   ||
// ||______________||
// | [] [] [] [] []	|
// |________________|
//

#define START_OSC			0x00 //data read at this instruction should be 0x9735 --> use for test connection
#define DRIV_OUT_CTRL		0x01
#define DRIV_WAV_CTRL		0x02
#define ENTRY_MOD			0x03
#define RESIZE_CTRL			0x04
#define DISP_CTRL1			0x07
#define DISP_CTRL2			0x08
#define DISP_CTRL3			0x09
#define DISP_CTRL4			0x0A
#define RGB_DISP_IF_CTRL1	0x0C
#define FRM_MARKER_POS		0x0D
#define RGB_DISP_IF_CTRL2	0x0F
#define POW_CTRL1			0x10
#define POW_CTRL2			0x11
#define POW_CTRL3			0x12
#define POW_CTRL4			0x13
#define GRAM_HOR_AD			0x20
#define GRAM_VER_AD			0x21
#define RW_GRAM				0x22
#define POW_CTRL7			0x29
#define FRM_RATE_COL_CTRL	0x2B
#define GAMMA_CTRL1			0x30
#define GAMMA_CTRL2			0x31
#define GAMMA_CTRL3			0x32
#define GAMMA_CTRL4			0x35 
#define GAMMA_CTRL5			0x36
#define GAMMA_CTRL6			0x37
#define GAMMA_CTRL7			0x38
#define GAMMA_CTRL8			0x39
#define PIXEL_FORMAT		0x3A
#define GAMMA_CTRL9			0x3C
#define GAMMA_CTRL10		0x3D
#define HOR_START_AD		0x50
#define HOR_END_AD			0x51
#define VER_START_AD		0x52
#define VER_END_AD			0x53
#define GATE_SCAN_CTRL1		0x60
#define GATE_SCAN_CTRL2		0x61
#define GATE_SCAN_CTRL3		0x6A
#define PART_IMG1_DISP_POS	0x80
#define PART_IMG1_START_AD	0x81
#define PART_IMG1_END_AD	0x82
#define PART_IMG2_DISP_POS	0x83
#define PART_IMG2_START_AD	0x84
#define PART_IMG2_END_AD	0x85
#define PANEL_IF_CTRL1		0x90
#define PANEL_IF_CTRL2		0x92
#define PANEL_IF_CTRL3		0x93
#define PANEL_IF_CTRL4		0x95
#define PANEL_IF_CTRL5		0x97
#define PANEL_IF_CTRL6		0x98
#define INT_TIMING_CTRL1	0xE3
#define INT_TIMING_CTRL2	0xE7
#define INT_TIMING_CTRL3	0xEF

//touch screen LCD configuration
//#define TFT_ORN_PORTRAIT

#ifdef TFT_ORN_PORTRAIT
#define TFT_SIZE_X			240
#define TFT_SIZE_Y			320
#define VAL_ENTRY_MOD		0x0030//0x0030
#define BAR_MODE			0x0000
#define XY_POS_START		0xA700
#define START_ADX   		HOR_START_AD
#define END_ADX   			HOR_END_AD
#define START_ADY   		VER_START_AD
#define END_ADY   			VER_END_AD
#define GRAM_ADX			GRAM_HOR_AD
#define GRAM_ADY			GRAM_VER_AD
#define MAX_TRXN_LINES		13
#else
#define TFT_SIZE_X			320
#define TFT_SIZE_Y			240
#define VAL_ENTRY_MOD		0x0038//0x0028
#define BAR_MODE			0x0008
#define XY_POS_START		0x2700
#define START_ADX   		VER_START_AD
#define END_ADX   			VER_END_AD
#define START_ADY   		HOR_START_AD
#define END_ADY   			HOR_END_AD
#define GRAM_ADX			GRAM_VER_AD
#define GRAM_ADY			GRAM_HOR_AD
#define MAX_TRXN_LINES		8
#endif

//**** CPU SPEED IN Hz (MIP Rating) ****
#define F_CPU 				40000000    /* renesas internal clock max speed */
#define DLY_COUNT  			(F_CPU/2000000)
#define _delay_us(dly) 		{unsigned long x=0,y=dly; while(y--) {x=DLY_COUNT;while(x--);}}

//#define WHITE_COLOR_SCHEME
#define BLACK_COLOR_SCHEME

// Color codes
#ifdef WHITE_COLOR_SCHEME

                                 		// BBBBBGGGGGGRRRRR		//	RRRRR GGGGGG BBBBB 
#define BLACK              (unsigned int)0b0000000000000000		//	00000 000000 00000
#define WHITE              (unsigned int)0b1111111111111111		//	11111 111111 11111
//#define BRIGHTBLUE         (unsigned int)0b1111100000000000		//	00000 000000 11111
#define BRIGHTBLUE         (unsigned int)0b1111111111100000		//	00000 000000 11111
#else
#define WHITE              (unsigned int)0b0000000000000000		//	00000 000000 00000
#define BLACK              (unsigned int)0b1111111111111111		//	11111 111111 11111
//#define BRIGHTBLUE         (unsigned int)0b1111110000010000		//	10000 100000 11111
#define BRIGHTBLUE         (unsigned int)0b1111111111100000		//	10000 100000 11111
#endif

                                 		// BBBBBGGGGGGRRRRR		//	RRRRR GGGGGG BBBBB 
//#define BRIGHTGREEN        (unsigned int)0b0000011111100000		//	00000 111111 00000
#define BRIGHTGREEN        (unsigned int)0b0000011111100000		//	00000 111111 00000
#define BRIGHTCYAN         (unsigned int)0b1111111111100000		//	00000 111111 11111
#define BRIGHTRED          (unsigned int)0b0000000000011111		//	11111 000000 00000
#define BRIGHTMAGENTA      (unsigned int)0b1111100000011111		//	11111 000000 11111
#define BRIGHTYELLOW       (unsigned int)0b0000011111111111		//	11111 111111 00000
#define BLUE               (unsigned int)0b1000000000000000		//	00000 000000 10000
#define GREEN              (unsigned int)0b0000010000000000		//	00000 100000 00000
#define CYAN               (unsigned int)0b1000010000000000		//	00000 100000 10000
#define RED                (unsigned int)0b0000000000010000		//	10000 000000 00000
#define MAGENTA            (unsigned int)0b1000000000010000		//	10000 000000 10000
#define BROWN              (unsigned int)0b0000010000011111		//	11111 100000 00000
#define LIGHTGRAY          (unsigned int)0b1000010000010000		//	10000 100000 10000
#define DARKGRAY           (unsigned int)0b0100001000001000		//	01000 010000 01000
#define LIGHTBLUE          (unsigned int)0b1111110000010000		//	10000 100000 11111
#define LIGHTGREEN         (unsigned int)0b1000011111110000		//	10000 111111 10000
#define LIGHTCYAN          (unsigned int)0b1111111111110000		//	10000 111111 11111
#define LIGHTRED           (unsigned int)0b1000010000011111		//	11111 100000 10000
#define LIGHTMAGENTA       (unsigned int)0b1111110000011111		//	11111 100000 11111
#define YELLOW             (unsigned int)0b1000011111111111		//	11111 111111 10000

#define GRAY0       	   (unsigned int)0b1110011100011100		//	11100 111000 11100
#define GRAY1         	   (unsigned int)0b1100011000011000		//	11000 110000 11000   
#define GRAY2              (unsigned int)0b1010010100010100		//	10100 101000 10100   
#define GRAY3              (unsigned int)0b1000010000010000		//	10000 100000 10000
#define GRAY4              (unsigned int)0b0110001100001100		//	01100 011000 01100
#define GRAY5              (unsigned int)0b0100001000001000		//	01000 010000 01000
#define GRAY6	           (unsigned int)0b0010000100000100		//	00100 001000 00100

//**** I/O Ports
#define TFT_DATA_PORTLO		p6
#define TFT_DATA_PORTHI		p3
#define TFT_PORTLO_DIR		pd6
#define TFT_PORTHI_DIR		pd3

//**** I/O Command Port Bits
#define RD    				p5_3		/* LCD E Pin */
#define WR    				p5_2		/* LCD RW Pin */
#define RS    				p5_1		/* LCD RS pin */
#define RESET    			p5_4		/* LCD Reset pin */
#define CS    				p5_0		/* LCD Chip Select */
#define TFT_CONTROL_PORT	p5

//**** Margins
#define XL_MARGIN			0
#define XR_MARGIN			TFT_SIZE_X - 1
#define YU_MARGIN			0
#define YL_MARGIN			TFT_SIZE_Y - 1
#define FULL_SCREEN_SIZE	(unsigned long)((unsigned long)TFT_SIZE_X * (unsigned long)TFT_SIZE_Y)

typedef enum
{
	TFT_MODE_NORMAL,
	TFT_MODE_INVERSE,
	TFT_MODE_FULL,
} tft_mode;




void DelayMs(unsigned long delay);
void TFT_Init_IO(void);
void TFT_9328_Init(void);
void TFT_9325_Init(void);
void TFT_9325New_Init(void);
static void TFT_Set_Port_Read(void);
static void TFT_Set_Port_Write(void);
void Send_TFT_RData(unsigned int index, unsigned int data);
unsigned int Read_TFT_RData(unsigned int index);
void Send_TFT_Command(unsigned int cmd);
void Send_TFT_Data(unsigned int data);
unsigned int Read_TFT_Data();
unsigned int Read_TFT_GData(unsigned int index);
void Set_GRAM_Add(signed int x, signed int y);
void Set_Window_Add(signed int start_x, signed int end_x, signed int start_y, signed int end_y);
void Reset_Window_Add();
void Clear_Device(unsigned int clear_color);
void Clear_Window(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int clear_color);
void Frame_Show(unsigned int * bitmap);
void Show_Picture(unsigned far int *bitmap, unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char mode, unsigned int bg_color);
void Draw_Straight_Line(int start_x,int start_y,int end_x,int end_y,unsigned int line_color);
void Draw_Line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int line_colour);
void Fill_Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int fill_color, unsigned int change_color,unsigned char mode);
void Draw_Rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int line_color);
void Draw_Window(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom,unsigned int dcolor,unsigned int lcolor);
void Write_Center_Text(unsigned int x_start,unsigned int x_end,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode);
void Write_Center_Text_Bar(unsigned int x_start,unsigned int x_end,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode);
void Write_Right_Text(unsigned int rp,unsigned int y,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode);
void Write_Right_Text_Bar(unsigned int rp,unsigned int y,unsigned int x_start,unsigned far char * chr_array_ptr,unsigned far char * fnt,unsigned int font_color,unsigned int back_color,unsigned char mode);
void Write_Text(unsigned int x, unsigned int y, unsigned far char * chr_array_ptr, unsigned far char * fnt, unsigned int font_color,unsigned int back_color, unsigned char mode);
void Write_Text_Bar(unsigned int x, unsigned int y, unsigned int txt_start,unsigned int x_end,unsigned far char * chr_array_ptr, unsigned far char * fnt, unsigned int font_color,unsigned int back_color, unsigned char mode);
unsigned int Get_Font_Shape(unsigned char cur_char,unsigned far char * fnt);
void Get_Font_Size(unsigned char cur_char,unsigned far char * fnt);
void Write_Cur_Char(unsigned far char * fnt, unsigned int font_color,unsigned int back_color,unsigned char mode);

#endif
