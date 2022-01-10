/* 
 * File:   main.h
 * Author: ALI
 *
 * Created on 14 December, 2021, 6:44 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define COOKING_MODE_DISPLAY   0X01
#define MICRO_MODE             0X02
#define TIME_DISPLAY           0x03
#define MICRO_MODE_RESET       0x10
#define RESET_NOTHING          0X0F

#define FAN                     RC2
#define FAN_DDR                 TRISC2
void power_on_screen(void);
void cooking_mode_display(void);
void clear_clcd(void);
void clcd_write(unsigned char byte, unsigned char mode);
void set_time(unsigned char key);
#endif	/* MAIN_H */

