#ifndef _LUNARCALENDAR_H_
#define _LUNARCALENDAR_H_

//!struct { 
//!      int1 s; // c0 
//!      int1 n; // c1 
//!      int1 e; // c2 
//!      int1 w; // c3 
//!      int1 trigger; // c4 
//!      int1 bottom; // c5 
//!      int1 top; // c6 
//!      int1 comp_off; // c7 
//!   } bits;
//!struct{
//!	int8 N_AL_DT_DL :5;
//!	int8 T_AL_DT_DL :4;
//!	int8 SN_CT_AL :1;
//!	int8 TN_B_THT :1;
//!	int8 SN_CT_DL :2;
//!}MONTH_INFO;
//!
//!struct bfield { 
//!   int8 status_bat       : 5;   //unit has low battery 
//!   int32 status_dead_bat  : 4;   //unit has dead battery, will be shutting down 
//!   int8 status_tampered  : 1;   //unit has been tampered with 
//!   int8 status_bad_comms : 1;   //some failed packets or bad acks 
//!   int8 status_off_air   : 1;   //unit is not communicating 
//!}; 
//!
//!struct MONTH_INFO
//!{
//!	int8 ngay_am_lich_dau_thag_duog_lich :8;
//!	int8 thag_am_lich_dau_thag_duog_lich :4;
//!	int8 so_ngay_cua_thag_am_lich_dau_thag_duog_lich :1;
//!	int8 thag_nhuan_bang_thag_hien_tai :1;
//!	int8 so_ngay_cua_thag_duog_lich :2;
//!};

//!union LUNAR_RECORD
//!{
//!	unsigned int Word; 
//!	struct MONTH_INFO Info;
//!};
int8 ngay_am_lich_dau_thag_duog_lich;// 5 bit :8;
int8 thag_am_lich_dau_thag_duog_lich;// 4 bit :4;
int8 so_ngay_cua_thag_am_lich_dau_thag_duog_lich; // 1 bit :1;
int8 thag_nhuan_bang_thag_hien_tai; // 1 bit  :1;
int8 so_ngay_cua_thag_duog_lich; // 2 bit :2;
//!int16 T_AL_DT_DL_dat=4;
//!int16 SN_CT_AL_dat=1;
//!int16 TN_B_THT_dat=1;
//!int16 SN_CT_DL_dat=2;
//int16 Word_dat;

void Solar2Lunar(unsigned char SolarDay, unsigned char SolarMonth, unsigned int SolarYear,
	unsigned char * LunarDay, unsigned char * LunarMonth, unsigned int * LunarYear);

int8 ngay_am,thang_am,nam_am;

#endif
