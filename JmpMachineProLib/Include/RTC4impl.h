//-----------------------------------------------------------------------------
//  File: RTC4impl.h                            Copyright (c) 2018 SCANLAB GmbH
//-----------------------------------------------------------------------------
//
//
//
//  Abstract
//      RTC4 function prototypes for implicitly linking - also known as
//      static load linking - to the RTC4DLL.DLL or RTC4DLLx64.DLL.
//
//  Authors
//      Gerald Schmid, Bernhard Schrems, Christian Lutz 
//
//  Revision History
//      +-----+---------+--------+------------------------------------------+
//      | Mod |  Date   | Author | Description                              |
//      +-----+---------+--------+------------------------------------------+
//      | 0.0 | 13Dec00 |  GS    | * initial release                        |
//      | 0.1 | 10Jan01 |  BS    | * standard calling                       |
//      | 0.2 | 10Jan01 |  BS    | * use of const in parameter declarations |
//      | 0.3 | 18Jun15 |  CLU   | * include guard                          |
//      | 0.4 | 15Dec15 | BS,CLU | * 64bit compatibility                    |
//      | 0.5 | 29Sep16 |  CLU   | * Company name updated                   |
//      |     |         |        |                                          |
//      +-----+---------+--------+------------------------------------------+
//
//      This file was automatically generated on Mrz 28, 2018.
//
//  NOTE
//      You need to link the (Visual C++) import library RTC4DLL.LIB or
//      RTC4DLLx64.LIB for building a Win32 or a Win64 application
//      respectively.
//
//-----------------------------------------------------------------------------

#pragma once

__declspec(dllimport) short __stdcall getmemory(unsigned short adr);
__declspec(dllimport) void __stdcall n_get_waveform(unsigned short n, unsigned short channel, unsigned short istop, signed short *memptr);
__declspec(dllimport) void __stdcall get_waveform(unsigned short channel, unsigned short istop, signed short *memptr);
__declspec(dllimport) void __stdcall n_measurement_status(unsigned short n, unsigned short *busy, unsigned short *position);
__declspec(dllimport) void __stdcall measurement_status(unsigned short *busy, unsigned short *position);
__declspec(dllimport) short __stdcall n_load_varpolydelay(unsigned short n, const char* stbfilename, unsigned short tableno);
__declspec(dllimport) short __stdcall load_varpolydelay(const char* stbfilename, unsigned short tableno);
__declspec(dllimport) short __stdcall n_load_program_file(unsigned short n, const char* name);
__declspec(dllimport) short __stdcall load_program_file(const char* name);
__declspec(dllimport) short __stdcall n_load_correction_file(unsigned short n, const char* filename, short cortable, double kx, double ky, double phi, double xoffset, double yoffset);
__declspec(dllimport) short __stdcall load_correction_file(const char* filename, short cortable, double kx, double ky, double phi, double xoffset, double yoffset);
__declspec(dllimport) short __stdcall n_load_z_table(unsigned short n, double a, double b, double c);
__declspec(dllimport) short __stdcall load_z_table(double a, double b, double c);
__declspec(dllimport) void __stdcall n_list_nop(unsigned short n);
__declspec(dllimport) void __stdcall list_nop(void);
__declspec(dllimport) void __stdcall n_set_end_of_list(unsigned short n);
__declspec(dllimport) void __stdcall set_end_of_list(void);
__declspec(dllimport) void __stdcall n_jump_abs_3d(unsigned short n, short x, short y, short z);
__declspec(dllimport) void __stdcall jump_abs_3d(short x, short y, short z);
__declspec(dllimport) void __stdcall n_jump_abs(unsigned short n, short x, short y);
__declspec(dllimport) void __stdcall jump_abs(short x, short y);
__declspec(dllimport) void __stdcall n_mark_abs_3d(unsigned short n, short x, short y, short z);
__declspec(dllimport) void __stdcall mark_abs_3d(short x, short y, short z);
__declspec(dllimport) void __stdcall n_mark_abs(unsigned short n, short x, short y);
__declspec(dllimport) void __stdcall mark_abs(short x, short y);
__declspec(dllimport) void __stdcall n_jump_rel_3d(unsigned short n, short dx, short dy, short dz);
__declspec(dllimport) void __stdcall jump_rel_3d(short dx, short dy, short dz);
__declspec(dllimport) void __stdcall n_jump_rel(unsigned short n, short dx, short dy);
__declspec(dllimport) void __stdcall jump_rel(short dx, short dy);
__declspec(dllimport) void __stdcall n_mark_rel_3d(unsigned short n, short dx, short dy, short dz);
__declspec(dllimport) void __stdcall mark_rel_3d(short dx, short dy, short dz);
__declspec(dllimport) void __stdcall n_mark_rel(unsigned short n, short dx, short dy);
__declspec(dllimport) void __stdcall mark_rel(short dx, short dy);
__declspec(dllimport) void __stdcall n_write_8bit_port_list(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_8bit_port_list(unsigned short value);
__declspec(dllimport) void __stdcall n_write_da_1_list(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_da_1_list(unsigned short value);
__declspec(dllimport) void __stdcall n_write_da_2_list(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_da_2_list(unsigned short value);
__declspec(dllimport) void __stdcall n_set_matrix_list(unsigned short n, unsigned short i, unsigned short j, double mij);
__declspec(dllimport) void __stdcall set_matrix_list(unsigned short i, unsigned short j, double mij);
__declspec(dllimport) void __stdcall n_set_defocus_list(unsigned short n, short value);
__declspec(dllimport) void __stdcall set_defocus_list(short value);
__declspec(dllimport) void __stdcall n_set_control_mode_list(unsigned short n, unsigned short mode);
__declspec(dllimport) void __stdcall set_control_mode_list(unsigned short mode);
__declspec(dllimport) void __stdcall n_set_offset_list(unsigned short n, short xoffset, short yoffset);
__declspec(dllimport) void __stdcall set_offset_list(short xoffset, short yoffset);
__declspec(dllimport) void __stdcall n_long_delay(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall long_delay(unsigned short value);
__declspec(dllimport) void __stdcall n_laser_on_list(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall laser_on_list(unsigned short value);
__declspec(dllimport) void __stdcall n_set_jump_speed(unsigned short n, double speed);
__declspec(dllimport) void __stdcall set_jump_speed(double speed);
__declspec(dllimport) void __stdcall n_set_mark_speed(unsigned short n, double speed);
__declspec(dllimport) void __stdcall set_mark_speed(double speed);
__declspec(dllimport) void __stdcall n_set_laser_delays(unsigned short n, short ondelay, short offdelay);
__declspec(dllimport) void __stdcall set_laser_delays(short ondelay, short offdelay);
__declspec(dllimport) void __stdcall n_set_scanner_delays(unsigned short n, unsigned short jumpdelay, unsigned short markdelay, unsigned short polydelay);
__declspec(dllimport) void __stdcall set_scanner_delays(unsigned short jumpdelay, unsigned short markdelay, unsigned short polydelay);
__declspec(dllimport) void __stdcall n_set_list_jump(unsigned short n, unsigned short position);
__declspec(dllimport) void __stdcall set_list_jump(unsigned short position);
__declspec(dllimport) void __stdcall n_set_input_pointer(unsigned short n, unsigned short pointer);
__declspec(dllimport) void __stdcall set_input_pointer(unsigned short pointer);
__declspec(dllimport) void __stdcall n_list_call(unsigned short n, unsigned short position);
__declspec(dllimport) void __stdcall list_call(unsigned short position);
__declspec(dllimport) void __stdcall n_list_return(unsigned short n);
__declspec(dllimport) void __stdcall list_return(void);
__declspec(dllimport) void __stdcall n_z_out_list(unsigned short n, short z);
__declspec(dllimport) void __stdcall z_out_list(short z);
__declspec(dllimport) void __stdcall n_set_standby_list(unsigned short n, unsigned short half_period, unsigned short pulse);
__declspec(dllimport) void __stdcall set_standby_list(unsigned short half_period, unsigned short pulse);
__declspec(dllimport) void __stdcall n_timed_jump_abs(unsigned short n, short x, short y, double time);
__declspec(dllimport) void __stdcall timed_jump_abs(short x, short y, double time);
__declspec(dllimport) void __stdcall n_timed_mark_abs(unsigned short n, short x, short y, double time);
__declspec(dllimport) void __stdcall timed_mark_abs(short x, short y, double time);
__declspec(dllimport) void __stdcall n_timed_jump_rel(unsigned short n, short dx, short dy, double time);
__declspec(dllimport) void __stdcall timed_jump_rel(short dx, short dy, double time);
__declspec(dllimport) void __stdcall n_timed_mark_rel(unsigned short n, short dx, short dy, double time);
__declspec(dllimport) void __stdcall timed_mark_rel(short dx, short dy, double time);
__declspec(dllimport) void __stdcall n_set_laser_timing(unsigned short n, unsigned short halfperiod, unsigned short pulse1, unsigned short pulse2, unsigned short timebase);
__declspec(dllimport) void __stdcall set_laser_timing(unsigned short halfperiod, unsigned short pulse1, unsigned short pulse2, unsigned short timebase);
__declspec(dllimport) void __stdcall n_set_wobbel_xy(unsigned short n, unsigned short long_wob, unsigned short trans_wob, double frequency);
__declspec(dllimport) void __stdcall set_wobbel_xy(unsigned short long_wob, unsigned short trans_wob, double frequency);
__declspec(dllimport) void __stdcall n_set_wobbel(unsigned short n, unsigned short amplitude, double frequency);
__declspec(dllimport) void __stdcall set_wobbel(unsigned short amplitude, double frequency);
__declspec(dllimport) void __stdcall n_set_fly_x(unsigned short n, double kx);
__declspec(dllimport) void __stdcall set_fly_x(double kx);
__declspec(dllimport) void __stdcall n_set_fly_y(unsigned short n, double ky);
__declspec(dllimport) void __stdcall set_fly_y(double ky);
__declspec(dllimport) void __stdcall n_set_fly_rot(unsigned short n, double resolution);
__declspec(dllimport) void __stdcall set_fly_rot(double resolution);
__declspec(dllimport) void __stdcall n_fly_return(unsigned short n, short x, short y);
__declspec(dllimport) void __stdcall fly_return(short x, short y);
__declspec(dllimport) void __stdcall n_calculate_fly(unsigned short n, unsigned short direction, double distance);
__declspec(dllimport) void __stdcall calculate_fly(unsigned short direction, double distance);
__declspec(dllimport) void __stdcall n_write_io_port_list(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_io_port_list(unsigned short value);
__declspec(dllimport) void __stdcall n_select_cor_table_list(unsigned short n, unsigned short heada, unsigned short headb);
__declspec(dllimport) void __stdcall select_cor_table_list(unsigned short heada, unsigned short headb);
__declspec(dllimport) void __stdcall n_set_wait(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall set_wait(unsigned short value);
__declspec(dllimport) void __stdcall n_simulate_ext_start(unsigned short n, short delay, short encoder);
__declspec(dllimport) void __stdcall simulate_ext_start(short delay, short encoder);
__declspec(dllimport) void __stdcall n_write_da_x_list(unsigned short n, unsigned short x, unsigned short value);
__declspec(dllimport) void __stdcall write_da_x_list(unsigned short x, unsigned short value);
__declspec(dllimport) void __stdcall n_set_pixel_line(unsigned short n, unsigned short pixelmode, unsigned short pixelperiod, double dx, double dy);
__declspec(dllimport) void __stdcall set_pixel_line(unsigned short pixelmode, unsigned short pixelperiod, double dx, double dy);
__declspec(dllimport) void __stdcall n_set_pixel(unsigned short n, unsigned short pulswidth, unsigned short davalue, unsigned short adchannel);
__declspec(dllimport) void __stdcall set_pixel(unsigned short pulswidth, unsigned short davalue, unsigned short adchannel);
__declspec(dllimport) void __stdcall n_set_extstartpos_list(unsigned short n, unsigned short position);
__declspec(dllimport) void __stdcall set_extstartpos_list(unsigned short position);
__declspec(dllimport) void __stdcall n_laser_signal_on_list(unsigned short n);
__declspec(dllimport) void __stdcall laser_signal_on_list(void);
__declspec(dllimport) void __stdcall n_laser_signal_off_list(unsigned short n);
__declspec(dllimport) void __stdcall laser_signal_off_list(void);
__declspec(dllimport) void __stdcall n_set_firstpulse_killer_list(unsigned short n, unsigned short fpk);
__declspec(dllimport) void __stdcall set_firstpulse_killer_list(unsigned short fpk);
__declspec(dllimport) void __stdcall n_set_io_cond_list(unsigned short n, unsigned short mask_1, unsigned short mask_0, unsigned short mask_set);
__declspec(dllimport) void __stdcall set_io_cond_list(unsigned short mask_1, unsigned short mask_0, unsigned short mask_set);
__declspec(dllimport) void __stdcall n_clear_io_cond_list(unsigned short n, unsigned short mask_1, unsigned short mask_0, unsigned short mask_clear);
__declspec(dllimport) void __stdcall clear_io_cond_list(unsigned short mask_1, unsigned short mask_0, unsigned short mask_clear);
__declspec(dllimport) void __stdcall n_list_jump_cond(unsigned short n, unsigned short mask_1, unsigned short mask_0, unsigned short position);
__declspec(dllimport) void __stdcall list_jump_cond(unsigned short mask_1, unsigned short mask_0, unsigned short position);
__declspec(dllimport) void __stdcall n_list_call_cond(unsigned short n, unsigned short mask_1, unsigned short mask_0, unsigned short position);
__declspec(dllimport) void __stdcall list_call_cond(unsigned short mask_1, unsigned short mask_0, unsigned short position);
__declspec(dllimport) void __stdcall n_save_and_restart_timer(unsigned short n);
__declspec(dllimport) void __stdcall save_and_restart_timer(void);
__declspec(dllimport) void __stdcall n_set_ext_start_delay_list(unsigned short n, short delay, short encoder);
__declspec(dllimport) void __stdcall set_ext_start_delay_list(short delay, short encoder);
__declspec(dllimport) void __stdcall n_set_trigger(unsigned short n, unsigned short sampleperiod, unsigned short channel1, unsigned short channel2);
__declspec(dllimport) void __stdcall set_trigger(unsigned short sampleperiod, unsigned short signal1, unsigned short signal2);
__declspec(dllimport) void __stdcall n_arc_rel(unsigned short n, short dx, short dy, double angle);
__declspec(dllimport) void __stdcall arc_rel(short dx, short dy, double angle);
__declspec(dllimport) void __stdcall n_arc_abs(unsigned short n, short x, short y, double angle);
__declspec(dllimport) void __stdcall arc_abs(short x, short y, double angle);
__declspec(dllimport) void __stdcall drilling(short pulsewidth, short relencoderdelay);
__declspec(dllimport) void __stdcall regulation(void);
__declspec(dllimport) void __stdcall flyline(short encoderdelay);
__declspec(dllimport) unsigned short __stdcall n_get_input_pointer(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_input_pointer(void);
__declspec(dllimport) void __stdcall select_rtc(unsigned short cardno);
__declspec(dllimport) unsigned short __stdcall rtc4_count_cards(void);
__declspec(dllimport) void __stdcall n_get_status(unsigned short n, unsigned short *busy, unsigned short *position);
__declspec(dllimport) void __stdcall get_status(unsigned short *busy, unsigned short *position);
__declspec(dllimport) unsigned short __stdcall n_read_status(unsigned short n);
__declspec(dllimport) unsigned short __stdcall read_status(void);
__declspec(dllimport) unsigned short __stdcall n_get_startstop_info(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_startstop_info(void);
__declspec(dllimport) unsigned short __stdcall n_get_marking_info(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_marking_info(void);
__declspec(dllimport) unsigned short __stdcall get_dll_version(void);
__declspec(dllimport) void __stdcall n_set_start_list_1(unsigned short n);
__declspec(dllimport) void __stdcall set_start_list_1(void);
__declspec(dllimport) void __stdcall n_set_start_list_2(unsigned short n);
__declspec(dllimport) void __stdcall set_start_list_2(void);
__declspec(dllimport) void __stdcall n_set_start_list(unsigned short n, unsigned short listno);
__declspec(dllimport) void __stdcall set_start_list(unsigned short listno);
__declspec(dllimport) void __stdcall n_execute_list_1(unsigned short n);
__declspec(dllimport) void __stdcall execute_list_1(void);
__declspec(dllimport) void __stdcall n_execute_list_2(unsigned short n);
__declspec(dllimport) void __stdcall execute_list_2(void);
__declspec(dllimport) void __stdcall n_execute_list(unsigned short n, unsigned short listno);
__declspec(dllimport) void __stdcall execute_list(unsigned short listno);
__declspec(dllimport) void __stdcall n_write_8bit_port(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_8bit_port(unsigned short value);
__declspec(dllimport) void __stdcall n_write_io_port(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_io_port(unsigned short value);
__declspec(dllimport) void __stdcall n_auto_change(unsigned short n);
__declspec(dllimport) void __stdcall auto_change(void);
__declspec(dllimport) void __stdcall n_auto_change_pos(unsigned short n, unsigned short start);
__declspec(dllimport) void __stdcall auto_change_pos(unsigned short start);
__declspec(dllimport) void __stdcall aut_change(void);
__declspec(dllimport) void __stdcall n_start_loop(unsigned short n);
__declspec(dllimport) void __stdcall start_loop(void);
__declspec(dllimport) void __stdcall n_quit_loop(unsigned short n);
__declspec(dllimport) void __stdcall quit_loop(void);
__declspec(dllimport) void __stdcall n_set_list_mode(unsigned short n, unsigned short mode);
__declspec(dllimport) void __stdcall set_list_mode(unsigned short mode);
__declspec(dllimport) void __stdcall n_stop_execution(unsigned short n);
__declspec(dllimport) void __stdcall stop_execution(void);
__declspec(dllimport) unsigned short __stdcall n_read_io_port(unsigned short n);
__declspec(dllimport) unsigned short __stdcall read_io_port(void);
__declspec(dllimport) void __stdcall n_write_da_1(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_da_1(unsigned short value);
__declspec(dllimport) void __stdcall n_write_da_2(unsigned short n, unsigned short value);
__declspec(dllimport) void __stdcall write_da_2(unsigned short value);
__declspec(dllimport) void __stdcall n_set_max_counts(unsigned short n, long counts);
__declspec(dllimport) void __stdcall set_max_counts(long counts);
__declspec(dllimport) long __stdcall n_get_counts(unsigned short n);
__declspec(dllimport) long __stdcall get_counts(void);
__declspec(dllimport) void __stdcall n_set_matrix(unsigned short n, double m11, double m12, double m21, double m22);
__declspec(dllimport) void __stdcall set_matrix(double m11, double m12, double m21, double m22);
__declspec(dllimport) void __stdcall n_set_offset(unsigned short n, short xoffset, short yoffset);
__declspec(dllimport) void __stdcall set_offset(short xoffset, short yoffset);
__declspec(dllimport) void __stdcall n_goto_xyz(unsigned short n, short x, short y, short z);
__declspec(dllimport) void __stdcall goto_xyz(short x, short y, short z);
__declspec(dllimport) void __stdcall n_goto_xy(unsigned short n, short x, short y);
__declspec(dllimport) void __stdcall goto_xy(short x, short y);
__declspec(dllimport) unsigned short __stdcall n_get_hex_version(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_hex_version(void);
__declspec(dllimport) void __stdcall n_disable_laser(unsigned short n);
__declspec(dllimport) void __stdcall disable_laser(void);
__declspec(dllimport) void __stdcall n_enable_laser(unsigned short n);
__declspec(dllimport) void __stdcall enable_laser(void);
__declspec(dllimport) void __stdcall n_stop_list(unsigned short n);
__declspec(dllimport) void __stdcall stop_list(void);
__declspec(dllimport) void __stdcall n_restart_list(unsigned short n);
__declspec(dllimport) void __stdcall restart_list(void);
__declspec(dllimport) void __stdcall n_get_xyz_pos(unsigned short n, short *x, short *y, short *z);
__declspec(dllimport) void __stdcall get_xyz_pos(short *x, short *y, short *z);
__declspec(dllimport) void __stdcall n_get_xy_pos(unsigned short n, short *x, short *y);
__declspec(dllimport) void __stdcall get_xy_pos(short *x, short *y);
__declspec(dllimport) void __stdcall n_select_list(unsigned short n, unsigned short list_2);
__declspec(dllimport) void __stdcall select_list(unsigned short list_2);
__declspec(dllimport) void __stdcall n_z_out(unsigned short n, short z);
__declspec(dllimport) void __stdcall z_out(short z);
__declspec(dllimport) void __stdcall n_set_firstpulse_killer(unsigned short n, unsigned short fpk);
__declspec(dllimport) void __stdcall set_firstpulse_killer(unsigned short fpk);
__declspec(dllimport) void __stdcall n_set_standby(unsigned short n, unsigned short half_period, unsigned short pulse);
__declspec(dllimport) void __stdcall set_standby(unsigned short half_period, unsigned short pulse);
__declspec(dllimport) void __stdcall n_laser_signal_on(unsigned short n);
__declspec(dllimport) void __stdcall laser_signal_on(void);
__declspec(dllimport) void __stdcall n_laser_signal_off(unsigned short n);
__declspec(dllimport) void __stdcall laser_signal_off(void);
__declspec(dllimport) void __stdcall n_set_delay_mode(unsigned short n, unsigned short varpoly, unsigned short directmove3d, unsigned short edgelevel, unsigned short minjumpdelay, unsigned short jumplengthlimit);
__declspec(dllimport) void __stdcall set_delay_mode(unsigned short varpoly, unsigned short directmove3d, unsigned short edgelevel, unsigned short minjumpdelay, unsigned short jumplengthlimit);
__declspec(dllimport) void __stdcall n_set_piso_control(unsigned short n, unsigned short l1, unsigned short l2);
__declspec(dllimport) void __stdcall set_piso_control(unsigned short l1, unsigned short l2);
__declspec(dllimport) void __stdcall n_select_status(unsigned short n, unsigned short mode);
__declspec(dllimport) void __stdcall select_status(unsigned short mode);
__declspec(dllimport) void __stdcall n_get_encoder(unsigned short n, short *zx, short *zy);
__declspec(dllimport) void __stdcall get_encoder(short *zx, short *zy);
__declspec(dllimport) void __stdcall n_select_cor_table(unsigned short n, unsigned short heada, unsigned short headb);
__declspec(dllimport) void __stdcall select_cor_table(unsigned short heada, unsigned short headb);
__declspec(dllimport) void __stdcall n_execute_at_pointer(unsigned short n, unsigned short position);
__declspec(dllimport) void __stdcall execute_at_pointer(unsigned short position);
__declspec(dllimport) unsigned short __stdcall n_get_head_status(unsigned short n, unsigned short head);
__declspec(dllimport) unsigned short __stdcall get_head_status(unsigned short head);
__declspec(dllimport) void __stdcall n_simulate_encoder(unsigned short n, unsigned short channel);
__declspec(dllimport) void __stdcall simulate_encoder(unsigned short channel);
__declspec(dllimport) void __stdcall n_set_hi(unsigned short n, double galvogainx, double galvogainy, short galvooffsetx, short galvooffsety, short head);
__declspec(dllimport) void __stdcall set_hi(double galvogainx, double galvogainy, short galvooffsetx, short galvooffsety, short head);
__declspec(dllimport) void __stdcall n_release_wait(unsigned short n);
__declspec(dllimport) void __stdcall release_wait(void);
__declspec(dllimport) unsigned short __stdcall n_get_wait_status(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_wait_status(void);
__declspec(dllimport) void __stdcall n_set_control_mode(unsigned short n, unsigned short mode);
__declspec(dllimport) void __stdcall set_control_mode(unsigned short mode);
__declspec(dllimport) void __stdcall n_set_laser_mode(unsigned short n, unsigned short mode);
__declspec(dllimport) void __stdcall set_laser_mode(unsigned short mode);
__declspec(dllimport) void __stdcall n_set_ext_start_delay(unsigned short n, short delay, short encoder);
__declspec(dllimport) void __stdcall set_ext_start_delay(short delay, short encoder);
__declspec(dllimport) void __stdcall n_home_position(unsigned short n, short xhome, short yhome);
__declspec(dllimport) void __stdcall home_position(short xhome, short yhome);
__declspec(dllimport) void __stdcall n_set_rot_center(unsigned short n, long center_x, long center_y);
__declspec(dllimport) void __stdcall set_rot_center(long center_x, long center_y);
__declspec(dllimport) void __stdcall n_dsp_start(unsigned short n);
__declspec(dllimport) void __stdcall dsp_start(void);
__declspec(dllimport) void __stdcall n_write_da_x(unsigned short n, unsigned short x, unsigned short value);
__declspec(dllimport) void __stdcall write_da_x(unsigned short x, unsigned short value);
__declspec(dllimport) unsigned short __stdcall n_read_ad_x(unsigned short n, unsigned short x);
__declspec(dllimport) unsigned short __stdcall read_ad_x(unsigned short x);
__declspec(dllimport) unsigned short __stdcall n_read_pixel_ad(unsigned short n, unsigned short pos);
__declspec(dllimport) unsigned short __stdcall read_pixel_ad(unsigned short pos);
__declspec(dllimport) short __stdcall n_get_z_distance(unsigned short n, short x, short y, short z);
__declspec(dllimport) short __stdcall get_z_distance(short x, short y, short z);
__declspec(dllimport) unsigned short __stdcall n_get_io_status(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_io_status(void);
__declspec(dllimport) double __stdcall n_get_time(unsigned short n);
__declspec(dllimport) double __stdcall get_time(void);
__declspec(dllimport) void __stdcall n_set_defocus(unsigned short n, short value);
__declspec(dllimport) void __stdcall set_defocus(short value);
__declspec(dllimport) void __stdcall n_set_softstart_mode(unsigned short n, unsigned short mode, unsigned short number, unsigned short restartdelay);
__declspec(dllimport) void __stdcall set_softstart_mode(unsigned short mode, unsigned short number, unsigned short resetdelay);
__declspec(dllimport) void __stdcall n_set_softstart_level(unsigned short n, unsigned short index, unsigned short level);
__declspec(dllimport) void __stdcall set_softstart_level(unsigned short index, unsigned short level);
__declspec(dllimport) void __stdcall n_control_command(unsigned short n, unsigned short head, unsigned short axis, unsigned short data);
__declspec(dllimport) void __stdcall control_command(unsigned short head, unsigned short axis, unsigned short data);
__declspec(dllimport) short __stdcall load_cor(const char* filename);
__declspec(dllimport) short __stdcall load_pro(const char* filename);
__declspec(dllimport) unsigned short __stdcall n_get_serial_number(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_serial_number(void);
__declspec(dllimport) long __stdcall n_get_serial_number_32(unsigned short n);
__declspec(dllimport) long __stdcall get_serial_number_32(void);
__declspec(dllimport) unsigned short __stdcall n_get_rtc_version(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_rtc_version(void);
__declspec(dllimport) void __stdcall get_hi_data(unsigned short *x1, unsigned short *x2, unsigned short *y1, unsigned short *y2);
__declspec(dllimport) short __stdcall n_auto_cal(unsigned short n, unsigned short head, unsigned short command);
__declspec(dllimport) short __stdcall auto_cal(unsigned short head, unsigned short command);
__declspec(dllimport) unsigned short __stdcall n_get_list_space(unsigned short n);
__declspec(dllimport) unsigned short __stdcall get_list_space(void);
__declspec(dllimport) short __stdcall teachin(const char* filename, short xin, short yin, short zin, double ll0, short *xout, short *yout, short *zout);
__declspec(dllimport) short __stdcall n_get_value(unsigned short n, unsigned short signal);
__declspec(dllimport) short __stdcall get_value(unsigned short signal);
__declspec(dllimport) void __stdcall n_set_io_bit(unsigned short n, unsigned short mask1);
__declspec(dllimport) void __stdcall set_io_bit(unsigned short mask1);
__declspec(dllimport) void __stdcall n_clear_io_bit(unsigned short n, unsigned short mask0);
__declspec(dllimport) void __stdcall clear_io_bit(unsigned short mask0);
__declspec(dllimport) void __stdcall set_duty_cycle_table(unsigned short index, unsigned short dutycycle);
__declspec(dllimport) void __stdcall n_move_to(unsigned short n, unsigned short position);
__declspec(dllimport) void __stdcall move_to(unsigned short position);
