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

#define BLACK              (unsigned int)0x0000	//0b0000000000000000		//	00000 000000 00000
#define BRIGHTBLUE         (unsigned int)0xf800	//0b1111100000000000		//	00000 000000 11111
#define BRIGHTGREEN        (unsigned int)0x0600	//0b0000011000000000		//	00000 111111 00000
#define BRIGHTCYAN         (unsigned int)0xFFE0	//0b1111111111100000		//	00000 111111 11111
#define BRIGHTRED          (unsigned int)0x001F	//0b0000000000011111		//	11111 000000 00000
#define BRIGHTMAGENTA      (unsigned int)0xF81F	//0b1111100000011111		//	11111 000000 11111
#define BRIGHTYELLOW       (unsigned int)0x07FF	//0b0000011111111111		//	11111 111111 00000
#define BLUE               (unsigned int)0x8000	//0b1000000000000000		//	00000 000000 10000
#define GREEN              (unsigned int)0x0400	//0b0000010000000000		//	00000 100000 00000
#define CYAN               (unsigned int)0x8400	//0b1000010000000000		//	00000 100000 10000
#define RED                (unsigned int)0x0010	//0b0000000000010000		//	10000 000000 00000
#define MAGENTA            (unsigned int)0x8010	//0b1000000000010000		//	10000 000000 10000
#define BROWN              (unsigned int)0x041F	//0b0000010000011111		//	11111 100000 00000
#define LIGHTGRAY          (unsigned int)0x8410	//0b1000010000010000		//	10000 100000 10000
#define DARKGRAY           (unsigned int)0x4208	//0b0100001000001000		//	01000 010000 01000
#define LIGHTBLUE          (unsigned int)0xFC10	//0b1111110000010000		//	10000 100000 11111
#define LIGHTGREEN         (unsigned int)0x87F0	//0b1000011111110000		//	10000 111111 10000
#define LIGHTCYAN          (unsigned int)0xFFF0	//0b1111111111110000		//	10000 111111 11111
#define LIGHTRED           (unsigned int)0x841F	//0b1000010000011111		//	11111 100000 10000
#define LIGHTMAGENTA       (unsigned int)0xFC1F	//0b1111110000011111		//	11111 100000 11111
#define YELLOW             (unsigned int)0x87FF	//0b1000011111111111		//	11111 111111 10000
#define WHITE              (unsigned int)0xffff	//0b1111111111111111		//	11111 111111 11111

#define GRAY0       	   (unsigned int)0xE71C	//0b1110011100011100		//	11100 111000 11100
#define GRAY1         	   (unsigned int)0xC618	//0b1100011000011000		//	11000 110000 11000   
#define GRAY2              (unsigned int)0xA514	//0b1010010100010100		//	10100 101000 10100   
#define GRAY3              (unsigned int)0x8410	//0b1000010000010000		//	10000 100000 10000
#define GRAY4              (unsigned int)0x630C	//0b0110001100001100		//	01100 011000 01100
#define GRAY5              (unsigned int)0x4208	//0b0100001000001000		//	01000 010000 01000
#define GRAY6	           (unsigned int)0x2104	//0b0010000100000100		//	00100 001000 00100

//**** I/O Ports
//#define TFT_DATA_PORTLO		p6
#define TFT_DATA_PORT		p3
//#define TFT_PORTLO_DIR		pd6
#define TFT_PORT_DIR		pd3//1

#define TFT_OUTPUT_DIRECTION		0xff
#define TFT_INPUT_DIRECTION			0x00
#define _Tft_Port_Input		{TFT_PORT_DIR = TFT_INPUT_DIRECTION}
#define _Tft_Port_Output	{TFT_PORT_DIR = TFT_OUTPUT_DIRECTION}	


//**** I/O Command Port Bits
#define TFT_RD_PIN    		p5_3		/* LCD E Pin */
#define TFT_WR_PIN    		p5_2		/* LCD RW Pin */
#define TFT_RS_PIN  		p5_1		/* LCD TFT_RS_PIN pin */
#define TFT_RESET_PIN    	p5_4		/* LCD Reset pin */
#define TFT_CS_PIN    		p5_0		/* LCD Chip Select */

#define ILI9341_NOP                                                                      0x00
#define ILI9341_RESET                                                                    0x01
#define ILI9341_READ_DISPLAY_IDENTIFICATION_INFORMATION                                  0x04
#define ILI9341_READ_DISPLAY_STATUS                                                      0x09
#define ILI9341_READ_DISPLAY_POWER_MODE                                                  0x0A
#define ILI9341_READ_DISPLAY_MADCTL                                                      0x0B
#define ILI9341_READ_DISPLAY_PIXEL_FORMAT                                                0x0C
#define ILI9341_READ_DISPLAY_IMAGE_FORMAT                                                0x0D
#define ILI9341_READ_DISPLAY_SIGNAL_MODE                                                 0x0E
#define ILI9341_READ_DISPLAY_SELF_DIAGNOSTIC_RESULT                                      0x0F
#define ILI9341_ENTER_SLEEP_MODE                                                         0x10
#define ILI9341_SLEEP_OUT                                                                0x11
#define ILI9341_PARTIAL_MODE_ON                                                          0x12
#define ILI9341_NORMAL_DISPLAY_MODE_ON                                                   0x13
#define ILI9341_DISPLAY_INVERSION_OFF                                                    0x20
#define ILI9341_DISPLAY_INVERSION_ON                                                     0x21
#define ILI9341_GAMMA                                                                    0x26
#define ILI9341_DISPLAY_OFF                                                              0x28
#define ILI9341_DISPLAY_ON                                                               0x29
#define ILI9341_COLUMN_ADDR                                                              0x2A
#define ILI9341_PAGE_ADDR                                                                0x2B
#define ILI9341_GRAM                                                                     0x2C
#define ILI9341_COLOR_SET                                                                0x2D
#define ILI9341_MEMORY_READ                                                              0x2E
#define ILI9341_PARTIAL_AREA                                                             0x30
#define ILI9341_VERTICAL_SCROLLING_DEFINITION                                            0x33
#define ILI9341_TEARING_EFFECT_LINE_OFF                                                  0x34
#define ILI9341_TEARING_EFFECT_LINE_ON                                                   0x35
#define ILI9341_MAC                                                                      0x36
#define ILI9341_VERTICAL_SCROLLING_START_ADDRESS                                         0x37
#define ILI9341_IDLE_MODE_OFF                                                            0x38
#define ILI9341_IDLE_MODE_ON                                                             0x39
#define ILI9341_PIXEL_FORMAT                                                             0x3A
#define ILI9341_WMC                                                                      0x3C
#define ILI9341_RMC                                                                      0x3E
#define ILI9341_SET_TEAR_SCANLINE                                                        0x44
#define ILI9341_WDB                                                                      0x51
#define ILI9341_READ_DISPLAY_BRIGHTNESS                                                  0x52
#define ILI9341_WCD                                                                      0x53
#define ILI9341_READ_CTRL_DISPLAY                                                        0x54
#define ILI9341_WCABC                                                                    0x55
#define ILI9341_RCABC                                                                    0x56
#define ILI9341_WCABCMB                                                                  0x5E
#define ILI9341_RCABCMB                                                                  0x5F
#define ILI9341_RGB_INTERFACE                                                            0xB0
#define ILI9341_FRC                                                                      0xB1
#define ILI9341_FRAME_CTRL_NM                                                            0xB2
#define ILI9341_FRAME_CTRL_IM                                                            0xB3
#define ILI9341_FRAME_CTRL_PM                                                            0xB4
#define ILI9341_BPC                                                                      0xB5
#define ILI9341_DFC                                                                      0xB6
#define ILI9341_ENTRY_MODE_SET                                                           0xB7
#define ILI9341_BACKLIGHT_CONTROL_1                                                      0xB8
#define ILI9341_BACKLIGHT_CONTROL_2                                                      0xB9
#define ILI9341_BACKLIGHT_CONTROL_3                                                      0xBA
#define ILI9341_BACKLIGHT_CONTROL_4                                                      0xBB
#define ILI9341_BACKLIGHT_CONTROL_5                                                      0xBC
#define ILI9341_BACKLIGHT_CONTROL_6                                                      0xBD
#define ILI9341_BACKLIGHT_CONTROL_7                                                      0xBE
#define ILI9341_BACKLIGHT_CONTROL_8                                                      0xBF
#define ILI9341_POWER1                                                                   0xC0
#define ILI9341_POWER2                                                                   0xC1
#define ILI9341_VCOM1                                                                    0xC5
#define ILI9341_VCOM2                                                                    0xC7
#define ILI9341_POWERA                                                                   0xCB
#define ILI9341_POWERB                                                                   0xCF
#define ILI9341_READ_ID1                                                                 0xDA
#define ILI9341_READ_ID2                                                                 0xDB
#define ILI9341_READ_ID3                                                                 0xDC
#define ILI9341_PGAMMA                                                                   0xE0
#define ILI9341_NGAMMA                                                                   0xE1
#define ILI9341_DTCA                                                                     0xE8
#define ILI9341_DTCB                                                                     0xEA
#define ILI9341_POWER_SEQ                                                                0xED
#define ILI9341_3GAMMA_EN                                                                0xF2
#define ILI9341_INTERFACE                                                                0xF6
#define ILI9341_PRC                                                                      0xF7

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
/*
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
*/
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



unsigned int Read_TFT_RData(unsigned int index);
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
void Clear_Device_ILI9341(unsigned int clear_color);
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
