//#define ENGLISH  1                                            //REMARK for USING FARSI Script
//#define TITR_FONT_FOR_36     1


//#define IGNORE_PASS     1                    //REMARK FOR R&D

#define  VERSION  " 1.0  SN:000001"
#define  MASTER_PASSWORD "374522"

#define   TEXT1_LOGO_ENG   ""
#define   TEXT2_LOGO_ENG   ""
#define   TEXT3_LOGO_ENG   ""

#define   TEXT1_LOGO_FA     "©›÷¨"
#define   TEXT2_LOGO_FA     "íþ÷ø¤—îó"
#define   TEXT3_LOGO_FA     "–¡—þ‘•"

#define   TEXT_TEL_ENG       "(021) 55436628-30 : öêó—"


#include "mega128.h"
//#include <mega2560.h>         

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <delay.h>  

#include "defaults.h"  

#include "initial.h"  

#include "serial.h"   

#include "at24c64.c"

#include "lcd24064.c"    

#include "AD7780.h" 

#include "funcs.h"   


                                                                                
void  initialize(void);   

// Timer 1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{       
unsigned char tov1;

// Reinitialize Timer 1 value  for 50ms
TCNT1H=0xF3;//0xE7;
TCNT1L=0xCB;//0x96;

if(buz_start)         //buz_start=1;  is  very short beep 
 {                        //buz_start=1;buz_cntr=10;   is  long beep 
    buz_start=0;    
    if(!buz_cntr)buz_cntr=1;
    if(alarm==ALARM_ON)BUZ=1;
 }  
if(!buz_cntr)BUZ=0;
else buz_cntr--;                    


///////////KEY section
if(!all_keys_temp_disable && !insert_delete_key_in_buf_inuse)
   {
      if(temp_key==0)
        {               
              #ifdef KEYBOARD_4x4 
                if((!COL1) || (!COL2) ||(!COL3) ||(!COL4))
                 {
                   temp_key=get_key_4x4();
                   if(temp_key)key=temp_key;   
                 }
              #else     
                if((!COL1) || (!COL2) ||(!COL3) ||(!COL4) ||(!COL5))
                 {
                   temp_key=get_key_5x4();
                   if(temp_key)key=temp_key;   
                 }
              #endif              
                       
        }  
      else 
        #ifdef KEYBOARD_4x4 
          if(COL1 && COL2 && COL3 && COL4)temp_key=0;      
        #else   
          if(COL1 && COL2 && COL3 && COL4 && COL5)temp_key=0;      
        #endif
      
      if(key)
         {
          // key_navigation(key);
           insert_delete_key_in_buf(key,'i');
           key=0; 
         }  
   } 
   
if(!sec_in_c && SEC_IN)   //RUN in RISING edge of SEC_IN
  {
    if(++second>59)
     {
       if(++minute>59)
         {
          if(++hour>23)
              {
                hour=0;      
              }
           minute=0;
         }
       second=0; 
       display_home_pos_en=1;
     }  
     if(page==0)
       {display_blink_handle_en=1;
        if(second&1)blink_en=0;else blink_en=1;}      
     
     if(second==0 && minute==1 && hour==0)rtc_get_date(&day,&month,&year);
     
    /* if(++update_timer_from_rtc>UPDATE_TIME)
       {
          rtc_get_date(&day,&month,&year);
          rtc_get_time(&hour,&minute,&second);
       } */ 
       
      if(backlight_cntr!=0xFFFF && backlight_time!=0)
      { 
        if(++backlight_cntr>backlight_time)
        { 
           backlight_cntr=0xFFFF;
           BL=0;
        }
      }  
     
     if(test_lcd_cntr)
      {
   
         if((++test_lcd_cntr)>TEST_LCD_MAX_TIME)
            {
              test_lcd_cntr=0;
              display_area_en=1; 
              display_home_pos_en=1;        
              display_weight_en=1;   
              clear_all_graph_en=1;
            }                
         else if((test_lcd_cntr%4)==0)set_all_graph_en=1;
         else if((test_lcd_cntr%2)==0)clear_all_graph_en=1;
      }
      
     if(show_errorcode_instead_xtra_cntr_scale1)show_errorcode_instead_xtra_cntr_scale1--;  
     if(show_errorcode_instead_xtra_cntr_scale2)show_errorcode_instead_xtra_cntr_scale2--;
      
         
  }   
sec_in_c = SEC_IN;   

if(++fifth_sec>=2)        //run every 4*50ms
 {
   fifth_sec=0;
   if(scales_func_mode==1 || scales_func_mode==0)scale1_data_read_en=1;
   if(scales_func_mode==2 || scales_func_mode==0)scale2_data_read_en=1;   
 }   
 
 
 if(com1_func==PCWORK_S1 || com2_func==PCWORK_S1 || com1_func==PCWORK_S2 || com2_func==PCWORK_S2 ||
    com1_func==REMOTE_S1 || com2_func==REMOTE_S1 || com1_func==REMOTE_S2 || com2_func==REMOTE_S2)
   {
        if(++sendout_pcwork_data_cntr>SENDOUT_PCWORK_DATA_TIME)
          {
             sendout_pcwork_data_en=1;
             sendout_pcwork_data_cntr=0;    
          }
   }
   
   
if(half_sec){half_sec=0;return;} //half_sec is bit
half_sec=1;                      //below section run every 2 cycles == 0.1 second    

//if(scales_func_mode==1 || scales_func_mode==0)scale1_data_read_en=1;
//if(scales_func_mode==2 || scales_func_mode==0)scale2_data_read_en=1;



  /*
  if(scale2_value==1)
   {
     if(scale1_value>45200){scale1_value-=42000; }   
     else scale2_value=0;
   }
  else
  {
  scale1_value+=3; 
  if(scale1_value>20)scale1_value+=1;  
  if(scale1_value>150)scale1_value+=78;  
  if(scale1_value>1500)scale1_value+=800;  
  if(scale1_value>16000)scale1_value+=14000;
  if(scale1_value>180000)scale1_value+=80000;     
  if(scale1_value>800000){scale2_value=1;} 
  
  }
  display_weight_en=1; 
 // scale2_value+=78;
  */                                                                        



if(blink_get_data_cntr)                                          
 {
   blink_get_data_cntr++;                                                                           
   if(blink_en==0 && blink_get_data_cntr>BLINK_ON_TIME)
     {display_blink_handle_en=1;blink_en=1;blink_get_data_cntr=1;}   
   if(blink_en==1 && blink_get_data_cntr>BLINK_OFF_TIME)
     {display_blink_handle_en=1;blink_en=0;blink_get_data_cntr=1;}       
 } 
 
if(no_display_change_cntr)no_display_change_cntr--;  
if(all_keys_temp_disable)all_keys_temp_disable--;  
if(page_4_out_cntr){if(--page_4_out_cntr==0){page=0;clear_home_pos_en=1;display_home_pos_en=1;}} 
if(tenfold_out_cntr)tenfold_out_cntr--;
if(page_19_next_letter_cntr)
    {
      if(--page_19_next_letter_cntr==0)
        {
         if(page==19 || page==23) 
          get_key_character=0xFF;    // start from NUMBER and overwrite on next   
          if(get_cursor<USER_TEXTS_LENGHT)get_cursor++;
          get_character_index=1; 
          display_home_pos_en=1;   
          blink_get_data_cntr=1;              
        }
    } 
    
if(calibration_state_counter)
   {
     if(--calibration_state_counter==0)
      {
        if(calibration_state==2)         
         {
           calibration_state=3;     
           if(get_value_i4==1)
             {
               sum_tempf = sum_tempf / average_index;
               scale1_zero_mv_v=sum_tempf;       //calcualte value for zero_mv_v  
               load_float_in_string(scale1_zero_mv_v,get_value_ch); 
               if(scale1_powerup_zero==0)poweron_zero_s1_state=0; else poweron_zero_s1_state=POWER_UP_ZERO_TIME;    //cancel PowerOn reset process          
             }
           else
             {
               sum_tempf = sum_tempf / average_index;
               scale2_zero_mv_v=sum_tempf;       //calcualte value for zero_mv_v  
               load_float_in_string(scale2_zero_mv_v,get_value_ch);          
               if(scale2_powerup_zero==0)poweron_zero_s2_state=0; else poweron_zero_s2_state=POWER_UP_ZERO_TIME;    //cancel PowerOn reset process                                
             }  

           page=25; 
           previous_page=2;
           display_home_pos_en=1; 
           clear_home_pos_en=1;                           
           
           
           //load_float_in_string(zero_mv_v);
           get_value_i1=strlen(get_value_ch);               //number of digits
           get_value_i2=0;  
           get_value_i3=0;               //use for edited_value flag in page==25
                  
           blink_get_data_cntr=1;   
           get_character_index=strlen(get_value_ch); 
           get_cursor=0;                 
   
         } 
        if(calibration_state==6)        //measure actual value for calib_weight
         {
           calibration_state=7;   
           
           if(get_value_i4==1)
                 {                                
                     sum_tempf = sum_tempf / average_index;
                     sum_tempf = sum_tempf * scale1_capacity[2];       //500000 gr      capacity
                     sum_tempf = sum_tempf / scale1_default_calib_weight;         //2mv/v             sensitivity
                     sum_tempf = sum_tempf * 1000;
                     sum_tempf = sum_tempf / 128; 
                     if(sum_tempf>0)
                       {
                        scale1_load_mv_v = sum_tempf;    
                        sum_tempf = scale1_default_calib_weight;
                       }  
                      load_float_in_string(scale1_load_mv_v+0.000005,get_value_ch);
                      get_value_i1=7;//strlen(get_value_ch);               //number of digits
                      get_value_i2=0;get_value_i3=0;               //use for edited_value flag in page 25
                      blink_get_data_cntr=1;   
                      get_character_index=strlen(get_value_ch); 
                      get_cursor=0;    
                      poweron_zero_s1_state=0;   
                      scale1_poweron_offset_zero_value=0;   
                      pb_zero_s1_state=0xFF;        //exit from PB zero
                 }
           else  
                 {
                     sum_tempf = sum_tempf / average_index;
                     sum_tempf = sum_tempf * scale2_capacity[2];       //500000 gr      capacity
                     sum_tempf = sum_tempf / scale2_default_calib_weight;         //2mv/v             sensitivity
                     sum_tempf = sum_tempf * 1000;
                     sum_tempf = sum_tempf / 128; 
                     if(sum_tempf>0)
                       {
                        scale2_load_mv_v = sum_tempf;    
                        sum_tempf = scale2_default_calib_weight;
                       }  
                      load_float_in_string(scale2_load_mv_v+0.000005,get_value_ch);
                      get_value_i1=7;//strlen(get_value_ch);               //number of digits
                      get_value_i2=0;get_value_i3=0;               //use for edited_value flag in page 25
                      blink_get_data_cntr=1;   
                      get_character_index=strlen(get_value_ch); 
                      get_cursor=0;    
                      poweron_zero_s2_state=0;       
                      scale2_poweron_offset_zero_value=0;
                      pb_zero_s2_state=0xFF;        //exit from PB zero
                 }            
                 
               
           get_value_i3=0;      //modified flag for value
           page=22; 
           previous_page=2;
           display_home_pos_en=1; 
           clear_home_pos_en=1;  
         }         
 
      }
   }
    

}




void main(void)
{
  
  unsigned char temp2,offset,tempch,temp_dww1[40];;
  unsigned char zero1_track_cntr=0,fix1_track_cntr=0,_auto_zero_range_flag;
  unsigned char zero2_track_cntr=0,fix2_track_cntr=0;    
  unsigned char scale1_range_select=0,scale2_range_select=0;  
  unsigned char previous_scale1_error,previous_scale2_error;

  unsigned long int templ;
//  unsigned long int previous_scale1_value=0,previous_scale2_value=0;     
  float previous_scale1_value=0,previous_scale2_value=0;     
  float scale2_poweron_offset_zero_value=0;
  float tempf;      
  
  float scale1_offset_zero_value,scale2_offset_zero_value,_offset_zero_s1_value_temp=0,_offset_zero_s2_value_temp=0;
  float _pb_zero_s1_value_temp,_pb_zero_s2_value_temp;
  
   
  initialize();     

 // BUZ=1;    
  delay_ms(100);
  BUZ=0;
  
ALARM=0;  

  delay_ms(100);
  
  load_initial_values();
  
  
  
  no_display_change_cntr=0;
  
  clear_all_graph_en=1;  
  clear_home_pos_en=1; 
  display_area_en=1; 
  display_weight_en=1; 
  display_home_pos_en=1;   
  
  LC1_SCK=1;
  LC2_SCK=1;  
  
  if(scale1_powerup_zero==0)poweron_zero_s1_state=0; else poweron_zero_s1_state=1;    //cancel PowerOn reset process          
  if(scale2_powerup_zero==0)poweron_zero_s2_state=0; else poweron_zero_s2_state=1;    //cancel PowerOn reset process     
  
  
   /*
       sprintf(temp_str," fdALLAHoAKBAR     "); 
  write_e2prom_page(temp_str,3);       



  
  sprintf(temp_str,"  AMINAM      ");  
  
 read_e2prom_page(temp_str,3);    

  
   temp2=240-LCD_getstringlength2(temp_str);
   temp2/=2; 
   LCD_setxy(temp2,45); 
   LCD_printstring2(temp_str,1);        
                                 */                 
                /*
sprintf(weighing_data_temp.customer_name,"salaam");       

weighing_data_temp.conseq1_serial=1450;
                                 

write_weighing_data_eeprom(weighing_data_temp,100);    


weighing_data_temp.conseq1_serial=2134; 
sprintf(weighing_data_temp.customer_name,"%d",weighing_data_temp.conseq1_serial);  
                                 
                                 
read_weighing_data_eeprom(&weighing_data_temp,100);   

          sprintf(weighing_data_temp.customer_name,"%d",weighing_data_temp.conseq1_serial);  
  
   temp2=240-LCD_getstringlength2(weighing_data_temp.customer_name);
   temp2/=2; 
   LCD_setxy(temp2,45); 
   LCD_printstring2(weighing_data_temp.customer_name,1); */
      
      
  display_logo();           //stay almost 10 seocnd here if press a KEY then return   
  
  
  
// Reset Source checking
 if (MCUCSR & 4)
   {
   // Brown-Out Reset
   MCUCSR&=0xE0;
   errormsg(BROWN_OUT_ALARM,1);       //A07  BROWN_OUT

   }
else if (MCUCSR & 8)
   {
   // Watchdog Reset
   MCUCSR&=0xE0;
   errormsg(WATCH_DOG_ALARM,1);      //WATCH DOG                   

   }

  
   


          
  while(1)
    { 
        
      #asm("wdr")
        
      delay_ms(10);     

      
      if(scale1_data_read_en)
       { 
         display_weight_en=1;  
         scale1_data_read_en=0;  
         temp2=read_LC1(); 
         if(temp2)delay_ms(1);

        if(temp2==0)            //successful 
          {
           
           scale1_seq_err_cntr=0;
            
           scale_float_value = scale1_raw_value;      

           scale_float_value = scale_float_value / (0x80000);       //2^19 = 524288
             
           scale_float_value = scale_float_value - 1;        //
           
/*           tempf = prevoius_adc_temp_1 * 100;                          //spike in weighing
           if(tempf<scale_float_value){prevoius_adc_temp_1=scale_float_value;continue;}
            prevoius_adc_temp_1=scale_float_value; */         
             
             
           if(calibration_state==2)      //calculating zero mv/v
            {
               if(get_value_i4==1) //1 means scale1
                 {
                   tempf = (-1)*scale_float_value * 1000;
                   tempf = tempf /128;
                   sum_tempf += tempf;   
                   average_index++;
                 }
           } 
   
                        
           scale_float_value +=  (scale1_zero_mv_v / 1000 * 128);
           
           
           if(calibration_state==6)       //calcualting scale1 mv/v for "scale1_default_calib_weight"
            {
               if(get_value_i4==1) //1 means scale1
                 {
                   tempf = scale_float_value;
                   sum_tempf += tempf;   
                   average_index++;
                 }            
            }                   
            
            
           scale_float_value = scale_float_value * scale1_capacity[2];       //500000 gr      capacity   
           
           
           scale_float_value = scale_float_value / scale1_load_mv_v;         //2mv/v             sensitivity
           scale_float_value = scale_float_value * 1000;
           
           scale_float_value = scale_float_value / scale1_loadcells_no;
             
           scale_float_value = scale_float_value / 128;        
             
           //  scale_float_value += 470;          //calib
             
              
             //scale_float_value is the float value of LOAD now

           
            if(poweron_zero_s1_state)
             {             
                 poweron_zero_s1_state++;
                 if(poweron_zero_s1_state<POWER_UP_ZERO_TIME)goto scale2_section;    //10 for 2 sconds     //****consider this***\\
                 poweron_zero_s1_state=0;
                 
                 tempf =scale1_capacity[2];
                 if(scale1_powerup_zero==1)tempf = tempf * 2.0;
                 if(scale1_powerup_zero==2)tempf = tempf * 10.0;   
                 tempf = tempf / 100;
                 
                 scale1_poweron_offset_zero_value=0;      

                  if(scale_float_value<=tempf && scale_float_value>=0)
                    {
                       scale1_poweron_offset_zero_value=scale_float_value;
                    }
                    
                if(scale_float_value>=(-tempf) && scale_float_value<0)
                    {
                        scale1_poweron_offset_zero_value=scale_float_value;
                    }  
                    
                  if(scale_float_value>tempf && scale_float_value>=0)
                    {
                         errormsg(POWERUP_ZERO_OVER_S1_ALARM,20); //M 031
                    }
                    
                if(scale_float_value<(-tempf) && scale_float_value<0)
                    {
                         errormsg(POWERUP_ZERO_UNDER_S1_ALARM,20); //M 032
                    }                     
                    
                        
                 continue;                       
             }
           scale_float_value = scale_float_value - scale1_poweron_offset_zero_value;      //applying POWERON Zero 
                  
                
                
           if(pb_zero_s1_state)
            {
             if(pb_zero_s1_state==1)       //scale 1 is selected
              {
                pb_zero_s1_state=2;
                if(scale_float_value>=0)
                { 
                   tempf = scale1_pb_zero_p;
                   tempf = tempf / 100;
                   tempf = tempf * scale1_capacity[2];
                   if(scale_float_value<tempf)
                      {
                         _pb_zero_s1_value_temp=scale_float_value;
                         //scale_float_value=0; 
//                         _pb_zero_s1_flag=1;  
                         if(scale1_auto_zero_range)scale1_offset_zero_value=0;
                      } 
                   else  pb_zero_s1_state=0;  
                      
                 }
                else
                 {   
                     tempf = scale1_pb_zero_n;
                     tempf = tempf / 100;
                     tempf = tempf * scale1_capacity[2];
                     tempf = (-1)*tempf;
                     if(scale_float_value>tempf)
                       {
                          _pb_zero_s1_value_temp=scale_float_value;
                          //scale_float_value=0; 
//                          _pb_zero_s1_flag=1;          
                          if(scale1_auto_zero_range)scale1_offset_zero_value=0;
                       }
                      else  pb_zero_s1_state=0;    
                                        
                   } 

                }
                         
            else
            if(pb_zero_s1_state==2)
              { 
                //scale1_value=0;     
                zero1_track_cntr=0;
                fix1_track_cntr=0;
                pb_zero_s1_state=3;
                scale1_mode=0;
              }
                    
            else  
            if(pb_zero_s1_state==3)
              { 
                if(scale1_mode=='f')pb_zero_s1_state=4; 
              }              
                   
            else
            if(pb_zero_s1_state==4)
              { 
                if(scale1_mode=='z')
                   {
                     pb_zero_s1_state=0;  
                     _pb_zero_s1_value_temp=0; 
                   } 
              }  
            else
            if(pb_zero_s1_state==0xFF)         //make 0xFF by push button again in key-navigation
              { 
                     pb_zero_s1_state=0;  
                     _pb_zero_s1_value_temp=0;  
                     scale1_mode=0;       
                     zero1_track_cntr=0;
                     fix1_track_cntr=0;                           
              }    
              
//             if(scale1_sign=='-' && scale1_mode=='f')pb_zero_s1_state=0xFF; 
                                        
            
            }  //if(pb_zero_s1_state)     

              
            scale_float_value = scale_float_value - _pb_zero_s1_value_temp;
             
            if(manual_tare_s1_state)
              {
                if(manual_tare_s1_state==0xFF)
                  {
                    manual_tare_s1_state = 0;
                  }     
                else scale_float_value = scale_float_value - scale1_manual_tare;
              } 
              
            if(pb_tare_s1_state==1)
             {
               if(scale1_mode=='f' || scale1_mode=='z')
                 {
                   pb_tare_s1_state=2; 
                   scale1_pb_tare = scale_float_value;
                   pb_tare_s1_state=2;
                 }
             }  
            else 
            if(pb_tare_s1_state>1 && pb_tare_s1_state!=0xFF )
             {
                scale_float_value = scale_float_value - scale1_pb_tare;   
                
                if(scale1_mode=='f' && pb_tare_s1_state==2)pb_tare_s1_state=3;   
                else
                if(scale1_mode==0 && pb_tare_s1_state==3)pb_tare_s1_state=4;
                else
                if(scale1_mode=='f' && pb_tare_s1_state==4 && scale1_sign=='-')pb_tare_s1_state=0xFF;
                
             }                     
            else
            if(pb_tare_s1_state==0xFF)
            {
               scale1_pb_tare=0;
               pb_tare_s1_state=0;
            }              
           
            _auto_zero_range_flag=0;  
            if(scale1_auto_zero_range  && scale1_mode=='f')
              {
                 if(scale1_ranges==2)tempf =scale1_interval[0];
                 if(scale1_ranges==1)tempf =scale1_interval[1];
                 if(scale1_ranges==0)tempf =scale1_interval[2];   
                 
                 if(scale1_auto_zero_range==1)tempf = tempf * 0.5;
                 if(scale1_auto_zero_range==2)tempf = tempf * 1.0;
                 if(scale1_auto_zero_range==3)tempf = tempf * 3.0;
                 
                 if(scale_float_value<tempf && scale_float_value>=0)
                    {
                       _offset_zero_s1_value_temp=scale_float_value;
                       scale_float_value=0; 
                       _auto_zero_range_flag=1;  
                    }
                    
                 if(scale_float_value>(-tempf) && scale_float_value<0)
                    {
                    _offset_zero_s1_value_temp=scale_float_value;
                     scale_float_value=0;
                     _auto_zero_range_flag=1;
                    }                    

              }                 
                
             //applying scale1_offset_zero_value must be after
              
             if(_auto_zero_range_flag==0) 
              {
                  scale_float_value = scale_float_value - scale1_offset_zero_value;
              }    

             
             if(scale_float_value<0)
                {
                   scale1_sign = '-';  
                   scale_float_value= -scale_float_value;  
                }
             else
                {
                   scale1_sign = ' ';  
                }       
                          

             if((calibration_state && get_value_i4==1) ||
                (tenfold_out_cntr && using_scale==1))tenfold_float_value=scale_float_value*10; else tenfold_float_value=0;


             scale1_value = (unsigned long int)(scale_float_value);            
             
             tempf = scale1_interval[2];
             tempf = tempf * scale1_overload;
             tempf += scale1_capacity[2];             
             
             if(scale1_value>tempf)
              {
                scale1_value = 0xFFFFFFFF;   
                fix1_track_cntr=0;
                zero1_track_cntr=0;
                scale1_mode=0;   
                
                 if(previous_scale1_error!='L')
                  {
                    previous_scale1_error='L';
                    errormsg(OVERLOAD_S1_ALARM,5);     //M030
                  }                
         
              } 
             else if(previous_scale1_error=='L')previous_scale1_error=0;         
              
              if(scale1_mode=='f')scale1_range_select= scale1_range_display;
              if(scale1_mode=='z')scale1_range_select= 0;
             
             if(scale1_ranges==2)
               {
                   if(scale1_value<scale1_capacity[0] && scale1_range_select==0)
                    {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value);  
                           scale1_value = scale1_value / scale1_interval[0];
                           scale1_value = scale1_value * scale1_interval[0];   
                           scale1_range_display=0;
                    }
                   else           
                   if(scale1_value<scale1_capacity[1] && scale1_range_select<=1)
                    {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value);   
                           scale1_value = scale1_value / scale1_interval[1];
                           scale1_value = scale1_value * scale1_interval[1];
                           scale1_range_display=1;
                    }                    
                   else           
                    {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value);   
                           scale1_value = scale1_value / scale1_interval[2];
                           scale1_value = scale1_value * scale1_interval[2];
                           scale1_range_display=2;
                    }                        
               
               }   
             else         
             if(scale1_ranges==1)
               {
       
                   if(scale1_value<scale1_capacity[1] && scale1_range_select<=1)
                    {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value);   
                           scale1_value = scale1_value / scale1_interval[1];
                           scale1_value = scale1_value * scale1_interval[1];
                           scale1_range_display=1;
                    }                    
                   else           
                    {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value);     
                           scale1_value = scale1_value / scale1_interval[2];
                           scale1_value = scale1_value * scale1_interval[2];
                           scale1_range_display=2;
                    }                        
               
               }       
             else         

               {
                           if(tenfold_float_value)scale1_value = (unsigned long int)(tenfold_float_value); 
                           scale1_value = scale1_value / scale1_interval[2];
                           scale1_value = scale1_value * scale1_interval[2];
                           scale1_range_display=2;
               }                                 
             
           if(tenfold_float_value && scale1_value!=0xFFFFFFFF )scale1_value /= 10;   
           

          
           if(scale1_value==0)scale1_sign = ' ';            // -0 is not acceptable  
           
           if(scale1_value==0)
              {
                
                if(zero1_track_cntr<scale1_motion_counter)zero1_track_cntr++;
                else
                  {
                     if(scale1_mode!='z'){buz_start=1;buz_cntr=2;}  
                     scale1_mode='z';
                     scale1_offset_zero_value = _offset_zero_s1_value_temp;  
                  }
              }
             else 
             {
                zero1_track_cntr=0;     
                tempf =scale1_interval[2-scale1_ranges];  //tempf is DEVISION value in grams
                if(scale1_motion_window==0)tempf *= 0.5;
                else tempf *= scale1_motion_window;   //scale1_motion_window = 1,2,3 Devision
                                                      //scale_float_value is ABS value of weigh in grams
          
           //     if(scale1_value==previous_scale1_value)
               if( (scale_float_value<(previous_scale1_value+tempf)) &&  
                   (scale_float_value>(previous_scale1_value-tempf)) )
                  {
                      if(fix1_track_cntr<scale1_motion_counter)fix1_track_cntr++;
                      else
                        {
                          if(scale1_mode!='f'){buz_start=1;buz_cntr=1;}
                          scale1_mode='f';  
                        }              
                  }
                 else  
                  {
                    fix1_track_cntr=0;
                    scale1_mode=0;
                  }  
             
              }           
            if(scale1_mode!='f')previous_scale1_value =  scale_float_value;        //in fixed mode comapre is done by the
            
            //using from values of weighing
            


             //saving values
            
            if((first_weighing_state)&& (using_scale==1))  //using scale1
              {
                if((first_weighing_state<5) && (scale1_mode=='f'))temp_reg_first_weigh = scale1_value;//invert_unit_fp(scale1_value,scale1_unit,scale1_float);     //save values in grams
              }
            
            if((second_weighing_state) && (using_scale==1))
              {
                if((second_weighing_state<6) && (scale1_mode=='f'))temp_reg_second_weigh = scale1_value;//invert_unit_fp(scale1_value,scale1_unit,scale1_float);     //save values in grams
                if(temp_reg_second_weigh>=temp_reg_first_weigh)temp_reg_total_weigh = temp_reg_second_weigh - temp_reg_first_weigh;
                else temp_reg_total_weigh = temp_reg_first_weigh - temp_reg_second_weigh;
                
              }  
                
            if((basic_weighing_state) && (using_scale==1))
              {      
                if((basic_weighing_state<5) && (scale1_mode=='f'))temp_reg_second_weigh = scale1_value;//invert_unit_fp(scale1_value,scale1_unit,scale1_float);     //save values in grams
                if(temp_reg_second_weigh>=temp_reg_first_weigh)temp_reg_total_weigh = temp_reg_second_weigh - temp_reg_first_weigh;
                else temp_reg_total_weigh = temp_reg_first_weigh - temp_reg_second_weigh;              
              }        
              
             //apply value for output software
             
             
             if(software!=SW_NONE)
               {
                  
                  if(scale1_sign!='-' || scale1_value!=0xFFFFFFFF){OUT1=0;OUT3=0;}
                  else
                  if(software==SW_SIMPLE)
                   {
                      if(scale1_set_point1)
                        {
                           if(scale1_value<scale1_set_point1)OUT1=0;
                           else OUT1=1;
                        }  
                      if(scale1_set_point2)
                        {
                           if(scale1_value<scale1_set_point2)OUT3=0;
                           else OUT3=1;
                        }                        
                                     
                   }

               
               } 
              else  {OUT1=0;OUT3=0;}
                                                                                 
          
           }       
        else
           {
              if(++scale1_seq_err_cntr>50)
               {
                 scale1_seq_err_cntr=0;
                 scale1_value=0;    
                 previous_scale1_value=0;
                 scale1_value=0xFFFFFFFF; 
                 
                 if(temp2!=previous_scale1_error)
                  {
                    previous_scale1_error=temp2;
                    if(temp2=='D')errormsg(NO_ADC_CARD_FOUND_S1_ALARM,5);       //s020
                    if(temp2=='E')errormsg(ADC_TIME_OVER_S1_ALARM,1);               //A010
                  }
                    
                 
               }
           }      
               
       }             
       


scale2_section:

       
         //*************************///////
      if(scale2_data_read_en)
       { 
         display_weight_en=1;  
         scale2_data_read_en=0;  
         temp2=read_LC2();  
         if(temp2)delay_ms(1);

        if(temp2==0)            //successful 
          {
           
           scale2_seq_err_cntr=0;
            
           scale_float_value = scale2_raw_value;      

           scale_float_value = scale_float_value / (0x80000);       //2^19 = 524288
             
           scale_float_value = scale_float_value - 1;      
           
/*           tempf = prevoius_adc_temp_1 * 100;                          //spike in weighing
           if(tempf<scale_float_value){prevoius_adc_temp_1=scale_float_value;continue;}
            prevoius_adc_temp_1=scale_float_value; */         
             
             
           if(calibration_state==2)      //calculating zero mv/v
            {
               if(get_value_i4==2) //2 means scale2
                 {
                   tempf = (-1)*scale_float_value * 1000;
                   tempf = tempf /128;
                   sum_tempf += tempf;   
                   average_index++;
                 }
           } 
   
                        
           scale_float_value +=  (scale2_zero_mv_v / 1000 * 128);
           
           
           if(calibration_state==6)       //calcualting scale2 mv/v for "scale_default_calib_weight"
            {
               if(get_value_i4==2) //2 means scale2
                 {
                   tempf = scale_float_value;
                   sum_tempf += tempf;   
                   average_index++;
                 }            
            }                   
            
            
           scale_float_value = scale_float_value * scale2_capacity[2];       //500000 gr      capacity

           scale_float_value = scale_float_value / scale2_load_mv_v;         //2mv/v             sensitivity
           scale_float_value = scale_float_value * 1000;      
           
           scale_float_value = scale_float_value / scale2_loadcells_no;
             
           scale_float_value = scale_float_value / 128;        
             
           //  scale_float_value += 470;          //calib
             
              
             //scale_float_value is the float value of LOAD now

           
            if(poweron_zero_s2_state)
             {             
                 poweron_zero_s2_state++;
                 if(poweron_zero_s2_state<POWER_UP_ZERO_TIME)continue;    //10 for 2 sconds
                 poweron_zero_s2_state=0;
                 
                 tempf =scale2_capacity[2];
                 if(scale2_powerup_zero==1)tempf = tempf * 2.0;
                 if(scale2_powerup_zero==2)tempf = tempf * 10.0;   
                 tempf = tempf / 100;
                 
                 scale2_poweron_offset_zero_value=0;      

                  if(scale_float_value<=tempf && scale_float_value>=0)
                    {
                       scale2_poweron_offset_zero_value=scale_float_value;
                    }
                    
                if(scale_float_value>=(-tempf) && scale_float_value<0)
                    {
                        scale2_poweron_offset_zero_value=scale_float_value;
                    }    
                    
                  if(scale_float_value>tempf && scale_float_value>=0)
                    {
                         errormsg(POWERUP_ZERO_OVER_S2_ALARM,20); //M 031
                    }
                    
                if(scale_float_value<(-tempf) && scale_float_value<0)
                    {
                         errormsg(POWERUP_ZERO_UNDER_S2_ALARM,20); //M 032
                    }                         
                    
                     
                 continue;                       
             }
           scale_float_value = scale_float_value - scale2_poweron_offset_zero_value;      //applying POWERON Zero 
                  
                
                
           if(pb_zero_s2_state)
            {
             if(pb_zero_s2_state==1)       //scale 1 is selected
              {
                pb_zero_s2_state=2;
                if(scale_float_value>=0)
                { 
                   tempf = scale2_pb_zero_p;
                   tempf = tempf / 100;
                   tempf = tempf * scale2_capacity[2];
                   if(scale_float_value<tempf)
                      {
                         _pb_zero_s2_value_temp=scale_float_value;
                         //scale_float_value=0; 
//                         _pb_zero_s2_flag=1;  
                         if(scale2_auto_zero_range)scale2_offset_zero_value=0;
                      } 
                   else  pb_zero_s2_state=0;  
                      
                 }
                else
                 {   
                     tempf = scale2_pb_zero_n;
                     tempf = tempf / 100;
                     tempf = tempf * scale2_capacity[2];
                     tempf = (-1)*tempf;
                     if(scale_float_value>tempf)
                       {
                          _pb_zero_s2_value_temp=scale_float_value;
                          //scale_float_value=0; 
//                          _pb_zero_s2_flag=1;          
                          if(scale2_auto_zero_range)scale2_offset_zero_value=0;
                       }
                      else  pb_zero_s2_state=0;    
                                        
                   } 

                }
                         
            else
            if(pb_zero_s2_state==2)
              { 
                //scale2_value=0;     
                zero2_track_cntr=0;
                fix2_track_cntr=0;
                pb_zero_s2_state=3;
                scale2_mode=0;
              }
                    
            else  
            if(pb_zero_s2_state==3)
              { 
                if(scale2_mode=='f')pb_zero_s2_state=4; 
              }              
                   
            else
            if(pb_zero_s2_state==4)
              { 
                if(scale2_mode=='z')
                   {
                     pb_zero_s2_state=0;  
                     _pb_zero_s2_value_temp=0; 
                   } 
              }  
            else
            if(pb_zero_s2_state==0xFF)         //make 0xFF by push button again in key-navigation
              { 
                     pb_zero_s2_state=0;  
                     _pb_zero_s2_value_temp=0;  
                     scale2_mode=0;       
                     zero2_track_cntr=0;
                     fix2_track_cntr=0;                           
              }    
              
//             if(scale2_sign=='-' && scale2_mode=='f')pb_zero_s2_state=0xFF; 
                                        
            
            }  //if(pb_zero_s2_state)     

              
            scale_float_value = scale_float_value - _pb_zero_s2_value_temp;
             
            if(manual_tare_s2_state)
              {
                if(manual_tare_s2_state==0xFF)
                  {
                    manual_tare_s2_state = 0;
                  }     
                else scale_float_value = scale_float_value - scale2_manual_tare;
              }   
              
            if(pb_tare_s2_state==1)
             {
               if(scale2_mode=='f' || scale2_mode=='z')
                 {
                   pb_tare_s2_state=2; 
                   scale2_pb_tare = scale_float_value;
                   pb_tare_s2_state=2;
                 }
             }      
            else
            if(pb_tare_s2_state>1 && pb_tare_s2_state!=0xFF )
             {
                scale_float_value = scale_float_value - scale2_pb_tare;   
                
                if(scale2_mode=='f' && pb_tare_s2_state==2)pb_tare_s2_state=3;   
                else
                if(scale2_mode==0 && pb_tare_s2_state==3)pb_tare_s2_state=4;
                else
                if(scale2_mode=='f' && pb_tare_s2_state==4 && scale2_sign=='-')pb_tare_s2_state=0xFF;
                
             }                     
            else
            if(pb_tare_s2_state==0xFF)
            {
               scale2_pb_tare=0;
               pb_tare_s2_state=0;
            }              
             
           
            _auto_zero_range_flag=0;  
            if(scale2_auto_zero_range && scale2_mode=='f')
              {
                 if(scale2_ranges==2)tempf =scale2_interval[0];
                 if(scale2_ranges==1)tempf =scale2_interval[1];
                 if(scale2_ranges==0)tempf =scale2_interval[2];   
                 
                 if(scale2_auto_zero_range==1)tempf = tempf * 0.5;
                 if(scale2_auto_zero_range==2)tempf = tempf * 1.0;
                 if(scale2_auto_zero_range==3)tempf = tempf * 3.0;
                 
                 if(scale_float_value<tempf && scale_float_value>=0)
                    {
                       _offset_zero_s2_value_temp=scale_float_value;
                       scale_float_value=0; 
                       _auto_zero_range_flag=1;  
                    }
                    
                 if(scale_float_value>(-tempf) && scale_float_value<0)
                    {
                    _offset_zero_s2_value_temp=scale_float_value;
                     scale_float_value=0;
                     _auto_zero_range_flag=1;
                    }                    
              
              }                 
                
             //applying scale2_offset_zero_value must be after
              
             if(_auto_zero_range_flag==0) 
              {
                  scale_float_value = scale_float_value - scale2_offset_zero_value;
              }    

             
             if(scale_float_value<0)
                {
                   scale2_sign = '-';  
                   scale_float_value= -scale_float_value;  
                }
             else
                {
                   scale2_sign = ' ';  
                }       
                          

           
             if((calibration_state && get_value_i4==2) ||
                (tenfold_out_cntr && using_scale==2))tenfold_float_value=scale_float_value*10; 
             else tenfold_float_value=0;

             scale2_value = (unsigned long int)(scale_float_value);            
             
             tempf = scale2_interval[2];
             tempf = tempf * scale2_overload;
             tempf += scale2_capacity[2];             
             
             if(scale2_value>tempf)
              {
                scale2_value = 0xFFFFFFFF;   
                fix2_track_cntr=0;
                zero2_track_cntr=0;
                scale2_mode=0;    
                
                 if(previous_scale2_error!='L')
                  {
                    previous_scale2_error='L';
                    errormsg(OVERLOAD_S2_ALARM,5);     //M030
                  }                   
                            
              } 
             else if(previous_scale2_error=='L')previous_scale2_error=0; 
             
                      
              if(scale2_mode=='f')scale2_range_select= scale2_range_display;
              if(scale2_mode=='z')scale2_range_select= 0;              
              
             if(scale2_ranges==2)
               {
                   if(scale2_value<scale2_capacity[0] && scale2_range_select==0)
                    {
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value);   
                           scale2_value = scale2_value / scale2_interval[0];
                           scale2_value = scale2_value * scale2_interval[0];
                           scale2_range_display=0;  
                    }
                   else           
                   if(scale2_value<scale2_capacity[1] && scale2_range_select<=1)
                    {                                                                                      
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value); 
                           scale2_value = scale2_value / scale2_interval[1];
                           scale2_value = scale2_value * scale2_interval[1];
                           scale2_range_display=1;
                    }                    
                   else           
                    {
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value);      
                           scale2_value = scale2_value / scale2_interval[2];
                           scale2_value = scale2_value * scale2_interval[2];
                           scale2_range_display=2;
                    }                        
               
               }   
             else         
             if(scale2_ranges==1)
               {
       
                   if(scale2_value<scale2_capacity[1] && scale2_range_select<=1)
                    {
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value);
                           scale2_value = scale2_value / scale2_interval[1];
                           scale2_value = scale2_value * scale2_interval[1];
                           scale2_range_display=1;
                    }                    
                   else           
                    {
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value);     
                           scale2_value = scale2_value / scale2_interval[2];
                           scale2_value = scale2_value * scale2_interval[2];
                           scale2_range_display=2;
                    }                        
               
               }       
             else         

               {
                           if(tenfold_float_value)scale2_value = (unsigned long int)(tenfold_float_value);         
                           scale2_value = scale2_value / scale2_interval[2];
                           scale2_value = scale2_value * scale2_interval[2];
                           scale2_range_display=2;
               }                                 
             
           if(tenfold_float_value && scale2_value!=0xFFFFFFFF )scale2_value /= 10;   
           

          
           if(scale2_value==0)scale2_sign = ' ';            // -0 is not acceptable  
           
           if(scale2_value==0)
              {
                
                if(zero2_track_cntr<scale2_motion_counter)zero2_track_cntr++;
                else
                  {
                     if(scale2_mode!='z'){buz_start=1;buz_cntr=2;}
                     scale2_mode = 'z';
                     scale2_offset_zero_value = _offset_zero_s2_value_temp;    
                  }
              }
             else 
             {
                zero2_track_cntr=0;     
                tempf =scale2_interval[2-scale2_ranges];  //tempf is DEVISION value in grams
                if(scale2_motion_window==0)tempf *= 0.5;
                else tempf *= scale2_motion_window;   //scale2_motion_window = 1,2,3 Devision
                                                      //scale_float_value is ABS value of weigh in grams
          
           //     if(scale2_value==previous_scale2_value)
               if( (scale_float_value<(previous_scale2_value+tempf)) &&  
                   (scale_float_value>(previous_scale2_value-tempf)) )
                  {
                      if(fix2_track_cntr<scale2_motion_counter)fix2_track_cntr++;
                      else
                        {
                          if(scale2_mode!='f'){buz_start=1;buz_cntr=1;}                      
                          scale2_mode = 'f';     
                        }              
                  }
                 else  
                  {
                    fix2_track_cntr=0;
                    scale2_mode=0;
                  }  
             
              }           
            if(scale2_mode!='f')previous_scale2_value =  scale_float_value;        //in fixed mode comapre is done by the
            
            //using from values of weighing
            


             //saving values
            
            if((first_weighing_state)&& (using_scale==2))  //using scale2
              {
                if((first_weighing_state<5) && (scale2_mode=='f'))temp_reg_first_weigh = scale2_value;//invert_unit_fp(scale2_value,scale2_unit,scale2_float);     //save values in grams
              }
            
            if((second_weighing_state) && (using_scale==2))
              {
                if((second_weighing_state<6) && (scale2_mode=='f'))temp_reg_second_weigh = scale2_value;//invert_unit_fp(scale2_value,scale2_unit,scale2_float);     //save values in grams
                if(temp_reg_second_weigh>=temp_reg_first_weigh)temp_reg_total_weigh = temp_reg_second_weigh - temp_reg_first_weigh;
                else temp_reg_total_weigh = temp_reg_first_weigh - temp_reg_second_weigh;
                
              }  
                
            if((basic_weighing_state) && (using_scale==2))
              {      
                if((basic_weighing_state<5) && (scale2_mode=='f'))temp_reg_second_weigh = scale2_value;//invert_unit_fp(scale2_value,scale2_unit,scale2_float);     //save values in grams
                if(temp_reg_second_weigh>=temp_reg_first_weigh)temp_reg_total_weigh = temp_reg_second_weigh - temp_reg_first_weigh;
                else temp_reg_total_weigh = temp_reg_first_weigh - temp_reg_second_weigh;              
              }        
              

             if(software!=SW_NONE)
               {
                  
                  if(scale2_sign!='-' || scale2_value!=0xFFFFFFFF){OUT2=0;OUT4=0;}
                  else
                  if(software==SW_SIMPLE)
                   {
                      if(scale2_set_point1)
                        {
                           if(scale1_value<scale2_set_point1)OUT2=0;
                           else OUT2=1;
                        }  
                      if(scale2_set_point2)
                        {
                           if(scale1_value<scale2_set_point2)OUT4=0;
                           else OUT4=1;
                        }                        
                                     
                   }

               
               } 
              else  {OUT2=0;OUT4=0;}


            

           }       
        else
           {
              if(++scale2_seq_err_cntr>50)
               {
                 scale2_seq_err_cntr=0;
                 scale2_value=0;    
                 previous_scale2_value=0;
                 scale2_value=0xFFFFFFFF;   
                 
                 if(temp2!=previous_scale2_error)
                   {
                     previous_scale2_error=temp2;
                     if(temp2=='D')errormsg(NO_ADC_CARD_FOUND_S2_ALARM,5);     
                     if(temp2=='E')errormsg(ADC_TIME_OVER_S2_ALARM,1); 
                   }                     
                 
                 
               }                          
               
           
               
           }      
               

           
       }         
        //********************************//
                      
     if(sendout_pcwork_data_en)
      {
        sendout_pcwork_data_en=0;
        
        if(com1_func==PCWORK_S1 || com1_func==REMOTE_S1)sendout_pcwork_data_s1(0);    //scale 1   via USART0 (COM1)
        if(com1_func==PCWORK_S2 || com1_func==REMOTE_S2)sendout_pcwork_data_s2(0);    //scale 2   via USART0 (COM1)
        
        if(com2_func==PCWORK_S1 || com2_func==REMOTE_S1)sendout_pcwork_data_s1(1);    //scale 1   via USART1 (COM2)
        if(com2_func==PCWORK_S2 || com2_func==REMOTE_S2)sendout_pcwork_data_s2(1);    //scale 2   via USART1 (COM2)
        
      
      }
    

       if(test_lcd_cntr)
        {
               backlight_cntr=0;
               
               BL=1;  
             
              if(clear_all_graph_en)
                {
                  clear_all_graph_en=0;
                  LCD_clear_graph();
                }
              if(set_all_graph_en)
                {
                  set_all_graph_en=0;
                  LCD_set_all_graph();
                }        
             
             insert_delete_key_in_buf_inuse=1;    
             if(apply_key)
               {
                       test_lcd_cntr=TEST_LCD_MAX_TIME;      
                       insert_delete_key_in_buf(0,'d');   
               }   
             insert_delete_key_in_buf_inuse=0;  

           continue;
        }

      if(clear_all_graph_en)
        {
          clear_all_graph_en=0;
          LCD_clear_graph();
        }
      if(clear_home_pos_en)
        {
          if( no_display_change_cntr==0)
             {          
                  clear_home_pos();
                  clear_home_pos_en=0;
             }
        }
      if(display_home_pos_en)     //must be applied after clear_home_pos_en
        {
          if( no_display_change_cntr==0)
             {
                 display_home_pos();
                 display_home_pos_en=0;
             }
        }
      if(display_area_en)
       { 
          if( no_display_change_cntr==0)
             {          
                  display_area_en=0;
                  display_area(); 
             }       
       } 
      if(display_weight_en)
       {
          display_weight_en=0;
          display_weight();
       } 
              

      if(display_blink_handle_en)
        {
          display_blink_handle();
          display_blink_handle_en=0;
        }
      
      if(display_home_pos_message_code)
        {
          clear_home_pos();
          display_home_pos_message(display_home_pos_message_code);
          display_home_pos_message_code=0;
        }
      
     
     insert_delete_key_in_buf_inuse=1;
     if(apply_key)
       {
       
         if(backlight_cntr==0xFFFF)    //BackLight is off
          {
             BL=1; 
          }   
         else key_navigation(apply_key);
          insert_delete_key_in_buf(0,'d');   

          backlight_cntr=0;
         
       } 
     insert_delete_key_in_buf_inuse=0;  
        
     

     
     
     

    }

} 






