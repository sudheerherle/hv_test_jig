unsigned char Send_CCTalk_Data(struct general_cctalk_data * cctalk_info_buffer,unsigned char cctalk_header,unsigned char reply_flag, unsigned int time_value);
unsigned char Wait4_UART0_Data();
void Calculate_CCTalkData_Chksum(struct general_cctalk_data * cctalk_info_buffer);
unsigned char Compare_CCTalkData_Chksum();
unsigned int Calculate_CCITT_CRC(unsigned far char * data_buffer_ptr,unsigned int nob);
