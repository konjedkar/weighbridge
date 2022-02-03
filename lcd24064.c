/* ----------------------------------------------------------
 * Program to control a T6963C-based 240x64 pixel LCD display
 * 
 * 
 *
 * Written by John P. Beale May 3-4, 1997  beale@best.com
 *
 * Converted by Nico Sachs Feb. 2006 for AVR-GCC
 *
 *  Based on information from Steve Lawther,
 *  "Writing Software for T6963C based Graphic LCDs", 1997 which is at
 *  http://ourworld.compuserve.com/homepages/steve_lawther/t6963c.pdf
 *
 *  and the Toshiba T6963C data sheet, also on Steve's WWW page
 *
 *  and info at: http://www.citilink.com/~jsampson/lcdindex.htm
 *               http://www.cs.colostate.edu/~hirsch/LCD.html
 *               http://www.hantronix.com/
 * ----------------------------------------------------------
 */                


/*
 *  FG                (1)  frame ground
 *  GND               (2)  ground 
 *  VDD               (3)  LCD logic supply  +5 DC
 *  VO                (4)  Contrast
 *  /WR               (5)  <--> 
 *  /RD               (6)  <--> 
 *  /CE or /CS        (7)  <--> 
 *  CD or RS          (8)  <-->
 *  /RST              (9)  <-->   

 *  D0                (10) <--> 
 *  D1                (11) <--> 
 *  D2                (12) <--> 
 *  D3                (13) <--> 
 *  D4                (14) <--> 
 *  D5                (15) <--> 
 *  D6                (16) <--> 
 *  D7                (17) <--> 

 *  FS                (18)           font select       HI : narrow            LOW : Wide   ( normal )
                                                       40 with 6x8 font       30 with 8x8 font 
 *  Vout              (19)           Variable Resistor with GND and centre with VO ( Pin 4 )
 *  BLA               (20)           Back light +
 *  BLK               (21)           Back light -
 *  DSPON             (22)           I don't Know   but i think means display ON or ...
 */


#define LCD_W           PORTE.3
#define LCD_W_DIR       DDRE.3

#define LCD_R           PORTE.4
#define LCD_R_DIR       DDRE.4 

#define LCD_CE          PORTE.5
#define LCD_CE_DIR      DDRE.5    

#define LCD_CD          PORTE.6
#define LCD_CD_DIR      DDRE.6  

#define LCD_RST         PORTE.7
#define LCD_RST_DIR     DDRE.7


/***************************************/

/****************************************/
									//**
									//**
#define LCD_DATA_PORT 		PORTF	//**
#define LCD_DATA_PIN		PINF	//**
#define LCD_DATADIR		DDRF	//**
									//**
//**************************************
//**************************************

// ----- Definitions concerning LCD internal memory  ------

#define LCD_G_BASE 0x0200 //0x0200            // base address of graphics memory
#define LCD_T_BASE 0x0000            // base address of text memory
#define LCD_BYTES_PER_ROW 30         // how many bytes per row on screen



#define LCD_wr_high() LCD_W=1  //glcd_wr_PORT  |=  _BV(glcd_wr_PIN);
#define LCD_wr_low()  LCD_W=0  //glcd_wr_PORT  &= ~_BV(glcd_wr_PIN);

#define LCD_rd_high() LCD_R=1 //glcd_rd_PORT  |=  _BV(glcd_rd_PIN);
#define LCD_rd_low()  LCD_R=0 //glcd_rd_PORT  &= ~_BV(glcd_rd_PIN);

#define LCD_ce_high() LCD_CE=1 //glcd_ce_PORT  |=  _BV(glcd_ce_PIN);
#define LCD_ce_low()  LCD_CE=0 //glcd_ce_PORT  &= ~_BV(glcd_ce_PIN);

#define LCD_cd_high() LCD_CD=1 //glcd_cd_PORT  |=  _BV(glcd_cd_PIN);
#define LCD_cd_low()  LCD_CD=0 //glcd_cd_PORT &= ~_BV(glcd_cd_PIN);

#define LCD_rst_high() LCD_RST=1 //glcd_rst_PORT  |=  _BV(glcd_rst_PIN);
#define LCD_rst_low()  LCD_RST=0 //glcd_rst_PORT  &= ~_BV(glcd_rst_PIN);


#define LCD_SET_DATADIR_OUT()	(LCD_DATADIR = 0xFF)
#define LCD_SET_DATADIR_IN()	(LCD_DATADIR = 0x00)

unsigned char currentx=0,currenty=0;                                                                     



#define LCD_XMAX 240        // limits of (x,y) LCD graphics drawing
#define LCD_XMIN 0
#define LCD_YMAX 64  //128
#define LCD_YMIN 0



void LCD_set_address(unsigned int addr){   
LCD_dput(addr%256);
LCD_dput(addr>>8);
LCD_cput(0x24);
}


void LCD_set_all_graph()    // clear graphics memory of LCD
{
int i;
LCD_set_address(LCD_G_BASE);     // addrptr at address G_BASE1
for (i=0;i<1920;i++) {   // must be  3840 ?????????????????     5120
	LCD_dput(0xFF); LCD_cput(0xc0);               // write data, inc ptr.
	}	 // end for(i)
} // end LCD_clear_graph()

void LCD_clear_graph()    // clear graphics memory of LCD
{
int i;
LCD_set_address(LCD_G_BASE);     // addrptr at address G_BASE1
for (i=0;i<1920;i++) {   // must be  3840 ?????????????????     5120
	LCD_dput(0); LCD_cput(0xc0);               // write data, inc ptr.
	}	 // end for(i)
} // end LCD_clear_graph()   

void clear_home_pos(void)
{
int i;
LCD_set_address(LCD_G_BASE+LCD_BYTES_PER_ROW*40);     // addrptr at address G_BASE1
for (i=0;i<720;i++) {   // must be  3840 ?????????????????     5120
	LCD_dput(0); LCD_cput(0xc0);               // write data, inc ptr.
	}	 // end for(i)
}      


void LCD_clear_area_pos(char x1,char y1,char x2,char y2)
{
int i,j;

for (j=y1;j<y2;j++)
         {   
	   LCD_set_address(LCD_G_BASE+LCD_BYTES_PER_ROW*j+x1);
	   for (i=x1;i<x2;i++)
	    {
        	LCD_dput(0); 
        	LCD_cput(0xc0);   
            }     
	 }	 // end for(i)  
	
	
}

//----------------------------------------------------------------

void LCD_clear_text()
{
 int i;   
 LCD_set_address(LCD_T_BASE);     // addrptr at address T_BASE1

 for (i=0;i<240;i++) {  //  must be 480   ???????????     640  
      LCD_dput(0); LCD_cput(0xc0);               // write data, inc ptr.
 } // end for(i)

} // LCD_clear_text1()

//------------------------------------------------------------------------------- 


//acts like putchar() but shows the character on lcd in current position
void LCD_draw_icon(flash unsigned char *icon,char width,char height){   
   unsigned int rows,offset,mapping,allbytes,i,j;
   unsigned char x,y,temp_currentx,temp_currenty,byte,temp_pc;
   char bg;
  
   allbytes = width;   
   allbytes = allbytes / 8;
   if(width % 8)allbytes++;
   allbytes = allbytes * height;    

   
   temp_currentx=currentx; 
   temp_currenty=currenty;        
   
  j=0;                                            
  for(i=0;i<allbytes;i++)
   {
        byte = *(icon+i);  //font_Arial_18_n_bnnn_0_n_char_table[offset+i]; 
        temp_pc=0x80;
        while(j<width)
              {
                if(byte&temp_pc)LCD_setpixel(currentx,currenty); else  LCD_clrpixel(currentx,currenty);  
   
                currentx++;  
                temp_pc=temp_pc>>1;  
                j++;
                if(j%8==0)break;
              }
        if(j%8==0 && j<width)continue;
        j=0;
        currenty++;    
        currentx=temp_currentx;
   }     
  currenty=temp_currenty;    
  currentx=temp_currentx+width;


} 

     
/*
//acts like putchar() but shows the character on lcd in current position
void LCD_putnumber_7seg(char c,char background){   
   unsigned int rows,offset,width,mapping;
   unsigned char x,y,i,j,allbytes,temp_currentx,temp_currenty,byte,temp_pc;
   char bg;
   bg=(background)?1:0;          
   
   mapping = font_7_Segment_18_n_bnnn_0_n_mapping_table[c];        //conver ascii code --> order in table  eg. A=10
   offset=font_7_Segment_18_n_bnnn_0_n_offset_table[mapping];    //index of 1st Byte of CODE
   width=14;      //width of code
   allbytes = (font_7_Segment_18_n_bnnn_0_n_offset_table[mapping+1]-offset);   
   
   temp_currentx=currentx; 
   temp_currenty=currenty; 
   
  j=0;                                            
  for(i=0;i<allbytes;i++)
   {
        byte = font_7_Segment_18_n_bnnn_0_n_char_table[offset+i]; 
        temp_pc=0x80;
        while(j<width)
              {
                if(byte&temp_pc)LCD_setpixel(currentx,currenty); //else  LCD_clrpixel(currentx,currenty);  
   
                currentx++;  
                temp_pc=temp_pc>>1;  
                j++;
                if(j%8==0)break;
              }
        if(j%8==0 && j<width)continue;
        j=0;
        currenty++;    
        currentx=temp_currentx;
   }     
  currenty=temp_currenty;    
  currentx=temp_currentx+width;


} */ 

//acts like putchar() but shows the character on lcd in current position
void LCD_putnumber_arial(char c,char fontsize){   
   unsigned int rows,offset,width,mapping;
   unsigned char x,y,i,j,allbytes,temp_currentx,temp_currenty,byte,temp_pc;
       
   
   
   if(c=='-')mapping=11; 
   else if(c=='.')mapping=12;
   else if(c=='|')mapping=13;  
   else if(c>='0')mapping=c-'0';
   else mapping=10;
   
  
   if(fontsize==18)
    {
           offset=mapping*38;    //index of 1st Byte of CODE
           if(c=='.')width=4; 
           else if(c=='|')width=1; 
           else width=13;      //width of code
           allbytes = 38;     
    }
   else if(fontsize==36)
    {
           offset=mapping*84;    //index of 1st Byte of CODE
          if(c=='.')width=6; 
           else if(c=='|')width=1; 
           else width=20;      //width of code
           allbytes = 84;     
    }
   
   temp_currentx=currentx; 
   temp_currenty=currenty;        
   
  j=0;                                            
  for(i=0;i<allbytes;i++)
   { 
   if(fontsize==18)byte = font_Arial_18_n_bnnn_0_n_char_table[offset+i]; 
   else if(fontsize==36)byte = font_Arial_36_n_bnnn_0_n_char_table[offset+i];   
        
        temp_pc=0x80;
        while(j<width)
              {
                if(byte&temp_pc)LCD_setpixel(currentx,currenty); else  LCD_clrpixel(currentx,currenty);  
   
                currentx++;  
                temp_pc=temp_pc>>1;  
                j++;
                if(j%8==0)break;
              }
        if(j%8==0 && j<width)continue;   

        if(j==width && width<8){if(fontsize==18)i++; else i+=2;};
        j=0;
        currenty++;    
        currentx=temp_currentx;
   }     
  currenty=temp_currenty;    
  currentx=temp_currentx+width;


} 


void LCD_put_stringnumber_arial(unsigned char *numstr,unsigned char fontsize) //fontsize--> 18 | 36
{
  unsigned char temp_lsta=0;
   
   while(*(numstr+temp_lsta))
   {LCD_putnumber_arial(*(numstr+temp_lsta),fontsize);
    temp_lsta++;}


}     



//acts like putchar() but shows the character on lcd in current position
void LCD_puttahoma(unsigned char c,unsigned char font_width)
{   
   unsigned int rows,offset,width,mapping;
   unsigned char x,y,i,j,allbytes,temp_currentx,temp_currenty,byte,temp_pc;
       
   
   mapping = font_Times_New_Roman_10_n_bnnn_0_n_mapping_table[c];      
   
   if(font_width==7 || font_width=='m')
    {
     offset=mapping*9;    //index of 1st Byte of CODE
     width=7;      //width of code
     allbytes = 9;   
    }
   else
   if(font_width==8 || font_width=='n')     //'n' is same as 8 but is different in background (for NEGATIVE)
    {    
      offset=mapping*11;    //index of 1st Byte of CODE
      width=8;      //width of code
      allbytes = 11;               
    }
   
   
   temp_currentx=currentx; 
   temp_currenty=currenty;  
   
   //if((mapping<10 || c=='.' || c==' ') && ((font_width==7) || (font_width=='m')))temp_currentx--;      
   
  j=0;                                            
  for(i=0;i<allbytes;i++)
   { 
     if((font_width==7) || ( font_width=='m'))
      {
       byte = font_Tahoma7_char_table[offset+i];
      }   
     else
     if((font_width==8) || (font_width=='n'))byte = font_Tahoma8_char_table[offset+i];               

        
     temp_pc=0x80;
     while(j<width)
              {
                if((font_width!='n') && (font_width!='m'))
                 {
                        if(byte&temp_pc)LCD_setpixel(currentx,currenty); else  LCD_clrpixel(currentx,currenty);  
                 }
                else
                 {
                        if(byte&temp_pc)LCD_clrpixel(currentx,currenty); else  LCD_setpixel(currentx,currenty);
                 } 
   
                currentx++;  
                temp_pc=temp_pc>>1;  
                j++;
                if(j%8==0)break;
              }
        if(j%8==0 && j<width)continue;
        j=0;
        currenty++;    
        currentx=temp_currentx;
   }     
  currenty=temp_currenty;    
  currentx=temp_currentx+width;     
  
  if((mapping<10 || c=='.' || c==' ') && ((font_width==7) || (font_width=='m')))currentx--;


} 


void LCD_put_string_tahoma(unsigned char *numstr,unsigned char font_width)
{
  unsigned char temp_lsta=0;
   
 //  if(font_width!=7 && font_width!='n')    
 
   
 
   
   while(*(numstr+temp_lsta))
   {LCD_puttahoma(*(numstr+temp_lsta),font_width);
    temp_lsta++;}


}


unsigned char LCD_get_len_string_tahoma7(unsigned char *numstr)
{
    unsigned char temp_gsta=0,temp_gsta2,temp_gsta3=0;  
    
    
   while(*(numstr+temp_gsta))
   {
     temp_gsta2=*(numstr+temp_gsta); 
     if(temp_gsta2 == '.' || temp_gsta2 == ' ' || (temp_gsta2<='9' && temp_gsta2>='0') )temp_gsta3+=5;
     else temp_gsta3+=7;
     temp_gsta++;
   }


    return  temp_gsta3;

}



//acts like putchar() but shows the character on lcd in current position
void LCD_putchar(char c,char background){   
   unsigned int rows,offset,width,mapping;
   unsigned int x,y,i,j,allbytes,temp_currentx,temp_currenty,byte,temp_pc;
   char bg;
   //bg=(background)?1:0;       
   
   bg = background;
 
   
   mapping = font_Times_New_Roman_10_n_bnnn_0_n_mapping_table[c];        //conver ascii code --> order in table  eg. A=10
   offset=font_Times_New_Roman_10_n_bnnn_0_n_offset_table[mapping];    //index of 1st Byte of CODE
   width=font_Times_New_Roman_10_n_bnnn_0_n_width_table[mapping];      //width of code
   allbytes = (font_Times_New_Roman_10_n_bnnn_0_n_offset_table[mapping+1]-offset);   
   
   temp_currentx=currentx; 
   temp_currenty=currenty; 
   
  j=0;                                            
  for(i=0;i<allbytes;i++)
   {
        byte = font_Times_New_Roman_10_n_bnnn_0_n_char_table[offset+i]; 
        temp_pc=0x80;
        while(j<width)
              {
                if(byte&temp_pc)
                {
                  if(background==0 || background==2)LCD_setpixel(currentx,currenty);     
                  else  LCD_clrpixel(currentx,currenty); 
                }
                else
                {
                  if(background==0)LCD_clrpixel(currentx,currenty);
                  else if(background==2);
                  else  LCD_setpixel(currentx,currenty); 
                }  

                currentx++;  
                temp_pc=temp_pc>>1;  
                j++;
                if(j%8==0)break;
              }
        if(j%8==0 && j<width)continue;   
        
       
        j=0;
        currenty++;    
        currentx=temp_currentx;
   }     
  currenty=temp_currenty;    
  currentx=temp_currentx+width;


}              





//returnt the lengh of character 'c' (character width in dots)
char LCD_getcharlength(char c){
   return(font_Times_New_Roman_10_n_bnnn_0_n_width_table[font_Times_New_Roman_10_n_bnnn_0_n_mapping_table[c]]);
}

//returnt the lengh of flash string 'string' (string width in dots)
char LCD_getstringlength(flash char *string){
   char len=0;
   while(*string){      
      len+=LCD_getcharlength(*string);
      string++;
   }   
   return len;
}  

//returnt the lengh of flash string 'string' (string width in dots)
char LCD_getstringlength2( char *string){
   char len=0;
   while(*string){      
      len+=LCD_getcharlength(*string);
      string++;
   }   
   return len;
}  


LCD_printstring(flash char *string,unsigned char bg)
 {
  unsigned char i=0;
  while(string[i])
    {
      LCD_putchar(string[i],bg);
      i++;
      if(i>200)return;
   }   
 
 }   
 
LCD_printstring2(char *string,unsigned char bg)
 {
  unsigned char i=0;
  while(string[i])
    {
      LCD_putchar(string[i],bg);
      i++;
      if(i>200)return;
   }   
 
 }        
 
LCD_printspace_8(char spc)
 {
  unsigned char i;  
  i=spc;
  while(i)
    {
      LCD_putchar(127,0);   //
      i--;
     }   
 
 }   
 

LCD_printspace_7(char spc)
 {
  unsigned char i;  
  i=spc;
  while(i)
    {
      LCD_puttahoma(' ',7);
      i--;
     }   
 
 }  


void LCD_print(int x,int y, flash char *string)  // send string of characters to LCD
{
int i;
int c;
LCD_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      LCD_dput(c);
      LCD_cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end LCD_string

//--------------------------------------------------------------------------------


void LCD_print_ram(int x,int y,char *string)  // send string of characters to LCD
{
int i;
int c;
LCD_xy(x,y);
  for (i=0;string[i]!=0;i++) {
      c = string[i] - 0x20;     // convert ASCII to LCD char address
      if (c<0) c=0;
      LCD_dput(c);
      LCD_cput(0xc0);               // write character, increment memory ptr.
  } // end for

} // end LCD_string

//--------------------------------------------------------------------------------


void LCD_setpixel(int column, int row)  // set single pixel in 240x64 array
{

int addr;       // memory address of byte containing pixel to write
  if( (column>=LCD_XMAX) || (row>=LCD_YMAX) )return;
  addr =  LCD_G_BASE + (row*LCD_BYTES_PER_ROW)  + (column>>3);
  LCD_set_address(addr);  // set LCD addr. pointer
  LCD_cput(0xf8 | ((7-(column&7))) );  // set bit-within-byte command  
}            

//--------------------------------------------------------------------------------
void LCD_setxy(int x, int y)  // set single pixel in 240x64 array
{

int addr;       // memory address of byte containing pixel to write      
  if( (x>=LCD_XMAX) || (y>=LCD_YMAX) )return;
  addr =  LCD_G_BASE + (y*LCD_BYTES_PER_ROW)  + (x>>3);
  LCD_set_address(addr);  // set LCD addr. pointer
  
  currentx = x;
  currenty = y;
}

//--------------------------------------------------------------------------------

void LCD_clrpixel(int column, int row)
{
int addr;       // memory address of byte containing pixel to write
  if( (column>=LCD_XMAX) || (row>=LCD_YMAX) )return;
  addr =  LCD_G_BASE + (row*LCD_BYTES_PER_ROW)  + (column>>3);
  LCD_set_address(addr);  // set LCD addr. pointer
  LCD_cput(0xf0 | ((7-(column&7))) );  // set bit-within-byte command


}

//---------------------------------------------------------------------------------

void LCD_pixel(int column, int row,char show)
{
int addr;       // memory address of byte containing pixel to write
if( (column>=LCD_XMAX) || (row>=LCD_YMAX) )return;
addr =  LCD_G_BASE + (row*LCD_BYTES_PER_ROW)  + (column/8);
LCD_set_address(addr);   // set LCD addr. pointer
if(show)
  LCD_cput(0xf8 | ((7-column%8)) );  // set bit-within-byte command
else
  LCD_cput(0xf0 | ((7-column%8)) );  // set bit-within-byte command
}

//---------------------------------------------------------------------------------


void LCD_show(flash char * s,int start_line,int how_many_line){
int addr,i;
addr =  LCD_G_BASE +start_line*30;
LCD_set_address(addr);   // set LCD addr. pointer
LCD_cput(0xB0);
for(i=0;i<how_many_line*30;i++){
	LCD_dput(s[i]); 
	}
LCD_cput(0xB2);
}



void LCD_xy(int x, int y)  // set memory pointer to (x,y) position (text)
{
int addr;

  addr = LCD_T_BASE + (y * LCD_BYTES_PER_ROW) + x;
  LCD_set_address(addr);   // set LCD addr. pointer

} // lcd_xy1()



/* ==============================================================
 * Low-level I/O routines to interface to LCD display
 * based on four routines:
 *
 *          LCD_dput(): write data byte
 *          LCD_cput(): write control byte
 *          LCD_dget(): read data byte         (UNTESTED)
 *          LCD_sget(): read status
 * ==============================================================
 */

void LCD_setup()  // make sure control lines are at correct levels
{
 
/* DDR(LCD_ce_PORT) |= _BV(LCD_ce_PIN);
 DDR(LCD_rd_PORT) |= _BV(LCD_rd_PIN);
 DDR(LCD_wr_PORT) |= _BV(LCD_wr_PIN);
 DDR(LCD_cd_PORT) |= _BV(LCD_cd_PIN);
 DDR(LCD_rst_PORT) |= _BV(LCD_rst_PIN); */   
 
   LCD_RST_DIR=1;
   LCD_CE_DIR=1;
   LCD_CD_DIR=1;
   LCD_R_DIR=1;
   LCD_W_DIR=1; 
 
 
 

LCD_ce_high();
LCD_rd_high();
LCD_wr_high();
LCD_cd_high();


 LCD_SET_DATADIR_OUT();

 LCD_rst_high();

 delay_ms(5);

 LCD_rst_low();

 delay_ms(5);

 LCD_rst_high();

} // end LCD_setup()

//---------------------------------------------------------------------------

void LCD_init()  // initialize LCD memory and display modes
{
 LCD_dput(LCD_G_BASE%256);
 LCD_dput(LCD_G_BASE>>8);
 LCD_cput(0x42);       // set graphics memory to address LCD_G_BASE

 LCD_dput(LCD_BYTES_PER_ROW);
 LCD_dput(0);
 LCD_cput(0x43);  // n bytes per graphics line  

 LCD_dput(LCD_T_BASE%256);
 LCD_dput(LCD_T_BASE>>8);
 LCD_cput(0x40);       // text memory at address LCD_T_BASE1 

 LCD_dput(LCD_BYTES_PER_ROW%256);
 LCD_dput(LCD_BYTES_PER_ROW>>8);
 LCD_cput(0x41);  // n bytes per text line 

 LCD_cput(0x80);  // mode set: Graphics OR Text, ROM CGen
 

 LCD_cput(0xa0);  // cursor is 8 lines high
 LCD_dput(0x00);
 LCD_dput(0x00);
 LCD_cput(0x21);  // put cursor at (x,y) location   

 LCD_cput(0x9C);  
	      // (For cursor to be visible, need to set up position

} // end LCD_init()


// -------------------------------------------------------------------------------

int LCD_sget(void)  // get LCD display status byte
{
int LCD_status;

 
  LCD_SET_DATADIR_IN();	

  LCD_cd_high();		// bring LCD C/D line high (read status byte)
  LCD_rd_low();		// bring LCD /RD line low (read active)
  LCD_ce_low();		// bring LCD /CE line low (chip-enable active)

  #asm("nop")
  #asm("nop")
  delay_us(1);

  LCD_status = LCD_DATA_PIN;      // read LCD status byte

  LCD_ce_high();         // bring LCD /CE line high, disabling it
  LCD_rd_high();         // deactivate LCD read mode
  LCD_SET_DATADIR_OUT(); // make 8-bit parallel port an output port  
  
 /* printf("%x ",LCD_status);
  delay_ms(1);   */

  return(LCD_status);
} // sget1()

//------------------------------------------------------------------------------------------

void LCD_dput(int byte) // write data byte to LCD module over par. port
		    // assume PC port in data OUTPUT mode
{
  do {} while ((0x03 & LCD_sget()) != 0x03); // wait until display ready
  LCD_cd_low();
  LCD_wr_low();         // activate LCD's write mode

  LCD_DATA_PORT = byte;          // write value to data port
  LCD_ce_low();                       // pulse enable LOW > 80 ns (hah!)

  #asm("nop")
  #asm("nop")
  delay_us(1);

  LCD_ce_high();                       // return enable HIGH
  LCD_wr_high();                       // restore Write mode to inactive

 // using my P5/75 MHz PC with ISA bus, CE stays low for 2 microseconds

} // end dput1()

//------------------------------------------------------------------------------------------

int LCD_dget(void)      // get data byte from LCD module
{
int LCD_byte;

  do {} while ((0x03 & LCD_sget()) != 0x03); // wait until display ready       

  LCD_SET_DATADIR_IN(); // make PC's port an input port
  LCD_wr_high();   // make sure WRITE mode is inactive
  LCD_cd_low();   // data mode
  LCD_rd_low();   // activate READ mode
  LCD_ce_low();   // enable chip, which outputs data

  LCD_byte = LCD_DATA_PIN;  // read data from LCD

  LCD_ce_high();   // disable chip
  LCD_rd_high();   // turn off READ mode
  LCD_SET_DATADIR_OUT(); // make 8-bit parallel port an output port

  return(LCD_byte);
} // LCD_dget1()

//----------------------------------------------------------------------------------------

void LCD_cput(int byte) // write command byte to LCD module
		    // assumes port is in data OUTPUT mode
{
  do {} while ((0x03 & LCD_sget()) != 0x03); // wait until display ready

  LCD_DATA_PORT = byte;  // present data to LCD on PC's port pins

  LCD_cd_high();         // control/status mode
  LCD_rd_high();         // make sure LCD read mode is off
  LCD_wr_low();         // activate LCD write mode
  LCD_ce_low();         // pulse ChipEnable LOW, > 80 ns, enables LCD I/O
  LCD_ce_high();         // disable LCD I/O
  LCD_wr_high();         // deactivate write mode

} // LCD_cput()










/***********************************************************************
Draws a line from x1,y1 go x2,y2. Line can be drawn in any direction.
Set show1 to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void LCD_line(int x1, int y1, int x2, int y2, unsigned char show) 
{
  int dy ;
  int dx ;
  int stepx, stepy, fraction;
  dy = y2 - y1;
  dx = x2 - x1;
  if (dy < 0) 
  {
    dy = -dy;
    stepy = -1;
  }
  else 
  {
    stepy = 1;
  }
  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }
  dy <<= 1;
  dx <<= 1;
  LCD_pixel(x1,y1,show);
  if (dx > dy)
  {
    fraction = dy - (dx >> 1); 
    while (x1 != x2)
    {
      if (fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;  
      LCD_pixel(x1,y1,show);
    }
  }
  else
  {
    fraction = dx - (dy >> 1);
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }
      y1 += stepy;
      fraction += dx;
      LCD_pixel(x1,y1,show);
    }
  }
}







/***********************************************************************
Draws a circle with center at x,y with given radius.
Set show1 to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void LCD_circle(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
    LCD_pixel(x + xc, y + yc, show);
    LCD_pixel(x + xc, y - yc, show);
    LCD_pixel(x - xc, y + yc, show);
    LCD_pixel(x - xc, y - yc, show);
    LCD_pixel(x + yc, y + xc, show);
    LCD_pixel(x + yc, y - xc, show);
    LCD_pixel(x - yc, y + xc, show);
    LCD_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}

 
void LCD_circle_half(int x, int y, int radius, unsigned char show)
{
  int xc = 0;
  int yc ;
  int p ;
  yc=radius;
  p = 3 - (radius<<1);
  while (xc <= yc)  
  {
//    LCD_pixel(x + xc, y + yc, show);
    LCD_pixel(x + xc, y - yc, show);
//    LCD_pixel(x - xc, y + yc, show);
    LCD_pixel(x - xc, y - yc, show);
//    LCD_pixel(x + yc, y + xc, show);
    LCD_pixel(x + yc, y - xc, show);
//    LCD_pixel(x - yc, y + xc, show);
    LCD_pixel(x - yc, y - xc, show);
    if (p < 0)
      p += (xc++ << 2) + 6;
    else
      p += ((xc++ - yc--)<<2) + 10;
  }
}








void LCD_box(int x1, int y1, int x2, int y2, unsigned char show)
{
LCD_line(x1,y1,x2,y1,show);  // up
LCD_line(x1,y2,x2,y2,show);  // down
LCD_line(x2,y1,x2,y2,show);  // right
LCD_line(x1,y1,x1,y2,show);  // left
}


/*

//***********************************************************************
//Draws a line from x,y at given degree from inner_radius to outer_radius.
//Set show to 1 to draw pixel, set to 0 to hide pixel.
//***********************************************************************
void LCD_degree_line(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  LCD_line(fx,fy,tx,ty,show);
}


void LCD_degree_line_bold(int x, int y, int degree, int inner_radius, int outer_radius, unsigned char show)
{
  int fx,fy,tx,ty;

  fx = x + (inner_radius * sin(degree * 3.14 / 180));    //  ???????????????
  fy = y - (inner_radius * cos(degree * 3.14 / 180));
  tx = x + (outer_radius * sin(degree * 3.14 / 180));
  ty = y - (outer_radius * cos(degree * 3.14 / 180));
  LCD_line(fx,fy,tx,ty,show);
  LCD_line(fx+1,fy+1,tx+1,ty+1,show);
  LCD_line(fx-1,fy-1,tx-1,ty-1,show);
}




*/


 /*
void LCD_fill(int x1, int y1, int x2, int y2, unsigned char persent,char first)
{
char M,horizon_line,horizon_line2,i,str1[10];
if(persent>100)return;
if(!first){
	LCD_line(x1,y2,x2,y2,1);  // down
	LCD_line(x2,y1,x2,y2,1);  // right
	LCD_line(x1,y1,x1,y2,1);  // left
	first=1;
	}
M=100/abs(y2-y1);

horizon_line=persent/M;
for(i=0;i<horizon_line;i++)
	LCD_line(x1+2,y2-2-i,x2-2,y2-2-i,1);

horizon_line2=100/M;
for(i=horizon_line;i<horizon_line2;i++)
	LCD_line(x1+2,y2-2-i,x2-2,y2-2-i,0);


sprintf(str1,"%02d%% ",persent);
LCD_print_ram( (x2+x1)/16-1 , (y2+y1)/16,str1);
}
 */


