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

void Show_All_Transactions()
{
	unsigned int cur_e2_add;
	unsigned int noof_trans;
	signed char result;
	unsigned int curx,cury;
	unsigned char nob;
	unsigned int color;
	Draw_Main_Window();
	Write_Text(5,8,"TEST NUMBER:",(unsigned far char *)&arial_narrow_bold14[0],BRIGHTRED,WHITE,TFT_MODE_FULL);
	//Write_Text_Bar(4,36,5,271,&hv_input_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text_Bar(4,36,5,271,&hv_input_text[0],(unsigned far char *)&arial_narrow_bold14[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
	//Write_Text_Bar(272,36,272,TFT_SIZE_X-4,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text_Bar(271,36,272,TFT_SIZE_X-4,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,59,&anode_hv_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(190,59,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,59,&vdc_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,82,&cathode_hv_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(190,82,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,82,&vdc_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,105,&bleeder_anode_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(190,105,&plus_symbol[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,105,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,128,&bleeder_cathode_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(190,128,&minus_symbol[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,128,&mvdc_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	//Write_Text_Bar(4,151,5,271,&filament_input_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text_Bar(4,151,5,271,&filament_input_text[0],(unsigned far char *)&arial_narrow_bold14[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
	//Write_Text_Bar(272,151,272,TFT_SIZE_X-4,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text_Bar(271,151,272,TFT_SIZE_X-4,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
	Write_Text(5,174,&large_filament_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,174,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(5,197,&small_filament_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Write_Text(272,197,&vac_text[0],(unsigned far char *)&arial_narrow_bold14[0],BLACK,WHITE,TFT_MODE_FULL);
	Display_Searching_Message();
	noof_trans = 1;
	cur_e2_add = pointer_info_buffer.cur_data_pointer;
	cur_machine_state = PRINT_TEST_STATE;
	while (true)
	{
		if (noof_trans > TOTAL_TRANS_CONST)
		{
			result = Wait4_User_Confirmation(false);
			break;
		}
		if (cur_e2_add == E2EVENTSTART_ADD_CONST)
		{
			cur_e2_add = E2LASTEVENT_ADD_CONST;
		}
		else
		{
			cur_e2_add = cur_e2_add - TRAN_DATA_BYTES;
		}
		gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,cur_e2_add,(unsigned char *)&test_data_buffer.test_number,TRAN_DATA_BYTES);
		if (test_data_buffer.test_number > 0)
		{
			nob = Conv_Data2_Ascii(test_data_buffer.test_number,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,12,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTRED,WHITE,TFT_MODE_FULL);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_signal_input,&temp_disp_array[0]);
			//Write_Right_Text_Bar(270,40,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
			Write_Right_Text_Bar(270,40,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.hv_anode_volt,HVT_LO_CONST,HVT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,63,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.hv_cathode_volt,HVT_LO_CONST,HVT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.hv_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,86,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.bleeder_anode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_anode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,109,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.bleeder_cathode_volt,BLEEDER_LO_CONST,BLEEDER_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.bleeder_cathode_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,132,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.filament_signal_input,&temp_disp_array[0]);
			//Write_Right_Text_Bar(270,155,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],BLACK,BRIGHTYELLOW,TFT_MODE_FULL);
			Write_Right_Text_Bar(270,155,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],LIGHTBLUE,BRIGHTYELLOW,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.short_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.short_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,178,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			color = BLACK;//Check_Range(test_data_buffer.long_filament_volt,FILAMENT_LO_CONST,FILAMENT_HI_CONST);
			nob = Conv_Data2_AsciiDecimal(test_data_buffer.long_filament_volt,&temp_disp_array[0]);
			Write_Right_Text_Bar(270,201,200,&temp_disp_array[0],(unsigned far char *)&arial_narrow_bold16SI[0],color,WHITE,TFT_MODE_FULL);
			result = Wait4_User_Confirmation(true);
			if (result == -1)
			{
				break;
			}
			else if (result == 1)
			{
				//Send_Test_Report();
				break;
			}
			Display_Searching_Message();
		}
		else
		{
			result = Wait4_User_Confirmation(false);
			/*if (result == 1)
			{
				Write_Text(5,(TFT_SIZE_Y - 17),"SEND",(unsigned far char *)&arial_narrow_bold12SI[0],GRAY3,WHITE,TFT_MODE_FULL);
				Send_Test_Report();
			}*/
			break;
		}
		noof_trans++;
	}
}

/*void Send_Test_Report()
{
	unsigned char return_value;
	unsigned int temp_value;
	temp_value = 0;
	while (true)
	{
		if (pointer_info_buffer.cur_data_pointer == pointer_info_buffer.last_download_pointer)
		{
			if (temp_value == 0)
			{
				Clear_Window(10,YL_MARGIN - 91,XR_MARGIN - 20,70,WHITE);
				Draw_Window(10,YL_MARGIN - 91,XR_MARGIN - 10,YL_MARGIN - 25,GRAY6,GRAY0);
				//Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 86,"Please connect the unit",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 67,"No data to download.",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				//Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 48,"the application.",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				Delay_Seconds_With_Key(20);
			}
			else
			{
				Clear_Window(10,YL_MARGIN - 91,XR_MARGIN - 20,70,WHITE);
				Draw_Window(10,YL_MARGIN - 91,XR_MARGIN - 10,YL_MARGIN - 25,GRAY6,GRAY0);
				//Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 86,"Please connect the unit",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 67,"Download Complete.",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				//Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 48,"the application.",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
				Delay_Seconds_With_Key(20);
			}
			break;
		}
		else
		{
			gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,pointer_info_buffer.last_download_pointer,(unsigned char *)&pc_data_buffer.pc_test_data.test_number,TRAN_DATA_BYTES);
		}
		pc_data_buffer.nob = 40;
		pc_data_buffer.unit_address = hvt_slno_loc;
		pc_data_buffer.data_address = pointer_info_buffer.last_download_pointer;
		return_value = Send_CCTalk_Data((struct cctalk_data *)&pc_data_buffer.dest_addr,DATA_DOWNLOAD_HEADER,true,MAXPC_TIMEOUT_CONST);
		if (return_value == true)
		{
			//update the last download ptr to next
			if (pc_recd_buffer.data_address == pointer_info_buffer.last_download_pointer)
			{
				pointer_info_buffer.last_download_pointer = pointer_info_buffer.last_download_pointer + TRAN_DATA_BYTES;
				if (pointer_info_buffer.last_download_pointer >= E2EVENTEND_ADD_CONST)
				{
					pointer_info_buffer.last_download_pointer = E2EVENTSTART_ADD_CONST;
				}
			}
			gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,POINTERINFO_ADD_CONST,(unsigned char *)&pointer_info_buffer.total_bytes,POINTER_INFO_BYTES);
			temp_value++;
		}
		else
		{
			//display error message
			Clear_Window(10,YL_MARGIN - 91,XR_MARGIN - 20,70,WHITE);
			Draw_Window(10,YL_MARGIN - 91,XR_MARGIN - 10,YL_MARGIN - 25,GRAY6,GRAY0);
			Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 86,"Please connect the unit",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
			Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 67,"to pc and load",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
			Write_Center_Text(0,TFT_SIZE_X,YL_MARGIN - 48,"the application.",(unsigned far char *)&arial_narrow_bold16SI[0],BRIGHTBLUE,WHITE,TFT_MODE_FULL);
			//Delay_Half_Seconds(10);
			Delay_Seconds_With_Key(20);
			break;
		}
	}
}*/

void Send_Saved_Data()
{
	struct cctalk_data pc_data_buffer;
	unsigned char return_value;
	unsigned int temp_value;
	temp_value = 0;
	while (true)
	{
		if (pointer_info_buffer.cur_data_pointer == pointer_info_buffer.last_download_pointer)
		{
			break;
		}
		else
		{
			gsc_i2c_retsts = ReadFromi2c(E2_ADD_CONST,0x02,pointer_info_buffer.last_download_pointer,(unsigned char *)&pc_data_buffer.pc_test_data.test_number,TRAN_DATA_BYTES);
		}
		pc_data_buffer.nob = 36;
		//pc_data_buffer.unit_address = hvt_slno_loc;
		pc_data_buffer.data_address = pointer_info_buffer.last_download_pointer;
		return_value = Send_CCTalk_Data((struct general_cctalk_data *)&pc_data_buffer.dest_addr,DATA_DOWNLOAD_HEADER,true,MAXPC_TIMEOUT_CONST);
		if (return_value == true)
		{
			//update the last download ptr to next
			if (pc_recd_buffer.data_address == pointer_info_buffer.last_download_pointer)
			{
				pointer_info_buffer.last_download_pointer = pointer_info_buffer.last_download_pointer + TRAN_DATA_BYTES;
				if (pointer_info_buffer.last_download_pointer >= E2EVENTEND_ADD_CONST)
				{
					pointer_info_buffer.last_download_pointer = E2EVENTSTART_ADD_CONST;
				}
			}
			gsc_i2c_retsts = Write2i2c(E2_ADD_CONST,0x02,POINTERINFO_ADD_CONST,(unsigned char *)&pointer_info_buffer.total_bytes,POINTER_INFO_BYTES);
			temp_value++;
		}
	}
}

