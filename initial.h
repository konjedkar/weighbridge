
#include "Times_New_Roman_10_n_bnnn_0_n.h"      
#include "font_Tahoma8.h"  
//#include "7_Segment_24_n_bnnn_0_n.c"     
#include "ARIAL_24_n_bnnn_0_n.c"   
//#include "Font_Gloucester_20.h"


// I2C Bus functions
#asm
   .equ __i2c_port=0x12 ;PORTD
   .equ __sda_bit=7
   .equ __scl_bit=6
#endasm
#include <i2c.h>

// DS1307 Real Time Clock functions
#include <ds1307.h>


//#define KEYBOARD_4x4  1

//defines   
#ifdef KEYBOARD_4x4        

    #define ROW1 PORTC.0 
    #define ROW1_DDR DDRC.0

    #define ROW2 PORTC.1
    #define ROW2_DDR DDRC.1

#define ROW3 PORTC.2
#define ROW3_DDR DDRC.2

#define ROW4 PORTC.3
#define ROW4_DDR DDRC.3

#define COL1 PINC.4 
#define COL1_DDR DDRC.4

#define COL2 PINC.5 
#define COL2_DDR DDRC.5

#define COL3 PINC.6 
#define COL3_DDR DDRC.6  

#define COL4 PINC.7 
#define COL4_DDR DDRC.7  

#else 


#define ROW1 0x04          //PORTG.2
#define ROW1_DDR 0x04     //DDRG.2 

#define ROW2 PORTC.7
#define ROW2_DDR DDRC.7

#define ROW3 PORTC.6
#define ROW3_DDR DDRC.6

#define ROW4 PORTC.5
#define ROW4_DDR DDRC.5

#define COL5 PINC.0 
#define COL5_DDR DDRC.0

#define COL4 PINC.1 
#define COL4_DDR DDRC.1

#define COL3 PINC.2 
#define COL3_DDR DDRC.2  

#define COL2 PINC.3 
#define COL2_DDR DDRC.3   

#define COL1 PINC.4 
#define COL1_DDR DDRC.4

#endif


#define BUZ PORTB.0 
#define BUZ_DDR DDRB.0                                                        

#define ALARM    PORTB.7 
#define ALARM_DDR DDRB.7 

#define BL    PORTD.1 
#define BL_DDR DDRD.1 

#define DIR     PORTD.4 
#define DIR_DDR DDRD.4  

#define SEC_IN     PIND.5 
#define SEC_IN_DDR DDRD.5   


#define LC1_SCK     PORTA.6     
#define LC1_SCK_DDR DDRA.6 

#define LC1_DI      PINA.7   
#define LC1_DI_OUT  PORTA.7     
#define LC1_DI_DDR  DDRA.7

#define LC2_SCK     PORTA.4     
#define LC2_SCK_DDR DDRA.4  

#define LC2_DI      PINA.5     
#define LC2_DI_OUT  PORTA.5   
#define LC2_DI_DDR  DDRA.5


#define OUT1              PORTA.0 
#define OUT1_DDR         DDRA.0 

#define OUT2              PORTA.1 
#define OUT2_DDR         DDRA.1 

#define OUT3              PORTA.2 
#define OUT3_DDR         DDRA.2 

#define OUT4              PORTA.3 
#define OUT4_DDR         DDRA.3 



#define WRITE_PROTECT         (PING&0x10)       //write protection jumper
#define WRITE_PROTECT_OUT   (0x10)       //write protection jumper
#define WRITE_PROTECT_DDR   (~0x10)       //write protection jumper


#define PB_DEBOUNCE   2   //debounce for Push Buttons in ms   

#define SENDOUT_PCWORK_DATA_TIME    10         //*50msec

#define TEST_LCD_MAX_TIME   22          //second

#define BLINK_ON_TIME   10
#define BLINK_OFF_TIME   4

#define TENFOLD_TIMEOUT  100            //10second

#define UPDATE_TIME     347             //update time interval    
#define POWER_UP_ZERO_TIME 10           //10 ==> 1 sec

#define WEIGHING_DATA_SAVE_INDEX   0         
#define CONSEQ1_H_SAVE_INDEX   1         
#define CONSEQ1_L_SAVE_INDEX   2  
#define WRITE_ERROR_INDEX       3     
#define WRITE_ERROR_OV           4 
#define NO_OF_ERRORS_H           5 
#define NO_OF_ERRORS_L           6
#define NOT_DEFINE7                7
#define NOT_DEFINE8                8
#define NOT_DEFINE9                9


 
#define WEIGHING_DATA_START_PAGE_ADDRESS   6        //6*32 byte = the first 1024 bytes of AT24c64 EEPROM is for general purpose

#define START_OF_EEPROM_SAVING_PARAMETERS_ADDRESS      10
#define SIZE_OF_EEPROM_SAVING_PARAMETERS_ADDRESS      180                //   START_OF_EEPROM_SAVING_PARAMETERS_ADDRESS + 
                                                                                             //    SIZE_OF_EEPROM_SAVING_PARAMETERS_ADDRESS<192(6*32)
                                                                                             
  


//**************** ERROR CODE *******************//
#define MIN_CODE_OF_ERROR      11

#define BROWN_OUT_ALARM                          11                                
#define WATCH_DOG_ALARM                           12 
#define POWERUP_ZERO_OVER_S1_ALARM           13                //M 031
#define POWERUP_ZERO_OVER_S2_ALARM           14                //M 031
#define POWERUP_ZERO_UNDER_S1_ALARM         15                 //M 032
#define POWERUP_ZERO_UNDER_S2_ALARM         16                 //M 032
#define OVERLOAD_S1_ALARM                         17                 //M030
#define OVERLOAD_S2_ALARM                         18               //M030
#define NO_ADC_CARD_FOUND_S1_ALARM           19                //s020
#define NO_ADC_CARD_FOUND_S2_ALARM           20                //s020
#define ADC_TIME_OVER_S1_ALARM                 21                  //A010
#define ADC_TIME_OVER_S2_ALARM                 22                 //A010
#define CALIBRATION_LOCKED_ALARM               23                //A08      
#define LOAD_FACTORY_ALARM                       24                //s22    
#define LOAD_EEPROM_PARAMETER_ALARM           25                  //s23     
#define SAVE_EEPROM_PARAMETER_ALARM           26                   //s24

#define MAX_CODE_OF_ERROR      26
//**************** ERROR CODE *******************//


  flash unsigned char errtext11[]="A07";
  flash unsigned char errtext12[]="A06";
  flash unsigned char errtext13[]="M031";     
  flash unsigned char errtext14[]="M031";
  flash unsigned char errtext15[]="M032";
  flash unsigned char errtext16[]="M032";
  flash unsigned char errtext17[]="M030";
  flash unsigned char errtext18[]="M030";
  flash unsigned char errtext19[]="S020";
  flash unsigned char errtext20[]="S020";
  flash unsigned char errtext21[]="A010";
  flash unsigned char errtext22[]="A010";
  flash unsigned char errtext23[]="A08";
  flash unsigned char errtext24[]="S022";
  flash unsigned char errtext25[]="S023";
  flash unsigned char errtext26[]="S024";
  flash unsigned char * flash errorshortcode_table[MAX_CODE_OF_ERROR-MIN_CODE_OF_ERROR+1]=
                                   {errtext11,errtext12,errtext13,errtext14,errtext15,errtext16,errtext17,errtext18,errtext19,
                                     errtext20,errtext21,errtext22,errtext23,errtext24,errtext25,errtext26}; 


                                                                                         
                                                                                             
flash unsigned char MASTER_PASS[]=MASTER_PASSWORD;


#ifdef KEYBOARD_4x4
 #define CLEAR_KEY   'C'
 #define ENTER_KEY   'E'  
 #ifdef ENGLISH
   #define RIGHT_KEY   '6'
   #define LEFT_KEY    '4'  
 #else
   #define RIGHT_KEY   '4'
   #define LEFT_KEY    '6' 
 #endif
 #define UPARROW_KEY 'U'     
 #define PBZERO_KEY  '1'
 #define PBTARE_KEY  '2'     
 #define FUNC_KEY    'F'
 #define MINUS_KEY   'F'
 #define UP_KEY      '2'
 #define DOWN_KEY    '8' 
 #define INFO_KEY    'I'  
 
 flash unsigned char KEYS[20]=
   { 
              '1' , '2' , '3' , 'C' ,
              '4' , '5' , '6' , 'U' ,
              '7' , '8' , '9' , 'I' ,
              '.' , '0' , 'F' , 'E'
   };  
     
#else     

 #define CLEAR_KEY   'C'
 #define ENTER_KEY   'E'  
 #ifdef ENGLISH
    #define RIGHT_KEY   '6'
    #define LEFT_KEY    '4'
 #else    
    #define RIGHT_KEY   '4'
    #define LEFT_KEY    '6' 
 #endif  
 #define UPARROW_KEY 'U'     
 #define PBZERO_KEY  'Z'
 #define PBTARE_KEY  'T'     
 #define FUNC_KEY    'F'
 #define MINUS_KEY   '-'
 #define UP_KEY      '8'
 #define DOWN_KEY    '2' 
 #define INFO_KEY    'I'     
                            
 flash unsigned char KEYS[20]=
   { 
            'I' ,  '7' , '8' , '9' , 'C' ,
            'T' ,  '4' , '5' , '6' , '+' ,
            'Z' ,  '1' , '2' , '3' , 'U' ,
            'F' ,  '.' , '0' , '-' , 'E'
   };  
 
 
#endif


#ifdef KEYBOARD_4x4
 flash unsigned char key_character_table_0[]={'0',' ',0}; 
 flash unsigned char key_character_table_1[]={'1',' ',0};
 flash unsigned char key_character_table_2[]={'2','A','B','C','a','b','c',0};
 flash unsigned char key_character_table_3[]={'3','D','E','F','d','e','f',0};
 flash unsigned char key_character_table_4[]={'4','G','H','I','g','h','i',0};
 flash unsigned char key_character_table_5[]={'5','J','K','L','j','k','l',0};
 flash unsigned char key_character_table_6[]={'6','M','N','O','m','n','o',0};
 flash unsigned char key_character_table_7[]={'7','P','Q','R','S','p','q','r','s',0};
 flash unsigned char key_character_table_8[]={'8','T','U','V','t','u','v',0};
 flash unsigned char key_character_table_9[]={'9','W','X','Y','Z','w','x','y','z',0};    
#else       
 flash unsigned char key_character_table_0[]={'0',' ','/',' ',0}; 
 flash unsigned char key_character_table_1[]={'1','S','T','U','s','t','u',0};
 flash unsigned char key_character_table_2[]={'2','V','W','X','v','w','x',0};
 flash unsigned char key_character_table_3[]={'3','Y','Z','y','z',0};
 flash unsigned char key_character_table_4[]={'4','J','K','L','j','k','l',0};
 flash unsigned char key_character_table_5[]={'5','M','N','O','m','n','o',0};
 flash unsigned char key_character_table_6[]={'6','P','Q','R','p','q','r',0};
 flash unsigned char key_character_table_7[]={'7','A','B','C','a','b','c',0};
 flash unsigned char key_character_table_8[]={'8','D','E','F','d','e','f',0};
 flash unsigned char key_character_table_9[]={'9','G','H','I','g','h','i',0};    
#endif


flash unsigned char * flash key_character_table_n_address[10]=
  {key_character_table_0,key_character_table_1,key_character_table_2,key_character_table_3,key_character_table_4,
   key_character_table_5,key_character_table_6,key_character_table_7,key_character_table_8,key_character_table_9};
   

   
flash unsigned char baud_rate_0[]="  300";   
flash unsigned char baud_rate_1[]="  600";
flash unsigned char baud_rate_2[]=" 1200";
flash unsigned char baud_rate_3[]=" 2400";
flash unsigned char baud_rate_4[]=" 4800";
flash unsigned char baud_rate_5[]=" 9600";
flash unsigned char baud_rate_6[]="19200"; 
flash unsigned char * flash baud_rate_table[7]={baud_rate_0,baud_rate_1,baud_rate_2,baud_rate_3,baud_rate_4,
                                           baud_rate_5,baud_rate_6};    
                                           
flash unsigned char data_bits_table[2]={'7','8'};                                           
flash unsigned char parity_table[3]={'N','E','O'};
flash unsigned char stop_bits_table[2]={'1','2'};



#define  PRINTER        0
#define  REMOTE_S1     1
#define  PCWORK_S1     2
#define  REMOTE_S2     3
#define  PCWORK_S2     4
#define  RS485           5
#define  NOTUSE         6  

#ifdef ENGLISH
flash unsigned char com_func_table_0[]="Printer";  
flash unsigned char com_func_table_1[]="Remote Disp. S1"; 
flash unsigned char com_func_table_2[]="PC (W. Out) S1";
flash unsigned char com_func_table_3[]="Remote Disp. S2";
flash unsigned char com_func_table_4[]="PC (W. Out) S2";
flash unsigned char com_func_table_5[]="RS485 BUS";
flash unsigned char com_func_table_6[]="Not Used";
#else
flash unsigned char com_func_table_0[]="¤ğ•‘";   
flash unsigned char com_func_table_1[]="S1 ùşø÷‘™ ¤ğªş‘õ÷";
flash unsigned char com_func_table_2[]="S1 ¤—øş•õ‘î ü›ø¤¡";
flash unsigned char com_func_table_3[]="S2 ùşø÷‘™ ¤ğªş‘õ÷";
flash unsigned char com_func_table_4[]="S2 ¤—øş•õ‘î ü›ø¤¡";   
flash unsigned char com_func_table_5[]="RS485 §‘“";
flash unsigned char com_func_table_6[]="¢¤“¤‘î öø¢“";
#endif
flash unsigned char * flash com_func_table[7]={com_func_table_0,com_func_table_1,com_func_table_2,com_func_table_3,
                                                                     com_func_table_4,com_func_table_5,com_func_table_6}; 


#define SCHENCK  1
#define SYSTEC   2
#define FILINTEC  3
#define DDP8785   4
#define TOLIDO   5
#define OFF      0
flash unsigned char protocol_table_0[]="OFF";   
flash unsigned char protocol_table_1[]="SCHENCK3964R";
flash unsigned char protocol_table_2[]="SYSTEC";
flash unsigned char protocol_table_3[]="FILINTEC";
flash unsigned char protocol_table_4[]="DDP8785";
flash unsigned char protocol_table_5[]="TOLIDO";
flash unsigned char * flash protocol_table[6]={protocol_table_0,protocol_table_1,protocol_table_2,protocol_table_3,
                                          protocol_table_4,protocol_table_5}; 


#ifdef ENGLISH
flash unsigned char language_table_0[]="English   ";   
flash unsigned char language_table_1[]="Farsi      ";    
#else
flash unsigned char language_table_0[]="ü¨şóğ÷    ";   
flash unsigned char language_table_1[]="ü¨¤‘ê      "; 
#endif
flash unsigned char * flash language_table[2]={language_table_0,language_table_1};    


#define HIDEINFO  0
#define SHOWINFO  1

#ifdef ENGLISH
flash unsigned char homepos_table_0[]="Hidden Info";
flash unsigned char homepos_table_1[]="Display Info";    
#else
flash unsigned char homepos_table_0[]="ö¢¤î üê¡õ";
flash unsigned char homepos_table_1[]="   ©ş‘õ÷  ";    
#endif
flash unsigned char * flash homepos_table[2]={homepos_table_0,homepos_table_1};    
                                                                                                           

#ifdef ENGLISH
flash unsigned char calendar_table_0[]="Gregorian ";   
flash unsigned char calendar_table_1[]="Jalali    ";
#else
flash unsigned char calendar_table_0[]="ı¢‘óşõ     ";   
flash unsigned char calendar_table_1[]="ü¨õª ı¤›û";
#endif
flash unsigned char * flash calendar_table[2]={calendar_table_0,calendar_table_1};



#define ALARM_ON   1
#define ALARM_OFF  0
#ifdef ENGLISH
flash unsigned char alarm_table_0[]="Mute";
flash unsigned char alarm_table_1[]="Alarm";    
#else
flash unsigned char alarm_table_0[]="©øõ‘¡";
flash unsigned char alarm_table_1[]="öªø¤";    
#endif
flash unsigned char * flash alarm_table[2]={alarm_table_0,alarm_table_1};    




#define SW_NONE         0
#define SW_SIMPLE     1
#ifdef ENGLISH
flash unsigned char software_table_0[]="None       ";   
flash unsigned char software_table_1[]="Simple    ";   
flash unsigned char software_table_2[]="ZEUS       ";   
flash unsigned char software_table_3[]="FILING    ";
flash unsigned char software_table_4[]="BEALT      ";
flash unsigned char software_table_5[]="CHK Weigher";
#else
flash unsigned char software_table_0[]="ü›ø¤¡ öø¢“";   
flash unsigned char software_table_1[]="ù¢‘¨        ";   
flash unsigned char software_table_2[]="ZEUS       ";   
flash unsigned char software_table_3[]="FILING    ";
flash unsigned char software_table_4[]="BEALT      ";
flash unsigned char software_table_5[]="CHK Weigher";
#endif
flash unsigned char * flash software_table[6]={software_table_0,software_table_1,software_table_2
                                                                    ,software_table_3,software_table_4,software_table_5};   
    



flash unsigned char dongle_table_0[]="AD7780 ";   
flash unsigned char dongle_table_1[]="AD7730 ";
flash unsigned char dongle_table_2[]="NO DATA";
flash unsigned char dongle_table_3[]="NO DATA";
flash unsigned char * flash dongle_table[4]={dongle_table_0,dongle_table_1,dongle_table_2,dongle_table_3};
                                          

                             
#ifdef ENGLISH                                                                                                 
flash unsigned char scale_range_table_0[]="Single(1)";   
flash unsigned char scale_range_table_1[]="Dual  (2)";
flash unsigned char scale_range_table_2[]="Triple(3)";
#else
flash unsigned char scale_range_table_0[]="ıù÷ş¥ í—";   
flash unsigned char scale_range_table_1[]="ıù÷ş¥ ø¢";
flash unsigned char scale_range_table_2[]="ıù÷ş¥ ù¨";
#endif
flash unsigned char * flash scale_range_table[3]={scale_range_table_0,scale_range_table_1,scale_range_table_2};     

flash unsigned char scale_unit_table_0[]="g ";   
flash unsigned char scale_unit_table_1[]="Kg";
flash unsigned char scale_unit_table_2[]="Tn";
flash unsigned char * flash scale_unit_table[3]={scale_unit_table_0,scale_unit_table_1,scale_unit_table_2};  

#ifdef ENGLISH
flash unsigned char scale_digits_table_0[]=" 4 Digits ";   
flash unsigned char scale_digits_table_1[]=" 5 Digits "; 
flash unsigned char scale_digits_table_2[]=" 6 Digits ";
#else
flash unsigned char scale_digits_table_0[]="üõì¤ ¤‘ú ";   
flash unsigned char scale_digits_table_1[]="üõì¤ ´÷•  "; 
flash unsigned char scale_digits_table_2[]="üõì¤ ©ª   ";
#endif 
flash unsigned char * flash scale_digits_table[3]={scale_digits_table_0,scale_digits_table_1,scale_digits_table_2}; 


#ifdef ENFGLISH
flash unsigned char scale_float_table_0[]=" No Digit ";   
flash unsigned char scale_float_table_1[]=" 1 Digit  ";   
flash unsigned char scale_float_table_2[]=" 2 Digits "; 
flash unsigned char scale_float_table_3[]=" 3 Digits ";
#else
flash unsigned char scale_float_table_0[]="¤‘ªä öø¢“";   
flash unsigned char scale_float_table_1[]="ôì¤ íş";   
flash unsigned char scale_float_table_2[]="ôì¤ ø¢"; 
flash unsigned char scale_float_table_3[]="ôì¤ ù¨";
#endif 
flash unsigned char * flash scale_float_table[4]={scale_float_table_0,scale_float_table_1,scale_float_table_2,scale_float_table_3}; 


#define NUMBER_OF_SELECT_SCALE_TABLE  3   //3 must be 4 for enable sum of scales
#ifdef ENGLISH
flash unsigned char select_scale_table_0[]="Scale 1 and 2"; 
flash unsigned char select_scale_table_1[]="Only Scale 1 ";   
flash unsigned char select_scale_table_2[]="Only Scale 2 "; 
flash unsigned char select_scale_table_3[]="Sum of Scales";
#else
flash unsigned char select_scale_table_0[]="ñøî¨‘“ ø¢ ¤û"; 
flash unsigned char select_scale_table_1[]=" ñøî¨‘“ ¯ìê";   
flash unsigned char select_scale_table_2[]="‚ ñøî¨‘“ ¯ìê";                
flash unsigned char select_scale_table_3[]="‘ûñøî¨‘“ áøõ›õ"; 
#endif 
flash unsigned char * flash select_scale_table[4]={select_scale_table_0,select_scale_table_1,select_scale_table_2,select_scale_table_3};


#ifdef ENGLISH
flash unsigned char attributes_table_0[]="Bold";   
flash unsigned char attributes_table_1[]="Underlined"; 
flash unsigned char attributes_table_2[]="Expanded"; 
flash unsigned char attributes_table_3[]="Condensed"; 
flash unsigned char attributes_table_4[]="Italic"; 
flash unsigned char attributes_table_5[]="Special  1"; 
flash unsigned char attributes_table_6[]="Special  2"; 
flash unsigned char attributes_table_7[]="No Attr.";
#else
flash unsigned char attributes_table_0[]="ôş¡®";   
flash unsigned char attributes_table_1[]="¤¢ ¯¡¤ş¥"; 
flash unsigned char attributes_table_2[]="ù¢ª ¢÷•¨î"; 
flash unsigned char attributes_table_3[]="ôî¤—õ"; 
flash unsigned char attributes_table_4[]="íşó‘—ş"; 
flash unsigned char attributes_table_5[]=" ù¦şø"; 
flash unsigned char attributes_table_6[]="‚ ù¦şø"; 
flash unsigned char attributes_table_7[]="–ó‘Ÿ öø¢“";
#endif 
flash unsigned char * flash attributes_table[8]={attributes_table_0,attributes_table_1,attributes_table_2,attributes_table_3,
                                              attributes_table_4,attributes_table_5,attributes_table_6,attributes_table_7};     
                                              
                                              
#define NUMBER_OF_EMBED_TEXTS  13
flash unsigned char texts_table_0[]="Customer";  
flash unsigned char texts_table_1[]="Date";  
flash unsigned char texts_table_2[]="Time";   
flash unsigned char texts_table_3[]="Ticket No";   
flash unsigned char texts_table_4[]="Consec. No";   
flash unsigned char texts_table_5[]="Gross";   
flash unsigned char texts_table_6[]="Tare";   
flash unsigned char texts_table_7[]="Net";   
flash unsigned char texts_table_8[]="Total";   
flash unsigned char texts_table_9[]="Parts";  
flash unsigned char texts_table_10[]="Piece Wgt(g)"; 
flash unsigned char texts_table_11[]="Sort";
flash unsigned char texts_table_12[]="Scale No";
flash unsigned char * flash texts_table[NUMBER_OF_EMBED_TEXTS]=
                                             {texts_table_0,texts_table_1,texts_table_2,texts_table_3,
                                              texts_table_4,texts_table_5,texts_table_6,texts_table_7,
                                              texts_table_8,texts_table_9,texts_table_10,texts_table_11,
                                              texts_table_12};        
                                              
#ifdef ENGLISH                                          
flash unsigned char field_types_table_0[]="Fetch";   
flash unsigned char field_types_table_1[]="Input"; 
flash unsigned char field_types_table_2[]="Calculate"; 
flash unsigned char field_types_table_3[]="Text"; 
#else
flash unsigned char field_types_table_0[]="¤—õ¤‘• ¤¢ìõ";   
flash unsigned char field_types_table_1[]="ı¢ø¤ø"; 
flash unsigned char field_types_table_2[]="ù“¨‘Ÿõ"; 
flash unsigned char field_types_table_3[]="–“‘™ ö—õ"; 
#endif
flash unsigned char * flash field_types_table[4]={field_types_table_0,field_types_table_1,field_types_table_2,field_types_table_3};   


#define NUMBER_OF_FETCH_TEXTS  17
#ifdef ENGLISH
flash unsigned char fetch_types_table_0[]="*Hijri Date*";   
flash unsigned char fetch_types_table_1[]="*Date*";   
flash unsigned char fetch_types_table_2[]="*Time*";   
flash unsigned char fetch_types_table_3[]="*Consec. No1*";   
flash unsigned char fetch_types_table_4[]="*Consec. No2*";   
flash unsigned char fetch_types_table_5[]="*Gross*";   
flash unsigned char fetch_types_table_6[]="*Tare*";   
flash unsigned char fetch_types_table_7[]="*Net Weight*";   
flash unsigned char fetch_types_table_8[]="*Unit*";   
flash unsigned char fetch_types_table_9[]="*Av. Piece Wgt*";  
flash unsigned char fetch_types_table_10[]="*1st Weight*"; 
flash unsigned char fetch_types_table_11[]="*2nd Weight*"; 
flash unsigned char fetch_types_table_12[]="*ID*"; 
flash unsigned char fetch_types_table_13[]="*Inbound*"; 
flash unsigned char fetch_types_table_14[]="*Customer*"; 
flash unsigned char fetch_types_table_15[]="*Sort*"; 
flash unsigned char fetch_types_table_16[]="*ScaleNo*"; 
#else
flash unsigned char fetch_types_table_0[]="ü¨õª ¸ş¤‘—";   
flash unsigned char fetch_types_table_1[]="ı¢‘óşõ ¸ş¤‘—";   
flash unsigned char fetch_types_table_2[]="–ä‘¨";   
flash unsigned char fetch_types_table_3[]=" ñ‘ş¤¨ ù¤‘õª";   
flash unsigned char fetch_types_table_4[]="‚ ñ‘ş¤¨ ù¤‘õª";   
flash unsigned char fetch_types_table_5[]="«ó‘¡‘÷ ö¥ø";   
flash unsigned char fetch_types_table_6[]="ï÷¨¤‘• ö¥ø";   
flash unsigned char fetch_types_table_7[]="«ó‘¡ ö¥ø";   
flash unsigned char fetch_types_table_8[]="ı¤şğ ù¥¢÷ ¢Ÿø";   
flash unsigned char fetch_types_table_9[]="ùã¯ì ö¥ø öşğ÷‘şõ";  
flash unsigned char fetch_types_table_10[]="ñø öş¥ø—"; 
flash unsigned char fetch_types_table_11[]="ôø¢ öş¥ø—"; 
flash unsigned char fetch_types_table_12[]="öşª‘õ ù¤‘õª"; 
flash unsigned char fetch_types_table_13[]="­“ì ¢î"; 
flash unsigned char fetch_types_table_14[]="ı¤—ªõ ô‘÷"; 
flash unsigned char fetch_types_table_15[]="‘ó‘î ¢î"; 
flash unsigned char fetch_types_table_16[]="ñøî¨‘“ ù¤‘õª";
#endif
flash unsigned char * flash fetch_types_table[NUMBER_OF_FETCH_TEXTS]={fetch_types_table_0,fetch_types_table_1,fetch_types_table_2,fetch_types_table_3,
                                              fetch_types_table_4,fetch_types_table_5,fetch_types_table_6,fetch_types_table_7,
                                              fetch_types_table_8,fetch_types_table_9,fetch_types_table_10,
                                              fetch_types_table_11,fetch_types_table_12,fetch_types_table_13,
                                              fetch_types_table_14,fetch_types_table_15,fetch_types_table_16};   
                                               

#ifdef ENGLISH                                              
flash unsigned char motion_window_table_0[]=" 0.5 Div ";   
flash unsigned char motion_window_table_1[]=" 1.0 Div ";   
flash unsigned char motion_window_table_2[]=" 2.0 Div ";   
flash unsigned char motion_window_table_3[]=" 3.0 Div ";   
#else
flash unsigned char motion_window_table_0[]="–õ¨ì ôş÷";   
flash unsigned char motion_window_table_1[]="–õ¨ì   ";   
flash unsigned char motion_window_table_2[]="–õ¨ì ‚  ";   
flash unsigned char motion_window_table_3[]="–õ¨ì ƒ  ";  
#endif
flash unsigned char * flash motion_window_table[4]={motion_window_table_0,motion_window_table_1,
                                                       motion_window_table_2,motion_window_table_3};   
                                                       
                                                                                                     
#ifdef ENGLISH                                              
flash unsigned char auto_zero_range_table_0[]="  OFF    ";   
flash unsigned char auto_zero_range_table_1[]=" 0.5 Div ";   
flash unsigned char auto_zero_range_table_2[]=" 1.0 Div ";   
flash unsigned char auto_zero_range_table_3[]=" 3.0 Div ";
#else
flash unsigned char auto_zero_range_table_0[]="ñ‘ãê ¤şè";   
flash unsigned char auto_zero_range_table_1[]="–õ¨ì ôş÷";   
flash unsigned char auto_zero_range_table_2[]="–õ¨ì ";   
flash unsigned char auto_zero_range_table_3[]="–õ¨ì ƒ";
#endif   
flash unsigned char * flash auto_zero_range_table[4]={auto_zero_range_table_0,auto_zero_range_table_1,
                                                       auto_zero_range_table_2,auto_zero_range_table_3}; 
                                                       
#ifdef ENGLISH  
flash unsigned char powerup_zero_table_0[]="  OFF    ";   
flash unsigned char powerup_zero_table_1[]="2 percent";   
flash unsigned char powerup_zero_table_2[]="10percent";
#else
flash unsigned char powerup_zero_table_0[]="ñ‘ãê¤şè";   
flash unsigned char powerup_zero_table_1[]="¢¬¤¢ ‚  ";   
flash unsigned char powerup_zero_table_2[]="¢¬¤¢ € ";
#endif   
flash unsigned char * flash powerup_zero_table[3]={powerup_zero_table_0,powerup_zero_table_1,powerup_zero_table_2};                                                                                                             
                                               
//eeprom 
#define NUMBER_OF_USER_TEXTS  16 
#define USER_TEXTS_LENGHT       15                                           

eeprom unsigned char user_defines_texts_table_0[]= "USERDEF TEXT 00"; 
eeprom unsigned char user_defines_texts_table_1[]= "USERDEF TEXT 01";   
eeprom unsigned char user_defines_texts_table_2[]= "USERDEF TEXT 02";   
eeprom unsigned char user_defines_texts_table_3[]= "USERDEF TEXT 03";        
eeprom unsigned char user_defines_texts_table_4[]= "USERDEF TEXT 04";   
eeprom unsigned char user_defines_texts_table_5[]= "USERDEF TEXT 05";   
eeprom unsigned char user_defines_texts_table_6[]= "USERDEF TEXT 06";   
eeprom unsigned char user_defines_texts_table_7[]= "USERDEF TEXT 07";   
eeprom unsigned char user_defines_texts_table_8[]= "USERDEF TEXT 08";   
eeprom unsigned char user_defines_texts_table_9[]= "USERDEF TEXT 09"; 
eeprom unsigned char user_defines_texts_table_10[]="USERDEF TEXT 10";   
eeprom unsigned char user_defines_texts_table_11[]="USERDEF TEXT 11";   
eeprom unsigned char user_defines_texts_table_12[]="USERDEF TEXT 12";   
eeprom unsigned char user_defines_texts_table_13[]="USERDEF TEXT 13";   
eeprom unsigned char user_defines_texts_table_14[]="USERDEF TEXT 14";   
eeprom unsigned char user_defines_texts_table_15[]="USERDEF TEXT 15";   
eeprom unsigned char * flash user_defines_texts_table[NUMBER_OF_USER_TEXTS]=
                {user_defines_texts_table_0,user_defines_texts_table_1,user_defines_texts_table_2,user_defines_texts_table_3,
                 user_defines_texts_table_4,user_defines_texts_table_5,user_defines_texts_table_6,user_defines_texts_table_7,
                 user_defines_texts_table_8,user_defines_texts_table_9,user_defines_texts_table_10,user_defines_texts_table_11,
                 user_defines_texts_table_12,user_defines_texts_table_13,user_defines_texts_table_14,user_defines_texts_table_15};  



 

#define NUM_OF_FIRST_WEIGHING_MEMORY  200
struct weighing_data_record_format
  {
    unsigned char first_weighing_value_msb;
    unsigned char first_weighing_value_mid;
    unsigned char first_weighing_value_lsb;
    
    unsigned char vehicle_no_msb;
    unsigned char vehicle_no_mid;
    unsigned char vehicle_no_lsb;    

    unsigned int conseq1_serial ;
   
    //unsigned char sort_code;    
                                                   
    unsigned char date_day ;
    unsigned char date_month ;                                        
    unsigned char date_year ;                              
    
    unsigned char scale_no ;
    unsigned char time_hour ;
    unsigned char time_minute ;
    unsigned char time_second ;    
    
    unsigned char sort_code ; 
    
    unsigned char customer_name[17];     
  
  }weighing_data;   
  
//eeprom unsigned char EE_weighing_data_save_index=0;    

#define SORT_NAMES_INITIAL_ADDRESS  (NUM_OF_FIRST_WEIGHING_MEMORY+WEIGHING_DATA_START_PAGE_ADDRESS)*32    //(200+6)*32 = 6592
#define NUM_OF_SORT_NAMES       100                                                                                                    // 6592 + 100*16 = 8192

  
  
#define FIXED_TARE_INBOUND_CODES  100       
eeprom struct fixed_tare_msb_msb_record_format
  { 
    unsigned char fixed_tare_msb;                 
    unsigned char fixed_tare_mid;            
    unsigned char fixed_tare_lsb;      
  
  }fixed_tare[FIXED_TARE_INBOUND_CODES]={0};
eeprom unsigned char EE_fixed_tare_save_index;  


#define SORT_CODES  100
   

  
//EEPROM parameters     


//ALL of Below parameters until  EE_END_OF_SETTINGS are copied in AT24c64 External EEPROM  directly (MAX:192 byte)   



//  EE_START_OF_SETTINGS  is must the FIRST BYTE     

eeprom unsigned char EE_START_OF_SETTINGS;             
      /*
eeprom unsigned char EE_scales_func_mode=0,EE_geo_value=10;  //0 for both -  1 only scale1  -  2 only scale2  -  3 for sum 
eeprom unsigned char EE_scale1_ranges=0,EE_scale2_ranges=0;                 //0 single range   1 double range    2 triple range
eeprom unsigned long int EE_scale1_capacity[3]={300000,400000,500000},EE_scale2_capacity[3]={4000000,5500000,10000000};
eeprom unsigned long int EE_scale1_interval[3]={10,20,40},EE_scale2_interval[3]={10000,15000,30000};
eeprom unsigned char EE_scale1_digits=2,EE_scale2_digits=2;   //0->4 1->5 2->6
eeprom unsigned char EE_scale1_float=0,EE_scale2_float=0;   //0->0 1->1 2->2  3->3
eeprom unsigned char EE_scale1_unit=0,EE_scale2_unit=01;       //0->g 1->kg 2->t    
eeprom unsigned char EE_scale1_interval_unit=0,EE_scale2_interval_unit=0;       //0->g 1->kg 2->t 
eeprom unsigned char EE_scale1_capacity_unit=1,EE_scale2_capacity_unit=1;       //0->g 1->kg 2->t 
eeprom unsigned long int EE_scale1_set_point1=1000000,EE_scale1_set_point2=1500000;    
eeprom unsigned long int EE_scale2_set_point1=1000000,EE_scale2_set_point2=1500000;

eeprom unsigned char EE_scale1_motion_window=0,EE_scale2_motion_window=0;       //3->3.0D 2->2.0D 1->1.0D 0->0.5D
eeprom unsigned char EE_scale1_motion_counter=20,EE_scale2_motion_counter=10;   //1..20
eeprom unsigned char EE_scale1_filetr_size=11,EE_scale2_filetr_size=11;
eeprom unsigned char EE_scale1_auto_zero_range=3,EE_scale2_auto_zero_range=1;   //3->3.0D  2->1.0D 1->0.5D  0->OFF
eeprom unsigned char EE_scale1_pb_zero_p=2,EE_scale2_pb_zero_p=2;   //0..99
eeprom unsigned char EE_scale1_pb_zero_n=2,EE_scale2_pb_zero_n=2;   //0..99
eeprom unsigned char EE_scale1_powerup_zero=0,EE_scale2_powerup_zero=0;   //0-> OFF  1->2%  2->10%
eeprom unsigned char EE_scale1_overload=9,EE_scale2_overload=9;   //1..20

eeprom unsigned long int EE_scale1_default_calib_weight=1000,EE_scale2_default_calib_weight=1000; 

eeprom unsigned char EE_scale1_zero_mv_v_str[10]={'0','.','0','0','0','0','0',0},EE_scale1_load_mv_v_str[10]={'2','.','0','0','0','1',0};
eeprom unsigned char EE_scale2_zero_mv_v_str[10],EE_scale2_load_mv_v_str[10];
eeprom unsigned char EE_scale1_loadcells_no=1,EE_scale2_loadcells_no=1;              


eeprom unsigned char EE_com1_baud_rate=5,EE_com2_baud_rate=3;   //300 600 1200 2400 4800 9600 19200
eeprom unsigned char EE_com1_databits=1,EE_com2_databits=1;     // 7  8
eeprom unsigned char EE_com1_parity=0,EE_com2_parity=0;         // no even odd
eeprom unsigned char EE_com1_stop_bits=0,EE_com2_stop_bits=0;   //1 2      

eeprom unsigned char EE_com1_func=NOTUSE,EE_com2_func=PRINTER;   //0 1 2 3
eeprom unsigned char EE_com1_protocol=0,EE_com2_protocol=0;   //0 1 2 3
eeprom unsigned char EE_language=0; 
eeprom unsigned char EE_software=0;
eeprom unsigned char EE_backlight_time =0;
eeprom unsigned char EE_dongletype=0;   
eeprom unsigned char EE_homepos=1;      
eeprom unsigned char EE_calendar=1; 
                                                  */
                                                  
                                                  
                                                  
eeprom unsigned char EE_scales_func_mode=DEFAULT_scales_func_mode;
eeprom unsigned char EE_geo_value=DEFAULT_geo_value;  //0 for both -  1 only scale1  -  2 only scale2  -  3 for sum 
eeprom unsigned char EE_scale1_ranges=DEFAULT_scale1_ranges;
eeprom unsigned char EE_scale2_ranges=DEFAULT_scale2_ranges;                 //0 single range   1 double range    2 triple range
eeprom unsigned long int EE_scale1_capacity[3]={DEFAULT_scale1_capacity0,DEFAULT_scale1_capacity2,DEFAULT_scale1_capacity2};
eeprom unsigned long int EE_scale2_capacity[3]={DEFAULT_scale2_capacity0,DEFAULT_scale2_capacity1,DEFAULT_scale2_capacity2};
eeprom unsigned long int EE_scale1_interval[3]={DEFAULT_scale1_interval0,DEFAULT_scale1_interval1,DEFAULT_scale1_interval2};
eeprom unsigned long int EE_scale2_interval[3]={DEFAULT_scale2_interval0,DEFAULT_scale2_interval1,DEFAULT_scale2_interval2};
eeprom unsigned char EE_scale1_digits=DEFAULT_scale1_digits;
eeprom unsigned char EE_scale2_digits=DEFAULT_scale2_digits;   //0->4 1->5 2->6
eeprom unsigned char EE_scale1_float=DEFAULT_scale1_float;
eeprom unsigned char EE_scale2_float=DEFAULT_scale2_float;   //0->0 1->1 2->2  3->3
eeprom unsigned char EE_scale1_unit=DEFAULT_scale1_unit;
eeprom unsigned char EE_scale2_unit=DEFAULT_scale2_unit;       //0->g 1->kg 2->t    
eeprom unsigned char EE_scale1_interval_unit=DEFAULT_scale1_interval_unit;
eeprom unsigned char EE_scale2_interval_unit=DEFAULT_scale2_interval_unit;       //0->g 1->kg 2->t 
eeprom unsigned char EE_scale1_capacity_unit=DEFAULT_scale1_capacity_unit;
eeprom unsigned char EE_scale2_capacity_unit=DEFAULT_scale2_capacity_unit;       //0->g 1->kg 2->t 
eeprom unsigned long int EE_scale1_set_point1=DEFAULT_scale1_set_point1;
eeprom unsigned long int EE_scale1_set_point2=DEFAULT_scale1_set_point2;    
eeprom unsigned long int EE_scale2_set_point1=DEFAULT_scale1_set_point1;
eeprom unsigned long int EE_scale2_set_point2=DEFAULT_scale2_set_point2;

eeprom unsigned char EE_scale1_loadcells_no=DEFAULT_scale1_loadcells_no;
eeprom unsigned char EE_scale2_loadcells_no=DEFAULT_scale2_loadcells_no;
eeprom unsigned char EE_scale1_motion_window=DEFAULT_scale1_motion_window;
eeprom unsigned char EE_scale2_motion_window=DEFAULT_scale2_motion_window;       //3->3.0D 2->2.0D 1->1.0D 0->0.5D
eeprom unsigned char EE_scale1_motion_counter=DEFAULT_scale1_motion_counter;
eeprom unsigned char EE_scale2_motion_counter=DEFAULT_scale2_motion_counter;   //1..20
eeprom unsigned char EE_scale1_filetr_size=DEFAULT_scale1_filetr_size;
eeprom unsigned char EE_scale2_filetr_size=DEFAULT_scale2_filetr_size;
eeprom unsigned char EE_scale1_auto_zero_range=DEFAULT_scale1_auto_zero_range;
eeprom unsigned char EE_scale2_auto_zero_range=DEFAULT_scale2_auto_zero_range;   //3->3.0D  2->1.0D 1->0.5D  0->OFF
eeprom unsigned char EE_scale1_pb_zero_p=DEFAULT_scale1_pb_zero_p;
eeprom unsigned char EE_scale2_pb_zero_p=DEFAULT_scale2_pb_zero_p;   //0..99
eeprom unsigned char EE_scale1_pb_zero_n=DEFAULT_scale1_pb_zero_n;
eeprom unsigned char EE_scale2_pb_zero_n=DEFAULT_scale2_pb_zero_n;   //0..99
eeprom unsigned char EE_scale1_powerup_zero=DEFAULT_scale1_powerup_zero;
eeprom unsigned char EE_scale2_powerup_zero=DEFAULT_scale2_powerup_zero;   //0-> OFF  1->2%  2->10%
eeprom unsigned char EE_scale1_overload=DEFAULT_scale1_overload;
eeprom unsigned char EE_scale2_overload=DEFAULT_scale2_overload;   //1..20

eeprom unsigned long int EE_scale1_default_calib_weight=DEFAULT_scale1_default_calib_weight;
eeprom unsigned long int EE_scale2_default_calib_weight=DEFAULT_scale2_default_calib_weight; 

eeprom unsigned char EE_scale1_zero_mv_v_str[10]={'0','.','0','0','0','0','0',0};
eeprom unsigned char EE_scale1_load_mv_v_str[10]={'2','.','0','0','0','1',0};
eeprom unsigned char EE_scale2_zero_mv_v_str[10]={'0','.','0','0','0','0','0',0};
eeprom unsigned char EE_scale2_load_mv_v_str[10]={'2','.','0','0','0','1',0};

eeprom unsigned char EE_com1_baud_rate=DEFAULT_com1_baud_rate;
eeprom unsigned char EE_com2_baud_rate=DEFAULT_com2_baud_rate;   //300 600 1200 2400 4800 9600 19200
eeprom unsigned char EE_com1_databits=DEFAULT_com1_databits;
eeprom unsigned char EE_com2_databits=DEFAULT_com2_databits;     // 7  8
eeprom unsigned char EE_com1_parity=DEFAULT_com1_parity;
eeprom unsigned char EE_com2_parity=DEFAULT_com2_parity;         // no even odd
eeprom unsigned char EE_com1_stop_bits=DEFAULT_com1_stop_bits;
eeprom unsigned char EE_com2_stop_bits=DEFAULT_com2_stop_bits;   //1 2      

eeprom unsigned char EE_com1_func=DEFAULT_com1_func;
eeprom unsigned char EE_com2_func=DEFAULT_com2_func;   //0 1 2 3
eeprom unsigned char EE_com1_protocol=DEFAULT_com1_protocol;
eeprom unsigned char EE_com2_protocol=DEFAULT_com2_protocol;   //0 1 2 3
eeprom unsigned char EE_language=DEFAULT_language; 
eeprom unsigned char EE_software=DEFAULT_software;
eeprom unsigned char EE_backlight_time =DEFAULT_backlight_time;
eeprom unsigned char EE_dongletype=DEFAULT_dongletype;      
eeprom unsigned char EE_homepos=DEFAULT_homepos;      
eeprom unsigned char EE_calendar=DEFAULT_calendar;   
eeprom unsigned char EE_alarm=DEFAULT_alarm;                                                
                                                  
                                                  

eeprom unsigned char EE_END_OF_SETTINGS;


//***************ERROR LIST**********************//
#define SIZE_OF_ERROR_LIST     200  
eeprom struct ErrorOb
      {
       unsigned char ErrorSec;
       unsigned char ErrorMin;                                               
       unsigned char ErrorHour ;
       unsigned char ErrorDate ;        
       unsigned char ErrorMonth;
       unsigned char ErrorYear ;
       unsigned char ErrorQty  ;
       unsigned char ErrorNum  ;                                  
      }ErrorHistory[SIZE_OF_ERROR_LIST]; 


//*************************************************//

#define NUMBER_OF_FIELDS   128
eeprom struct print_data_format
  {
   unsigned char block;
   unsigned char line;   
   unsigned char column;      
   unsigned char attribute;
   unsigned char type;
   unsigned char data_index;
  }field[NUMBER_OF_FIELDS+1]={0}; 
unsigned char sorted_field_table[NUMBER_OF_FIELDS];


//*************************************************//



unsigned char LC1_status=0,LC2_status=0,scale1_data_read_en=0,scale2_data_read_en=0;
unsigned long int  scale1_manual_tare=0,scale1_pb_tare=0;
unsigned long int  scale2_manual_tare=0,scale2_pb_tare=0;   

//parameters  
unsigned char scales_func_mode,geo_value;  //0 for both -  1 only scale1  -  2 only scale2  -  3 for sum 

unsigned char scale1_ranges,scale2_ranges;                 //0 single range   1 double range    2 triple range
unsigned long int scale1_capacity[3],scale2_capacity[3];
unsigned long int scale1_interval[3],scale2_interval[3];
unsigned char scale1_digits,scale2_digits;   //0->4 1->5 2->6
unsigned char scale1_float,scale2_float;      //0->0 1->1 2->2  3->3
unsigned char scale1_unit,scale2_unit;          //0->g 1->kg 2->t 
unsigned char scale1_interval_unit,scale2_interval_unit;       //0->g 1->kg 2->t 
unsigned char scale1_capacity_unit,scale2_capacity_unit;       //0->g 1->kg 2->t     
unsigned long int  scale1_default_calib_weight; 
unsigned char scale1_loadcells_no,scale2_loadcells_no;         



unsigned char scale1_motion_window,scale2_motion_window;       //3->3.0D 2->2.0D 1->1.0D 0->0.5D
unsigned char scale1_motion_counter,scale2_motion_counter;   //1..20
unsigned char scale1_filetr_size,scale2_filetr_size;
unsigned char scale1_auto_zero_range,scale2_auto_zero_range;   //3->3.0D  2->1.0D 1->0.5D  0->OFF
unsigned char scale1_pb_zero_p,scale2_pb_zero_p;   //0..99
unsigned char scale1_pb_zero_n,scale2_pb_zero_n;   //0..99
unsigned char scale1_powerup_zero,scale2_powerup_zero;   //0-> OFF  1->2%  2->10%
unsigned char scale1_overload,scale2_overload;   //1..20     
unsigned long int  scale2_default_calib_weight;


float scale1_zero_mv_v,scale1_load_mv_v; 
float scale2_zero_mv_v,scale2_load_mv_v;

unsigned char com1_baud_rate,com2_baud_rate;   //300 600 1200 2400 4800 9600 19200
unsigned char com1_databits,com2_databits;     // 7  8
unsigned char com1_parity,com2_parity;         // no even odd
unsigned char com1_stop_bits,com2_stop_bits;   //1 2      

unsigned char com1_func=NOTUSE,com2_func=PRINTER;   //0 1 2 3
unsigned char com1_protocol,com2_protocol;   //0 1 2 3 4 5
unsigned char language; 
unsigned char software;
unsigned char backlight_time ;
unsigned char dongletype;    
unsigned char homepos;   
unsigned char calendar;

unsigned char alarm;  

//variables            
//unsigned char temp_str[48];       
unsigned char scale1_seq_err_cntr=0,scale2_seq_err_cntr=0;
unsigned char scale1_mode=0;    //'z' zero   'f' fix   0 unstable  
unsigned char scale2_mode=0;    //'z' zero   'f' fix   0 unstable
unsigned long int  scale1_value=0,scale1_set_point1,scale1_set_point2,scale1_raw_value=0x80000;          
float          scale1_poweron_offset_zero_value=0,scale2_poweron_offset_zero_value=0;
float          scale_float_value=0,tenfold_float_value=0;
unsigned char scale1_sign=' ',scale1_weig_string_length=0;
unsigned long int  scale2_value,scale2_set_point1,scale2_set_point2,scale2_raw_value=0x80000;    
unsigned long int  temp_reg_first_weigh,temp_reg_second_weigh=0,temp_reg_total_weigh=0,temp_truck_id=0;
unsigned char temp_inbound_code=0,temp_sort_code=0;
unsigned char temp_customer_name[17]={' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char scale2_sign=' ',scale2_weig_string_length=0;   
unsigned char scale1_range_display=0,scale2_range_display=0;
unsigned char using_scale=0;
unsigned char first_weighing_state=0,second_weighing_state=0,basic_weighing_state=0,view_1st_weighing=0;
unsigned char manual_tare_s1_state=0,manual_tare_s2_state=0,calibration_state=0,pb_zero_s1_state=0,pb_zero_s2_state=0;     
unsigned char poweron_zero_s1_state=0,poweron_zero_s2_state=0;
unsigned char pb_tare_s1_state=0,pb_tare_s2_state=0,previous_cal_state=0;
float sum_tempf;
unsigned char average_index;
unsigned int backlight_cntr=0;



unsigned char  page=0,menu_item=0,select_item=0,sub_select_item=0,previous_page=0;
unsigned char  display_home_pos_en=0,clear_home_pos_en=0,display_blink_handle_en=0,countout_display=0;
unsigned char  display_area_en=0,display_weight_en=0,clear_all_graph_en=0,set_all_graph_en=0; 
unsigned char  test_lcd_cntr=0,sdet_all_graph_en=0; 
unsigned char  get_pass[6],get_cursor,blink_get_data_cntr=0,blink_en=0;
unsigned char  get_key_character=0;  //last key pressed to input a string         
unsigned char  get_character_index=0; //index of the characters on a key     
unsigned char  page_19_next_letter_cntr=0,calibration_state_counter=0;  
unsigned char  no_display_change_cntr=0,all_keys_temp_disable=0,display_home_pos_message_code=0;
unsigned char  page_4_out_cntr=0,tenfold_out_cntr=0,buz_start,buz_cntr;
unsigned char  show_errorcode_instead_xtra_cntr_scale1,lasterrorcode_scale1;
unsigned char  show_errorcode_instead_xtra_cntr_scale2,lasterrorcode_scale2;
unsigned char  sendout_pcwork_data_cntr=0,sendout_pcwork_data_en=0;

bit half_sec=0,sec_in_c=0;
unsigned char year=13,month=8,day=1,hour=0,minute=0,second=0,fifth_sec=0;  
unsigned char year_print,month_print,day_print,hour_print,minute_print,second_print;  
unsigned int  update_timer_from_rtc=0;


unsigned char temp_key=0,key=0,key_buffer[4]={0,0,0,0},apply_key=0,insert_delete_key_in_buf_inuse=0;  

unsigned int  get_value_i1=0, get_value_i2=0,get_value_i3=0,get_value_i4=0,get_value_i5=0,get_value_i6=0,get_value_i7=0;
unsigned long int  get_value_l1=0, get_value_l2=0,get_value_l3=0;
float prevoius_adc_temp_1=0,prevoius_adc_temp_2=0;

unsigned char  get_value_ch[17]={' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',0};


eeprom unsigned char *EE_General_pointer;

eeprom unsigned char EE_Pass_1[]="140789";
eeprom unsigned char EE_Pass_2[]="618349";
eeprom unsigned char EE_Pass_3[]="505442";
eeprom unsigned char EE_Pass_4[]="122345";
     

unsigned char Pass_1[6];
unsigned char Pass_2[6];
unsigned char Pass_3[6];
unsigned char Pass_4[6];




/* ---------------LCD function defines---------------------------------- */
void LCD_dput(int byte); 			 // write data byte to LCD module
int  LCD_dget(void);    			 // get data byte from LCD module
int  LCD_sget(void);  			 // check LCD display status pbrt
void LCD_cput(int byte);			 // write command byte to LCD module
void LCD_setup();			 // make sure control lines are at correct levels
void LCD_init();     			 // initialize LCD memory and display modes
void LCD_print(int x,int y,flash char *string);  	 // send string of characters to LCD
void LCD_print_ram(int x,int y,char *string);  	 // send string of characters to LCD
void LCD_set_address(unsigned int addr);
void LCD_clear_graph();    		 // clear graphics memory of LCD
void LCD_clear_text();  		 // clear text memory of LCD
void LCD_xy(int x, int y); 		 // set memory pointer to (x,y) position (text)
void LCD_clrpixel(int column, int row);  // set single pixel in 240x64 array
void LCD_setpixel(int column, int row);  // set single pixel in 240x64 array
void LCD_pixel(int column, int row,char show1);

void LCD_show(flash char *,int,int);
void LCD_line(int x1, int y1, int x2, int y2, unsigned char show);
void LCD_circle(int x, int y, int radius, unsigned char show);
void LCD_circle_half(int x, int y, int radius, unsigned char show);
void LCD_box(int x1, int y1, int x2, int y2, unsigned char show);
//void glcd_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
//void LCD_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show);
void LCD_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first);

void jalali(unsigned char,unsigned char,unsigned char,
             unsigned char * ,unsigned char *,unsigned char *); 



          

#define ORDER_OF_X10      0 
#define ORDER_OF_W1       1
#define ORDER_OF_W2       2   
#define ORDER_OF_WB       3
#define ORDER_OF_SP       4  
#define ORDER_OF_MT       5
#define ORDER_OF_MZ       6  
#define ORDER_OF_ME       7    
  


#define ORDER_OF_FKEY_MAIN_LIST      1 
#define ORDER_OF_FKEY_SEL_SCALE       2
#define ORDER_OF_FKEY_MAIN_LIST      3
                                         




 flash unsigned char message1[]="Password is incorrect!!";

///////menus define here

#define NUMBER_OF_GROUPS 10
#ifdef ENGLISH
  flash unsigned char text1[]="1. Configurations";
  flash unsigned char text2[]="2. Manual Tare";
  flash unsigned char text3[]="3. Interface";
  flash unsigned char text4[]="4. Format";
  flash unsigned char text5[]="5. General"; 
  flash unsigned char text6[]="6. Application";
  flash unsigned char text7[]="7. Back Up";
  flash unsigned char text8[]="8. Restore";
  flash unsigned char text9[]="9. Set Point";
  flash unsigned char text10[]="10. Calibration";    
#else
  flash unsigned char text1[]="ı¢÷“¤îş• .";
  flash unsigned char text2[]="ü—¨¢ ï÷¨¤‘• .‚";
  flash unsigned char text3[]="ü“÷‘› ¯‘“—¤ .ƒ";
  flash unsigned char text4[]="”‘ ’ó‘ì .„";
  flash unsigned char text5[]="üõøõä .…"; 
  flash unsigned char text6[]="‘û¢¤“¤‘î .†";
  flash unsigned char text7[]="ı¤şğ ö‘“ş—ª• .‡";
  flash unsigned char text8[]="ü÷‘ª÷¥‘“ .ˆ";
  flash unsigned char text9[]="‘ú—÷şø• –¨ .‰";
  flash unsigned char text10[]="öøş¨¤“şó‘î .€";   
#endif 
 flash unsigned char * flash service_menus[NUMBER_OF_GROUPS]={text1,text2,text3,text4,text5,text6,text7,text8,text9,text10};   
#define ORDER_OF_CONFIGS         1 
#define ORDER_OF_TARE             2
#define ORDER_OF_INTERFACE      3
#define ORDER_OF_FORMAT          4
#define ORDER_OF_GENERAL         5
#define ORDER_OF_APPLICATION   6
#define ORDER_OF_BACKUP          7
#define ORDER_OF_RESTORE         8
#define ORDER_OF_SETPOINT       9
#define ORDER_OF_CALIBRATION  10



#define NUMBER_OF_CONFIGS 5

#ifdef ENGLISH
  flash unsigned char text11[]="Set Time";
  flash unsigned char text12[]="Set Date";
  flash unsigned char text13[]="View Errors List";
  flash unsigned char text14[]="Version and Serial No";
  //flash unsigned char text15[]="Mode"; 
  flash unsigned char text16[]="LCD Test";
#else
  flash unsigned char text11[]="–ä‘¨ ôşà÷—";
  flash unsigned char text12[]="¸ş¤‘— ôşà÷—";
  flash unsigned char text13[]="‘û‘¯¡ –¨şó ù¢û‘ªõ";
  flash unsigned char text14[]="ñ‘ş¤¨ ù¤‘õª ø ö¦¤ø";
 // flash unsigned char text15[]="¢øõ"; 
  flash unsigned char text16[]="LCD –¨—";
#endif

flash unsigned char * flash config_menus[NUMBER_OF_CONFIGS]={text11,text12,text13,text14,text16};   
#define ORDER_OF_SETTIME   1
#define ORDER_OF_SETDATE   2
#define ORDER_OF_ERRLIST   3   
#define ORDER_OF_VERSION  4
#define ORDER_OF_TEST       5


#define NUMBER_OF_TARE 2           
#ifdef ENGLISH                 
  flash unsigned char text60[]="Scale 1";
  flash unsigned char text63[]="Scale 2";
#else
  flash unsigned char text60[]=" ñøî¨‘“";
  flash unsigned char text63[]="‚ ñøî¨‘“";
#endif
flash unsigned char * flash tare_menus[NUMBER_OF_TARE]={text60,text63};   
#define ORDER_OF_MANUAL_TARE1   1
#define ORDER_OF_MANUAL_TARE2   2


#define NUMBER_OF_INTERFACES 6
#ifdef ENGLISH       
  flash unsigned char text17[]="COM1 Profile"; 
  flash unsigned char text18[]="COM1 Function"; 
  flash unsigned char text19[]="COM1 Protocol";
  flash unsigned char text20[]="COM2 Profile";   
  flash unsigned char text21[]="COM2 Function";
  flash unsigned char text22[]="COM2 Protocol";
#else
  flash unsigned char text17[]="COM1 –‘¬¡ªõ"; 
  flash unsigned char text18[]="COM1 ¢¤îóõä"; 
  flash unsigned char text19[]="COM1 ñî—ø¤•";
  flash unsigned char text20[]="COM2 –‘¬¡ªõ";   
  flash unsigned char text21[]="COM2 ¢¤îóõä";
  flash unsigned char text22[]="COM2 ñî—ø¤•";
#endif
flash unsigned char * flash interface_menus[NUMBER_OF_INTERFACES]={text17,text18,text19,text20,text21,text22}; 
#define ORDER_OF_COM1PROF   1
#define ORDER_OF_COM1FUNC   2
#define ORDER_OF_COM1PROT   3
#define ORDER_OF_COM2PROF   4
#define ORDER_OF_COM2FUNC   5
#define ORDER_OF_COM2PROT   6


#define NUMBER_OF_FORMAT 3
#ifdef ENGLISH  
  flash unsigned char text23[]="Edit Field";
  flash unsigned char text24[]="Input User Text";
  flash unsigned char text24_1[]="Delete All Fields";
#else
  flash unsigned char text23[]="¢óşê ©ş¤şø";
  flash unsigned char text24[]="¤“¤‘î ö—õ ö—¡‘¨";
  flash unsigned char text24_1[]="‘û¢óşê ùõû ö¢¤î í‘•";
#endif

#define ORDER_OF_INPUTX   2
#define ORDER_OF_FORMAT_DELETE   3

#define ORDER_OF_FIELDNO  1
#define ORDER_OF_BLOCKNO  2
#define ORDER_OF_LINENO   3
#define ORDER_OF_COLUNO   4
#define ORDER_OF_ATTR     5
#define ORDER_OF_TYPE     6  
#define ORDER_OF_TEXT     7
flash unsigned char * flash format_menus[NUMBER_OF_FORMAT]={text23,text24,text24_1};   
 


#define NUMBER_OF_GENERAL 8
#ifdef ENGLISH  
  flash unsigned char text30[]="Language";
  flash unsigned char text31[]="Software";
  flash unsigned char text32[]="Light Off";
  flash unsigned char text33[]="Dongle Type";    
  flash unsigned char text53[]="Home Pos. Info";
  flash unsigned char text52[]="Calendar";
  flash unsigned char text52_1[]="Count Out";
#else
  flash unsigned char text30[]="ö‘“¥";
  flash unsigned char text31[]="¤¥ê ô¤÷";
  flash unsigned char text32[]="ù÷şõ¥ §• ¤ø÷";
  flash unsigned char text33[]="ñ¨¢øó áø÷";            
  flash unsigned char text53[]="ù÷ş¥ –‘ä‘ó¯ ©ş‘õ÷";
  flash unsigned char text53_1[]="¤¥‘“ ¢¤îóõä";
  flash unsigned char text52[]="ôşøì—";  
  flash unsigned char text52_1[]="ñ¨¢øó ü›ø¤¡ ¦‘—óø";
#endif
flash unsigned char * flash general_menus[NUMBER_OF_GENERAL]={text30,text31,text32,text33,text53,text53_1,text52,text52_1};   
#define ORDER_OF_LANGUAGE   1
#define ORDER_OF_SOFTWARE   2
#define ORDER_OF_BL_TIME    3
#define ORDER_OF_DONGLE     4   
#define ORDER_OF_HOMEPOS    5
#define ORDER_OF_ALARM       6
#define ORDER_OF_CALENDAR   7
#define ORDER_OF_VOLTAGE    8



///*************************** APPLICATION menu and SUBmenus
#define NUMBER_OF_APPLICATION 4
#ifdef ENGLISH  
  flash unsigned char text34[]="1st Weigh File";
  flash unsigned char text35[]="Fixed Tare File";
  flash unsigned char text25[]="Memory check";     
  flash unsigned char text36[]="Sort File";  
#else
  flash unsigned char text34[]="ñø ı‘ûöş¥ø—";
  flash unsigned char text35[]="–“‘™ ı‘ûï÷¨¤‘•";
  flash unsigned char text25[]="ùàê‘Ÿ ö¢¤î í";     
  flash unsigned char text36[]="¤‘“ ¢î –¨şó";  
#endif
#define ORDER_OF_1STWEIGH   1
#define ORDER_OF_FIXEDTAR   2 
#define ORDER_OF_MEMCHECK   3
#define ORDER_OF_SORTFILE   4

flash unsigned char * flash application_menus[NUMBER_OF_APPLICATION]={text34,text35,text25,text36}; 
                               
#define NUMBER_OF_APP_1ST 4
#define NUMBER_OF_APP_FT  5  
#define NUMBER_OF_APP_MC  4     
#define NUMBER_OF_APP_SF  4      
flash unsigned char no_of_items_in_app_sub_menus[NUMBER_OF_APPLICATION]={NUMBER_OF_APP_1ST,NUMBER_OF_APP_FT,
                                                                     NUMBER_OF_APP_MC,NUMBER_OF_APP_SF};  

#ifdef ENGLISH  
  flash unsigned char text26[]="Print";
  flash unsigned char text27[]="Edit";
  flash unsigned char text27_1[]="View";
  flash unsigned char text28[]="Delete";  
  flash unsigned char text29[]="Delete One";  
  flash unsigned char text66[]="Print All"; 
  flash unsigned char text67[]="Print One";  
  flash unsigned char text68[]="Create New";  
  flash unsigned char text69[]="Delete All";
#else
  flash unsigned char text26[]="”‘";
  flash unsigned char text27[]="©ş¤şø";
  flash unsigned char text27_1[]="ù¢û‘ªõ";
  flash unsigned char text28[]="ö¢¤î í‘•";  
  flash unsigned char text29[]="üîş ö¢¤î í‘•";  
  flash unsigned char text66[]="ùõû ö¢¤î ”‘"; 
  flash unsigned char text67[]="üîş ö¢¤î ”‘";  
  flash unsigned char text68[]="¢ş¢› ö—¡‘¨";  
  flash unsigned char text69[]="ùõû ö¢¤î í‘•";
#endif  
  
#define ORDER_OF_1ST_PRINT    1
#define ORDER_OF_1ST_VIEW     2
#define ORDER_OF_1ST_DELETE   3
#define ORDER_OF_1ST_DELETEA  4       

#define ORDER_OF_1ST_VIEW_GET     100+ORDER_OF_1ST_VIEW
#define ORDER_OF_1ST_PRINT_GET    100+ORDER_OF_1ST_PRINT

flash unsigned char * flash first_file_submenus[NUMBER_OF_APP_1ST]={text26,text27_1,text28,text69}; 


#define ORDER_OF_FT_PRINT   1
#define ORDER_OF_FT_EDIT    2
#define ORDER_OF_FT_DELETE  3
#define ORDER_OF_FT_CREATN  4  
#define ORDER_OF_FT_DELETEA  5

#define ORDER_OF_FT_EDIT_GET      100+ORDER_OF_FT_EDIT

flash unsigned char * flash fixed_tare_submenus[NUMBER_OF_APP_FT]={text26,text27,text28,text68,text69};   

#define ORDER_OF_MC_PRINTO   1
#define ORDER_OF_MC_PRINTA   2
#define ORDER_OF_MC_DELETEO  3
#define ORDER_OF_MC_DELETEA  4
flash unsigned char * flash mem_check_submenus[NUMBER_OF_APP_MC]={text67,text66,text29,text69};  

#define ORDER_OF_SF_PRINT   1
#define ORDER_OF_SF_EDIT    2
#define ORDER_OF_SF_DELETE  3
#define ORDER_OF_SF_DELETEA 4         

flash unsigned char * flash sort_file_submenus[NUMBER_OF_APP_SF]={text26,text27,text28,text69};  
///*************************** APPLICATION 


#define NUMBER_OF_BACKUP 1                            
flash unsigned char text59[]="To PC";
flash unsigned char * flash backup_menus[NUMBER_OF_BACKUP]={text59};   
#define ORDER_OF_READYPC   1

   

#define NUMBER_OF_RESTORE 8          
#ifdef ENGLISH                    
  flash unsigned char text54[]="Factory Reset";  
  flash unsigned char text54_1[]="Load From EEPROM";
  flash unsigned char text54_2[]="Save to EEPROM";
  flash unsigned char text54_3[]="Clear Errors List";
  flash unsigned char text55[]="Change Password (1)";    
  flash unsigned char text56[]="Change Password (2)";
  flash unsigned char text57[]="Change Password (3)";
  flash unsigned char text58[]="Change Password (4)";
#else 
  flash unsigned char text54[]="ù÷‘¡¤‘î –‘õşà÷— ù“ –ªğ¥‘“";             
  flash unsigned char text54_1[]="EEPROM –‘õşà÷— ù“ –ªğ¥‘“";   
  flash unsigned char text54_2[]="EEPROM ¤¢ –‘õşà÷— ù¤ş¡£";
  flash unsigned char text54_3[]="‘û‘¯¡ –¨şó ö¢¤î í‘•";              
  flash unsigned char text55[]="1 ¢¤øø¨• ¤şşç—";    
  flash unsigned char text56[]="2 ¢¤øø¨• ¤şşç—";
  flash unsigned char text57[]="3 ¢¤øø¨• ¤şşç—";
  flash unsigned char text58[]="4 ¢¤øø¨• ¤şşç—";
#endif
flash unsigned char * flash restore_menus[NUMBER_OF_RESTORE]={text54,text54_1,text54_2,text54_3,text55,text56,text57,text58};  
#define ORDER_OF_FACTORY   1
#define ORDER_OF_LE2PROM   2
#define ORDER_OF_SE2PROM   3
#define ORDER_OF_DELERROR  4
#define ORDER_OF_CH_PAS1   5
#define ORDER_OF_CH_PAS2   6
#define ORDER_OF_CH_PAS3   7   
#define ORDER_OF_CH_PAS4   8   


#define NUMBER_OF_SETPOINT 4  
#ifdef ENGLISH                           
  flash unsigned char text61[]="Scale 1 Set Point 1";
  flash unsigned char text62[]="Scale 1 Set Point 2";
  flash unsigned char text64[]="Scale 2 Set Point 1";
  flash unsigned char text65[]="Scale 2 Set Point 2";
#else
  flash unsigned char text61[]=" ñøî¨‘“  –÷şø• –¨";          
  flash unsigned char text62[]=" ñøî¨‘“ ‚ –÷şø• –¨";
  flash unsigned char text64[]="‚ ñøî¨‘“  –÷şø• –¨";
  flash unsigned char text65[]="‚ ñøî¨‘“ ‚ –÷şø• –¨";
#endif
flash unsigned char * flash setpoint_menus[ORDER_OF_SETPOINT]={text61,text62,text64,text65};   
#define ORDER_OF_S1_SETPOINT1  1
#define ORDER_OF_S1_SETPOINT2  2
#define ORDER_OF_S2_SETPOINT1  3
#define ORDER_OF_S2_SETPOINT2  4


#define NUMBER_OF_CALIBRATION 4     
#ifdef ENGLISH 
  flash unsigned char text51[]="Using Scales";
  flash unsigned char text37[]="Scale1 Settings";
  flash unsigned char text38[]="Scale2 Settings";
  flash unsigned char text39[]="Calib and Zero mV/V";
#else
  flash unsigned char text51[]="ñ‘ãê ı‘ûñøî¨‘“";
  flash unsigned char text37[]=" ñøî¨‘“ –‘õşà÷—";
  flash unsigned char text38[]="‚ ñøî¨‘“ –‘õşà÷—";
  flash unsigned char text39[]="mV/V ö¢¤î ¤ê¬ ø ù¤“şó‘î";
#endif
flash unsigned char * flash calibration_menus[NUMBER_OF_CALIBRATION]={text51,text37,text38,text39}; 
#define ORDER_OF_SELECT         1 
#define ORDER_OF_SCALE1         2
#define ORDER_OF_SCALE2         3
#define ORDER_OF_CALCAL         4


#define NUMBER_OF_SCALE  13
#ifdef ENGLISH 
  flash unsigned char text46[]="Range";
  flash unsigned char text47_1[]="Low Interval";
  flash unsigned char text47_2[]="Mid Interval";
  flash unsigned char text47_3[]="High Interval";
  flash unsigned char text47_4[]="Interval Unit";  
  flash unsigned char text47_5[]="Low Capacity";
  flash unsigned char text47_6[]="Mid Capacity";
  flash unsigned char text47_7[]="High Capacity";
  flash unsigned char text47_8[]="Capacity Unit";
  flash unsigned char text49[]="Unit";
  flash unsigned char text50[]="Digits";      
  flash unsigned char text47_9[]="Point";
  flash unsigned char text42[]="Adaptation";
#else
  flash unsigned char text46[]="‘ûù÷ş¥ ¢¢ã—";
  flash unsigned char text47_1[]="öşş‘• ù÷ş¥";
  flash unsigned char text47_2[]="ü÷‘şõ ù÷ş¥";
  flash unsigned char text47_3[]="‘ó‘“ ù÷ş¥";
  flash unsigned char text47_4[]="ù÷ş¥ ¢Ÿø";  
  flash unsigned char text47_5[]="öşş‘• –şê¤à";
  flash unsigned char text47_6[]="ü÷‘şõ –şê¤à";
  flash unsigned char text47_7[]="‘ó‘“ –şê¤à";
  flash unsigned char text47_8[]="–şê¤à ¢Ÿø";
  flash unsigned char text49[]="ñøî¨‘“ ¢Ÿø";
  flash unsigned char text50[]="‘û ôì¤";      
  flash unsigned char text47_9[]="¤‘ªä ôì¤";
  flash unsigned char text42[]="ëş“¯—";
#endif
flash unsigned char * flash calibration_scale_menus[NUMBER_OF_SCALE]={text46,text47_1,text47_2,text47_3,text47_4,
                                                                   text47_5,text47_6,text47_7,text47_8,text49,text50,text47_9,text42};  
#define ORDER_OF_SCALE_RANGE               1
#define ORDER_OF_SCALE_INTERVAL1         2
#define ORDER_OF_SCALE_INTERVAL2         3
#define ORDER_OF_SCALE_INTERVAL3         4
#define ORDER_OF_SCALE_INTERVAL_UNIT   5      
#define ORDER_OF_SCALE_CAPACITY1         6
#define ORDER_OF_SCALE_CAPACITY2         7
#define ORDER_OF_SCALE_CAPACITY3         8
#define ORDER_OF_SCALE_CAPACITY_UNIT   9
#define ORDER_OF_SCALE_UNIT               10
#define ORDER_OF_SCALE_DIGITS             11  
#define ORDER_OF_SCALE_FLOAT              12  
#define ORDER_OF_ADAPTATION               13  


#define NUMBER_OF_CAL_ADAPT  8
#ifdef ENGLISH 
  flash unsigned char text70[]="MotionWindow";
  flash unsigned char text71[]="MotionCounter";
  flash unsigned char text72[]="Filter Size";
  flash unsigned char text73[]="AutoZeroRange";
  flash unsigned char text74[]="Pb Zero Pos.";
  flash unsigned char text75[]="Pb Zero Neg.";  
  flash unsigned char text76[]="PowerUp Zero";  
  flash unsigned char text77[]="OverLoad"; 
#else
  flash unsigned char text70[]="öş¥ø— ©¥¤ó ù¢ø¢Ÿõ";
  flash unsigned char text71[]="öş¥ø— ©¥¤ó ù¢÷¤‘õª";
  flash unsigned char text72[]="¤—óşê ù¥¢÷";
  flash unsigned char text73[]="¤‘î¢ø¡ ¤ê¬ ù¢ø¢Ÿõ";
  flash unsigned char text74[]="ü—¨¢ ¤ê¬ ı‘ó‘“ ¢¬¤¢";
  flash unsigned char text75[]="ü—¨¢ ¤ê¬ öşş‘• ¢¬¤¢";  
  flash unsigned char text76[]="–¤‘—¨ ¤ê¬ ù¢ø¢Ÿõ";  
  flash unsigned char text77[]="¤‘“ ùê‘® ù¢ø¢Ÿõ"; 
#endif
flash unsigned char * flash calibration_adapt_menus[NUMBER_OF_CAL_ADAPT]={text70,text71,text72,text73,text74,text75,text76,text77}; 

#define ORDER_OF_ADAPT_MOTION_W      100
#define ORDER_OF_ADAPT_MOTION_C      101
#define ORDER_OF_ADAPT_FILTER_S        102
#define ORDER_OF_ADAPT_AUTO_Z_R       103
#define ORDER_OF_ADAPT_PB_ZEROP       104
#define ORDER_OF_ADAPT_PB_ZERON       105
#define ORDER_OF_ADAPT_PUP_ZERO       106
#define ORDER_OF_ADAPT_OVERLOAD       107



#define NUMBER_OF_SCALE1 13
#define NUMBER_OF_SCALE2 13

flash unsigned char no_of_items_in_sub_menus[NUMBER_OF_CALIBRATION]=
       {0,NUMBER_OF_SCALE1,NUMBER_OF_SCALE2,0};  
        


flash unsigned char no_of_items_in_menus[NUMBER_OF_GROUPS]=
       {NUMBER_OF_CONFIGS,NUMBER_OF_TARE,NUMBER_OF_INTERFACES,
        NUMBER_OF_FORMAT,NUMBER_OF_GENERAL,NUMBER_OF_APPLICATION,
        NUMBER_OF_BACKUP,NUMBER_OF_RESTORE,NUMBER_OF_SETPOINT,NUMBER_OF_CALIBRATION};

                                                        
        
unsigned char  convert_num_point_str(unsigned char *,unsigned long int ,unsigned char , unsigned char, unsigned char);
void printf_longnum(unsigned char *,unsigned long int ,unsigned char );
void read_sort_name(unsigned char *,unsigned char );
void load_initial_values(void);
unsigned long int convert_unit(unsigned long int ,unsigned char );
unsigned char select_unit_str(unsigned char *,unsigned char );