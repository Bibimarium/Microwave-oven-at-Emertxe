/*
 * File:   main.c
 * Author: ALI
 *
 * Created on 14 December, 2021, 6:19 PM
 */


#include <xc.h>
#include "clcd.h"
#include "main.h"
#include "matrix_keypad.h"
#include "timers.h"

#pragma config WDTE=OFF

int sec,min;
unsigned char operation_mode,reset_flag;
static void init_config(void) {
    init_clcd();
    
    //initilization of MKP module
    init_matrix_keypad();
    
    init_timer2(); //initilizing timer 2
     
    PEIE=1;
    GIE=1;
    
    FAN_DDR=0;  //FAN pin setting as output
    FAN=0;  //FAN is kept off
}

void main(void) {
    unsigned char key;
    
    init_config();
    power_on_screen();
    clear_clcd();
    operation_mode=COOKING_MODE_DISPLAY;
    while (1) {
        key=read_matrix_keypad(STATE);  //0,1,2...*
        if(operation_mode == MICRO_MODE)
        {
            ;
        }
        else if(key==1)
        {
            operation_mode=MICRO_MODE;
            reset_flag=MICRO_MODE_RESET;
            clear_clcd();
            clcd_print(" Power = 900W ",LINE2(0));
            __delay_ms(3000);
            clear_clcd();
        }
        switch(operation_mode)
        {
            case COOKING_MODE_DISPLAY:
              cooking_mode_display();
            break;
            case MICRO_MODE:
               set_time(key); 
                break;
            case TIME_DISPLAY:
                time_display_screen();
                break;  
        }
        reset_flag=RESET_NOTHING;
              
    }

}
void clear_clcd(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(100);
}
void power_on_screen(void)
{
    for(int i=0;i<16;i++)
    {
      clcd_putch(BLOCK,LINE1(i));
    }
    clcd_print("  Powering ON   ",LINE2(0));
    clcd_print(" Microwave Oven ",LINE3(0));
    for(int i=0;i<16;i++)
    {
      clcd_putch(BLOCK,LINE4(i));
    }
    //3sec delay
    __delay_ms(3000);
}
void cooking_mode_display(void)
{
    clcd_print("1.Micro",LINE1(0));
    clcd_print("2.Grill",LINE2(0));
    clcd_print("3.Convection",LINE3(0));
    clcd_print("4.Start",LINE4(0));
    
 }
void set_time(unsigned char key)
{
     static int wait,blink,key_count,blink_pos;
     if(reset_flag== MICRO_MODE_RESET)
     {
        wait=15;
        blink=1;
        key_count=0;
        sec=0;
        min=0;
        key= ALL_RELEASED;
        blink_pos=0;
    clcd_print("SET TIME (MM:SS)",LINE1(0));
    clcd_print("TIME- ",LINE2(0));
    clcd_putch(':',LINE2(8));
    clcd_print("*:CLEAR  #:ENTER",LINE4(0));
     }
    //based on MKP press
     if(key!='*' && key!='#' && key!=ALL_RELEASED)
     {
         //ACCEPT sec field
         key_count++;
         if(key_count<=2)
         {
             sec=sec * 10 + key;
             blink_pos=0;
         }
         else if(key_count>2 && key_count<=4)
         {
             min= min * 10 + key;
             blink_pos=1;
         }
         
     }
     if(key == '*')
     {
         //to clear sec
         if(key_count<=2)
         {
             sec=0;
             key_count=0;
         }
         else if(key_count>2 && key_count<=4)
         {
             min=0;
             key_count=0;
         }
     }
     if(key == '#')//enter key use to give confirmation
     {
         clear_clcd();
         operation_mode=TIME_DISPLAY;
         FAN = 1;  //TURN on the fan 
         TMR2ON = 1;
     }
    if(wait++==15)
    {
        wait=0;
        blink=!blink;
        clcd_putch(sec/10 + '0',LINE2(9));
        clcd_putch(sec%10 + '0',LINE2(10));
        clcd_putch(min/10 + '0',LINE2(6));
        clcd_putch(min%10 + '0',LINE2(7));
        
    }
     
    if(blink)
    {
        switch( blink_pos)
        {
            case 0: 
                clcd_print("  ",LINE2(9)); // to blink sec
                break;
            case 1:
                clcd_print("  ",LINE2(6)); //to blink min
                break;
                
        }
    }
        
    
   
}