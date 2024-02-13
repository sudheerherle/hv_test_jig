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

void Draw_Main_Window()
{
	//Reset_Window_Add();
	Clear_Device_ST7789(BLUE);
	Draw_Window(XL_MARGIN,YU_MARGIN,XR_MARGIN,YL_MARGIN,GRAY6,GRAY0);
}

void Display_Key_Menu()
{
	Clear_Window(XL_MARGIN + 5,(TFT_SIZE_Y - 17),TFT_SIZE_X - 10,13,WHITE);
	Draw_Window(XL_MARGIN,YL_MARGIN - 21,XR_MARGIN,YL_MARGIN,GRAY6,GRAY0);
	Write_Text(5,(TFT_SIZE_Y - 17),"UP",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	Write_Center_Text(0,TFT_SIZE_X,(TFT_SIZE_Y - 17),"DOWN",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	Write_Right_Text(XR_MARGIN - 5,(TFT_SIZE_Y - 17),"SELECT",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
}

unsigned char Key_Type_Press_Event(unsigned int key_timeout_value)
{
	unsigned char return_value;
	key_timeout_ctr = key_timeout_value;
	key_timeout_flag = true;
	key_press_flag = false;
	while (true)
	{
		if (key_press_flag == true)
		{
			key_timeout_ctr = key_timeout_value;
			key_press_flag = false;
			if (first_key_flag == true)
			{
				return_value = 1;
				break;
			}
			if (second_key_flag == true)
			{
				return_value = 2;
				break;
			}
			if (third_key_flag == true)
			{
				return_value = 3;
				break;
			}
		}
		else if (key_timeout_flag == false)
		{
			return_value = 0;
			break;
		}
		//check for the serial data from the PC
		Chk_Serial_Data();
		if (shift_machine_state == READ_TEST_STATE)
		{
			//send data to pc
			shift_machine_state = IDLE_TEST_STATE;
			if (cur_machine_state == HV_TEST_STATE)
			{
				Send_Current_HVData();
			}
			else if (cur_machine_state == FIL_TEST_STATE)
			{
				Send_Current_FILData();
			}
		}
		if (shift_machine_state == DATA_TEST_STATE)
		{
			//send data to pc
			shift_machine_state = IDLE_TEST_STATE;
			Send_Saved_Data();
		}
		if (shift_machine_state == START_TEST_STATE)
		{
			shift_machine_state = IDLE_TEST_STATE;
			Start_New_Test();
			return_value = 4;
			break;
		}
	}
	return(return_value);
}

unsigned long ReadN_Average_ADC(unsigned char channel,unsigned long avg,unsigned long base_value)
{
	unsigned int cur_adc_value;
	unsigned long cur_adc_total;
	unsigned long sample_no;
	unsigned long usl;
	sample_no = avg;
	cur_adc_total = 0;
	for (usl=0;usl<sample_no;usl++)
	{
		cur_adc_value = ReadADC(channel);
		cur_adc_total = cur_adc_total + (unsigned long)cur_adc_value;
	}
	//sample_no--;
	//if (sample_no == 0)
	//{
		cur_adc_total = cur_adc_total / avg;
		cur_adc_total = ((cur_adc_total * base_value) / (unsigned long)MAX_ADC_REF_CONST);
	//}
	//cur_adc_total = (((unsigned long)cur_adc_value * base_value) / (unsigned long)MAX_ADC_REF_CONST);
	return (cur_adc_total);
}

void Conv_Data2_BCD(unsigned long temp_hex_loc,unsigned char * temp_chr_ptr)
{
	unsigned char usc;
	unsigned long temp_value;
	temp_value = 1000000000;
	for (usc=0;usc<10;usc++)
	{
		temp_chr_ptr[usc] = temp_hex_loc / temp_value;
		temp_hex_loc = temp_hex_loc % temp_value;
		temp_value = temp_value / (unsigned long)10;
	}
}

unsigned char Conv_Data2_Ascii(unsigned long temp_hex_loc,unsigned char * temp_chr_ptr)
{
	unsigned char usc;
	unsigned char data_found_flag;
	unsigned char total_data_ctr;
	unsigned char temp_data_loc[11];
	total_data_ctr = 0;
	data_found_flag = false;
	Conv_Data2_BCD(temp_hex_loc,&temp_data_loc[0]);
	for (usc=0;usc<9;usc++)
	{
		if ((temp_data_loc[usc] == 0) && (data_found_flag == false))
		{
			//temp_ptr_loc[usc] = 32;
		}
		else
		{
			//temp_chr_ptr[usc] = temp_data_loc[usc] + 48;
			temp_chr_ptr[total_data_ctr] = temp_data_loc[usc] + 48;
			data_found_flag = true;
			total_data_ctr++;
		}
	}
	temp_chr_ptr[total_data_ctr] = temp_data_loc[usc] + 48;
	total_data_ctr++;
	temp_chr_ptr[total_data_ctr] = 0;
	total_data_ctr++;
	return (total_data_ctr);
}

unsigned char Conv_Data2_AsciiDecimal(unsigned long temp_hex_loc,unsigned char * temp_chr_ptr)
{
	unsigned char usc;
	unsigned char data_found_flag;
	unsigned char total_data_ctr;
	unsigned char temp_data_loc[11];
	total_data_ctr = 0;
	data_found_flag = false;
	Conv_Data2_BCD(temp_hex_loc,&temp_data_loc[0]);
	for (usc=0;usc<7;usc++)
	{
		if ((temp_data_loc[usc] == 0) && (data_found_flag == false))
		{
			//temp_ptr_loc[usc] = 32;
		}
		else
		{
			temp_chr_ptr[total_data_ctr] = temp_data_loc[usc] + 48;
			data_found_flag = true;
			total_data_ctr++;
		}
	}
	temp_chr_ptr[total_data_ctr] = temp_data_loc[usc] + 48;
	total_data_ctr++;
	temp_chr_ptr[total_data_ctr] = '.';
	total_data_ctr++;
	temp_chr_ptr[total_data_ctr] = temp_data_loc[usc+1] + 48;
	total_data_ctr++;
	temp_chr_ptr[total_data_ctr] = temp_data_loc[usc+2] + 48;
	total_data_ctr++;
	temp_chr_ptr[total_data_ctr] = 0;
	total_data_ctr++;
	return (total_data_ctr);
}

void Delay_Half_Seconds(unsigned int delayhalf_sec_ctr)
{
	key_timeout_ctr = delayhalf_sec_ctr;
	key_timeout_flag = true;
	while (key_timeout_flag == true)
	{
	}
}

void Display_HVTKey_Menu(unsigned char cur_flag)
{
	if (cur_flag == 0)
	{
		Clear_Window(XL_MARGIN + 5,(TFT_SIZE_Y - 17),TFT_SIZE_X - 10,13,WHITE);
		Draw_Window(XL_MARGIN,YL_MARGIN - 21,XR_MARGIN,YL_MARGIN,GRAY6,GRAY0);
		Write_Text(5,(TFT_SIZE_Y - 17),"CONTINUE",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
		Write_Center_Text(0,TFT_SIZE_X,(TFT_SIZE_Y - 17),"SKIP",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
		Write_Right_Text(XR_MARGIN - 5,(TFT_SIZE_Y - 17),"SAVE",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
	}
	if (cur_flag == 1)
	{
		Write_Text(5,(TFT_SIZE_Y - 17),"CONTINUE",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	}
	if (cur_flag == 2)
	{
		Write_Right_Text(XR_MARGIN - 5,(TFT_SIZE_Y - 17),"SAVE",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	}
	if (cur_flag == 3)
	{
		Write_Text(5,(TFT_SIZE_Y - 17),"CONTINUE",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
	}
}

void Write2_DAC_Register(unsigned int required_dac_value)
{
	unsigned char data_address;
	unsigned char dac_data[3];
	data_address = required_dac_value >> 8;
	dac_data[0] = required_dac_value;
	dac_data[1] = data_address;
	dac_data[2] = required_dac_value;
	gsc_i2c_retsts = Write2i2c(0xc0,0x01,data_address,(unsigned char *)&dac_data[0],1);
}

void Display_Searching_Message()
{
	Clear_Window(XL_MARGIN + 5,(TFT_SIZE_Y - 17),TFT_SIZE_X - 10,13,WHITE);
	Draw_Window(XL_MARGIN,YL_MARGIN - 21,XR_MARGIN,YL_MARGIN,GRAY6,GRAY0);
	Write_Text(5,(TFT_SIZE_Y - 17),"SEARCHING",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
}

signed char Wait4_User_Confirmation(unsigned char nextbut)
{
	signed char result;
	unsigned char key_type_value;
	Clear_Window(XL_MARGIN + 5,(TFT_SIZE_Y - 17),TFT_SIZE_X - 10,13,WHITE);
	Draw_Window(XL_MARGIN,YL_MARGIN - 21,XR_MARGIN,YL_MARGIN,GRAY6,GRAY0);
	//Write_Text(5,(TFT_SIZE_Y - 17),"SEND",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	Write_Text(5,(TFT_SIZE_Y - 17),"SEND",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
	Write_Center_Text(0,TFT_SIZE_X,(TFT_SIZE_Y - 17),"EXIT",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	if (nextbut == true)
	{
		Write_Right_Text(XR_MARGIN - 5,(TFT_SIZE_Y - 17),"NEXT",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTGREEN,WHITE,TFT_MODE_FULL);
	}
	while (true)
	{
		key_type_value = Key_Type_Press_Event(KEY_TIMEOUT_CONST);
		if (key_type_value == 1)
		{
			//Printing
			result = 1;
			break;
		}
		else if ((key_type_value == 2) || (key_type_value == 0))
		{
			//exit
			result = -1;
			break;
		}
		else if (key_type_value == 3)
		{
			//next page
			result = 0;
			if (nextbut == false)
			{
				result = -1;
			}
			break;
		}
	}
	return (result);
}

unsigned int Check_Range(unsigned long actual_value,unsigned long lower_range_value,unsigned long higher_range_value)
{
	unsigned int return_value;
	return_value = BRIGHTRED;
	if ((actual_value > lower_range_value) && (actual_value < higher_range_value))
	{
		return_value = BRIGHTGREEN;
	}
	return_value =  BLACK;
	return (return_value);
}

void Display_Updated_Message(unsigned long test_number,unsigned char continue_flag)
{
	unsigned char nob;
	unsigned int color;
	unsigned int color1;
	unsigned int color2;
	Clear_Window(10,YL_MARGIN - 96,XR_MARGIN - 20,76,WHITE);
	Write_Text(10,YL_MARGIN - 96,"TEST NUMBER",(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	nob = Conv_Data2_Ascii(test_number,&temp_disp_array[0]);
	Write_Right_Text(XR_MARGIN - 10,YL_MARGIN - 96,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold12SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
	Write_Right_Text(XR_MARGIN - 5,(TFT_SIZE_Y - 17),"SAVE",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
	if (continue_flag == true)
	{
		Delay_Seconds_With_Key(60);
	}
}

void Display_Filament_Interchange()
{
	unsigned char nob;
	Clear_Window(10,YL_MARGIN - 91,XR_MARGIN - 20,70,WHITE);
	Draw_Window(10,YL_MARGIN - 91,XR_MARGIN - 10,YL_MARGIN - 25,GRAY6,GRAY0);
	Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 67,&filnot_connected_text[0],(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTRED,WHITE,TFT_MODE_FULL);
	Delay_Seconds_With_Key(6000);
}

void Txr_Array_Data(unsigned char * to_array_ptr,unsigned char * from_array_ptr,unsigned char nob)
{
	unsigned char usc;
	for (usc=0;usc<nob;usc++)
	{
		to_array_ptr[usc] = from_array_ptr[usc];
	}
}

void Delay_Seconds_With_Key(unsigned int delayhalf_sec_ctr)
{
	unsigned char key_type_value;
	key_type_value = Key_Type_Press_Event(delayhalf_sec_ctr);
}



