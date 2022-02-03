void errormsg(unsigned char errorcode,unsigned char showtime)
{

   unsigned char  err1,err2,err3;
   unsigned int   err4;
                                        

   err1=read_e2prom_byte(WRITE_ERROR_INDEX);    
   err2=read_e2prom_byte(WRITE_ERROR_OV); 


   if(errorcode<MIN_CODE_OF_ERROR || errorcode>MAX_CODE_OF_ERROR)return;         
   err3=0;
    if(err1>=SIZE_OF_ERROR_LIST)err1=0;
    else 
    if(err1){if(errorcode==ErrorHistory[err1-1].ErrorNum){err1--;err3=ErrorHistory[err1].ErrorQty+1;}}
    else
     {
            if(err2)        //over flowed
            {
                if(errorcode==ErrorHistory[SIZE_OF_ERROR_LIST-1].ErrorNum){err1=SIZE_OF_ERROR_LIST-1;err3=ErrorHistory[err1].ErrorQty+1;}
            }  
            else
            {
                err1=0;
            }
     }
    

   
    ErrorHistory[err1].ErrorDate=day;                 
    ErrorHistory[err1].ErrorMonth=month;
    ErrorHistory[err1].ErrorYear=year;
    ErrorHistory[err1].ErrorHour=hour;
    ErrorHistory[err1].ErrorMin=minute;
    ErrorHistory[err1].ErrorSec=second;    
    ErrorHistory[err1].ErrorNum=errorcode;   
    ErrorHistory[err1].ErrorQty=err3;    
    
    err1++;
    if(err1>=SIZE_OF_ERROR_LIST){err1=0;write_e2prom_byte(1,WRITE_ERROR_OV);}    
   
   if(err3==0)write_e2prom_byte(err1,WRITE_ERROR_INDEX); 

 
   err1=read_e2prom_byte(NO_OF_ERRORS_L);  
   err4=read_e2prom_byte(NO_OF_ERRORS_H);     
   err4 *= 0x0100;
   err4+= err1;                //get_value_i3 is Number of ALL errors
                  
   err4++;   
                  
   write_e2prom_byte((unsigned char)(err4),NO_OF_ERRORS_L); 
   write_e2prom_byte((unsigned char)(err4>>8),NO_OF_ERRORS_H);    


   
   switch(errorcode)
     {
       case  POWERUP_ZERO_OVER_S1_ALARM :
       case  POWERUP_ZERO_UNDER_S1_ALARM :
       case  OVERLOAD_S1_ALARM :
       case  NO_ADC_CARD_FOUND_S1_ALARM :
       case  ADC_TIME_OVER_S1_ALARM : 
                    show_errorcode_instead_xtra_cntr_scale1=showtime;   
                    lasterrorcode_scale1=errorcode;       
                    buz_start=1;buz_cntr=10;
       break;
       case  POWERUP_ZERO_OVER_S2_ALARM :
       case  POWERUP_ZERO_UNDER_S2_ALARM :
       case  OVERLOAD_S2_ALARM :
       case  NO_ADC_CARD_FOUND_S2_ALARM :
       case  ADC_TIME_OVER_S2_ALARM : 
                    show_errorcode_instead_xtra_cntr_scale2=showtime;   
                    lasterrorcode_scale2=errorcode;     
                    buz_start=1;buz_cntr=10; 
       break;                               
       case  CALIBRATION_LOCKED_ALARM : 
       case  LOAD_FACTORY_ALARM : 
       case  LOAD_EEPROM_PARAMETER_ALARM : 
       case  SAVE_EEPROM_PARAMETER_ALARM :
       case  BROWN_OUT_ALARM : 
       case  WATCH_DOG_ALARM :  
                    show_errorcode_instead_xtra_cntr_scale1=showtime;   
                    lasterrorcode_scale1=errorcode; 
                    show_errorcode_instead_xtra_cntr_scale2=showtime;   
                    lasterrorcode_scale2=errorcode;                
       break;           

     };
       

   
   
}



unsigned char check_master_pass(unsigned char *chk_pass)
{

    if(*(chk_pass+0)!=MASTER_PASS[0])return 0;
    if(*(chk_pass+1)!=MASTER_PASS[1])return 0;
    if(*(chk_pass+2)!=MASTER_PASS[2])return 0;
    if(*(chk_pass+3)!=MASTER_PASS[3])return 0;
    if(*(chk_pass+4)!=MASTER_PASS[4])return 0;
    if(*(chk_pass+5)!=MASTER_PASS[5])return 0;

     return 0xFF;
}

unsigned char check_one_use_pass(unsigned char *chk_pass)
{
    unsigned char coup1,coup2,coup3,temp_coup1[10];        
    unsigned int   coup4,coup5;
    
    jalali(day,month,year,&coup1,&coup2,&coup3);       //coup1:day   coup2:month    coup3:year
    
    coup4 = coup3;          
    coup3=coup1;
    coup4 *= coup2;     
    
    coup5 = coup1;
    coup5 *= 18;
    
    coup4 += 635;
    coup4 -= coup5;
    
    sprintf(temp_coup1,"%04d",coup4);
    
    if(*(chk_pass+0)!=temp_coup1[0])return 0;
    if(*(chk_pass+1)!=temp_coup1[1])return 0;
    if(*(chk_pass+2)!=temp_coup1[2])return 0;
    if(*(chk_pass+3)!=temp_coup1[3])return 0;  
    
    coup1 = MASTER_PASS[0];
    coup1 -= '0';
    coup1 *= 10;     
    
    coup2 = MASTER_PASS[1];
    coup2 -= '0';       
    
    coup2+=coup1;   
    
    coup2 = 99 - coup2;  
    
    coup3 = coup3 /2;    
    coup3 = coup3 * 5;    
    
    coup2 += coup3;
    
    coup2 = coup2 % 100;
    
    sprintf(temp_coup1,"%02d",coup2);
    
    
    if(*(chk_pass+4)!=temp_coup1[0])return 0;
    if(*(chk_pass+5)!=temp_coup1[1])return 0;

     return 0xFF;
}




unsigned char select_unit_str(unsigned char *con_str,unsigned char using_s)
{
   unsigned char temp_s_str[3],out_unit=0;
   
  if(using_s==1)
    {
      if(scale1_unit==0)sprintf(temp_s_str,"gr"); 
      else if(scale1_unit==1)
         {
          if(scale1_float>0)sprintf(temp_s_str,"gr");
          else sprintf(temp_s_str,"kg");
         }
      else 
         {
          if(scale1_float>0)sprintf(temp_s_str,"kg");  
          else sprintf(temp_s_str,"tn");
         }
    }
  if(using_s==2)
    {
      if(scale2_unit==0)sprintf(temp_s_str,"gr"); 
      else if(scale2_unit==1)
         {
          if(scale2_float>0)sprintf(temp_s_str,"gr");
          else sprintf(temp_s_str,"kg");
         }
      else 
         {
          if(scale2_float>0)sprintf(temp_s_str,"kg");  
          else sprintf(temp_s_str,"tn");
         }
    }            
   
   if(con_str!=NULL)strcpy(con_str,temp_s_str);   
   
   if(*(temp_s_str)=='g')return 0;
   if(*(temp_s_str)=='k')return 1;
   if(*(temp_s_str)=='t')return 2;      
   
   
}              
                                    


unsigned char  convert_num_point_str(unsigned char *con_str/*,unsigned char need2conv*/,unsigned long int inp_reg,unsigned char using_s,
                                        unsigned char chr_width,unsigned char sign)
{
  unsigned char temp_cni=0,temp_cni2=0,temp_cni3=0,temp_cni4=0,temp_cnstr[10],float_p,s_digits,s_unit;  
  unsigned long int temp_lint;
  
  if(using_s==1)
   {
    float_p=scale1_float;
//    sign=scale1_sign;
    s_digits=scale1_digits;
    s_unit=scale1_unit;      

   }    
  else
   {
    float_p=scale2_float;
//    sign=scale2_sign;
    s_digits=scale2_digits;
    s_unit=scale2_unit;
   }
   
   temp_lint = inp_reg;    //if unit is in gram no float point is used
   if(s_unit>0)            // value is in grams and need to convert in current format of scale
     {
       
       if(s_unit>=1)                   
               {
                 temp_cni = 3 - float_p;
                 for(;temp_cni>0;temp_cni--)temp_lint = temp_lint / 10;         //gr-->kg 
               }
       if(s_unit==2)
                {
                 temp_lint = temp_lint / 1000;          //kg-->tn            //HERE scale_float_value is float value of weigh with sign in grams                 
                }           
                 
     }   
 
  if(s_digits==0)    //4digits
   {
     if(temp_lint>9999){sprintf(con_str,"----");return chr_width*4;}
   } 
  if(s_digits==1)    //5digits
   {
     if(temp_lint>99999){sprintf(con_str,"-----");return chr_width*5;}
   } 
  if(s_digits==2)    //6digits
   {
     if(temp_lint>999999){sprintf(con_str,"------");return chr_width*6;}
   }       
    
  if(temp_lint>99999)
           sprintf(temp_cnstr,"%d%04d",temp_lint/10000,temp_lint%10000); 
  else           
  if(temp_lint>9999)                        
           sprintf(temp_cnstr,"%d%04d",temp_lint/10000,temp_lint%10000); 
   else
   sprintf(temp_cnstr,"%d",temp_lint); 
   

  if(float_p>=strlen(temp_cnstr) && float_p && s_unit!=0)
      {
        if(float_p==3)sprintf(temp_cnstr,"%04d",temp_lint); 
        if(float_p==2)sprintf(temp_cnstr,"%03d",temp_lint);
        if(float_p==1)sprintf(temp_cnstr,"%02d",temp_lint);
      }     
   

  if(sign=='-')*(con_str)='-';else *(con_str)=' ';         
                     
  if(float_p && s_unit!=0)temp_cni2 = strlen(temp_cnstr) - float_p+1;else temp_cni2= 0xff;
       

 temp_cni3=0; 
 temp_cni=1;
  do
  {
    temp_cni4=*(temp_cnstr+temp_cni3);
    *(con_str+temp_cni) = temp_cni4;
  
    temp_cni++; 
    temp_cni3++;
    
    if(temp_cni2==temp_cni){*(con_str+temp_cni)='.';temp_cni++;}
  }while(temp_cni4);    
  
  temp_cni4=strlen(con_str) *  chr_width;   
  if(chr_width==13)
     {
          if(strrchr(con_str,'.'))return temp_cni4 - 9;
          else return temp_cni4;
     }
  else
     {
          if(strrchr(con_str,'.'))return temp_cni4 - 15;
          else return temp_cni4;
     }  
              
    
}



unsigned long int convert_unit(unsigned long int inp_reg,unsigned char unit_code)
{
  if(unit_code==0)return inp_reg;
  if(unit_code==1)return (inp_reg / 1000);
  if(unit_code==2)return (inp_reg / 1000000);

}         


unsigned long int invert_unit(unsigned long int inp_reg,unsigned char unit_code)
{
  if(unit_code==0)return inp_reg;
  if(unit_code==1)return (inp_reg * 1000);
  if(unit_code==2)return (inp_reg * 1000000);

}

unsigned long int invert_unit_fp(unsigned long int inp_reg,unsigned char unit_code,unsigned char fp_code)
{
  unsigned long int inp_temp;   
  unsigned char zeros_no;
  
  inp_temp = inp_reg;    
  zeros_no=0;
  
  if(unit_code==0)return inp_temp;
  else
  if(unit_code==1)zeros_no = 3;
  else
  if(unit_code==2)zeros_no = 6;   
  
  if(fp_code==1)zeros_no-=1;
  if(fp_code==2)zeros_no-=2;
  if(fp_code==3)zeros_no-=3;     
  
  while(zeros_no)
   {
      inp_temp = inp_temp / 10;
      zeros_no--;
   }
 
 return inp_temp;  

}



void printf_longnum(unsigned char *out_text,unsigned long int longnum,unsigned char digs) 
{
    unsigned char temp_text1[10];
     
    if(digs==5)
     {
      if(longnum>10000)
        sprintf(temp_text1,"%d%04d",longnum/10000,longnum%10000); 
      else
        sprintf(temp_text1,"%d",longnum%10000); 
        
     //   5-strlen(temp_text1);
        
     }
    else
     {
      if(longnum>1000000)
        sprintf(temp_text1,"%3d%04d",longnum/10000,longnum%10000); 
      else
      if(longnum>100000)
        sprintf(temp_text1,"%2d%04d",longnum/10000,longnum%10000); 
      else   
      if(longnum>10000)
        sprintf(temp_text1,"%d%04d",longnum/10000,longnum%10000); 
      else     
        sprintf(temp_text1,"%d",longnum); 
     }             
 
     strcpy(out_text,temp_text1);

}  

           
void load_float_in_string(float floatnum,unsigned char *str_tmp)
{

     float lls1;   
     unsigned long int lls2,lls3;
     unsigned char lls4,lls5;     
     /* 
     lls1 = 100000 * modf(floatnum,&lls2);  
     for(lls3=0;lls3<16;lls3++)get_value_ch[lls3]=0; 
     
     lls4 = lls1 % 10; 
     lls1 = lls1 / 10; 
     lls3 = (char)(lls4);
     if(lls3==10){lls3=0;lls1+=1;}
     
     sprintf(get_value_ch,"%u.%04u%u",(int)(lls2),(int)(lls1),(int)(lls3));   
     */  
     
     
     if(floatnum<0){lls5=1;floatnum=-floatnum;}
     else lls5=0;
     
     for(lls3=0;lls3<16;lls3++)
            *(str_tmp+lls3)=0; 


     lls2 = floatnum + 0.000005;  
     lls1 = floatnum - lls2;  
     lls1 = 100000 * lls1;  
     
     lls3 = lls1;
     

     lls4 = (char)(lls3 % 10);
     lls3 = lls3 / 10;
     
     if(lls5==0)
         sprintf(str_tmp,"%u.%04u%u",(int)(lls2),(int)(lls3),lls4);       
     else
         sprintf(str_tmp,"-%u.%04u%u",(int)(lls2),(int)(lls3),lls4);       
     
     
}  


float convert_string_to_float(unsigned char *str_temp)
{    
     unsigned long int csf_l1,csf_l2,csf_l3; 
     
     unsigned char csf_n1,csf_n2,csf_n3,csf_i2,csf_i4;    
     
     float csf_f1,csf_f2,csf_f3;
           
           if(*str_temp=='-')
            {
             csf_i4=1; 
            } 
             else csf_i4=0;
             
             
             
           
           csf_i2=0;
           csf_n3 = strlen(str_temp); 
           for(csf_n1=0;csf_n1<csf_n3;csf_n1++)
                {
                 if( *(str_temp+csf_n1)=='.' ){csf_i2=csf_n1;break;}
                }
              
           
           
           if(csf_i2)csf_n2=csf_i2;   //get_value_i2 place of .
           else csf_n2=strlen(str_temp);  
           
           csf_l2=1;
           csf_l1=0;
           
           for(csf_n1=1;csf_n1<=csf_n2;csf_n1++)
            {
              if(csf_i4==1 && csf_n2==csf_n1)break;
              csf_l3  = *(str_temp + (csf_n2-csf_n1) )-'0';
              csf_l3 *= csf_l2;
              csf_l1 += csf_l3;
              csf_l2 *= 10;
            }  
             // csf_l1  integer part is saved

           
           csf_f1=0;  
           if(csf_i2)
            {
               csf_n2 = csf_i2;
               csf_n3 = strlen(str_temp);  
               csf_f2 = 0.10;
               
              for(csf_n1=csf_n2+1;csf_n1<csf_n3;csf_n1++)
               {
                 csf_f3  = *(str_temp + csf_n1 ) - '0';
                 csf_f3 *= csf_f2;
                 csf_f1 += csf_f3;
                 csf_f2 *= 0.10;                
               }               
              
              csf_f2 += 0.000001;
            }   
            
           csf_f1 +=  csf_l1;   
           
          if(csf_i4)csf_f1 = -csf_f1; 
          
           return csf_f1;   
}  



   

void write_weighing_data_eeprom(struct weighing_data_record_format record_format,unsigned char address)
{
   unsigned char str_wwd[32],temp_wwd;
   unsigned int itemp_wwd=0;
   
   
   
   
   str_wwd[0] = record_format.first_weighing_value_msb;
   str_wwd[1] = record_format.first_weighing_value_mid;
   str_wwd[2] = record_format.first_weighing_value_lsb;
   
   str_wwd[3] = record_format.vehicle_no_msb;
   str_wwd[4] = record_format.vehicle_no_mid;
   str_wwd[5] = record_format.vehicle_no_lsb;

   
   itemp_wwd = record_format.conseq1_serial;
   str_wwd[6] = itemp_wwd;  
   itemp_wwd >>=8;
   str_wwd[7] = itemp_wwd;
   

   str_wwd[8] = record_format.date_day;
   str_wwd[9] = record_format.date_month;
   str_wwd[10] = record_format.date_year;

   temp_wwd =  record_format.scale_no;
   temp_wwd =  temp_wwd <<6;
   temp_wwd &= 0b11000000;
   temp_wwd |= record_format.time_hour;
   
   str_wwd[11] = temp_wwd;
   
   str_wwd[12] =  record_format.time_minute;
   str_wwd[13] =  record_format.time_second;
   
   str_wwd[14] = record_format.sort_code;
   
   str_wwd[15] = record_format.customer_name[0];
   str_wwd[16] = record_format.customer_name[1];
   str_wwd[17] = record_format.customer_name[2];
   str_wwd[18] = record_format.customer_name[3];
   str_wwd[19] = record_format.customer_name[4];
   str_wwd[20] = record_format.customer_name[5];
   str_wwd[21] = record_format.customer_name[6];
   str_wwd[22] = record_format.customer_name[7];
   str_wwd[23] = record_format.customer_name[8];
   str_wwd[24] = record_format.customer_name[9];
   str_wwd[25] = record_format.customer_name[10];
   str_wwd[26] = record_format.customer_name[11];
   str_wwd[27] = record_format.customer_name[12];
   str_wwd[28] = record_format.customer_name[13];
   str_wwd[29] = record_format.customer_name[14];
   str_wwd[30] = record_format.customer_name[15];
   
   itemp_wwd =  address+WEIGHING_DATA_START_PAGE_ADDRESS;
   itemp_wwd <<=5;     
   itemp_wwd &= 0xFFE0;   
   
   write_e2prom_page(str_wwd,itemp_wwd,32);
 

}



void read_weighing_data_eeprom(struct weighing_data_record_format *record_format,unsigned char address)
 {
   unsigned char str_rwd[32],temp_rwd;
   unsigned int itemp_rwd;
   
   itemp_rwd =  address+WEIGHING_DATA_START_PAGE_ADDRESS;
   itemp_rwd <<=5;     
   itemp_rwd &= 0xFFE0;
   
   read_e2prom_page(str_rwd,itemp_rwd);
   
   record_format->first_weighing_value_msb = str_rwd[0];
   record_format->first_weighing_value_mid = str_rwd[1];
   record_format->first_weighing_value_lsb = str_rwd[2];    
   
   record_format->vehicle_no_msb = str_rwd[3];
   record_format->vehicle_no_mid = str_rwd[4];
   record_format->vehicle_no_lsb = str_rwd[5]; 

   itemp_rwd = str_rwd[7];  
   itemp_rwd <<=8;
   itemp_rwd +=str_rwd[6];
   record_format->conseq1_serial = itemp_rwd;

   record_format->date_day = str_rwd[8];
   record_format->date_month = str_rwd[9];
   record_format->date_year = str_rwd[10]; 

   temp_rwd = str_rwd[11];       //  str_rwd[11] = (2bits : scale_no) (6bits : time_hour)
   
   record_format->time_hour = (temp_rwd&0x3F);
   record_format->time_minute = str_rwd[12];
   record_format->time_second = str_rwd[13]; 
   
   temp_rwd = temp_rwd>>6;
   record_format->scale_no = (temp_rwd&0x03);     
   
   record_format->sort_code = str_rwd[14];            
   
   record_format->customer_name[0]  = str_rwd[15];
   record_format->customer_name[1]  = str_rwd[16];
   record_format->customer_name[2]  = str_rwd[17];
   record_format->customer_name[3]  = str_rwd[18];
   record_format->customer_name[4]  = str_rwd[19];
   record_format->customer_name[5]  = str_rwd[20];
   record_format->customer_name[6]  = str_rwd[21];
   record_format->customer_name[7]  = str_rwd[22];
   record_format->customer_name[8]  = str_rwd[23];
   record_format->customer_name[9]  = str_rwd[24];
   record_format->customer_name[10] = str_rwd[25];
   record_format->customer_name[11] = str_rwd[26];
   record_format->customer_name[12] = str_rwd[27];
   record_format->customer_name[13] = str_rwd[28];
   record_format->customer_name[14] = str_rwd[29];
   record_format->customer_name[15] = str_rwd[30];
   

 
 }         
 
void read_sort_name(unsigned char *record_format,unsigned char sort_code_address)
 {
   unsigned char str_rsn[32];
   unsigned int itemp_rsn;   
   
   itemp_rsn =  sort_code_address;
   itemp_rsn *= 16;     
   itemp_rsn &= 0xFFF0;   
   
   itemp_rsn += SORT_NAMES_INITIAL_ADDRESS;
   
   read_e2prom_page(str_rsn,itemp_rsn);        
   
   for(itemp_rsn=0;itemp_rsn<16;itemp_rsn++)
    {
       *(record_format+itemp_rsn) = str_rsn[itemp_rsn];
    }

   *(record_format+16) = 0;  
  if(*(record_format)==0 || *(record_format)==0xFF){*record_format='0';*(record_format+1)=0;}

 }   
 
void write_sort_name(unsigned char *record_format,unsigned char sort_code_address)
 {
   unsigned char str_wsn[32];
   unsigned int itemp_wsn;   
   
   for(itemp_wsn=0;itemp_wsn<16;itemp_wsn++)
    {
       str_wsn[itemp_wsn] = *(record_format+itemp_wsn);
    }

   str_wsn[15] = 0;    
   
   itemp_wsn =  sort_code_address;
   itemp_wsn *= 16;     
   itemp_wsn &= 0xFFF0;   
   
   itemp_wsn += SORT_NAMES_INITIAL_ADDRESS;   
   
   write_e2prom_page(str_wsn,itemp_wsn,16);

 }  
           


unsigned char check_ID_truck_in_memory(unsigned long int idchecked,unsigned char tempindex)      //get id_truck and returns inbound code
{
  unsigned char cit1,cit2,cit3,cit4,cit6;  
  
  struct weighing_data_record_format  weighing_data_temp;
  
  unsigned long int cit5;
  
  cit6=tempindex-1;
  
  cit5 = idchecked;
  cit1 = (unsigned char)(cit5);  //cit1 is lsb
  cit5 = (cit5>>8);
  cit2 = (unsigned char)(cit5);  //cit2 is mid
  cit5 = (cit5>>8);
  cit3 = (unsigned char)(cit5);  //cit3 is msb
  
  
  for(cit4=0;cit4<NUM_OF_FIRST_WEIGHING_MEMORY;cit4++)   
    {
       #asm("wdr")
       
       read_weighing_data_eeprom(&weighing_data_temp,cit6);
       
       if(cit1==weighing_data_temp.vehicle_no_lsb &&
          cit2==weighing_data_temp.vehicle_no_mid &&
          cit3==weighing_data_temp.vehicle_no_msb)return cit6; 
          
       if(cit6)cit6--;   
       else  cit6=NUM_OF_FIRST_WEIGHING_MEMORY-1;
    } 
  
  return 0xFF;          //0xFF means Failed  

}


void display_weight(void)
{    
unsigned char temp_dw1[50],temp_dw2[20],temp_kdw0[3],temp_dwstr[20],dw2=0,dw3,dw4,dw5;
unsigned long int temp_idw0,temp_idw1;
float temp_fdw;

 if(countout_display)
  {
 if(scales_func_mode==0)       //0 -->both    3 --> +   1-->1  2-->2
   {       
     temp_fdw = scale1_raw_value ;
     temp_fdw = temp_fdw / (0x80000);       //2^19 = 524288
     temp_fdw = temp_fdw - 1;   

     if(temp_fdw<0){sprintf(temp_dw2,"-");temp_fdw=-temp_fdw;} else sprintf(temp_dw2," ");
     
     dw5 = (unsigned char)( temp_fdw);
     temp_fdw = temp_fdw - dw5;   
     
     temp_fdw = temp_fdw * 100000;    
     
     temp_idw0 =(unsigned long int)( temp_fdw);
     
     temp_idw1 = temp_idw0 % 10;
     
     temp_idw0 = temp_idw0 / 10;   
     sprintf(temp_dw1,"%s%02d.%04d%d",temp_dw2,dw5,temp_idw0,temp_idw1);
     LCD_setxy(5,18);
     LCD_put_stringnumber_arial(temp_dw1,18);       
  

     temp_fdw = scale2_raw_value ;
     temp_fdw = temp_fdw / (0x80000);       //2^19 = 524288
     temp_fdw = temp_fdw - 1;   

     if(temp_fdw<0){sprintf(temp_dw2,"-");temp_fdw=-temp_fdw;} else sprintf(temp_dw2," ");
     
     dw5 = (unsigned char)( temp_fdw);
     temp_fdw = temp_fdw - dw5;   
     
     temp_fdw = temp_fdw * 100000;    
     
     temp_idw0 =(unsigned long int)( temp_fdw);
     
     temp_idw1 = temp_idw0 % 10;
     
     temp_idw0 = temp_idw0 / 10;   
     sprintf(temp_dw1,"%s%02d.%04d%d",temp_dw2,dw5,temp_idw0,temp_idw1);
     LCD_setxy(2+120,18);
     LCD_put_stringnumber_arial(temp_dw1,18);       
  
     
     
                          
     
                            
   
   }
 else  
 if(scales_func_mode==1)       //0 -->both    3 --> +   1-->1  2-->2
   {
     temp_fdw = scale1_raw_value ;
     temp_fdw = temp_fdw / (0x80000);       //2^19 = 524288
     temp_fdw = temp_fdw - 1;   

     if(temp_fdw<0){sprintf(temp_dw2,"-");temp_fdw=-temp_fdw;} else sprintf(temp_dw2," ");
     
     dw5 = (unsigned char)( temp_fdw);
     temp_fdw = temp_fdw - dw5;   
     
     temp_fdw = temp_fdw * 100000;    
     
     temp_idw0 =(unsigned long int)( temp_fdw);
     
     temp_idw1 = temp_idw0 % 10;
     
     temp_idw0 = temp_idw0 / 10;   
     sprintf(temp_dw1,"%s%02d.%04d%d",temp_dw2,dw5,temp_idw0,temp_idw1);
     LCD_setxy(30,5);
     LCD_put_stringnumber_arial(temp_dw1,36);       
     scale2_weig_string_length=dw3;      
     
     LCD_setxy(205,25);  
     sprintf(temp_dw1,"mV/V"); 
       LCD_put_string_tahoma(temp_dw1,8);
   
   
   }  
  else
 if(scales_func_mode==2)       //0 -->both    3 --> +   1-->1  2-->2
   {
     temp_fdw = scale2_raw_value ;
     temp_fdw = temp_fdw / (0x80000);       //2^19 = 524288
     temp_fdw = temp_fdw - 1;   

     if(temp_fdw<0){sprintf(temp_dw2,"-");temp_fdw=-temp_fdw;} else sprintf(temp_dw2," ");
     
     dw5 = (unsigned char)( temp_fdw);
     temp_fdw = temp_fdw - dw5;   
     
     temp_fdw = temp_fdw * 100000;    
     
     temp_idw0 =(unsigned long int)( temp_fdw);
     
     temp_idw1 = temp_idw0 % 10;
     
     temp_idw0 = temp_idw0 / 10;   
     sprintf(temp_dw1,"%s%02d.%04d%d",temp_dw2,dw5,temp_idw0,temp_idw1);
     LCD_setxy(30,5);
     LCD_put_stringnumber_arial(temp_dw1,36);       
     scale2_weig_string_length=dw3;      
     
     LCD_setxy(205,25);  
     sprintf(temp_dw1,"mV/V"); 
       LCD_put_string_tahoma(temp_dw1,8);
   }      
   
  
  
  
   return;
  }
                  
  
  
 if(scales_func_mode==0)       //0 -->both    3 --> +   1-->1  2-->2
   {
     
     LCD_setxy(20,2);
     if(scale1_mode=='z')LCD_draw_icon(zero,19,10);  
     else
     if(scale1_mode=='f')LCD_draw_icon(fixed,19,10);
     else LCD_draw_icon(blank,19,10);  
          
                                   
     dw3=convert_num_point_str(temp_dw1,scale1_value,1,13,scale1_sign);      
                   
     dw2 = 95 - dw3;  //right limit of NUMBER
     LCD_setxy(0,18); 
     if(scale1_weig_string_length>dw3)  
          for(dw4=0;dw4<dw2;dw4++)LCD_putnumber_arial('|',18);
     
     LCD_setxy(dw2,18);
     LCD_put_stringnumber_arial(temp_dw1,18);       
     scale1_weig_string_length=dw3;
     
     LCD_setxy(95,25);  
     if(manual_tare_s1_state || pb_tare_s1_state)
      { 
        if(manual_tare_s1_state)dw5='M';
        if(pb_tare_s1_state)dw5='P';
        
        if(scale1_unit==0)sprintf(temp_dw1,"g%cT",dw5); 
        else
        if(scale1_unit==1)sprintf(temp_dw1,"K%cT",dw5);
        else if(scale1_unit==2)sprintf(temp_dw1,"t%cT",dw5); 
        LCD_put_string_tahoma(temp_dw1,8);   
      }
      else     
      { 
        if(scale1_unit==0)sprintf(temp_dw1,"g  "); 
        else
        if(scale1_unit==1)sprintf(temp_dw1,"Kg ");
        else if(scale1_unit==2)sprintf(temp_dw1,"ton"); 
       LCD_put_string_tahoma(temp_dw1,8);
      }
     
     
     LCD_setxy(95,15);     
     if(tenfold_out_cntr && using_scale==1)LCD_puttahoma('X',8); else LCD_puttahoma(' ',8);        
     
     LCD_setxy(110,15);    
     if(pb_zero_s1_state)LCD_puttahoma('0',8); else LCD_puttahoma(' ',8);
                          
     
                            
     LCD_setxy(20+121,2);
     if(scale2_mode=='z')LCD_draw_icon(zero,19,10);  
     else
     if(scale2_mode=='f')LCD_draw_icon(fixed,19,10);
     else LCD_draw_icon(blank,19,10);  
          
                                   
     dw3=convert_num_point_str(temp_dw1,scale2_value,2,13,scale2_sign);      
                   
     dw2 = 95 - dw3;  //right limit of NUMBER
     LCD_setxy(121,18); 
     if(scale2_weig_string_length>dw3)  
          for(dw4=0;dw4<dw2;dw4++)LCD_putnumber_arial('|',18);
     
     LCD_setxy(121+dw2,18);
     LCD_put_stringnumber_arial(temp_dw1,18);       
     scale2_weig_string_length=dw3;

     LCD_setxy(121+95,25);  
     if(manual_tare_s2_state || pb_tare_s2_state)
      {                

        if(manual_tare_s2_state)dw5='M';
        if(pb_tare_s2_state)dw5='P';      
        if(scale2_unit==0)sprintf(temp_dw1,"g%cT",dw5); 
        else
        if(scale2_unit==1)sprintf(temp_dw1,"K%cT",dw5);
        else if(scale2_unit==2)sprintf(temp_dw1,"t%cT",dw5); 
        LCD_put_string_tahoma(temp_dw1,8);   
      }
      else     
      { 
        if(scale2_unit==0)sprintf(temp_dw1,"g  "); 
        else
        if(scale2_unit==1)sprintf(temp_dw1,"Kg ");
        else if(scale2_unit==2)sprintf(temp_dw1,"ton"); 
       LCD_put_string_tahoma(temp_dw1,8);
      }
     
     
     LCD_setxy(121+95,15);     
     if(tenfold_out_cntr && using_scale==2)LCD_puttahoma('X',8); else LCD_puttahoma(' ',8);
     LCD_setxy(121+110,15);    
     if(pb_zero_s2_state)LCD_puttahoma('0',8); else LCD_puttahoma(' ',8);

     if(show_errorcode_instead_xtra_cntr_scale1)
      {   
       if(lasterrorcode_scale1)strcpyf(temp_dw2,errorshortcode_table[lasterrorcode_scale1-MIN_CODE_OF_ERROR]);     
       sprintf(temp_dw1," %s ",temp_dw2);
       LCD_setxy(42 ,7);          
       LCD_put_string_tahoma(temp_dw1,'m');   //tempdw2 Second Line     
       while(currentx<113)LCD_puttahoma(' ',7);    
                
       } 
      else  
    if(homepos==SHOWINFO)
     {
       sprintf(temp_dw2,"%dW%d",scale1_range_display+scale1_ranges-1,scale1_ranges+1);   
               
       if(scale1_range_display==2 || scale1_ranges==0)temp_idw0=convert_unit(scale1_interval[2],scale1_interval_unit);
       else if(scale1_range_display==1)temp_idw0=convert_unit(scale1_interval[1],scale1_interval_unit);
       else if(scale1_range_display==0)temp_idw0=convert_unit(scale1_interval[0],scale1_interval_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,""); 
         
       sprintf(temp_dwstr," e:%d%s",temp_idw0,temp_kdw0);
       if(scale1_interval_unit==0)strcatf(temp_dwstr,"g");      
       if(scale1_interval_unit==1)strcatf(temp_dwstr,"KG");   
       if(scale1_interval_unit==2)strcatf(temp_dwstr,"tn"); 
     
       strcat(temp_dw2,temp_dwstr);
  
       LCD_setxy(42 ,7); 
       LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line       
       while(currentx<113)LCD_puttahoma(' ',7);   
     }
       
     if(show_errorcode_instead_xtra_cntr_scale2)
      {   
       if(lasterrorcode_scale2)strcpyf(temp_dw2,errorshortcode_table[lasterrorcode_scale2-MIN_CODE_OF_ERROR]); 
       sprintf(temp_dw1," %s ",temp_dw2);
       LCD_setxy(120+42 ,7);  
       LCD_put_string_tahoma(temp_dw1,'m');   //tempdw2 Second Line    
       while(currentx<235)LCD_puttahoma(' ',7);     
                 
       } 
      else     
     if(homepos==SHOWINFO)
     {
     sprintf(temp_dw2,"%dW%d",scale2_range_display+scale2_ranges-1,scale2_ranges+1);   
               
     if(scale2_range_display==2 || scale2_ranges==0)temp_idw0=convert_unit(scale2_interval[2],scale2_interval_unit);
     else if(scale2_range_display==1)temp_idw0=convert_unit(scale2_interval[1],scale2_interval_unit);
     else if(scale2_range_display==0)temp_idw0=convert_unit(scale2_interval[0],scale2_interval_unit);
     if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,""); 
         
     sprintf(temp_dwstr," e:%d%s",temp_idw0,temp_kdw0);
     if(scale2_interval_unit==0)strcatf(temp_dwstr,"g");      
     if(scale2_interval_unit==1)strcatf(temp_dwstr,"KG");   
     if(scale2_interval_unit==2)strcatf(temp_dwstr,"tn"); 
     
     strcat(temp_dw2,temp_dwstr);  
     LCD_setxy(120+42 ,7); 
     LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line       
     while(currentx<235)LCD_puttahoma(' ',7);
     }

                 

   }  
   
   
 if(scales_func_mode==1)       //0 -->both    3 --> +   1-->1  2-->2
   {
    
    LCD_setxy(10,2);
     if(scale1_mode=='z')LCD_draw_icon(zero,19,10);  
     else
     if(scale1_mode=='f')LCD_draw_icon(fixed,19,10);
     else LCD_draw_icon(blank,19,10);   
     
                                            
     dw3=convert_num_point_str(temp_dw1,scale1_value,1,20,scale1_sign);      
     
     
     dw2 = 200 - dw3;  //right limit of NUMBER
     LCD_setxy(55,5); 
     if(scale1_weig_string_length>dw3)  
          for(dw4=0;dw4<dw2-55;dw4++)LCD_putnumber_arial('|',36);
     
     LCD_setxy(dw2,5);
     LCD_put_stringnumber_arial(temp_dw1,36);       
     scale1_weig_string_length=dw3;      
     
     LCD_setxy(205,25);  
     if(manual_tare_s1_state || pb_tare_s1_state)
      {             

        if(manual_tare_s1_state)dw5='M';
        if(pb_tare_s1_state)dw5='P';      
        if(scale1_unit==0)sprintf(temp_dw1,"g %cT",dw5); 
        else
        if(scale1_unit==1)sprintf(temp_dw1,"Kg%cT",dw5);
        else if(scale1_unit==2)sprintf(temp_dw1,"tn%cT",dw5); 
        LCD_put_string_tahoma(temp_dw1,8);   
      }
      else     
      { 
        if(scale1_unit==0)sprintf(temp_dw1,"g   "); 
        else
        if(scale1_unit==1)sprintf(temp_dw1,"Kg  ");
        else if(scale1_unit==2)sprintf(temp_dw1,"ton "); 
       LCD_put_string_tahoma(temp_dw1,8);
      }
     
     
     LCD_setxy(205,15);     
     if(tenfold_out_cntr && using_scale==1)LCD_draw_icon(x10,11,5); else LCD_draw_icon(blank,11,5);    
     
     LCD_setxy(220,15);    
     if(pb_zero_s1_state)sprintf(temp_dw1,"PZ"); else  sprintf(temp_dw1,"  ");
     LCD_put_string_tahoma(temp_dw1,8);   
     
     
     if(show_errorcode_instead_xtra_cntr_scale1)
      {   
        sprintf(temp_dw2,"   ");   
        LCD_setxy(18,28);               
        LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line   
           
        if(lasterrorcode_scale1)strcpyf(temp_dw2,errorshortcode_table[lasterrorcode_scale1-MIN_CODE_OF_ERROR]);     
        sprintf(temp_dw1," %s ",temp_dw2);
        LCD_setxy(5 ,20);        
        sprintf(temp_dw2,"  ");LCD_put_string_tahoma(temp_dw2,7);  
        LCD_put_string_tahoma(temp_dw1,'m');   //tempdw2 Second Line     
        while(currentx<55)LCD_puttahoma(' ',7);    
       } 
     else       
     if(homepos==SHOWINFO)
     {  
       sprintf(temp_dw2,"%dW%d",scale1_range_display+scale1_ranges-1,scale1_ranges+1);   
       LCD_setxy(18,28);               
       LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line          
               
       if(scale1_range_display==2 || scale1_ranges==0)temp_idw0=convert_unit(scale1_interval[2],scale1_interval_unit);
       else if(scale1_range_display==1)temp_idw0=convert_unit(scale1_interval[1],scale1_interval_unit);
       else if(scale1_range_display==0)temp_idw0=convert_unit(scale1_interval[0],scale1_interval_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,""); 
         
       sprintf(temp_dwstr,"e:%d%s",temp_idw0,temp_kdw0);
       if(scale1_interval_unit==0)strcatf(temp_dwstr,"g");      
       if(scale1_interval_unit==1)strcatf(temp_dwstr,"KG");   
       if(scale1_interval_unit==2)strcatf(temp_dwstr,"tn"); 
     
       LCD_setxy(2,20);               
       LCD_put_string_tahoma(temp_dwstr,7);   //tempdw2 Second Line       
       while(currentx<55)LCD_puttahoma(' ',7);      
     }
  
     
     
   }    
   
 if(scales_func_mode==2)       //0 -->both    3 --> +   1-->1  2-->2
   {
    

     LCD_setxy(10,2);
     if(scale2_mode=='z')LCD_draw_icon(zero,19,10);  
     else
     if(scale2_mode=='f')LCD_draw_icon(fixed,19,10);
     else LCD_draw_icon(blank,19,10);   
     
                                            
     dw3=convert_num_point_str(temp_dw1,scale2_value,2,20,scale2_sign);      
     
     
     dw2 = 200 - dw3;  //right limit of NUMBER
     LCD_setxy(55,5); 
     if(scale2_weig_string_length>dw3)  
          for(dw4=0;dw4<dw2-55;dw4++)LCD_putnumber_arial('|',36);
     
     LCD_setxy(dw2,5);
     LCD_put_stringnumber_arial(temp_dw1,36);       
     scale2_weig_string_length=dw3;      
     
     LCD_setxy(205,25);  
     if(manual_tare_s2_state || pb_tare_s2_state)
      {             

        if(manual_tare_s2_state)dw5='M';
        if(pb_tare_s2_state)dw5='P';      
        if(scale2_unit==0)sprintf(temp_dw1,"g %cT",dw5); 
        else
        if(scale2_unit==1)sprintf(temp_dw1,"Kg%cT",dw5);
        else if(scale2_unit==2)sprintf(temp_dw1,"tn%cT",dw5); 
        LCD_put_string_tahoma(temp_dw1,8);   
      }
      else                                                                         
      {     
        if(scale2_unit==0)sprintf(temp_dw1,"g   "); 
        else
        if(scale2_unit==1)sprintf(temp_dw1,"Kg  ");
        else if(scale2_unit==2)sprintf(temp_dw1,"ton "); 
       LCD_put_string_tahoma(temp_dw1,8);
      }
     
     
     LCD_setxy(205,15);     
     if(tenfold_out_cntr && using_scale==2)LCD_draw_icon(x10,11,5); else LCD_draw_icon(blank,11,5);
     
     LCD_setxy(220,15);    
     if(pb_zero_s2_state)sprintf(temp_dw1,"PZ"); else  sprintf(temp_dw1,"  ");
     LCD_put_string_tahoma(temp_dw1,8);     

     if(show_errorcode_instead_xtra_cntr_scale2)
      {   
        sprintf(temp_dw2,"   ");   
        LCD_setxy(18,28);               
        LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line   
           
        if(lasterrorcode_scale2)strcpyf(temp_dw2,errorshortcode_table[lasterrorcode_scale2-MIN_CODE_OF_ERROR]);     
        sprintf(temp_dw1," %s ",temp_dw2);
        LCD_setxy(2 ,20);   
//        sprintf(temp_dw2,"  ");LCD_put_string_tahoma(temp_dw2,7);       
        LCD_put_string_tahoma(temp_dw1,'m');   //tempdw2 Second Line     
        //while(currentx<55)LCD_puttahoma(' ',7);    
       } 
      else   
     if(homepos==SHOWINFO)
     {     
       sprintf(temp_dw2,"%dW%d",scale2_range_display+scale2_ranges-1,scale2_ranges+1);   
       LCD_setxy(18,28);               
       LCD_put_string_tahoma(temp_dw2,7);   //tempdw2 Second Line            
               
       if(scale2_range_display==2 || scale2_ranges==0)temp_idw0=convert_unit(scale2_interval[2],scale2_interval_unit);
       else if(scale2_range_display==1)temp_idw0=convert_unit(scale2_interval[1],scale2_interval_unit);
       else if(scale2_range_display==0)temp_idw0=convert_unit(scale2_interval[0],scale2_interval_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,""); 
         
       sprintf(temp_dwstr,"e:%d%s",temp_idw0,temp_kdw0);
       if(scale2_interval_unit==0)strcatf(temp_dwstr,"g");      
       if(scale2_interval_unit==1)strcatf(temp_dwstr,"KG");   
       if(scale2_interval_unit==2)strcatf(temp_dwstr,"tn"); 
     
       LCD_setxy(2,20);               
       LCD_put_string_tahoma(temp_dwstr,7);   //tempdw2 Second Line       
       while(currentx<55)LCD_puttahoma(' ',7);                                                                            
     }
     
     
   }          

}
 

void display_blink_handle(void)
{
   unsigned char temp_dw1[50],dbh1=0,dbh2=0;
   
   if(page==0)
   {  
      LCD_setxy(184,40);
      if(no_display_change_cntr==0)
       {
        //  if(blink_en==0)sprintf(temp_dw1,":");
        //     else sprintf(temp_dw1," ");     
          sprintf(temp_dw1,"%02d",second);
          LCD_put_string_tahoma(temp_dw1,8);    
       }
      
   }   
  
   if(page==3 || page==14)
   {  
     #ifdef ENGLISH
     dbh2=135;
     #else
     dbh2=15;
     #endif           
      dbh1=get_pass[get_cursor];
      LCD_setxy(dbh2+get_cursor*16,45);
      if(blink_en==0)sprintf(temp_dw1," %c ",dbh1);
         else sprintf(temp_dw1,"   ");
      
      LCD_put_string_tahoma(temp_dw1,8);     
  
   }    
   
   
   
  if(page==5)
   { 
     #ifdef ENGLISH
     dbh2=0;     
     #else
     dbh2=100;
     #endif
     if(get_cursor==0)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c%c",get_value_i1,get_value_i2);  
         else sprintf(temp_dw1,"  ");    
      LCD_setxy(120-dbh2,45);    
     } 
     else   
     if(get_cursor==1)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c%c",get_value_i3,get_value_i4);  
         else sprintf(temp_dw1,"  "); 
      LCD_setxy(160-dbh2,45);    
     }      
     LCD_put_string_tahoma(temp_dw1,8);   
   }      
   
  if(page==6)
   {    
     #ifdef ENGLISH
     dbh2=0;     
     #else
     dbh2=100;
     #endif   
     if(get_cursor==0)
     {
      if(blink_en==0)sprintf(temp_dw1,"20%c%c",get_value_i1,get_value_i2);  
         else sprintf(temp_dw1,"20  ");    
      LCD_setxy(104-dbh2,45);    
     } 
     else   
     if(get_cursor==1)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c%c",get_value_i3,get_value_i4);  
         else sprintf(temp_dw1,"  "); 
      LCD_setxy(160-dbh2,45);    
     }      
     else   
     if(get_cursor==2)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c%c",get_value_i5,get_value_i6);  
         else sprintf(temp_dw1,"  "); 
      LCD_setxy(200-dbh2,45);    
     }       
     LCD_put_string_tahoma(temp_dw1,8);   
   }  
   
 if(page==7)
   {
     #ifdef ENGLISH
     dbh2=0;     
     #else
     dbh2=100;
     #endif             
     if(get_cursor==0)
     {
      if(blink_en==0)strcpyf(temp_dw1,baud_rate_table[get_value_i1]); 
         else strcpyf(temp_dw1,"     ");   
      LCD_setxy(104-dbh2,45);     
     } 
     else   
     if(get_cursor==1)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c",data_bits_table[get_value_i2]); 
         else strcpyf(temp_dw1," ");   
      LCD_setxy(152-dbh2,45);     
     } 
     else    
     if(get_cursor==2)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c",parity_table[get_value_i3]); 
         else strcpyf(temp_dw1," ");   
      LCD_setxy(168-dbh2,45);     
     } 
     else     
     if(get_cursor==3)
     {
      if(blink_en==0)sprintf(temp_dw1,"%c",stop_bits_table[get_value_i4]); 
         else strcpyf(temp_dw1," ");   
      LCD_setxy(184-dbh2,45);     
     } 
            
     LCD_put_string_tahoma(temp_dw1,8);       
     
   }    
   
 if(page==8)
   { 
     
    /* if(blink_en==0)strcpyf(temp_dw1,com_func_table[get_value_i1]); 
         else strcpyf(temp_dw1,"            ");   
     #ifdef ENGLISH
     LCD_setxy(120,45);
     #else
     LCD_setxy(20,45);
     #endif        
            
     LCD_put_string_tahoma(temp_dw1,8); */ 
     
     if(select_item==ORDER_OF_COM1PROT || select_item==ORDER_OF_COM2PROT)      
                strcpyf(temp_dw1,protocol_table[get_value_i1]); 
     if(select_item==ORDER_OF_COM1FUNC || select_item==ORDER_OF_COM2FUNC)
                strcpyf(temp_dw1,com_func_table[get_value_i1]); 
     #ifdef ENGLISH                                        
     LCD_setxy(120,45);  
     if(blink_en)LCD_printspace_8(12);
     else  LCD_put_string_tahoma(temp_dw1,8);      
     #else
     LCD_setxy(20,45);  
     if(blink_en)LCD_printspace_8(12);
     else LCD_printstring2(temp_dw1,0);
     #endif      
     
       
   }   
   
 if(page==9)
   {      

     if(select_item==ORDER_OF_LANGUAGE)
      {
             strcpyf(temp_dw1,language_table[get_value_i1]); 
      }      
     if(select_item==ORDER_OF_SOFTWARE)
      {
             strcpyf(temp_dw1,software_table[get_value_i1]); 
      } 
     if(select_item==ORDER_OF_CALENDAR)
      {
             strcpyf(temp_dw1,calendar_table[get_value_i1]); 
      } 
     if(select_item==ORDER_OF_ALARM)
      {
             strcpyf(temp_dw1,alarm_table[get_value_i1]); 
      }                   
     if(select_item==ORDER_OF_BL_TIME)
      {
           sprintf(temp_dw1,"%3d",get_value_i1); 
      }       
     if(select_item==ORDER_OF_DONGLE)
      {
            strcpyf(temp_dw1,dongle_table[get_value_i1]); 
      } 
     if(select_item==ORDER_OF_HOMEPOS)
      {
             strcpyf(temp_dw1,homepos_table[get_value_i1]); 
      }       
         
        
     #ifdef ENGLISH                                        
     LCD_setxy(120,45);  
     if(blink_en)LCD_printspace_8(12);
     else  LCD_put_string_tahoma(temp_dw1,8);      
     #else
     LCD_setxy(20,45);  
     if(blink_en)LCD_printspace_8(12);
     else LCD_printstring2(temp_dw1,0);
     #endif          
    
     
   } 
   
  if(page==11)
   { 

   }                                                                                                                                  
   
   
 if(page==12)
   {
         
     if(sub_select_item==ORDER_OF_SCALE_RANGE)
      {
             strcpyf(temp_dw1,scale_range_table[get_value_i1]); 
      } 
     if((sub_select_item==ORDER_OF_SCALE_UNIT)||(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)||(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT))
      {
             strcpyf(temp_dw1,scale_unit_table[get_value_i1]); 
      }   
     if(sub_select_item==ORDER_OF_SCALE_DIGITS)
      {
             strcpyf(temp_dw1,scale_digits_table[get_value_i1]); 
      }   
     if(sub_select_item==ORDER_OF_SCALE_FLOAT)
      {
            strcpyf(temp_dw1,scale_float_table[get_value_i1]); 
      }         

     if(sub_select_item==ORDER_OF_ADAPT_MOTION_W)
      {
             strcpyf(temp_dw1,motion_window_table[get_value_i1]);
      }   
           
     if(sub_select_item==ORDER_OF_ADAPT_MOTION_C)
      {
              sprintf(temp_dw1,"%02d",get_value_i1);
      }   
           
     if(sub_select_item==ORDER_OF_ADAPT_FILTER_S)
      {
            sprintf(temp_dw1,"%02d",get_value_i1);
      }                          
      
     if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R)
      {
          strcpyf(temp_dw1,auto_zero_range_table[get_value_i1]);
      }   
           
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP)
      {
            sprintf(temp_dw1,"%02d",get_value_i1);
      }                                                                         
      
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON)
      {
             sprintf(temp_dw1,"%02d",get_value_i1);
      }   
           
     if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO)
      {
           strcpyf(temp_dw1,powerup_zero_table[get_value_i1]);
      }   
           
     if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD)       
      {
           sprintf(temp_dw1,"%02d",get_value_i1);
      }        
      

     
     #ifdef ENGLISH                                        
      LCD_setxy(150,45);  
      if(blink_en)LCD_printspace_8(9);
      else  LCD_put_string_tahoma(temp_dw1,8);      
     #else
      LCD_setxy(10,45);  
      if(blink_en)LCD_printspace_8(9);
      else LCD_printstring2(temp_dw1,0);
     #endif            
     
     
   }    
   
 if(page==13)
   {
    /* if(blink_en==0)strcpyf(temp_dw1,select_scale_table[get_value_i1]); 
         else strcpyf(temp_dw1,"             ");   
     #ifdef ENGLISH
     LCD_setxy(90,45);
     #else
     LCD_setxy(10,45);
     #endif       
     LCD_put_string_tahoma(temp_dw1,8);         */
     
     strcpyf(temp_dw1,select_scale_table[get_value_i1]);   
     
     #ifdef ENGLISH                                        
     LCD_setxy(90,45);  
     if(blink_en)LCD_printspace_8(13);
     else  LCD_put_string_tahoma(temp_dw1,8);      
     #else
     LCD_setxy(10,45);  
     if(blink_en)LCD_printspace_8(13);
     else LCD_printstring2(temp_dw1,0);
     #endif         
     
   }     
   

 if(page==17 || page==21 || page==25)
   {
     #ifdef ENGLISH
     dbh2=160;     
     #else
     dbh2=20;
     #endif    
      if(get_character_index==get_value_i1)LCD_setxy((dbh2-8)+get_character_index*8,45); 
      else LCD_setxy(dbh2+get_character_index*8,45);  
      if(blink_en==1)sprintf(temp_dw1,"\"");
      else 
        {
          if(get_character_index==get_value_i1)sprintf(temp_dw1,"%c",get_value_ch[get_value_i1-1]); 
          else   
          if(get_character_index==0)sprintf(temp_dw1,"%c",get_value_ch[0]); 
          else sprintf(temp_dw1," ");
        }
      
      LCD_put_string_tahoma(temp_dw1,8); 
   } 
   
 if(page==18)
   {

     if(sub_select_item==ORDER_OF_ATTR)strcpyf(temp_dw1,attributes_table[get_value_i1]);   
     if(sub_select_item==ORDER_OF_TYPE)strcpyf(temp_dw1,field_types_table[get_value_i6]);  
     if(sub_select_item==ORDER_OF_TEXT)
        {
          if(get_value_i6==0)strcpyf(temp_dw1,fetch_types_table[get_value_i7]);  
          if(get_value_i6==3)  //select text
              {
                if(get_value_i7<NUMBER_OF_EMBED_TEXTS)strcpyf(temp_dw1,texts_table[get_value_i7]);  
                // else strcpyf(temp_dw1,user_defines_texts_table[get_value_i7-10]); 
                 else
                   {
                     dbh1=0;dbh2=0;
                     while(1)
                      {
                        dbh2=*(user_defines_texts_table[get_value_i7-NUMBER_OF_EMBED_TEXTS]+dbh1);
                        temp_dw1[dbh1]=dbh2;
                        if(dbh2==0)break;
                        if(++dbh1>14){temp_dw1[dbh1]=0;break;}
                      }
                   }      
                   
             
              } 
             
        }
           
       
        
     #ifdef ENGLISH                                        
     LCD_setxy(120,45);  
     if(blink_en)
        {
           if(sub_select_item==ORDER_OF_ATTR || sub_select_item==ORDER_OF_TYPE )LCD_printspace_8(10);  
           else LCD_printspace_8(15);
        }  
     else  LCD_put_string_tahoma(temp_dw1,8);      
     #else
     LCD_setxy(20,45);   
   
     if(blink_en)//LCD_printspace_8(16);
      {
           if(sub_select_item==ORDER_OF_ATTR || sub_select_item==ORDER_OF_TYPE )LCD_printspace_8(10);  
           else LCD_printspace_8(15);       
      }
     else
       {
            if(sub_select_item==ORDER_OF_TEXT && (get_value_i6==3))LCD_put_string_tahoma(temp_dw1,8);   
                else LCD_printstring2(temp_dw1,0);       
       }
     #endif           
          
     
   }   
   
   if(page==19 || page==23)
   {    
     #ifdef ENGLISH
     dbh2=0;     
     #else
     dbh2=100;
     #endif     
      dbh1=get_value_ch[get_cursor];
      if(get_cursor>=USER_TEXTS_LENGHT)
        {
           LCD_setxy(104-dbh2+USER_TEXTS_LENGHT*8,45);
           if(blink_en==0)sprintf(temp_dw1,"%c",get_value_ch[USER_TEXTS_LENGHT-1]);
             else sprintf(temp_dw1,"\"");           
        }
      else 
        {
         LCD_setxy(112-dbh2+get_cursor*8,45);
         if(blink_en==0)sprintf(temp_dw1,"%c",dbh1);
             else sprintf(temp_dw1,"\"");   
        }         

      LCD_put_string_tahoma(temp_dw1,8);   
      
      dbh1 = get_cursor;
      for(;dbh1<=USER_TEXTS_LENGHT;dbh1++)LCD_puttahoma(' ',8);
      
   }          
         
   

}  


void display_home_pos_message(char c)
{
   unsigned char temp_dw1[50],dhp1;
   
                          
   
                     
 #ifdef ENGLISH     
   if(c=='C') sprintf(temp_dw1,"  Calibration is Locked ");   
   if(c=='i') sprintf(temp_dw1,"  Password is incorrect!!  ");  
   if(c=='t') sprintf(temp_dw1,"  Time is INVALID!!  ");  
   if(c=='T') sprintf(temp_dw1,"  Time is Set to %02d:%02d ",hour,minute);  
   if(c=='d') sprintf(temp_dw1,"  Date is INVALID!!  ");  
   if(c=='D') sprintf(temp_dw1,"  Date is Set to 20%02d/%02d/%02d ",year,month,day);      
   if(c=='v') {sprintf(temp_dw1,"  Version: ");strcatf(temp_dw1,VERSION);}
   if(c=='O') sprintf(temp_dw1,"  OK! Setting is saved.  "); 
   if(c=='w') sprintf(temp_dw1,"  Values were deleted..  "); 
   if(c=='P') sprintf(temp_dw1,"      Now Prining...     "); 
   if(c=='N') sprintf(temp_dw1,"  Oops! Setting Failed!! ");    
   if(c=='W') sprintf(temp_dw1,"  EEPROM is Write-Protect!! ");   
   if(c=='p') sprintf(temp_dw1,"  New Password is:   %c %c %c %c %c %c ",get_pass[0],get_pass[1],get_pass[2],
                                                                get_pass[3],get_pass[4],get_pass[5],get_pass[6]);  
   if(c=='w') sprintf(temp_dw1,"  Please wait... ");   
   if(c=='f') sprintf(temp_dw1,"  Truck ID is not found!  ");                                                                 
   if(c=='b') sprintf(temp_dw1,"  Inbound Code is wrong!  ");  
   if(c=='x') sprintf(temp_dw1,"  Fixed Tare Code is wrong!  ");  
 #else
   if(c=='C') sprintf(temp_dw1," ñ®ê ˘¢™ ÒÍÏ ˆ¯˛®ê§ì˛ÛëÓ ñëı˛‡˜ó §¢ §˛˛Áó ");   
   if(c=='i') sprintf(temp_dw1," ñ®ê ˘ëìó™ê §¯ì‰ •ı§  ");       
   if(c=='t') sprintf(temp_dw1," ñ®ê ˘ëìó™ê ˘¢™ ¢§ê¯ ˆëı•  ");  
   if(c=='T') sprintf(temp_dw1,"  ñ®ê %02d:%02d ˘¢™ ¢§ê¯ ˆëı• ",hour,minute);  
   if(c=='d') sprintf(temp_dw1," ñ®ê ˘ëìó™ê ˘¢™ ¢§ê¯ ∏˛§ëó  ");  
   if(c=='D') sprintf(temp_dw1,"  ñ®ê 20%02d/%02d/%02d ˘¢™ ¢§ê¯ ∏˛§ëó  ",year,month,day);      
   //if(c=='v') {sprintf(temp_dw1,VERSION);strcatf(temp_dw1,"  :Òë˛§® ¯ ˆ¶§¯ ");}
   if(c=='v') {sprintf(temp_dw1,"  Version: ");strcatf(temp_dw1,VERSION);}                                             
   if(c=='O') sprintf(temp_dw1,"  ¢™ ˘§˛°£ ˘¢™ Ùëõ˜ê ñëı˛‡˜ó  "); 
   if(c=='w') sprintf(temp_dw1,"  ¢˜¢™ Ìëï §˛¢ëÏı  "); 
   if(c=='P') sprintf(temp_dw1,"      ...  ˆ¢§Ó îëù Òëü §¢     "); 
   if(c=='N') sprintf(temp_dw1,"  ¢™˜ Òëı‰ê ñëı˛‡˜ó  ");    
   if(c=='W') sprintf(temp_dw1,"  ˘¢™ ñ‡Íëüı ˝•ë® ˘§˛°£ §ìê§ì §¢ ˘‡Íëü ¸® ˝ç");   
   if(c=='p') sprintf(temp_dw1,"    %c %c %c %c %c %c : ¢˛¢õ §¯ì‰ •ı§",get_pass[0],get_pass[1],get_pass[2],
                                                                get_pass[3],get_pass[4],get_pass[5],get_pass[6]);  
   if(c=='w') sprintf(temp_dw1,"  ... ¢˛˜ëıì §‡ó˜ı ëÍØÛ ");   
   if(c=='f') sprintf(temp_dw1,"  ¢™˜ ê¢˛ï ˆ˛™ëı ˘§ëı™  ");                                                                 
   if(c=='b') sprintf(temp_dw1,"  ñ®ê ˘ëìó™ê ≠ìÏ ¢Ó  ");  
   if(c=='x') sprintf(temp_dw1," ñ®ê ˘ëìó™ê ñìëô Ô˜®§ëï ¢Ó  ");   
 #endif
                                                                
   dhp1=240-LCD_getstringlength2(temp_dw1);
   dhp1/=2; 
   LCD_setxy(dhp1,45); 
   LCD_printstring2(temp_dw1,1);      
                                                                                                    
}                                                                                                   


void display_home_pos(void)
{
 unsigned char temp_dw1[60],temp_dw2[10],temp_dw3[10];
 unsigned char dhp1,dhp2,dhp3;   
 unsigned long int dhpl1;
 
 if(page==0)
   {
     LCD_setxy(24,40);    
     if(calendar==1)                     //jalali calendar
        {
             jalali(day,month,year,&dhp1,&dhp2,&dhp3);
             if(dhp3<50)sprintf(temp_dw2,"14");
                else sprintf(temp_dw2,"13");
             sprintf(temp_dw1," %s%02d/%02d/%02d - %02d:%02d:%02d ",temp_dw2,dhp3,dhp2,dhp1,hour,minute,second);  
        }
     else   sprintf(temp_dw1," 20%02d/%02d/%02d - %02d:%02d:%02d ",year,month,day,hour,minute,second);  


     LCD_put_string_tahoma(temp_dw1,8);


     
            #ifdef ENGLISH     
             
             LCD_setxy(4,52);  
             sprintf(temp_dw1," F ");LCD_put_string_tahoma(temp_dw1,'n');  
             sprintf(temp_dw1,"Hotkeys"); 
             LCD_put_string_tahoma(temp_dw1,8);    
             LCD_setxy(currentx+52,52);  
             sprintf(temp_dw1,"Info");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Settings"); 
             LCD_put_string_tahoma(temp_dw1,8);
            #else  
                      
             LCD_setxy(4,52);  
             sprintf(temp_dw1," F ");LCD_put_string_tahoma(temp_dw1,'n');   
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1,"§ì˜ë˛ı ˝ë˚¢˛ÛÓ"); 
             LCD_printstring2(temp_dw1,2);   
             LCD_setxy(currentx+52,52);  
             sprintf(temp_dw1,"Info");LCD_put_string_tahoma(temp_dw1,'n');
             sprintf(temp_dw1,"ñëı˛‡˜ó ˝¯˜ı"); 
             LCD_setxy(currentx,50);  
             LCD_printstring2(temp_dw1,2);              
            #endif      
     
     
        
   }  
 
 else  
 if(page==1)
   {       
     sprintf(temp_dw1,"<   ");  
     strcatf(temp_dw1,service_menus[menu_item-1]);
     strcatf(temp_dw1,"  >");    
     dhp1=240-LCD_getstringlength2(temp_dw1);

     #ifdef ENGLISH 
       dhp1=0;    //*align text*\\
     #endif
     
     LCD_setxy(dhp1,45);
     LCD_printstring2(temp_dw1,0);   
   }    
   
 else  
 if(page==2)
   {       
     
     sprintf(temp_dw1,"<   ");      
     #ifdef ENGLISH                                 
       strcatf(temp_dw1,service_menus[menu_item-1]);     
       strcatf(temp_dw1,": ");
     #endif
     if(menu_item==ORDER_OF_CONFIGS)    strcatf(temp_dw1,config_menus[select_item-1]);
     if(menu_item==ORDER_OF_TARE)       strcatf(temp_dw1,tare_menus[select_item-1]);
     if(menu_item==ORDER_OF_INTERFACE)  strcatf(temp_dw1,interface_menus[select_item-1]);
     if(menu_item==ORDER_OF_FORMAT)     strcatf(temp_dw1,format_menus[select_item-1]);
     if(menu_item==ORDER_OF_GENERAL)    strcatf(temp_dw1,general_menus[select_item-1]);
     if(menu_item==ORDER_OF_APPLICATION)strcatf(temp_dw1,application_menus[select_item-1]);
     if(menu_item==ORDER_OF_BACKUP)     strcatf(temp_dw1,backup_menus[select_item-1]);
     if(menu_item==ORDER_OF_RESTORE)    strcatf(temp_dw1,restore_menus[select_item-1]);
     if(menu_item==ORDER_OF_SETPOINT)   strcatf(temp_dw1,setpoint_menus[select_item-1]);
     if(menu_item==ORDER_OF_CALIBRATION)strcatf(temp_dw1,calibration_menus[select_item-1]);           
     #ifndef ENGLISH        
       strcatf(temp_dw1," : ");                         
       strcatf(temp_dw1,service_menus[menu_item-1]); 
     #endif     
     strcatf(temp_dw1,"   >");         

     
     dhp1=240-LCD_getstringlength2(temp_dw1);
           
     #ifdef ENGLISH 
        dhp1=0; //*align text*\\
     #endif
     LCD_setxy(dhp1,45);
     LCD_printstring2(temp_dw1,0);   
   }    
   
 else  
 if(page==3)
   {                                                              
     if(select_item==0)
      {
       if(menu_item==ORDER_OF_INTERFACE)dhp1=1;
       if(menu_item==ORDER_OF_FORMAT)dhp1=1;
       if(menu_item==ORDER_OF_GENERAL)dhp1=2;
       if(menu_item==ORDER_OF_APPLICATION)dhp1=2; 
       if(menu_item==ORDER_OF_RESTORE)dhp1=3;          
       if(menu_item==ORDER_OF_CALIBRATION)dhp1=4;
       #ifdef ENGLISH       
        sprintf(temp_dw1,"Enter Password (%d) : ",dhp1);    
        LCD_setxy(0,45);     
       #else
        sprintf(temp_dw1,": ¢˛˜Ó ¢§ê¯ ê§ %d ¢§¯¯®ï",dhp1);         
        LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);
       #endif 
      }
     else
      {
       if(select_item==ORDER_OF_CH_PAS1)dhp1=1;
       if(select_item==ORDER_OF_CH_PAS2)dhp1=2;
       if(select_item==ORDER_OF_CH_PAS3)dhp1=3;
       if(select_item==ORDER_OF_CH_PAS4)dhp1=4;
       #ifdef ENGLISH                
        sprintf(temp_dw1,"Enter Old Password (%d) :",dhp1);
        LCD_setxy(0,45);    
       #else            
        sprintf(temp_dw1,": (%d) ¸ı˛¢Ï ¢§¯¯®ï",dhp1);    
        LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);
       #endif          
      } 


     LCD_printstring2(temp_dw1,0);   
        
     for(dhp1=0;dhp1<6;dhp1++) 
     {
  //      dhp2=get_pass[dhp1];         //unremark for show all password during enter it
        if(get_cursor==dhp1)dhp2=get_pass[get_cursor];else dhp2='*';      //remark for show all password  during enter it 
        #ifdef ENGLISH        
        LCD_setxy(135+dhp1*16,45);sprintf(temp_dw1," %c ",dhp2);LCD_put_string_tahoma(temp_dw1,8);      
        #else 
        LCD_setxy(15+dhp1*16,45);sprintf(temp_dw1," %c ",dhp2);LCD_put_string_tahoma(temp_dw1,8);    
        #endif
     }

   }    
 else  
 if(page==4)
   {
     if(menu_item==1)
      {     
             #ifdef ENGLISH
             LCD_setxy(0,40);  
             sprintf(temp_dw1,"0");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"x10   ");LCD_put_string_tahoma(temp_dw1,8); 
             sprintf(temp_dw1,"1");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"st W. ");LCD_put_string_tahoma(temp_dw1,8);       
             sprintf(temp_dw1,"2");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"nd W. ");LCD_put_string_tahoma(temp_dw1,8);      
             sprintf(temp_dw1,"3");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Basic W");LCD_put_string_tahoma(temp_dw1,8);       
   
             LCD_setxy(0,52);  
             sprintf(temp_dw1,"4");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Saved ");LCD_put_string_tahoma(temp_dw1,8); 
             sprintf(temp_dw1,"5");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Tare  ");LCD_put_string_tahoma(temp_dw1,8);     
             sprintf(temp_dw1,"6");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Zero  ");LCD_put_string_tahoma(temp_dw1,8);                     
             sprintf(temp_dw1,"8");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"Menu  ");LCD_put_string_tahoma(temp_dw1,8);    
             #else
             LCD_setxy(240-9,40);  
             sprintf(temp_dw1,"0");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(180-9,40);  
             sprintf(temp_dw1,"1");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(120-9,40);        
             sprintf(temp_dw1,"2");LCD_put_string_tahoma(temp_dw1,'n'); 
            LCD_setxy(60-9,40);       
             sprintf(temp_dw1,"3");LCD_put_string_tahoma(temp_dw1,'n'); 
 
   
             LCD_setxy(240-9,52);  
             sprintf(temp_dw1,"4");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(180-9,52);   
             sprintf(temp_dw1,"5");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(120-9,52);       
             sprintf(temp_dw1,"6");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(60-9,52);                   
             sprintf(temp_dw1,"8");LCD_put_string_tahoma(temp_dw1,'n'); 
            
              sprintf(temp_dw1,"§ìê§ì ÅÄ");   
              LCD_setxy(240-9-LCD_getstringlength2(temp_dw1),38);       
              LCD_printstring2(temp_dw1,2);      
              
              sprintf(temp_dw1,"Ò¯ê ˆ˛•¯ó");   
              LCD_setxy(180-9-LCD_getstringlength2(temp_dw1),38);       
              LCD_printstring2(temp_dw1,2);     
              
              sprintf(temp_dw1,"Ù¯¢ ˆ˛•¯ó");   
              LCD_setxy(120-9-LCD_getstringlength2(temp_dw1),38);       
              LCD_printstring2(temp_dw1,2);          
              
              sprintf(temp_dw1,"˘§ëìÓ˛ .ó");   
              LCD_setxy(60-9-LCD_getstringlength2(temp_dw1),38);       
              LCD_printstring2(temp_dw1,2);      
              


              sprintf(temp_dw1,"˘¢™ ˘§˛°£");   
              LCD_setxy(240-9-LCD_getstringlength2(temp_dw1),50);       
              LCD_printstring2(temp_dw1,2);      
              
              sprintf(temp_dw1,"Ô˜®§ëï");   
              LCD_setxy(180-9-LCD_getstringlength2(temp_dw1),50);       
              LCD_printstring2(temp_dw1,2);     
              
              sprintf(temp_dw1,"¸ó®¢ §Í¨");   
              LCD_setxy(120-9-LCD_getstringlength2(temp_dw1),50);       
              LCD_printstring2(temp_dw1,2);          
              
              sprintf(temp_dw1,"Ù˛‡˜ó ˝¯˜ı");   
              LCD_setxy(60-9-LCD_getstringlength2(temp_dw1),50);       
              LCD_printstring2(temp_dw1,2);                   
                                         
            
                          
             #endif  
            // sprintf(temp_dw1,"3");LCD_put_string_tahoma(temp_dw1,'n'); 
           //  sprintf(temp_dw1," BasicW");LCD_put_string_tahoma(temp_dw1,8);                
           //  LCD_put_string_tahoma(temp_dw1,8);  
      } 
     if(menu_item==2)
      {
           #ifdef ENGLISH
             LCD_setxy(0,40); 
             sprintf(temp_dw1,"Select Scale:");  
             LCD_put_string_tahoma(temp_dw1,8);      
             LCD_setxy(0,52);    
             sprintf(temp_dw1,"1");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Scale 1");  
             LCD_put_string_tahoma(temp_dw1,8);   
             LCD_setxy(120,52);    
             sprintf(temp_dw1,"2");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Scale 2");  
             LCD_put_string_tahoma(temp_dw1,8);       
           #else           
             sprintf(temp_dw1,": ¢˛˜Ó íë°ó˜ê ê§ §‡˜¢§¯ı Ò¯Ó®ëì "); 
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),38); 
             LCD_printstring2(temp_dw1,2);      
             LCD_setxy(4,52);    
             sprintf(temp_dw1," 1 ");LCD_put_string_tahoma(temp_dw1,'n');         
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," Ò¯Ó®ëì");  
             LCD_printstring2(temp_dw1,2);   
             LCD_setxy(currentx+50,52);     
             sprintf(temp_dw1," 2 ");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," Ò¯Ó®ëì");  
             LCD_printstring2(temp_dw1,2);
           #endif                   
      }      
   }  
   
 else  
 if(page==5)
   {
     #ifdef ENGLISH
      sprintf(temp_dw1,"ENTER TIME:"); 
      LCD_setxy(0,45);   
      LCD_printstring2(temp_dw1,0);   
      LCD_setxy(120,45);               
     #else 
      sprintf(temp_dw1,":¢˛˜Ó ¢§ê¯ ê§ ñ‰ë®"); 
      LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);   
      LCD_printstring2(temp_dw1,0);   
      LCD_setxy(20,45);               
     #endif           
      sprintf(temp_dw1,"%c%c : %c%c",get_value_i1,get_value_i2,get_value_i3,get_value_i4);  
      LCD_put_string_tahoma(temp_dw1,8);     
   }   
     
 else  
 if(page==6)
   {
     #ifdef ENGLISH
      sprintf(temp_dw1,"ENTER DATE:"); 
      LCD_setxy(0,45);   
      LCD_printstring2(temp_dw1,0);   
      LCD_setxy(104,45);               
     #else    
      sprintf(temp_dw1,":¢˛˜Ó ¢§ê¯ ê§ ∏˛§ëó"); 
      LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);   
      LCD_printstring2(temp_dw1,0);   
      LCD_setxy(4,45);               
      #endif 
      sprintf(temp_dw1,"20%c%c / %c%c / %c%c",get_value_i1,get_value_i2,get_value_i3,get_value_i4,get_value_i5,get_value_i6);  
      LCD_put_string_tahoma(temp_dw1,8);     
   }   
   
 else  
 if(page==7)
   {
     #ifdef ENGLISH
     if(select_item==ORDER_OF_COM1PROF)sprintf(temp_dw1,"COM1 Profile: ");   
     if(select_item==ORDER_OF_COM2PROF)sprintf(temp_dw1,"COM2 Profile: ");    
     LCD_setxy(0,45); 
     dhp2=0;  
     #else     
     if(select_item==ORDER_OF_COM1PROF)sprintf(temp_dw1," : COM1 ñë¨°™ı");   
     if(select_item==ORDER_OF_COM2PROF)sprintf(temp_dw1," : COM2 ñë¨°™ı ");    
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);         
     dhp2=100;
     #endif 
     
     LCD_printstring2(temp_dw1,0); 
     
     strcpyf(temp_dw1,baud_rate_table[get_value_i1]);   
     LCD_setxy(104-dhp2,45); 
     LCD_put_string_tahoma(temp_dw1,8);   
     
     sprintf(temp_dw1,"/%c/%c/%c",data_bits_table[get_value_i2],parity_table[get_value_i3],stop_bits_table[get_value_i4]);
     LCD_setxy(144-dhp2,45);  
                
     LCD_put_string_tahoma(temp_dw1,8);   
    
   }     
   
 else  
 if(page==8)
   {
     #ifdef ENGLISH
     if(select_item==ORDER_OF_COM1FUNC)sprintf(temp_dw1,"COM1 Function: ");   
     if(select_item==ORDER_OF_COM2FUNC)sprintf(temp_dw1,"COM2 Function: ");
     if(select_item==ORDER_OF_COM1PROT)sprintf(temp_dw1,"COM1 Protocol: ");   
     if(select_item==ORDER_OF_COM2PROT)sprintf(temp_dw1,"COM2 Protocol: ");           
     LCD_setxy(0,45); 
     dhp2=0;  
     #else     
     if(select_item==ORDER_OF_COM1FUNC)sprintf(temp_dw1," : COM1 ¢§ÓÛı‰");   
     if(select_item==ORDER_OF_COM2FUNC)sprintf(temp_dw1," : COM2 ¢§ÓÛı‰ ");    
     if(select_item==ORDER_OF_COM1PROT)sprintf(temp_dw1," : COM1 ÒÓó¯§ï ");   
     if(select_item==ORDER_OF_COM2PROT)sprintf(temp_dw1," : COM2 ÒÓó¯§ï ");     
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);         
     dhp2=100;
     #endif      
     

     LCD_printstring2(temp_dw1,0); 
     
     /*strcpyf(temp_dw1,com_func_table[get_value_i1]);   
     LCD_setxy(120-dhp2,45); 
     LCD_put_string_tahoma(temp_dw1,8);  */       
     
     display_blink_handle_en=1;

   }           
   
 else  
 if(page==9)                                                                                                       
   {
     #ifdef ENGLISH
     if(select_item==ORDER_OF_LANGUAGE)sprintf(temp_dw1,"Set Language: ");                                                 
     if(select_item==ORDER_OF_SOFTWARE)sprintf(temp_dw1,"Set Software: ");    
     if(select_item==ORDER_OF_BL_TIME)sprintf(temp_dw1,"Set Light Off Time: ");   
     if(select_item==ORDER_OF_DONGLE)sprintf(temp_dw1,"Set Dongle Type: ");     
     if(select_item==ORDER_OF_CALENDAR)sprintf(temp_dw1,"Set Calendar: ");
     if(select_item==ORDER_OF_ALARM)sprintf(temp_dw1,"Set Buzzer Function: ");
     if(select_item==ORDER_OF_VOLTAGE)sprintf(temp_dw1,"All values are Count Out");         
     if(select_item==ORDER_OF_HOMEPOS)sprintf(temp_dw1,"Set Interval Info:"); 
     dhp2=0;            
     LCD_setxy(0,45);     
     LCD_printstring2(temp_dw1,0);  
     #else   
     if(select_item==ORDER_OF_LANGUAGE)sprintf(temp_dw1,": ¢˛˜Ó íë°ó˜ê ê§ ˆëì• ");   
     if(select_item==ORDER_OF_SOFTWARE)sprintf(temp_dw1,": ¢˛˜Ó íë°ó˜ê ê§ §ê•Íê Ù§˜ ·¯˜ "); 
     if(select_item==ORDER_OF_BL_TIME)sprintf(temp_dw1,": ¢˛˜Ó Ù˛‡˜ó ê§ ˘˜˛ı• ßï §¯˜ ˆëı•");   
     if(select_item==ORDER_OF_DONGLE)sprintf(temp_dw1,": ¢˛˜Ó íë°ó˜ê ê§ Ò®¢¯Û ·¯˜ ");     
     if(select_item==ORDER_OF_CALENDAR)sprintf(temp_dw1,": Ù˛¯Ïó ©˛ëı˜ ·¯˜ ");
     if(select_item==ORDER_OF_ALARM)sprintf(temp_dw1,": §•ëì ¢§ÓÛı‰ ");
     if(select_item==ORDER_OF_VOLTAGE)sprintf(temp_dw1,"Ò®¢¯Û ¸õ¯§° ¶ëóÛ¯ ©˛ëı˜");     
     if(select_item==ORDER_OF_HOMEPOS)sprintf(temp_dw1,": ˘˜˛• ñë‰ëÛØê ©˛ëı˜ ");     
     dhp2=100;         
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);
     LCD_printstring2(temp_dw1,0);   
     #endif

     if(select_item!=ORDER_OF_VOLTAGE)display_blink_handle_en=1;
   }   
   
 else  
 if(page==10)
   {       
     #ifdef ENGLISH
      sprintf(temp_dw1,"<  10. Calibration: ");  
      if(select_item==(ORDER_OF_SCALE1))strcatf(temp_dw1,"S1");  
      else
      if(select_item==(ORDER_OF_SCALE2))strcatf(temp_dw1,"S2");       
      else
      strcatf(temp_dw1,calibration_menus[select_item-1]);
      strcatf(temp_dw1,": ");
      strcatf(temp_dw1,calibration_scale_menus[sub_select_item-1]);
      strcatf(temp_dw1,"   >");
      dhp1=0; 
     #else 
      sprintf(temp_dw1,"<  "); 
      strcatf(temp_dw1,calibration_scale_menus[sub_select_item-1]);        
      strcatf(temp_dw1," : ");     
      if(select_item==(ORDER_OF_SCALE1))strcatf(temp_dw1,"S1");  
      else
      if(select_item==(ORDER_OF_SCALE2))strcatf(temp_dw1,"S2");       
      else
      strcatf(temp_dw1,calibration_menus[select_item-1]);     
      strcatf(temp_dw1," : ˘§ì˛ÛëÓ .ÅÄ  >");           
      dhp1=240-LCD_getstringlength2(temp_dw1);
     #endif
     LCD_setxy(dhp1,45);
     LCD_printstring2(temp_dw1,0);   
   } 
 else  
 if(page==11)
   {  
    dhp1= ErrorHistory[get_value_i4].ErrorNum;    
    if(get_value_i1==0 && get_value_i2==0 && (dhp1<MIN_CODE_OF_ERROR || dhp1>MAX_CODE_OF_ERROR))
      {
         #ifdef ENGLISH
          sprintf(temp_dw1," No Errors Found ");  
         #else  
          sprintf(temp_dw1," ñ®ê ˘¢™˜ ñìô ¸˛ëØ° µ˛˚ "); 
         #endif      
          LCD_setxy((240-LCD_getstringlength2(temp_dw1))/2,45);
          LCD_printstring2(temp_dw1,1);  
      }
    else
     {  
        sprintf(temp_dw1,"<"); 
        LCD_setxy(0,45);
        LCD_printstring2(temp_dw1,0); 
        sprintf(temp_dw1,">"); 
        LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);
        LCD_printstring2(temp_dw1,0);     
                                                                                                       
          
        if(calendar==1)                     //jalali calendar      
            {
                 jalali(ErrorHistory[get_value_i4].ErrorDate, ErrorHistory[get_value_i4].ErrorMonth,ErrorHistory[get_value_i4].ErrorYear,&dhp1,&dhp2,&dhp3);
                 if(dhp3<50)sprintf(temp_dw2,"14");
                    else sprintf(temp_dw2,"13");
                 sprintf(temp_dw1,"%s%02d/%02d/%02d-%02d:%02d:%02d (%d)",temp_dw2,dhp3,dhp2,dhp1,
                                    ErrorHistory[get_value_i4].ErrorHour,ErrorHistory[get_value_i4].ErrorMin,ErrorHistory[get_value_i4].ErrorSec,get_value_i3);  
            }
        else   sprintf(temp_dw1,"20%02d/%02d/%02d-%02d:%02d:%02d (%d)",
                                        ErrorHistory[get_value_i4].ErrorYear,ErrorHistory[get_value_i4].ErrorMonth,ErrorHistory[get_value_i4].ErrorDate,
                                        ErrorHistory[get_value_i4].ErrorHour,ErrorHistory[get_value_i4].ErrorMin,ErrorHistory[get_value_i4].ErrorSec,get_value_i3);  

          
          dhp1=strlen(temp_dw1)*8;
          if(dhp1>240)dhp1=0;
          else dhp1 = 240 - dhp1; 
          LCD_setxy(dhp1/2,40);  
          LCD_put_string_tahoma(temp_dw1,8);  
          
          dhp1= ErrorHistory[get_value_i4].ErrorNum;     
          
          if(dhp1<MIN_CODE_OF_ERROR || dhp1>MAX_CODE_OF_ERROR)sprintf(temp_dw1," "); 
          else
             {
                  #ifdef ENGLISH     
                    sprintf(temp_dw1,"%d",get_value_i4+1);  
                    strcatf(temp_dw1," / ");    
                    if(get_value_i2==0)sprintf(temp_dw2,"%d",get_value_i1+1);  
                    else sprintf(temp_dw2,"%d",SIZE_OF_ERROR_LIST); 
                    strcat(temp_dw1,temp_dw2);
                    strcatf(temp_dw1," - ");
                                                                                           
                    
                    
                    if(dhp1==BROWN_OUT_ALARM)strcatf(temp_dw1,"A11 Turn off by Low Voltage");  
                    if(dhp1==WATCH_DOG_ALARM)strcatf(temp_dw1,"A12 Reset by Watchdog");                 
                    
                    if(dhp1==POWERUP_ZERO_OVER_S1_ALARM)strcatf(temp_dw1,"M31 PowerUp Zero Over S1");              //M 031
                    if(dhp1==POWERUP_ZERO_OVER_S2_ALARM)strcatf(temp_dw1,"M31 PowerUp Zero Over S2");              //M 031   
                    
                    if(dhp1==POWERUP_ZERO_UNDER_S1_ALARM)strcatf(temp_dw1,"M32 PowerUp Zero Under S1");             //M 032
                    if(dhp1==POWERUP_ZERO_UNDER_S2_ALARM)strcatf(temp_dw1,"M32 PowerUp Zero Under S2");             //M 032 
                    
                    if(dhp1==OVERLOAD_S1_ALARM)strcatf(temp_dw1,"M30 Scale1 in OverLoad");                         //M30
                    if(dhp1==OVERLOAD_S2_ALARM)strcatf(temp_dw1,"M30 Scale2 in OverLoad");                         //M30 
                    
                    if(dhp1==NO_ADC_CARD_FOUND_S1_ALARM)strcatf(temp_dw1,"S20 ADC S1 Not Found");             //s20
                    if(dhp1==NO_ADC_CARD_FOUND_S2_ALARM)strcatf(temp_dw1,"S20 ADC S2 Not Found");             //s20 
                    
                    if(dhp1==ADC_TIME_OVER_S1_ALARM)strcatf(temp_dw1,"A10 ADC S1 Not Respond");                   //A10
                    if(dhp1==ADC_TIME_OVER_S2_ALARM)strcatf(temp_dw1,"A10 ADC S2 Not Respond");                   //A10 
                    
                    if(dhp1==CALIBRATION_LOCKED_ALARM)strcatf(temp_dw1,"A08 Calibration was locked");                 //A08  
                    if(dhp1==LOAD_FACTORY_ALARM)strcatf(temp_dw1,"S22 Load Factory Applied");                         //s22  
                    if(dhp1==LOAD_EEPROM_PARAMETER_ALARM)strcatf(temp_dw1,"S23 Load From EEPROM");              //s23 
                    if(dhp1==SAVE_EEPROM_PARAMETER_ALARM)strcatf(temp_dw1,"S24 Save to EEPROM");              //s24

                    
                    strcatf(temp_dw1,"(%d times) ",ErrorHistory[get_value_i4].ErrorQty+1);     
                     
                  #else 
                    sprintf(temp_dw1,"(§ëì %d) ",ErrorHistory[get_value_i4].ErrorQty+1);  
                    
                    
                    if(dhp1==BROWN_OUT_ALARM)strcatf(temp_dw1,"¶ëóÛ¯ ¢¯ìıÓ §Øë°ì ©¯ıë° A11");  
                    if(dhp1==WATCH_DOG_ALARM)strcatf(temp_dw1,"Ôê¢ úê¯ ñ®˛§ A12");  

                    if(dhp1==POWERUP_ZERO_OVER_S1_ALARM)strcatf(temp_dw1,"S1 ñ§ëó®ê §Í¨ •ê §óê§Í M31");              //M 031
                    if(dhp1==POWERUP_ZERO_OVER_S2_ALARM)strcatf(temp_dw1,"S2 ñ§ëó®ê §Í¨ •ê §óê§Í M31");              //M 031   
                    
                    if(dhp1==POWERUP_ZERO_UNDER_S1_ALARM)strcatf(temp_dw1,"S1 ñ§ëó®ê §Í¨ •ê §óıÓ M32");             //M 032
                    if(dhp1==POWERUP_ZERO_UNDER_S2_ALARM)strcatf(temp_dw1,"S2 ñ§ëó®ê §Í¨ •ê §óıÓ M32");             //M 032 
                    
                    if(dhp1==OVERLOAD_S1_ALARM)strcatf(temp_dw1,"S1 §¢ §ëì ˘ÍëÆê M30");                         //M30
                    if(dhp1==OVERLOAD_S2_ALARM)strcatf(temp_dw1,"S2 §¢ §ëì ˘ÍëÆê M30");                         //M30 
                    
                    if(dhp1==NO_ADC_CARD_FOUND_S1_ALARM)strcatf(temp_dw1,"S1 ñ§ëÓ ¢¯õ¯ Ù¢‰ S20");             //s20
                    if(dhp1==NO_ADC_CARD_FOUND_S2_ALARM)strcatf(temp_dw1,"S2 ñ§ëÓ ¢¯õ¯ Ù¢‰ S20");             //s20 
                    
                    if(dhp1==ADC_TIME_OVER_S1_ALARM)strcatf(temp_dw1,"A10 ¢˚¢¸ı˜ ∏®ëï S1 ñ§ëÓ");                   //A10
                    if(dhp1==ADC_TIME_OVER_S2_ALARM)strcatf(temp_dw1,"A10 ¢˚¢¸ı˜ ∏®ëï S2 ñ§ëÓ");                   //A10 
                    
                    if(dhp1==CALIBRATION_LOCKED_ALARM)strcatf(temp_dw1,"A08 ˆ¯˛®ê§ì˛ÛëÓ ˘ì ¸®§ó®¢ Ù¢‰");                 //A08  
                    if(dhp1==LOAD_FACTORY_ALARM)strcatf(temp_dw1,"S22 ˘˜ë°§ëÓ ñëı˛‡˜ó ˘ì ñ™•ëì");                         //s22  
                    if(dhp1==LOAD_EEPROM_PARAMETER_ALARM)strcatf(temp_dw1,"S23 ˘‡Íëü ¸®˝ç ñëı˛‡˜ó ˘ì ñ™•ëì");              //s23 
                    if(dhp1==SAVE_EEPROM_PARAMETER_ALARM)strcatf(temp_dw1,"S24 ˘‡Íëü ¸®˝ç §¢ ñëı˛‡˜ó ˘§˛°£");              //s24                     
                    

                    strcatf(temp_dw1," - ");   
                    if(get_value_i2==0)sprintf(temp_dw2,"%d",get_value_i1+1);  
                    else sprintf(temp_dw2,"%d",SIZE_OF_ERROR_LIST);
                    strcat(temp_dw1,temp_dw2);
                    strcatf(temp_dw1," •ê ");
                    sprintf(temp_dw2,"%d",get_value_i4+1);
                    strcat(temp_dw1,temp_dw2);   
                  #endif
             }
          
          dhp1 = LCD_getstringlength2(temp_dw1);
          if(dhp1<240)dhp1=240-dhp1; else dhp1=0;
          LCD_setxy(dhp1/2,50); 
          LCD_printstring2(temp_dw1,2);             

                         
          
      }
    
   }       
 else  
 if(page==12)    
       
   {  
     #ifdef ENGLISH
     if(sub_select_item==ORDER_OF_SCALE_RANGE) sprintf(temp_dw1,"Set S%d Range: ",select_item-1);   
     if(sub_select_item==ORDER_OF_SCALE_UNIT)  sprintf(temp_dw1,"Set S%d Unit:  ",select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)  sprintf(temp_dw1,"Set S%d Interval Unit:  ",select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)  sprintf(temp_dw1,"Set S%d Capacity Unit:  ",select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_DIGITS)sprintf(temp_dw1,"Set S%d Digits: ",select_item-1);      
     if(sub_select_item==ORDER_OF_SCALE_FLOAT)sprintf(temp_dw1,"Set S%d Float Point: ",select_item-1); 
     if(sub_select_item==ORDER_OF_ADAPT_MOTION_W)sprintf(temp_dw1,"Set S%d Motion Window: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_MOTION_C)sprintf(temp_dw1,"Set S%d Motion Counter: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_FILTER_S)sprintf(temp_dw1,"Set S%d Filter Size: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R)sprintf(temp_dw1,"Set S%d Auto Zero Range: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP)sprintf(temp_dw1,"Set S%d PButton Zero P: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON)sprintf(temp_dw1,"Set S%d PButton Zero N: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO)sprintf(temp_dw1,"Set S%d Power Up Zero: ",select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD)sprintf(temp_dw1,"Set S%d OverLoad Div: ",select_item-1);
     LCD_setxy(0,45);     
     #else    
     if(sub_select_item==ORDER_OF_SCALE_RANGE) sprintf(temp_dw1,": %c Ò¯Ó®ëì ˝ë˚˘˜˛• ¢ê¢„ó",'Ä'+select_item-1);   
     if(sub_select_item==ORDER_OF_SCALE_UNIT)  sprintf(temp_dw1,": %c Ò¯Ó®ëì ˝§˛˘•ê¢˜ê ¢üê¯",'Ä'+select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)  sprintf(temp_dw1,": %c Ò¯Ó®ëì ˘˜˛• ©˛ëı˜ ¢üê¯",'Ä'+select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)  sprintf(temp_dw1,": %c Ò¯Ó®ëì ñ˛Í§‡ ©˛ëı˜ ¢üê¯",'Ä'+select_item-1); 
     if(sub_select_item==ORDER_OF_SCALE_DIGITS)sprintf(temp_dw1,": %c Ò¯Ó®ëì ©˛ëı˜ ÙëÏ§ê ¢ê¢„ó",'Ä'+select_item-1);      
     if(sub_select_item==ORDER_OF_SCALE_FLOAT)sprintf(temp_dw1,": %c Ò¯Ó®ëì ©˛ëı˜ ˝§ë™‰ê ÙÏ§ ¢ê¢„ó",'Ä'+select_item-1); 
     if(sub_select_item==ORDER_OF_ADAPT_MOTION_W)sprintf(temp_dw1,": %c Ò¯Ó®ëì ˆ˛•¯ó ©•§Û ˘¢¯¢üı",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_MOTION_C)sprintf(temp_dw1,": %c Ò¯Ó®ëì ˆ˛•¯ó ©•§Û ˘¢˜§ëı™",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_FILTER_S)sprintf(temp_dw1,": %c Ò¯Ó®ëì Òëó˛õ˛¢ §óÛ˛Í ˘•ê¢˜ê",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R)sprintf(temp_dw1,": %c Ò¯Ó®ëì §ëÓ¢¯° §Í¨ ˘¢¯¢üı",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP)sprintf(temp_dw1,": %c Ò¯Ó®ëì ¸ó®¢ §Í¨ ˝ëÛëì ¢ü",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON)sprintf(temp_dw1,": %c Ò¯Ó®ëì ¸ó®¢ §Í¨ ˆ˛˛ëï ¢ü",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO)sprintf(temp_dw1,": %c Ò¯Ó®ëì ñ§ëó®ê §Í¨ ˘¢¯¢üı",'Ä'+select_item-1);
     if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD)sprintf(temp_dw1,": %c Ò¯Ó®ëì §ëì ˘ÍëÆê ©˛ëı˜ §ôÓê¢ü",'Ä'+select_item-1);
     dhp1=240-LCD_getstringlength2(temp_dw1);  
     LCD_setxy(dhp1,45);   
     #endif     
                                             
     LCD_printstring2(temp_dw1,0);   
     
     display_blink_handle_en=1;   

   }  
 else  
 if(page==13)
   {
     #ifdef ENGLISH
     sprintf(temp_dw1,"Using Scales: ");        
     LCD_setxy(0,45);     
     #else    
     sprintf(temp_dw1,": Òë„Í ˝ë˚ Ò¯Ó®ëì");   
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);       
     #endif 
     LCD_printstring2(temp_dw1,0);         
     
     display_blink_handle_en=1;  
     
   }   
 else  
 if(page==14)
   {       

     if(select_item==ORDER_OF_CH_PAS1)dhp1=1;
     if(select_item==ORDER_OF_CH_PAS2)dhp1=2;
     if(select_item==ORDER_OF_CH_PAS3)dhp1=3;
     if(select_item==ORDER_OF_CH_PAS4)dhp1=4;     
     #ifdef ENGLISH  
     sprintf(temp_dw1,"Enter New Password (%d) :",dhp1); 
     LCD_setxy(0,45);       
     dhp3=135;     
     #else 
     sprintf(temp_dw1,": (%d) ¢˛¢õ ¢§¯¯®ï",dhp1);
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
     dhp3=15;
     #endif              
     
     
     LCD_printstring2(temp_dw1,0);   
        
     for(dhp1=0;dhp1<6;dhp1++) 
     {
     //   dhp2=get_pass[dhp1];         //unremark for show all password during enter it
        if(get_cursor==dhp1)dhp2=get_pass[get_cursor];else dhp2='*';      //remark for show all password  during enter it 
            
        LCD_setxy(dhp3+dhp1*16,45);sprintf(temp_dw1," %c ",dhp2);LCD_put_string_tahoma(temp_dw1,8);    
     }

   }  
 else  
 if(page==15)
   {     
      if(menu_item==ORDER_OF_RESTORE)    //ORDER_OF_RESTORE order of interface menu
       {
        if(select_item==ORDER_OF_FACTORY)
          {         
                #ifdef ENGLISH  
                sprintf(temp_dw1,"All settings lost!! Press ENT for accept ");    
                LCD_setxy(0,45);
                #else    
                sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT ñëı˛‡˜ó ˘ı˚ ˆ¢™ Ìëï ˝ê§ì");    
                LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
                #endif  
                LCD_printstring2(temp_dw1,0);
          }   
        else
        if(select_item==ORDER_OF_LE2PROM)
          {         
                #ifdef ENGLISH  
                sprintf(temp_dw1,"Press ENT to LOAD saved setting from EEPROM");    
                LCD_setxy(0,45);
                #else    
                sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT ˘‡Íëü ¸®˝ç •ê ¸ïÓ ˝ê§ì");    
                LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
                #endif  
                LCD_printstring2(temp_dw1,0);
          }   
        else
        if(select_item==ORDER_OF_SE2PROM)
          {         
                #ifdef ENGLISH  
                sprintf(temp_dw1,"Press ENT to SAVE setting into EEPROM");    
                LCD_setxy(0,45);
                #else    
                sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT ˘‡Íëü ¸®˝ç §¢ ˘§˛°£ ˝ê§ì");    
                LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
                #endif  
                LCD_printstring2(temp_dw1,0);
          }                      
        else
        if(select_item==ORDER_OF_DELERROR)
          {         
                #ifdef ENGLISH  
                sprintf(temp_dw1,"Press ENT to Clear Error List");    
                LCD_setxy(0,45);
                #else    
                sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT ë˚ëØ° ñ®˛Û ˆ¢§Ó Ìëï ˝ê§ì");    
                LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
                #endif  
                LCD_printstring2(temp_dw1,0);
          }          
        }          
   }    
 else  
 if(page==16)
   {   
     #ifdef ENGLISH  
     sprintf(temp_dw1,"Is PC Ready to Save Data (Y/N) ?");   
     LCD_setxy(0,45);
     #else    
     sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT §ó¯˛ïıëÓ ˆ¢§Ó ˘¢ëıç •ê ßï");    
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
     #endif  
     LCD_printstring2(temp_dw1,0);         
   }    
 else  
 if(page==17)
   {                                                
     //if(select_item==1){if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}   
     //else      {if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");} 
     
    #ifdef ENGLISH
     if(menu_item==ORDER_OF_TARE)           
       {
             select_unit_str(temp_dw3,select_item);
             sprintf(temp_dw1,"Set  S%d  Manual Tare (%s): ",select_item,temp_dw3);   
       }
     if(menu_item==ORDER_OF_GENERAL)
       {
             sprintf(temp_dw1,"Set LightOff: ");       
       }  
     if(menu_item==ORDER_OF_SETPOINT)
       {
             if(select_item==ORDER_OF_S1_SETPOINT1){select_unit_str(temp_dw3,1);sprintf(temp_dw1,"Set S1  SetPoint 1(%s):",temp_dw3);}  
             if(select_item==ORDER_OF_S1_SETPOINT2){select_unit_str(temp_dw3,1);sprintf(temp_dw1,"Set S1  SetPoint 2(%s):",temp_dw3);}  
             if(select_item==ORDER_OF_S2_SETPOINT1){select_unit_str(temp_dw3,2);sprintf(temp_dw1,"Set S2  SetPoint 1(%s):",temp_dw3);}  
             if(select_item==ORDER_OF_S2_SETPOINT2){select_unit_str(temp_dw3,2);sprintf(temp_dw1,"Set S2  SetPoint 2(%s):",temp_dw3);}      
       } 
     if(menu_item==ORDER_OF_FORMAT)
       {
          if(select_item==ORDER_OF_FIELDNO)
           {
             if(sub_select_item==ORDER_OF_FIELDNO)sprintf(temp_dw1,"Enter Field No: ");  
             if(sub_select_item==ORDER_OF_BLOCKNO)sprintf(temp_dw1,"Set Block (F No%2d): ",get_value_i2); 
             if(sub_select_item==ORDER_OF_LINENO)sprintf(temp_dw1,"Set Line (F No%2d): ",get_value_i2);  
             if(sub_select_item==ORDER_OF_COLUNO)sprintf(temp_dw1,"Set Column (F No%2d): ",get_value_i2);  
           }   
          if(select_item==ORDER_OF_INPUTX)
           {
             sprintf(temp_dw1,"Enter UserDef Text No: ");  
           }  
          if(select_item==ORDER_OF_FORMAT_DELETE)
           {
             sprintf(temp_dw1,"Press ENT to delete all fields...");  
           }                      
       }     
       
     if(menu_item==ORDER_OF_APPLICATION)                                    
       {

//          if(scales_func_mode==2){if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}
//        else {if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}

            if(scales_func_mode==2)select_unit_str(temp_dw3,2);   
            else select_unit_str(temp_dw3,1);   
          
          if(select_item==ORDER_OF_FIXEDTAR)
           {                                 
             if(sub_select_item==ORDER_OF_FT_EDIT)  sprintf(temp_dw1,"Enter FixedTare Code : "); 
             if(sub_select_item==ORDER_OF_FT_DELETE)sprintf(temp_dw1,"Enter FixedTare Code : ");  
             if(sub_select_item==ORDER_OF_FT_CREATN)sprintf(temp_dw1,"Create FixedTare No%d(%s): ",get_value_i3,temp_dw3); 
             if(sub_select_item==ORDER_OF_FT_EDIT_GET)sprintf(temp_dw1,"Edit FixedTare No%d(%s): ",get_value_i3,temp_dw3); 
             if(sub_select_item==ORDER_OF_FT_DELETEA)sprintf(temp_dw1,"Press ENT to delete All Fixed Tare...");  
           }                                                                                                                           
          if(select_item==ORDER_OF_1STWEIGH)         
           {                                 
             if((sub_select_item==ORDER_OF_1ST_VIEW)||(sub_select_item==ORDER_OF_1ST_PRINT)||(sub_select_item==ORDER_OF_1ST_DELETE))
                                                        sprintf(temp_dw1,"Enter 1st Weight Inbound : ");     
             if(sub_select_item==ORDER_OF_1ST_DELETEA)sprintf(temp_dw1,"Press ENT to DELETE ALL 1st weights....");
          //   if(sub_select_item==ORDER_OF_1ST_VIEW_GET)sprintf(temp_dw1,"View 1st Weight No%d(%s): ",get_value_i3,temp_dw3); 
            // if(sub_select_item==ORDER_OF_1ST_PRINT_GET)sprintf(temp_dw1,"Print 1st Weight No%d(%s): ",get_value_i3,temp_dw3); 
           }    
          if(select_item==ORDER_OF_SORTFILE)
           {                                 
             if(sub_select_item==ORDER_OF_SF_EDIT)  sprintf(temp_dw1,"Enter Sort Code to Edit: "); 
             if(sub_select_item==ORDER_OF_SF_DELETE)sprintf(temp_dw1,"Enter Sort Code to Delete: ");    
             if(sub_select_item==ORDER_OF_SF_DELETEA)sprintf(temp_dw1,"Press ENT to delete Sort File...");
           }                       
         
       }     
       
     if(menu_item==ORDER_OF_CALIBRATION)
       {
          if(select_item==ORDER_OF_CALCAL)
           {                                                                          
           
             /* if(get_value_i4==1)
               {
                 if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");           
               }   
              else   
               {
                 if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");           
               }   */          
               select_unit_str(temp_dw3,get_value_i4);
           
              if(calibration_state==0) sprintf(temp_dw1,"Enter Geo Value : "); 
              if(calibration_state==3) sprintf(temp_dw1,"Enter Zero(mV/V) : ");               
              if(calibration_state==5) sprintf(temp_dw1,"Enter Calib. Load (%s): ",temp_dw3); 
              if(calibration_state==8) sprintf(temp_dw1,"Enter Load(mV/V) : "); 
              if(calibration_state==9) sprintf(temp_dw1,"Enter No. of Loadcells : ");  //**9**\\        
              
              if(get_value_i4==1)
               {
                if(scale1_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");               
               }
              else
               {
                if(scale2_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                   
               }
              
              if(calibration_state==10)sprintf(temp_dw1,"Set S%d Capacity(%s): ",get_value_i4,temp_dw3);  //**9**\\
           }    
           
          if( (select_item==ORDER_OF_SCALE1))
           {   
                if(scale1_interval_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_interval_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");
                
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) sprintf(temp_dw1,"Set  S1  Interval 1(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) sprintf(temp_dw1,"Set  S1  Interval 2(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) sprintf(temp_dw1,"Set  S1  Interval 3(%s):",temp_dw3); 
                
                if(scale1_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                   
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) sprintf(temp_dw1,"Set  S1  Capacity 1(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) sprintf(temp_dw1,"Set  S1  Capacity 2(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) sprintf(temp_dw1,"Set  S1  Capacity 3(%s):",temp_dw3);                 
           }              
           
          if( (select_item==ORDER_OF_SCALE2))
           {           
                if(scale2_interval_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_interval_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");          
                 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) sprintf(temp_dw1,"Set  S2  Interval 1(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) sprintf(temp_dw1,"Set  S2  Interval 2(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) sprintf(temp_dw1,"Set  S2  Interval 3(%s):",temp_dw3); 
                               
                if(scale2_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                            
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) sprintf(temp_dw1,"Set  S2  Capacity 1(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) sprintf(temp_dw1,"Set  S2  Capacity 2(%s):",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) sprintf(temp_dw1,"Set  S2  Capacity 3(%s):",temp_dw3);                 
                
           }              

       }
                   
             LCD_setxy(0,45);   
             LCD_printstring2(temp_dw1,0);
             LCD_setxy(160,45);  
                    
     #else
      
     if(menu_item==ORDER_OF_TARE)           
       {
             select_unit_str(temp_dw3,select_item);
             sprintf(temp_dw1,": (%s) S%d ¸ó®¢ Ô˜®§ëï",temp_dw3,select_item);   
       }
     if(menu_item==ORDER_OF_GENERAL)
       {
             sprintf(temp_dw1,": ˘˜˛ı• ßï §¯˜ ¸™¯ıë° ˆëı•");       
       }  
     if(menu_item==ORDER_OF_SETPOINT)
       {
             if(select_item==ORDER_OF_S1_SETPOINT1){select_unit_str(temp_dw3,1);sprintf(temp_dw1,": (%s) Å Ò¯Ó®ëì Å ñ˜˛¯ï ñ®",temp_dw3);}  
             if(select_item==ORDER_OF_S1_SETPOINT2){select_unit_str(temp_dw3,1);sprintf(temp_dw1,": (%s) Ç Ò¯Ó®ëì Å ñ˜˛¯ï ñ®",temp_dw3);}  
             if(select_item==ORDER_OF_S2_SETPOINT1){select_unit_str(temp_dw3,2);sprintf(temp_dw1,": (%s) Å Ò¯Ó®ëì Ç ñ˜˛¯ï ñ®",temp_dw3);}  
             if(select_item==ORDER_OF_S2_SETPOINT2){select_unit_str(temp_dw3,2);sprintf(temp_dw1,": (%s) Ç Ò¯Ó®ëì Ç ñ˜˛¯ï ñ®",temp_dw3);}      
       } 
     if(menu_item==ORDER_OF_FORMAT)
       {
          if(select_item==ORDER_OF_FIELDNO)
           {
             if(sub_select_item==ORDER_OF_FIELDNO)sprintf(temp_dw1,": ¢˛˜Ó ¢§ê¯ ê§ ¢Û˛Í ˘§ëı™");  
             if(sub_select_item==ORDER_OF_BLOCKNO)sprintf(temp_dw1,": (%2d ¢Û˛Í) Ì¯Ûì ˘§ëı™",get_value_i2); 
             if(sub_select_item==ORDER_OF_LINENO)sprintf(temp_dw1,": (%2d ¢Û˛Í) Ø° ˘§ëı™",get_value_i2);  
             if(sub_select_item==ORDER_OF_COLUNO)sprintf(temp_dw1,": (%2d ¢Û˛Í) ˆ¯ó® ˘§ëı™",get_value_i2);  
           }   
          if(select_item==ORDER_OF_INPUTX)
           {
             sprintf(temp_dw1,": ¢˛˜Ó ¢§ê¯ ê§ §ì§ëÓ ˆóı ˘§ëı™");  
           }  
          if(select_item==ORDER_OF_FORMAT_DELETE)
           {
             sprintf(temp_dw1,"... ¢˛˚¢ §ë™Í ê§ ENT ë˚¢Û˛Í ˘ı˚ ˆ¢§Ó Ìëï ˝ê§ì");  
           }                      
       }     
       
     if(menu_item==ORDER_OF_APPLICATION)                                    
       {

//          if(scales_func_mode==2){if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}
//        else {if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}

            if(scales_func_mode==2)select_unit_str(temp_dw3,2);   
            else select_unit_str(temp_dw3,1);   
          
          if(select_item==ORDER_OF_FIXEDTAR)
           {                                 
             if(sub_select_item==ORDER_OF_FT_EDIT)  sprintf(temp_dw1,": ñìëô Ô˜®§ëï ¢Ó"); 
             if(sub_select_item==ORDER_OF_FT_DELETE)sprintf(temp_dw1,": ñìëô Ô˜®§ëï ¢Ó");  
             if(sub_select_item==ORDER_OF_FT_CREATN)sprintf(temp_dw1,":(%s) %d ¢Ó ñìëô Ô˜®§ëï ¢ëõ˛ê",temp_dw3,get_value_i3);           
             if(sub_select_item==ORDER_OF_FT_EDIT_GET)sprintf(temp_dw1,":(%s) %d ¢Ó ñìëô Ô˜®§ëï ©˛ê§˛¯",temp_dw3,get_value_i3);     
             if(sub_select_item==ORDER_OF_FT_DELETEA)sprintf(temp_dw1,"... ¢˛˚¢ §ë™Í ê§ ENT ë˚Ô˜®§ëï ˘ı˚ È£ü ˝ê§ì");  
           }  
          if(select_item==ORDER_OF_1STWEIGH)         
           {                                 
             if((sub_select_item==ORDER_OF_1ST_VIEW)||(sub_select_item==ORDER_OF_1ST_PRINT)||(sub_select_item==ORDER_OF_1ST_DELETE))
                                                        sprintf(temp_dw1,": Ò¯ê ˆ˛•¯ó ≠ìÏ ¢Ó");             
             if(sub_select_item==ORDER_OF_1ST_DELETEA)sprintf(temp_dw1,"¢˛˚¢ §ë™Í ê§ ENT Ò¯ê ˝ë˚ˆ˛•¯ó ˘ı˚ È£ü ˝ê§ì");
          //   if(sub_select_item==ORDER_OF_1ST_VIEW_GET)sprintf(temp_dw1,"View 1st Weight No%d(%s): ",get_value_i3,temp_dw3); 
            // if(sub_select_item==ORDER_OF_1ST_PRINT_GET)sprintf(temp_dw1,"Print 1st Weight No%d(%s): ",get_value_i3,temp_dw3); 
           }    
          if(select_item==ORDER_OF_SORTFILE)
           {                                 
             if(sub_select_item==ORDER_OF_SF_EDIT)  sprintf(temp_dw1,": ©˛ê§˛¯ ˝ê§ì §ëì ¢Ó"); 
             if(sub_select_item==ORDER_OF_SF_DELETE)sprintf(temp_dw1,":  ˆ¢§Ó Ìëï ˝ê§ì §ëì ¢Ó");
             if(sub_select_item==ORDER_OF_SF_DELETEA)sprintf(temp_dw1,"... ¢˛˚¢ §ë™Í ê§ ENT ë˚§ëì ¢Ó ˘ı˚ È£ü ˝ê§ì");                 
           }                       
         
       }     
       
     if(menu_item==ORDER_OF_CALIBRATION)
       {
          if(select_item==ORDER_OF_CALCAL)
           {                                                                          
           
             /* if(get_value_i4==1)
               {
                 if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");           
               }   
              else   
               {
                 if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");           
               }   */          
               select_unit_str(temp_dw3,get_value_i4);
           
              if(calibration_state==0)sprintf(temp_dw1,": ˘ì£ëõ §óıê§ëï "); 
              if(calibration_state==3)sprintf(temp_dw1,": ˘¢§ı §ëì mV/V §ê¢Ïı ");               
              if(calibration_state==5)sprintf(temp_dw1,": (%s) ˘§ì˛ÛëÓ ˘˜•¯ §ê¢Ïı ",temp_dw3); 
              if(calibration_state==8)sprintf(temp_dw1,": Ò®¢¯Û mV/V §ê¢Ïı "); 
              if(calibration_state==9)sprintf(temp_dw1,": Ò®¢¯Û ¢ê¢„ó ");     
              
              if(get_value_i4==1)
               {
                if(scale1_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");               
               }
              else
               {
                if(scale2_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                   
               }              
                   
              if(calibration_state==10)sprintf(temp_dw1,": (%s) %dÒ¯Ó®ëì ñ˛Í§‡ ",temp_dw3,get_value_i4);
           }    
           
          if( (select_item==ORDER_OF_SCALE1))
           {   
                if(scale1_interval_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_interval_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");
                
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ˆ˛˛ëï ˘˜˛• ",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ¸˜ë˛ı ˘˜˛• ",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ˝ëÛëì ˘˜˛• ",temp_dw3); 
                
                if(scale1_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale1_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                   
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ˆ˛˛ëï ñ˛Í§‡ ",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ¸˜ë˛ı ñ˛Í§‡ ",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) sprintf(temp_dw1,":(%s)  Å Ò¯Ó®ëì ˝ëÛëì ñ˛Í§‡ ",temp_dw3);                 
           }              
           
          if( (select_item==ORDER_OF_SCALE2))
           {           
                if(scale2_interval_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_interval_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");          
                 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ˆ˛˛ëï ˘˜˛•",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ¸˜ë˛ı ˘˜˛•",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ˝ëÛëì ˘˜˛•",temp_dw3); 
                               
                if(scale2_capacity_unit==0)sprintf(temp_dw3,"gr");
                else if(scale2_capacity_unit==1)sprintf(temp_dw3,"kg");
                else sprintf(temp_dw3,"tn");                            
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ˆ˛˛ëï ñ˛Í§‡",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ¸˜ë˛ı ñ˛Í§‡",temp_dw3); 
                if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) sprintf(temp_dw1,":(%s)  Ç Ò¯Ó®ëì ˝ëÛëì ñ˛Í§‡",temp_dw3);                 
                
           }              
           

           
       }
                   
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
             LCD_printstring2(temp_dw1,0);     
             LCD_setxy(20,45);            
            #endif     
            

             
             sprintf(temp_dw1,"%s",get_value_ch); 
             LCD_put_string_tahoma(temp_dw1,8);  
             for(dhp1=strlen(get_value_ch);dhp1<get_value_i1;dhp1++)LCD_puttahoma(' ',8);     
                        

   } 
   
 else  
 if(page==18)
   {
     #ifdef ENGLISH  
    
      if(sub_select_item==ORDER_OF_ATTR)sprintf(temp_dw1,"Set Attri. (F No%2d): ",get_value_i2); 
      if(sub_select_item==ORDER_OF_TYPE)sprintf(temp_dw1,"Set Type (F No%2d): ",get_value_i2); 
      if(sub_select_item==ORDER_OF_TEXT)
         {
           if(get_value_i6==0)sprintf(temp_dw1,"Select Fetch (F No%2d): ",get_value_i2);         
           if(get_value_i6==3)sprintf(temp_dw1,"Select Text (F No%2d): ",get_value_i2);
         }   
      LCD_setxy(0,45);  
      LCD_printstring2(temp_dw1,0);    
      
      LCD_setxy(120,45);   
      sprintf(temp_dw1,"              ");           
      LCD_put_string_tahoma(temp_dw1,8);     
             
      LCD_setxy(120,45);     
     #else 
      LCD_setxy(20,45);       
                
  
      if(sub_select_item==ORDER_OF_ATTR)sprintf(temp_dw1,": (%2d¢Û˛Í) ñ˜¯Í ñÛëü ",get_value_i2); 
      if(sub_select_item==ORDER_OF_TYPE)sprintf(temp_dw1,": (%2d¢Û˛Í) ·¯˜ Ù˛‡˜ó ",get_value_i2); 
      if(sub_select_item==ORDER_OF_TEXT)
          {
              if(get_value_i6==0)sprintf(temp_dw1,": (%2d¢Û˛Í) §óıê§ëï",get_value_i2);
              if(get_value_i6==3)sprintf(temp_dw1,": (%2d¢Û˛Í) ˆóı íë°ó˜ê ",get_value_i2);
          }
                     
      LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);   
      LCD_printstring2(temp_dw1,0);
          
      LCD_setxy(20,45);
      sprintf(temp_dw1,"              ");         
      LCD_put_string_tahoma(temp_dw1,8);       
      
      LCD_setxy(20,45);
                    
     #endif 
      /*
     if(sub_select_item==ORDER_OF_ATTR)strcpyf(temp_dw1,attributes_table[get_value_i1]);   
     if(sub_select_item==ORDER_OF_TYPE)strcpyf(temp_dw1,field_types_table[get_value_i6]);  
     if(sub_select_item==ORDER_OF_TEXT)
        {
          if(get_value_i6==0)strcpyf(temp_dw1,fetch_types_table[get_value_i7]);  
          if(get_value_i6==3)  //select text
              {
                if(get_value_i7<NUMBER_OF_EMBED_TEXTS)strcpyf(temp_dw1,texts_table[get_value_i7]);  
                // else strcpyf(temp_dw1,user_defines_texts_table[get_value_i7-10]); 
                 else
                   {
                     dhp1=0;dhp2=0;
                     while(1)
                      {
                        dhp2=*(user_defines_texts_table[get_value_i7-NUMBER_OF_EMBED_TEXTS]+dhp1);
                        temp_dw1[dhp1]=dhp2;
                        if(dhp2==0)break;
                        if(++dhp1>14){temp_dw1[dhp1]=0;break;}
                      }
                   }      
                   
             
              } 
             
        }
           
        #ifdef ENGLISH
        LCD_put_string_tahoma(temp_dw1,8);    
        #else     
        if(sub_select_item==ORDER_OF_TEXT)LCD_put_string_tahoma(temp_dw1,8);   
        else LCD_printstring2(temp_dw1,0);
        #endif     */
        
   }  
   
 else  
 if(page==19)
   {       
     #ifdef ENGLISH
       if(select_item==ORDER_OF_INPUTX)
           {
              sprintf(temp_dw1,"Set UserDef %2d: ",get_value_i1);   
              LCD_setxy(0,45);   
              LCD_printstring2(temp_dw1,0);  
              LCD_setxy(112,45);                      
              for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
                {
                    if(get_value_ch[dhp1]==0)sprintf(temp_dw1," ");
                    else sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
                    LCD_put_string_tahoma(temp_dw1,8); 
                }                  
           }
       if(select_item==ORDER_OF_SORTFILE)
           {                                 
             if(sub_select_item==ORDER_OF_SF_EDIT)sprintf(temp_dw1,"Edit Sort Code %d: ",get_value_i1);  
               LCD_setxy(0,45);   
               LCD_printstring2(temp_dw1,0);  
               LCD_setxy(112,45);                 
               for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
                {
                    if(get_value_ch[dhp1]==0)break;
                     sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
                     LCD_put_string_tahoma(temp_dw1,8); 
                }             
             
           }   
       #else
       if(select_item==ORDER_OF_INPUTX)
           {
              sprintf(temp_dw1,": %2d §ì§ëÓ ˆóı",get_value_i1);   
              LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
              LCD_printstring2(temp_dw1,0);  
              LCD_setxy(12,45);                      
              for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
                {
                    if(get_value_ch[dhp1]==0)sprintf(temp_dw1," ");
                    else sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
                    LCD_put_string_tahoma(temp_dw1,8); 
                }                  
           }
       if(select_item==ORDER_OF_SORTFILE)
           {                                 
             if(sub_select_item==ORDER_OF_SF_EDIT)sprintf(temp_dw1,": %d §ëì ¢Ó ©˛ê§˛¯",get_value_i1);  
               LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
               LCD_printstring2(temp_dw1,0);  
               LCD_setxy(12,45);                 
               for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
                {
                    if(get_value_ch[dhp1]==0)break;
                     sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
                     LCD_put_string_tahoma(temp_dw1,8); 
                }             
             
           }             
       #endif   

   }  
   
 else  
 if(page==20)
   {       
     #ifdef ENGLISH
      sprintf(temp_dw1,"<  6. App : ");  
      strcatf(temp_dw1,application_menus[select_item-1]);
      strcatf(temp_dw1,": ");
      if(select_item==ORDER_OF_1STWEIGH)strcatf(temp_dw1,first_file_submenus[sub_select_item-1]);
      if(select_item==ORDER_OF_FIXEDTAR)strcatf(temp_dw1,fixed_tare_submenus[sub_select_item-1]);
      if(select_item==ORDER_OF_MEMCHECK)strcatf(temp_dw1,mem_check_submenus[sub_select_item-1]); 
      if(select_item==ORDER_OF_SORTFILE)strcatf(temp_dw1,sort_file_submenus[sub_select_item-1]);
     #else      
      sprintf(temp_dw1,"<  ");
      if(select_item==ORDER_OF_1STWEIGH)strcatf(temp_dw1,first_file_submenus[sub_select_item-1]);
      if(select_item==ORDER_OF_FIXEDTAR)strcatf(temp_dw1,fixed_tare_submenus[sub_select_item-1]);
      if(select_item==ORDER_OF_MEMCHECK)strcatf(temp_dw1,mem_check_submenus[sub_select_item-1]); 
      if(select_item==ORDER_OF_SORTFILE)strcatf(temp_dw1,sort_file_submenus[sub_select_item-1]);   
      strcatf(temp_dw1," : ");                                     
      strcatf(temp_dw1,application_menus[select_item-1]);
      //strcatf(temp_dw1," : ñë‰ëÛØê .Ü");
      strcatf(temp_dw1," : ");
      strcatf(temp_dw1,text6);
     #endif
     
     strcatf(temp_dw1,"   >");
     dhp1=240-LCD_getstringlength2(temp_dw1);
 
     #ifdef ENGLISH
       dhp1=0; //***\\
     #endif
     
     LCD_setxy(dhp1,45);
     LCD_printstring2(temp_dw1,0);   
   }     
 else  
 if(page==21)
   {
     #ifdef ENGLISH
     if(menu_item==1)
       {
             sprintf(temp_dw1,"Enter Truck ID: ",select_item);   
       }
     if(menu_item==2)
       {
             sprintf(temp_dw1,"Enter Inbound Code: ");       
       } 
     if(menu_item==3)
       {
             sprintf(temp_dw1,"Enter Sort Code: ");       
       }    
     if(menu_item==4)
       {
             sprintf(temp_dw1,"Enter Fixed-Tare Code: ");       
       }     
     if(menu_item==5)
       {
//             if(using_scale==1){if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}
  //           if(using_scale==2){if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}             
             
             get_value_i3=select_unit_str(temp_dw3,using_scale);
             
             sprintf(temp_dw1,"Enter 1st Weight (%s): ",temp_dw3);       
       } 
             LCD_setxy(0,45);   
             LCD_printstring2(temp_dw1,0); 
             LCD_setxy(160,45);            
      #else
     if(menu_item==1)
       {
             sprintf(temp_dw1," : ˆ˛™ëı ˘§ëı™",select_item);   
       }
     if(menu_item==2)
       {
             sprintf(temp_dw1," : ≠ìÏ ¢Ó");       
       } 
     if(menu_item==3)
       {
             sprintf(temp_dw1," : §ëì ¢Ó");       
       }    
     if(menu_item==4)
       {
             sprintf(temp_dw1," : ñìëô Ô˜®§ëï ¢Ó");       
       }     
     if(menu_item==5)
       {
//             if(using_scale==1){if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}
  //           if(using_scale==2){if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn");}             
             
             get_value_i3=select_unit_str(temp_dw3,using_scale);
             
             sprintf(temp_dw1,":(%s) Ò¯ê ˆ˛•¯ó §ê¢Ïı",temp_dw3);       
       }       
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);   
             LCD_printstring2(temp_dw1,0); 
             LCD_setxy(20,45);         
      #endif                
     

             sprintf(temp_dw1,"%s",get_value_ch); 
             LCD_put_string_tahoma(temp_dw1,8);  
             for(dhp1=strlen(get_value_ch);dhp1<get_value_i1;dhp1++)LCD_puttahoma(' ',8);           

   }   
 if(page==22)
   {
    if((second_weighing_state==2 && first_weighing_state==0) || (view_1st_weighing==1))
      {
            // read_weighing_data_eeprom(&weighing_data,temp_inbound_code);
             
             printf_longnum(temp_dw3,temp_truck_id,5); 
             LCD_setxy(0,40);              
             if(calendar==1)                     //jalali calendar
                {
                    jalali(weighing_data.date_day,weighing_data.date_month,weighing_data.date_year,&dhp1,&dhp2,&dhp3);
                    if(dhp3<50)sprintf(temp_dw2,"14");
                    else sprintf(temp_dw2,"13");
                    //sprintf(temp_dw1,"%s%02d/%02d/%02d-%02d %02d %s",temp_dw2,dhp3,dhp2,dhp1,hour,minute,temp_dw3);    
                    sprintf(temp_dw1,"%s%02d/%02d/%02d-%02d:%02d:%02d  ID:%s",temp_dw2, dhp3 , dhp2 , dhp1 , weighing_data.time_hour, 
                    weighing_data.time_minute , weighing_data.time_second,temp_dw3);
                                           
                }
             else   //sprintf(temp_dw1,"20%02d/%02d/%02d-%02d %02d %s",year,month,day,hour,minute,temp_dw3);    
             sprintf(temp_dw1,"20%02d/%02d/%02d-%02d:%02d:%02d  ID:%s",
             weighing_data.date_year , weighing_data.date_month,
             weighing_data.date_day , weighing_data.time_hour,
             weighing_data.time_minute , weighing_data.time_second,          
             temp_dw3);                          

             LCD_put_string_tahoma(temp_dw1,8);   
             LCD_setxy(0,52); 
                                                     
             
             if(using_scale==1)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale1_unit]);
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale1_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);
                }
             if(using_scale==2)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale2_unit]);   
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale2_unit),7);   
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);
                }     
                 

             
  
             #ifdef ENGLISH     
             
             sprintf(temp_dw1,"1stW:%s%s",temp_dw2,temp_dw3);
             LCD_put_string_tahoma(temp_dw1,8);               
             
             LCD_setxy(112,52);
             printf_longnum(temp_dw2,weighing_data.conseq1_serial,6); 
             sprintf(temp_dw1,"No:%s",temp_dw2);
             LCD_put_string_tahoma(temp_dw1,8);  
             
             LCD_setxy(185,52);
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');                   
             
             sprintf(temp_dw1,"Next");
             LCD_put_string_tahoma(temp_dw1,8);
             #else   
             
             sprintf(temp_dw1," :Åˆ˛•¯ó");  
             dhp1=240- LCD_getstringlength2(temp_dw1);  
             LCD_setxy(dhp1,50);
             LCD_printstring2(temp_dw1,2);   
             sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);  
             LCD_setxy(dhp1-strlen(temp_dw1)*8,52);
             LCD_put_string_tahoma(temp_dw1,8);                  
             
             LCD_setxy(0,52);
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');                   
             LCD_setxy(currentx,50); 
             sprintf(temp_dw1,"˘ıê¢ê   ");  
             LCD_printstring2(temp_dw1,2);  
               
             LCD_setxy(currentx,52);
             printf_longnum(temp_dw2,weighing_data.conseq1_serial,6); 
             sprintf(temp_dw1,"%s",temp_dw2);
             LCD_put_string_tahoma(temp_dw1,8);
             LCD_setxy(currentx,50); 
             sprintf(temp_dw1," :Òë˛§®");  
             LCD_printstring2(temp_dw1,2);                              
             #endif              
             
      }  
    if(second_weighing_state==3 || first_weighing_state==4 || view_1st_weighing==2)        //second_weighing_state==5 
      {
 
             #ifdef ENGLISH     
             LCD_setxy(0,40);   
             if(first_weighing_state || view_1st_weighing==2) sprintf(temp_dw1,"1stW> S%d> ",using_scale);   
             if(second_weighing_state)sprintf(temp_dw1,"2ndW> S%d> ",using_scale); 
             LCD_put_string_tahoma(temp_dw1,8);
             
             printf_longnum(temp_dw2,temp_truck_id,5);   
             sprintf(temp_dw1,"ID:%s-%s",temp_dw2,temp_customer_name);
             LCD_put_string_tahoma(temp_dw1,8);               
             LCD_setxy(0,52); 
             sprintf(temp_dw1,"Sort %d:",temp_sort_code);                                                         
             LCD_put_string_tahoma(temp_dw1,8);        
             
             read_sort_name(temp_dw1,temp_sort_code-1);   
             temp_dw1[16]=0;
             if(temp_dw1[0])LCD_put_string_tahoma(temp_dw1,8); 
             
             LCD_setxy(185,52); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');         
             
             sprintf(temp_dw1,"OK");
               LCD_put_string_tahoma(temp_dw1,8);             
                      
             #else  
                       
             printf_longnum(temp_dw2,temp_truck_id,5);   
             sprintf(temp_dw1,"%s-%s",temp_dw2,temp_customer_name);     
             LCD_setxy(0,40);
             LCD_put_string_tahoma(temp_dw1,8);               
             
             if(first_weighing_state || view_1st_weighing==2) sprintf(temp_dw1," <S%c <Åˆ˛•¯ó",'Ä'+using_scale);   
             if(second_weighing_state)sprintf(temp_dw1," <S%c <Çˆ˛•¯ó",'Ä'+using_scale);    
             dhp1=240- LCD_getstringlength2(temp_dw1);
             LCD_setxy(dhp1,38);  
             LCD_printstring2(temp_dw1,2);   
             

             
             sprintf(temp_dw1," : %d §ëì ¢Ó",temp_sort_code);
             dhp1=240- LCD_getstringlength2(temp_dw1);      
             LCD_setxy(dhp1,50); 
             LCD_printstring2(temp_dw1,2);     
       
             
             read_sort_name(temp_dw1,temp_sort_code-1);   
             temp_dw1[16]=0;      
              LCD_setxy(dhp1-strlen(temp_dw1)*8,52);
             if(temp_dw1[0])LCD_put_string_tahoma(temp_dw1,8); 
             
             LCD_setxy(0,52); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');     
                                                                                                                
             LCD_setxy(currentx,50); 
             sprintf(temp_dw1,"˘ıê¢ê");
              LCD_printstring2(temp_dw1,2);                         
             
             #endif
      
      }  
      
    if(first_weighing_state==5)
      {     
          
             if(using_scale==1)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale1_unit]);
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale1_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);
                }
             if(using_scale==2)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale2_unit]);   
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale2_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);
                }
      


             
            #ifdef ENGLISH     
             LCD_setxy(0,40);   
             sprintf(temp_dw1,"1stW> S%d> ",using_scale);   
             LCD_put_string_tahoma(temp_dw1,8);       
             sprintf(temp_dw1," %s%s",temp_dw2,temp_dw3);     
             LCD_put_string_tahoma(temp_dw1,8);               
             LCD_setxy(0,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');  
             sprintf(temp_dw1," Save    "); 
             LCD_put_string_tahoma(temp_dw1,8); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Print"); 
             LCD_put_string_tahoma(temp_dw1,8);
            #else  
             sprintf(temp_dw1,"    < S%c <Åˆ˛•¯ó",'Ä'+using_scale);   
             dhp1=240- LCD_getstringlength2(temp_dw1);
             LCD_setxy(dhp1,38);  
             LCD_printstring2(temp_dw1,2);    
             sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);  
             LCD_setxy(dhp1-strlen(temp_dw1)*8,40);    
             LCD_put_string_tahoma(temp_dw1,8);                       
             LCD_setxy(4,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');   
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1,"˘§˛°£"); 
             LCD_printstring2(temp_dw1,2);   
             LCD_setxy(currentx+52,52);  
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1,"˘§˛°£ ¯ îëù"); 
             LCD_setxy(currentx,50);  
             LCD_printstring2(temp_dw1,2);  
             
            #endif 
      }    

      
    if((second_weighing_state==4) || (basic_weighing_state==5))
      {
             #ifdef ENGLISH
             LCD_setxy(0,40);   

             if(using_scale==1)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale1_unit]);                           
                // printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale1_unit),7); 
                convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);

                 sprintf(temp_dw1,"1stW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);       
                 LCD_setxy(120,40);
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_second_weigh,scale1_unit),7);  
                 convert_num_point_str(temp_dw2,temp_reg_second_weigh,1,0,0);    
                 sprintf(temp_dw1,"2ndW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);     
                 LCD_setxy(0,52);         
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_total_weigh,scale1_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_total_weigh,1,0,0);

                 sprintf(temp_dw1,"NetW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);                                   
                }
             if(using_scale==2)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale2_unit]);                           
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale2_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);
                 sprintf(temp_dw1,"1stW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);       
                 LCD_setxy(120,40);
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_second_weigh,scale2_unit),7);  
                 convert_num_point_str(temp_dw2,temp_reg_second_weigh,2,0,0);

                 sprintf(temp_dw1,"2ndW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);     
                 LCD_setxy(0,52);         
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_total_weigh,scale2_unit),7); 
                 convert_num_point_str(temp_dw2,temp_reg_total_weigh,2,0,0);

                 sprintf(temp_dw1,"NetW: %s%s",temp_dw2,temp_dw3);     
                 LCD_put_string_tahoma(temp_dw1,8);                                   
                }                       
                                                                                            
     
             LCD_setxy(175,52);         
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');          
             
              sprintf(temp_dw1,"Print"); 
              LCD_put_string_tahoma(temp_dw1,8);

             #else       
             
             LCD_setxy(0,40);   
                                                                  
             if(using_scale==1)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale1_unit]);                           
                // printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale1_unit),7); 
                convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);

                 sprintf(temp_dw1," :Åˆ˛•¯ó");     
                 dhp1=240- LCD_getstringlength2(temp_dw1);
                 LCD_setxy(dhp1,38);        
                 LCD_printstring2(temp_dw1,2);   
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);    
                 LCD_setxy(dhp1-strlen(temp_dw1)*8,40);   
                 LCD_put_string_tahoma(temp_dw1,8);     
                 
                                                                                                                                          
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_second_weigh,scale1_unit),7);  
                 convert_num_point_str(temp_dw2,temp_reg_second_weigh,1,0,0);    
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);   
                 LCD_setxy(0,40);     
                 LCD_put_string_tahoma(temp_dw1,8);     
                 LCD_setxy(currentx,38);      
                 sprintf(temp_dw1," :Çˆ˛•¯ó");  
                 LCD_printstring2(temp_dw1,2);   
                          
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_total_weigh,scale1_unit),7); 
                 
                 convert_num_point_str(temp_dw2,temp_reg_total_weigh,1,0,0); 
                 sprintf(temp_dw1," :´Ûë° ˆ•¯");     
                 dhp1=240- LCD_getstringlength2(temp_dw1);
                 LCD_setxy(dhp1,50);        
                 LCD_printstring2(temp_dw1,2);   
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);    
                 LCD_setxy(dhp1-strlen(temp_dw1)*8,52);   
                 LCD_put_string_tahoma(temp_dw1,8);  

                                                   
                }
             if(using_scale==2)
                {
                 strcpyf(temp_dw3,scale_unit_table[scale2_unit]);                           
                // printf_longnum(temp_dw2,convert_unit(temp_reg_first_weigh,scale1_unit),7); 
                convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);

                 sprintf(temp_dw1," : Åˆ˛•¯ó");     
                 dhp1=240- LCD_getstringlength2(temp_dw1);
                 LCD_setxy(dhp1,38);        
                 LCD_printstring2(temp_dw1,2);   
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);    
                 LCD_setxy(dhp1-strlen(temp_dw1)*8,40);   
                 LCD_put_string_tahoma(temp_dw1,8);     
                 
                                                                                                                                          
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_second_weigh,scale1_unit),7);  
                 convert_num_point_str(temp_dw2,temp_reg_second_weigh,2,0,0);    
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);   
                 LCD_setxy(0,40);     
                 LCD_put_string_tahoma(temp_dw1,8);     
                 LCD_setxy(currentx,38);      
                 sprintf(temp_dw1," : Çˆ˛•¯ó");  
                 LCD_printstring2(temp_dw1,2);   
                          
                 //printf_longnum(temp_dw2,convert_unit(temp_reg_total_weigh,scale1_unit),7); 
                 
                 convert_num_point_str(temp_dw2,temp_reg_total_weigh,2,0,0); 
                 sprintf(temp_dw1," :´Ûë° ˆ•¯");     
                 dhp1=240- LCD_getstringlength2(temp_dw1);
                 LCD_setxy(dhp1,50);        
                 LCD_printstring2(temp_dw1,2);   
                 sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);    
                 LCD_setxy(dhp1-strlen(temp_dw1)*8,52);   
                 LCD_put_string_tahoma(temp_dw1,8);  
                                  
                }                       
                                                                                            

             LCD_setxy(0,52);         
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');             
             
              sprintf(temp_dw1," ˆ¢§Ó îëù");    
              LCD_setxy(currentx,50);  
              LCD_printstring2(temp_dw1,2);              
             #endif 
  
      }          
      
     
    if(second_weighing_state==0 && first_weighing_state==6)
      {
           #ifdef ENGLISH
             LCD_setxy(0,40); 
             sprintf(temp_dw1,"Inbound Code:   %3d",temp_inbound_code+1);    
             LCD_put_string_tahoma(temp_dw1,8);           
             LCD_setxy(0,52); 
             
             sprintf(temp_dw1,"Consequence No: %3d",weighing_data.conseq1_serial);    
             LCD_put_string_tahoma(temp_dw1,8);             
                
             LCD_setxy(175,52);         
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');
             sprintf(temp_dw1," EXIT"); 
             LCD_put_string_tahoma(temp_dw1,8);   
           #else      
             
             sprintf(temp_dw1,"   : ≠ìÏ ¢Ó");      
             dhp1=240-LCD_getstringlength2(temp_dw1);
             LCD_setxy(dhp1,38);    
             LCD_printstring2(temp_dw1,2);     
             sprintf(temp_dw1,"%3d",temp_inbound_code+1);  
             LCD_setxy(dhp1-strlen(temp_dw1)*8,40);
             LCD_put_string_tahoma(temp_dw1,8);      

             
             sprintf(temp_dw1,"   : Òë˛§® ˘§ëı™");      
             dhp1=240-LCD_getstringlength2(temp_dw1);
             LCD_setxy(dhp1,50);    
             LCD_printstring2(temp_dw1,2);     
             sprintf(temp_dw1,"%3d",weighing_data.conseq1_serial);  
             LCD_setxy(dhp1-strlen(temp_dw1)*8,52);
             LCD_put_string_tahoma(temp_dw1,8);                         
                
             LCD_setxy(5,52);         
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');
             sprintf(temp_dw1,"ˆë˛ëï ");   
             LCD_setxy(currentx,50);   
             LCD_printstring2(temp_dw1,2);              
           #endif
      }      
      
    if(basic_weighing_state==4 )
      {
             #ifdef ENGLISH
             LCD_setxy(0,40);   
             sprintf(temp_dw1,"W-Bas> S%d> ",using_scale);   
             LCD_put_string_tahoma(temp_dw1,8);
             sprintf(temp_dw1,"%s",temp_customer_name);  
             LCD_put_string_tahoma(temp_dw1,8);         
             #else
             sprintf(temp_dw1," <S%c <˘§ëìÓ˛ .ó",'Ä'+using_scale);   
             dhp1=240-LCD_getstringlength2(temp_dw1);
             LCD_setxy(dhp1,38); 
             LCD_printstring2(temp_dw1,2); 
             sprintf(temp_dw1,"%s",temp_customer_name);   
             temp_dw1[12]=0;            //maximum 12 character could  be shown
             LCD_setxy(dhp1-strlen(temp_dw1)*8,40); 
             LCD_put_string_tahoma(temp_dw1,8);             
             #endif
             
             if(using_scale==1)
                {strcpyf(temp_dw3,scale_unit_table[scale1_unit]);
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);}

                 
             if(using_scale==2)
                {strcpyf(temp_dw3,scale_unit_table[scale2_unit]);   
                 convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0); }

             
             
             if(temp_inbound_code)
             {
               #ifdef ENGLISH
                sprintf(temp_dw1," F.Tare:%d",temp_inbound_code);     
                LCD_setxy(240-strlen(temp_dw1)*8,40);
                LCD_put_string_tahoma(temp_dw1,8);
               #else 
                sprintf(temp_dw1,"%d",temp_inbound_code);         
                LCD_setxy(5,40);             
                LCD_put_string_tahoma(temp_dw1,8);   
                
                sprintf(temp_dw1,":Ô˜®§ëï ¢Ó");  
                LCD_setxy(currentx,38);  
                LCD_printstring2(temp_dw1,2);                   
                
                
               #endif  
             }    
             else
             {        //manual fixed-tare
                if(using_scale==1)
                  {
                   strcpyf(temp_dw3,scale_unit_table[scale1_unit]);
                   convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);
                  }
                if(using_scale==2)
                  {
                    strcpyf(temp_dw3,scale_unit_table[scale2_unit]);   
                    convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);
                  }             
                #ifdef ENGLISH
                sprintf(temp_dw1," 1stW:%s%s",temp_dw2,temp_dw3);      
                LCD_setxy(240-strlen(temp_dw1)*8,40);                                                      
                LCD_put_string_tahoma(temp_dw1,8);     
                #else    
                sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);         
                LCD_setxy(0,40);             
                LCD_put_string_tahoma(temp_dw1,8);   
                
                sprintf(temp_dw1," :Åˆ˛•¯ó");  
                LCD_setxy(currentx,38);  
                LCD_printstring2(temp_dw1,2);                  
                #endif          
              }                                       
              
             
             #ifdef ENGLISH
             LCD_setxy(0,52); 

             sprintf(temp_dw1,"Sort %d:",temp_sort_code);     
             LCD_put_string_tahoma(temp_dw1,8);               

             read_sort_name(temp_dw1,temp_sort_code-1);   
             temp_dw1[16]=0;
             if(temp_dw1[0])LCD_put_string_tahoma(temp_dw1,8); 
             
             LCD_setxy(185,52);  
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');
             sprintf(temp_dw1,"Next"); 
             LCD_put_string_tahoma(temp_dw1,8);     
             #else   
             sprintf(temp_dw1," : %d §ëì ¢Ó",temp_sort_code);
             dhp1=240- LCD_getstringlength2(temp_dw1);      
             LCD_setxy(dhp1,50); 
             LCD_printstring2(temp_dw1,2);     
       
             
             read_sort_name(temp_dw1,temp_sort_code-1);   
             temp_dw1[16]=0;      
              LCD_setxy(dhp1-strlen(temp_dw1)*8,52);
             if(temp_dw1[0])LCD_put_string_tahoma(temp_dw1,8); 
             
             LCD_setxy(0,52); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');     
             
             LCD_setxy(currentx,50); 
              sprintf(temp_dw1,"˘ıê¢ê");  
              LCD_printstring2(temp_dw1,2);            
             #endif
      }  
      
  /*  if(basic_weighing_state==5)
      {

             LCD_setxy(0,40);   

             if(using_scale==1){if(scale1_unit==0)sprintf(temp_dw3,"gr"); else if(scale1_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn"); }
             if(using_scale==2){if(scale2_unit==0)sprintf(temp_dw3,"gr"); else if(scale2_unit==1)sprintf(temp_dw3,"kg");else sprintf(temp_dw3,"tn"); }             
               
                       
             printf_longnum(temp_dw2,temp_reg_first_weigh,7); 
             sprintf(temp_dw1,"1stW: %s%s ",temp_dw2,temp_dw3);     
             LCD_put_string_tahoma(temp_dw1,8);    
             
            
             printf_longnum(temp_dw2,temp_reg_second_weigh,7); 
             sprintf(temp_dw1,"2ndW: %s%s",temp_dw2,temp_dw3);   
             LCD_setxy(240-strlen(temp_dw2)*8,40);    
             LCD_put_string_tahoma(temp_dw1,8);  
                                 
             LCD_setxy(0,52); 
             printf_longnum(temp_dw2,temp_reg_total_weigh,7); 
             sprintf(temp_dw1,"Net W: %s%s",temp_dw2,temp_dw3);     
             LCD_put_string_tahoma(temp_dw1,8);   
     
             LCD_setxy(168,52);  
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Print"); 
             LCD_put_string_tahoma(temp_dw1,8); 
      }       */  
      
    if(basic_weighing_state==2)
      {
        


                if(using_scale==1)
                  {
                   strcpyf(temp_dw3,scale_unit_table[scale1_unit]);
                   convert_num_point_str(temp_dw2,temp_reg_first_weigh,1,0,0);
                  }
                if(using_scale==2)
                  {
                    strcpyf(temp_dw3,scale_unit_table[scale2_unit]);   
                   convert_num_point_str(temp_dw2,temp_reg_first_weigh,2,0,0);

                  }    

                       
             #ifdef ENGLISH     
             
             sprintf(temp_dw1,"1stW: %s%s",temp_dw2,temp_dw3);        
             LCD_setxy(0,40);  
             LCD_put_string_tahoma(temp_dw1,8);               
             
             LCD_setxy(185,52);
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');                   
             sprintf(temp_dw1,"Next");
             LCD_put_string_tahoma(temp_dw1,8);         
             
             #else   
             
             sprintf(temp_dw1," :Åˆ˛•¯ó ");  
             dhp1=240- LCD_getstringlength2(temp_dw1);  
             LCD_setxy(dhp1,38);
             LCD_printstring2(temp_dw1,2);   
             sprintf(temp_dw1,"%s%s",temp_dw2,temp_dw3);  
             LCD_setxy(dhp1-strlen(temp_dw1)*8,40
             );
             LCD_put_string_tahoma(temp_dw1,8);                  
             
             LCD_setxy(0,52);
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');                   
             LCD_setxy(currentx,50); 
             sprintf(temp_dw1,"˘ıê¢ê");  
             LCD_printstring2(temp_dw1,2);  
     
             #endif                        
             
             

      }   
      
    if(calibration_state==1)                               
      {
             #ifdef ENGLISH
             LCD_setxy(0,40);   
             sprintf(temp_dw1,"Calibrate Zero on Scale%d ? ",get_value_i4);   
             LCD_put_string_tahoma(temp_dw1,8);   
             LCD_setxy(0,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');  
             sprintf(temp_dw1," Manual Set   "); 
             LCD_put_string_tahoma(temp_dw1,8); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Calibrate"); 
             LCD_put_string_tahoma(temp_dw1,8);                  
             #else      
             sprintf(temp_dw1," %c Ò¯Ó®ëì ˝¯§ §ì ˘¢§ı §ëì ˘§ì˛ÛëÓ ",'Ä'+get_value_i4);       
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),38);   
             LCD_printstring2(temp_dw1,2);       
             LCD_setxy(4,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');    
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," ¸ó®¢ Ù˛‡˜ó");     
             LCD_printstring2(temp_dw1,2);               
             LCD_setxy(currentx+52,52);  
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," §ëÓ¢¯° ˘§ì˛ÛëÓ"); 
             LCD_printstring2(temp_dw1,2);                   
             #endif
             
                                                                                               
      } 
      
    if(calibration_state==2 || calibration_state==6)
      {     
          #ifdef ENGLISH
             LCD_setxy(0,40);   
             sprintf(temp_dw1,"Wait to Complete Calibration...");   
             LCD_put_string_tahoma(temp_dw1,8);
          #else    
             sprintf(temp_dw1," ...¢˛˜ëıì §‡ó˜ı ˝§˛ ˘•ê¢˜ê ˆë˛ëï ëó ");       
             LCD_setxy((240-LCD_getstringlength2(temp_dw1))/2,45);   
             LCD_printstring2(temp_dw1,2);                
          #endif

      } 
      
    if(calibration_state==7)        
      {     
        #ifdef
        LCD_setxy(0,40);  
        if(get_value_i4==1)
            {
              if(sum_tempf<0)sprintf(temp_dw3," NEG ");   
              else  
                {
                  printf_longnum(temp_dw3,convert_unit(scale1_default_calib_weight,select_unit_str(NULL,1)),7);
                }
              strcpyf(temp_dw2,scale_unit_table[select_unit_str(NULL,1)]);
            }
        else 
           {
              if(sum_tempf<0)sprintf(temp_dw3," NEG ");   
              else  
                {
                 printf_longnum(temp_dw3,convert_unit(scale2_default_calib_weight,select_unit_str(NULL,2)),7);
                }
              strcpyf(temp_dw2,scale_unit_table[select_unit_str(NULL,2)]);
           } 
        sprintf(temp_dw1,"S%d Load: %s%s  %smV/V",get_value_i4,temp_dw3,temp_dw2,get_value_ch);
        LCD_put_string_tahoma(temp_dw1,8);    
        LCD_setxy(0,52);    
        sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');
        sprintf(temp_dw1," Repeat");
        LCD_put_string_tahoma(temp_dw1,8);    
        LCD_setxy(113,52);    
        sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n');
        sprintf(temp_dw1,"Edit and Save");          
        LCD_put_string_tahoma(temp_dw1,8);   
        #else
        LCD_setxy(0,40);  
        if(get_value_i4==1)
            {
              if(sum_tempf<0)sprintf(temp_dw3," NEG ");   
              else  
                {
                  printf_longnum(temp_dw3,convert_unit(scale1_default_calib_weight,select_unit_str(NULL,1)),7);
                }
              strcpyf(temp_dw2,scale_unit_table[select_unit_str(NULL,1)]);
            }
        else 
           {
              if(sum_tempf<0)sprintf(temp_dw3," NEG ");   
              else  
                {
                 printf_longnum(temp_dw3,convert_unit(scale2_default_calib_weight,select_unit_str(NULL,2)),7);
                }
              strcpyf(temp_dw2,scale_unit_table[select_unit_str(NULL,2)]);
           }               
           
        
        sprintf(temp_dw1,"< %c Ò¯Ó®ëì",'Ä'+get_value_i4);
        dhp1=240-LCD_getstringlength2(temp_dw1);                           
        LCD_setxy(dhp1,38);          
        LCD_printstring2(temp_dw1,2);   
        
        sprintf(temp_dw1,"%smV/V",get_value_ch);   
        LCD_setxy(10,40);   
        LCD_put_string_tahoma(temp_dw1,8);     
        
        
        LCD_setxy(0,52);  
        sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');   
        LCD_setxy(currentx,50);  
        sprintf(temp_dw1,"¢¢õı ˘§ì˛ÛëÓ"); 
        LCD_printstring2(temp_dw1,2);   
        LCD_setxy(currentx+20,52);  
        sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
        sprintf(temp_dw1,"˘§˛°£ ¯ ©˛ê§˛¯"); 
        LCD_setxy(currentx,50);  
        LCD_printstring2(temp_dw1,2);              
            
        #endif       
      } 

    if(calibration_state==4)
      {

             #ifdef ENGLISH
             LCD_setxy(0,40);   
             sprintf(temp_dw1,"Calibrate Load on Scale%d ?",get_value_i4);   
             LCD_put_string_tahoma(temp_dw1,8);
             
             LCD_setxy(0,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');  
             sprintf(temp_dw1," Manual Set   "); 
             LCD_put_string_tahoma(temp_dw1,8); 
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Calibrate"); 
             LCD_put_string_tahoma(temp_dw1,8);                   
             #else      
             sprintf(temp_dw1," %c Ò¯Ó®ëì ˝¯§ §ì §ëì ˆ¢§Ó ˘§ì˛ÛëÓ",'Ä'+get_value_i4);        
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),38);   
             LCD_printstring2(temp_dw1,2);       
             LCD_setxy(4,52);  
             sprintf(temp_dw1,"Clr");LCD_put_string_tahoma(temp_dw1,'n');    
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," ¸ó®¢ Ù˛‡˜ó");     
             LCD_printstring2(temp_dw1,2);               
             LCD_setxy(currentx+52,52);  
             sprintf(temp_dw1,"ENT");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," §ëÓ¢¯° ˘§ì˛ÛëÓ"); 
             LCD_printstring2(temp_dw1,2);                 
                           
             #endif             
      }                  
                
      
                 
      

   } 
   
 else                                                         
 if(page==23)
   {       
       #ifdef ENGLISH
       sprintf(temp_dw1,"Customer Name : ");  
       LCD_setxy(0,45);   
       LCD_printstring2(temp_dw1,0);  
       LCD_setxy(112,45);
       for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
        {
            if(get_value_ch[dhp1]==0)sprintf(temp_dw1," ");
            else sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
            LCD_put_string_tahoma(temp_dw1,8); 
        }  
       #else  
       sprintf(temp_dw1,": ˝§ó™ı Ùë˜ ");  
       LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);    
       LCD_printstring2(temp_dw1,2);  
       LCD_setxy(12,45);
       for(dhp1=0;dhp1<USER_TEXTS_LENGHT;dhp1++)
        {
            if(get_value_ch[dhp1]==0)sprintf(temp_dw1," ");
            else sprintf(temp_dw1,"%c",get_value_ch[dhp1]);
            LCD_put_string_tahoma(temp_dw1,8); 
        }        
       #endif 
   
   } 
   
 else  
 if(page==25)
   {         
     #ifdef ENGLISH
     if(menu_item==ORDER_OF_CALIBRATION)
       {
          if(select_item==ORDER_OF_CALCAL)
           {                 
              if(calibration_state==3)sprintf(temp_dw1,"Enter Zero (mV/V) : ");               
              if(calibration_state==8)sprintf(temp_dw1,"Enter Load (mV/V) : "); 
           }
           
       }
                                 
             LCD_setxy(0,45);   
             LCD_printstring2(temp_dw1,0); 
      
             LCD_setxy(160,45);  
             sprintf(temp_dw1,"%s",get_value_ch); 
             LCD_put_string_tahoma(temp_dw1,8);  
             for(dhp1=strlen(get_value_ch);dhp1<get_value_i1;dhp1++)LCD_puttahoma(' ',8);         
       #else

     if(menu_item==ORDER_OF_CALIBRATION)
       {
          if(select_item==ORDER_OF_CALCAL)
           {                 
              if(calibration_state==3)sprintf(temp_dw1,": ˘¢§ı §ëì mV/V §ê¢Ïı ");              
              if(calibration_state==8)sprintf(temp_dw1,": Ò®¢¯Û mV/V §ê¢Ïı ");    
           }
           
       }
                         
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),45);  
             LCD_printstring2(temp_dw1,0);     
             LCD_setxy(20,45);  
             sprintf(temp_dw1,"%s",get_value_ch); 
             LCD_put_string_tahoma(temp_dw1,8);  
             for(dhp1=strlen(get_value_ch);dhp1<get_value_i1;dhp1++)LCD_puttahoma(' ',8);                      
       #endif  

   }     
      
 else  
 if(page==26)
   {       
     #ifdef ENGLISH
     sprintf(temp_dw1,"<  10. Calibr: ");      
     
     if(select_item==(ORDER_OF_SCALE1))strcatf(temp_dw1,"S1");  
     else
     if(select_item==(ORDER_OF_SCALE2))strcatf(temp_dw1,"S2");       
     else
     strcatf(temp_dw1,calibration_menus[select_item-1]);     
     
     strcatf(temp_dw1,": ");
     strcatf(temp_dw1,"Adapt: ");  
     strcatf(temp_dw1,calibration_adapt_menus[sub_select_item-100]);

     strcatf(temp_dw1,"   >");

     dhp1=0; 
     
     LCD_setxy(dhp1,45);
     LCD_printstring2(temp_dw1,0);    
     #else       
     sprintf(temp_dw1,"<  ");    
     
     strcatf(temp_dw1,calibration_adapt_menus[sub_select_item-100]);   
     
     strcatf(temp_dw1," : Î˛ìØó : "); 
     
     if(select_item==(ORDER_OF_SCALE1))strcatf(temp_dw1,"S1");  
     else
     if(select_item==(ORDER_OF_SCALE2))strcatf(temp_dw1,"S2");       
     else
     strcatf(temp_dw1,calibration_menus[select_item-1]);     


     strcatf(temp_dw1,": ˘§ì˛ÛëÓ .ÅÄ  >");

     dhp1=0;
     
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),45); 
     LCD_printstring2(temp_dw1,0);         
     #endif
   } 

 else  
 if(page==27)   
   {
           #ifdef ENGLISH
             LCD_setxy(0,40); 
             sprintf(temp_dw1,"Select Scale:");  
             LCD_put_string_tahoma(temp_dw1,8);      
             LCD_setxy(0,52);    
             sprintf(temp_dw1,"1");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Scale 1");  
             LCD_put_string_tahoma(temp_dw1,8);   
             LCD_setxy(120,52);    
             sprintf(temp_dw1,"2");LCD_put_string_tahoma(temp_dw1,'n'); 
             sprintf(temp_dw1," Scale 2");  
             LCD_put_string_tahoma(temp_dw1,8);       
           #else
             sprintf(temp_dw1,": ¢˛˜Ó íë°ó˜ê ê§ §‡˜¢§¯ı Ò¯Ó®ëì"); 
             LCD_setxy(240-LCD_getstringlength2(temp_dw1),38); 
             LCD_printstring2(temp_dw1,2);      
             LCD_setxy(4,52);    
             sprintf(temp_dw1," 1 ");LCD_put_string_tahoma(temp_dw1,'n');         
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," Ò¯Ó®ëì");  
             LCD_printstring2(temp_dw1,2);   
             LCD_setxy(currentx+50,52);     
             sprintf(temp_dw1," 2 ");LCD_put_string_tahoma(temp_dw1,'n'); 
             LCD_setxy(currentx,50);  
             sprintf(temp_dw1," Ò¯Ó®ëì");  
             LCD_printstring2(temp_dw1,2);
           #endif              
    }      


}


void display_area(void)
{
  unsigned char temp_dw1[20];  
  unsigned char temp_dw2[20];
  unsigned char temp_dwstr[20];  
  unsigned long int temp_idw1,temp_idw2,temp_idw0;    
  unsigned char temp_kdw1[3],temp_kdw2[3],temp_kdw0[3],da1;   
  
  
   
  
  if(scales_func_mode==1 || scales_func_mode==2)  
  {
  LCD_line(0,38,239,38,1);
  LCD_line(0,0,239,0,1);
  LCD_line(0,0,0,38,1);  
  LCD_line(239,0,239,38,1);     
  } else
  {
   LCD_line(0,39,239,39,1);  
  
  } 
  
  sprintf(temp_dw1,""); 
  sprintf(temp_dw2,"");
  
  if(scales_func_mode==0)
   {
     LCD_setxy(0,0); 
     #ifdef ENGLISH
     LCD_printstring("S1",1);        
     #else 
     LCD_printstring("SÅ",1);     
     #endif             
     
     if(homepos==SHOWINFO && countout_display==0)
     {
       temp_idw0=convert_unit(scale1_capacity[2],scale1_capacity_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,"");
                  
       if(scale1_capacity_unit==0)sprintf(temp_kdw2,"g");      
       if(scale1_capacity_unit==1)
       {
         if(temp_kdw0[0]=='K')
          {
           sprintf(temp_kdw2,"T");
           sprintf(temp_kdw0,"");
          }
          else  sprintf(temp_kdw2,"KG");
       }   
      if(scale1_capacity_unit==2)sprintf(temp_kdw2,"T");      
      sprintf(temp_dw1,"MAX: %d%s%s",temp_idw0,temp_kdw0,temp_kdw2); 
     }
     
     LCD_setxy(42,0); 
     LCD_put_string_tahoma(temp_dw1,7);              //tempdw1 First Line


  
     
     LCD_line(119,0,119,39,1);  
     LCD_line(120,0,120,39,1);  
    
     LCD_setxy(121,0); 
     #ifdef ENGLISH
     LCD_printstring("S2",1);        
     #else 
     LCD_printstring("SÇ",1);     
     #endif
      
     if(homepos==SHOWINFO && countout_display==0)
     {     
       temp_idw0=convert_unit(scale2_capacity[2],scale2_capacity_unit);   
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,"");
     
       if(scale2_capacity_unit==0)sprintf(temp_kdw2,"g");      
       if(scale2_capacity_unit==1)
        {
         if(temp_kdw0[0]=='K')
          {
           sprintf(temp_kdw2,"T");
           sprintf(temp_kdw0,"");
          }
          else  sprintf(temp_kdw2,"KG");
        }   
       if(scale2_capacity_unit==2)sprintf(temp_kdw2,"T");      
       sprintf(temp_dw1,"MAX: %d%s%s",temp_idw0,temp_kdw0,temp_kdw2);         
    
       LCD_setxy(120+42,0); 
       LCD_put_string_tahoma(temp_dw1,7);              //tempdw1 First Line
      }     
   }  

  if(scales_func_mode==1)
   {  
     
     #ifdef ENGLISH
     sprintf(temp_dw1,"S1");      
     #else      
     sprintf(temp_dw1,"SÅ");
     #endif       
     
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),0);     
     LCD_printstring2(temp_dw1,1);                      

     if(homepos==SHOWINFO && countout_display==0)
     {     
       temp_idw0=convert_unit(scale1_capacity[2],scale1_capacity_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,"");
    
       if(scale1_capacity_unit==0)sprintf(temp_kdw2,"g");      
       if(scale1_capacity_unit==1)
       {
         if(temp_kdw0[0]=='K')
          {
           sprintf(temp_kdw2,"T");
           sprintf(temp_kdw0,"");
          }
          else  sprintf(temp_kdw2,"KG");
       }   
      if(scale1_capacity_unit==2)sprintf(temp_kdw2,"T");      
      sprintf(temp_dw1,"C:%d%s%s",temp_idw0,temp_kdw0,temp_kdw2);         
    
      LCD_setxy(2,12); 
      LCD_put_string_tahoma(temp_dw1,7);              //tempdw1 First Line
     }  
   }  
   
  if(scales_func_mode==2)
   {
                             
     #ifdef ENGLISH
     sprintf(temp_dw1,"S2");      
     #else      
     sprintf(temp_dw1,"SÇ");
     #endif       
     
     LCD_setxy(240-LCD_getstringlength2(temp_dw1),0);     
     LCD_printstring2(temp_dw1,1);      

     if(homepos==SHOWINFO && countout_display==0)
     {     
       temp_idw0=convert_unit(scale2_capacity[2],scale2_capacity_unit);
       if(temp_idw0>999){temp_idw0 /= 1000;sprintf(temp_kdw0,"K");} else sprintf(temp_kdw0,"");

       if(scale2_capacity_unit==0)sprintf(temp_kdw2,"g");      
       if(scale2_capacity_unit==1)
       {
         if(temp_kdw0[0]=='K')
          {
           sprintf(temp_kdw2,"T");
           sprintf(temp_kdw0,"");
          }
          else  sprintf(temp_kdw2,"KG");
       }   
      if(scale2_capacity_unit==2)sprintf(temp_kdw2,"T");      
      sprintf(temp_dw1,"C:%d%s%s",temp_idw0,temp_kdw0,temp_kdw2); 
    
      LCD_setxy(2,12); 
      LCD_put_string_tahoma(temp_dw1,7);              //tempdw1 First Line
     }
   }     


   

}

  



void key_navigation(unsigned char key_code)
 {
 unsigned char kn1,kn2,kn3;  
 unsigned char kn4; 
 unsigned long int knl1;
 float knf1,knf2,knf3;

                                                              
first:          
#ifndef KEYBOARD_4x4
      if(key_code==PBZERO_KEY)       // PB zero Button
       {
         if(calibration_state);
         else
         if(scales_func_mode==0 && page==0)                               //both scales are enable
                   {        
                      page=4;
                      page_4_out_cntr=50;        //5 second to enter a NUM
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_MZ;                           //select item save     
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                       
                   }    
         else    
         if(scales_func_mode==1)                               //both scales are enable
                   { 
                      if(pb_zero_s1_state==0)pb_zero_s1_state = 1;
                      else  if(pb_zero_s1_state) pb_zero_s1_state = 0xFF;         
                     // page=0;           
                   }           
         else         
         if(scales_func_mode==2)                               //both scales are enable              
                   {
                      if(pb_zero_s2_state==0)pb_zero_s2_state = 1;
                      else  if(pb_zero_s2_state) pb_zero_s2_state = 0xFF;         
                     // page=0;           
                   }   
   
          display_weight_en=1;                              
           
       }     
      else
      if(key_code==PBTARE_KEY)       // Manual Tare Button
       {

         if(calibration_state);
         else 
         if(scales_func_mode==0 && page==0)                               //both scales are enable
                   {        
                      page=4;
                      page_4_out_cntr=50;        //5 second to enter a NUM
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_MT;                           //select item save    
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                      
                   }    
         else    
         if(scales_func_mode==1) 
                   { 
                                                                        //********\\
//                       if( (manual_tare_s1_state==0) && (scale1_manual_tare!=0)) manual_tare_s1_state= 1;
  //                     else  if( manual_tare_s1_state)  manual_tare_s1_state= 0xFF;
                       if( manual_tare_s1_state)  manual_tare_s1_state=0xFF;            
                       if( (manual_tare_s1_state==0) && (pb_tare_s1_state==0)) pb_tare_s1_state= 1;
                       else  if( pb_tare_s1_state )  pb_tare_s1_state= 0xFF;                             
  
                     // page=0;           
                   }       
         else         
         if(scales_func_mode==2)
                   {
//                      if( (manual_tare_s2_state==0) && (scale2_manual_tare!=0))manual_tare_s2_state = 1;
  //                    else  if(manual_tare_s2_state) manual_tare_s2_state = 0xFF;
                       if( manual_tare_s2_state)  manual_tare_s2_state=0xFF;
                       if( (manual_tare_s2_state==0) && (pb_tare_s2_state==0)) pb_tare_s2_state= 1;
                       else  if( pb_tare_s2_state )  pb_tare_s2_state= 0xFF;                      
                               
                     // page=0;           
                   }   
    
          display_weight_en=1;     

       }             
#endif




    if(page==0)                  //menu_item-->service menu     select_item-->internal menus of service menu
     {  
      if(key_code==CLEAR_KEY) 
        {
          if(first_weighing_state==0 && second_weighing_state==0)
          {
           sprintf(temp_customer_name," ");
           temp_reg_second_weigh=0;
           temp_reg_first_weigh=0;
           temp_truck_id=0;    
           temp_reg_total_weigh=0;
           using_scale=0;
           display_home_pos_en=1; 
           clear_home_pos_en=1;        
          }
        } 
      if(key_code==INFO_KEY)         //Info Button in page==0--> MENU
        {
          page=1;
          menu_item=1;select_item=1;
          display_home_pos_en=1; 
          clear_home_pos_en=1;
        }  
      else
      if(key_code==FUNC_KEY)
       {
          page=4;  
          clear_home_pos_en=1; 
          display_home_pos_en=1;   
          page_4_out_cntr=50;
          menu_item=1;
          select_item=0;
       }  
       
      #ifdef KEYBOARD_4x4             
      else
      if(key_code==PBZERO_KEY)       // PB zero Button
       {
         if(calibration_state);
         else  
         if(scales_func_mode==0)                               //both scales are enable
                   {        
                      page=4;
                      page_4_out_cntr=50;        //5 second to enter a NUM
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_MZ;                           //select item save     
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                       
                   }    
         else    
         if(scales_func_mode==1)                               //both scales are enable
                   { 
                      if(pb_zero_s1_state==0)pb_zero_s1_state=1;
                      else  if(pb_zero_s1_state) pb_zero_s1_state=0xFF;         
                      page=0;           
                   }           
         else         
         if(scales_func_mode==2)                               //both scales are enable              
                   {
                      if(pb_zero_s2_state==0)pb_zero_s2_state=1;
                      else  if(pb_zero_s2_state) pb_zero_s2_state=0xFF;         
                      page=0;           
                   }   
   
          display_weight_en=1;                              
           
       }     
      else
      if(key_code==PBTARE_KEY)       // '2' must be replaced by Manual Tare Button
       {

         if(calibration_state);
         else 
         if(scales_func_mode==0)                               //both scales are enable
                   {        
                      page=4;
                      page_4_out_cntr=50;        //5 second to enter a NUM
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_MT;                           //select item save    
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;    
                      
                   }    
         else    
         if(scales_func_mode==1)                               //both scales are enable
                   { 
//                      if( (manual_tare_s1_state==0) && && (scale1_manual_tare!=0)) manual_tare_s1_state=1;
  //                    else  if( manual_tare_s1_state)  manual_tare_s1_state=0xFF;
                       if( manual_tare_s1_state)  manual_tare_s1_state=0xFF;            
                       if( (manual_tare_s1_state==0) && (pb_tare_s1_state==0)) pb_tare_s1_state= 1;
                       else  if( pb_tare_s1_state )  pb_tare_s1_state= 0xFF;                             
                      page=0;           
                   }       
         else         
         if(scales_func_mode==2)                               //both scales are enable              
                   {
    //                  if((manual_tare_s2_state==0)&&&& (scale2_manual_tare!=0)) manual_tare_s2_state=1;
      //                else  if(manual_tare_s2_state) manual_tare_s2_state=0xFF;
                       if( manual_tare_s2_state)  manual_tare_s2_state=0xFF;
                       if( (manual_tare_s2_state==0) && (pb_tare_s2_state==0)) pb_tare_s2_state= 1;
                       else  if( pb_tare_s2_state )  pb_tare_s2_state= 0xFF;                      
                      page=0;           
                   }   
          display_weight_en=1;     

       }             
      #endif
     }   //end page==0
     
     
    else
    if(page==1)
     {
      if(key_code==UPARROW_KEY)
        {
          page=0;
          menu_item=0;select_item=0;
          display_home_pos_en=1; 
          clear_home_pos_en=1;       
        }
      else   
      if((key_code==RIGHT_KEY)||(key_code==INFO_KEY))      //next menu item here is '6'
        { 
          if(++menu_item>NUMBER_OF_GROUPS)menu_item=1;
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }      
      else   
      if(key_code==LEFT_KEY)      //previous menu item here is '4'
        { 
          if(--menu_item<1)menu_item=NUMBER_OF_GROUPS;
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        } 
      else   
      if(key_code==PBZERO_KEY)      //previous menu item here is '4'
        {         
          if((menu_item==ORDER_OF_CALIBRATION)&&(!WRITE_PROTECT))
             {
              page=3;   //Go to Enter PAssword Page  
              get_pass[0]='*';get_pass[1]='*';
              get_pass[2]='*';get_pass[3]='*';   
              get_pass[4]='*';get_pass[5]='*'; 
              get_cursor=0;
              blink_get_data_cntr=1;   
              blink_en=0;
              get_key_character=0xFF;
              get_character_index=0;  
              select_item=0;    
              display_home_pos_en=1; 
              clear_home_pos_en=1;                             
             }   

        }          
      else  
      if(key_code==ENTER_KEY)      //select a menu
        { 
          page=2;
          select_item=1;     
          
          if(menu_item==ORDER_OF_INTERFACE)  page=3;   //Go to Enter PAssword Page    
          if(menu_item==ORDER_OF_FORMAT)     page=3;   //Go to Enter PAssword Page
          if(menu_item==ORDER_OF_GENERAL)    page=3;   //Go to Enter PAssword Page 
          if(menu_item==ORDER_OF_APPLICATION)page=3;   //Go to Enter PAssword Page   
          if(menu_item==ORDER_OF_RESTORE)    page=3;   //Go to Enter PAssword Page
          if(menu_item==ORDER_OF_CALIBRATION)
             {
               if(!WRITE_PROTECT)
                {
                  display_home_pos_message_code='C';            //A08 : calibration is locked  
                  all_keys_temp_disable=30;  
                  no_display_change_cntr=30;          //1 second  no need to display message cause of applying online   
                  page=1;                                    
                  errormsg(CALIBRATION_LOCKED_ALARM,3); //A08
                }                
                else  page=3;   //Go to Enter PAssword Page
             }   
          
          if(page==3)
           {
              get_pass[0]='*';get_pass[1]='*';
              get_pass[2]='*';get_pass[3]='*';   
              get_pass[4]='*';get_pass[5]='*'; 
              get_cursor=0;
              blink_get_data_cntr=1;   
              blink_en=0;
              get_key_character=0xFF;
              get_character_index=0;  
              select_item=0; 
           } 
           
          display_home_pos_en=1; 
          clear_home_pos_en=1;            
        }          
                     
     
     }   //end page==1   
     
    else
    if(page==2)         //internal menus of service
     {
      if((key_code==UPARROW_KEY)||(key_code==INFO_KEY))
        {
          page=1;
          select_item=0;
          display_home_pos_en=1; 
          clear_home_pos_en=1;       
        }
      else   
      if(key_code==RIGHT_KEY)      //next menu item here is '6'
        { 
          if(++select_item>no_of_items_in_menus[menu_item-1])select_item=1;
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }      
      else   
      if(key_code==LEFT_KEY)      //previous menu item here is '4'
        { 
          if(--select_item<1)select_item=no_of_items_in_menus[menu_item-1];
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }  
      else    
      if(key_code==ENTER_KEY)      //select a menu
        {                                                                
           if(menu_item==ORDER_OF_CONFIGS)    //ORDER_OF_CONFIGS order of configuration menu
             {
                if(select_item==ORDER_OF_SETTIME)      //set time menu           
                 { 
                  page=5;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;  
                  get_value_i1='0'+hour/10; 
                  get_value_i2='0'+hour%10;                 
                  get_value_i3='0'+minute/10;  
                  get_value_i4='0'+minute%10;  
                  blink_get_data_cntr=1;   
                  get_character_index=0;  
                  get_cursor=0;
                 }  
                if(select_item==ORDER_OF_SETDATE)  
                 { 
                  page=6;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;  
                  get_value_i1='0'+year/10; 
                  get_value_i2='0'+year%10;                 
                  get_value_i3='0'+month/10;  
                  get_value_i4='0'+month%10;  
                  get_value_i5='0'+day/10;  
                  get_value_i6='0'+day%10;                    
                  blink_get_data_cntr=1;   
                  get_character_index=0;  
                  get_cursor=0;
                 }   
                if(select_item==ORDER_OF_ERRLIST)      //View error list   
                 { 
                  page=11;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;     
                  
                  get_value_i1=read_e2prom_byte(WRITE_ERROR_INDEX);    
                  get_value_i2=read_e2prom_byte(WRITE_ERROR_OV);  
                  kn1=read_e2prom_byte(NO_OF_ERRORS_L);  
                  get_value_i3=read_e2prom_byte(NO_OF_ERRORS_H);     
                  get_value_i3 *= 0x0100;
                  get_value_i3+= kn1;                //get_value_i3 is Number of ALL errors   
                  
                  if(get_value_i1==0 && get_value_i2)get_value_i1=SIZE_OF_ERROR_LIST-1;
                  else  if(get_value_i1)get_value_i1--;
                  else get_value_i1=0;
                  get_value_i4=get_value_i1;     //get_value_i4 is index for browsing errors
                  

                 }                  
                if(select_item==ORDER_OF_VERSION)  
                 { 
                  display_home_pos_en=1;  
                  clear_home_pos_en=1; 
                  all_keys_temp_disable=50;  
                  display_home_pos_message_code='v';  //version          
                  no_display_change_cntr=50;          //3 second
                 }   
                if(select_item==ORDER_OF_TEST)  
                 { 
                  test_lcd_cntr=1; 
                  set_all_graph_en=1;
                 }   
                            
             }
             
           if(menu_item==ORDER_OF_TARE)    //ORDER_OF_TARE order of Tare menus
             {
                if(select_item==ORDER_OF_MANUAL_TARE1)         
                 { 
                  page=17; 
                  previous_page=2;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;  
                  //sprintf(get_value_ch,"%d",scale1_manual_tare); 
                  printf_longnum(get_value_ch,scale1_manual_tare,0);
                  get_value_i1=6;               //number of digits 

                  get_value_i3=select_unit_str(NULL,1);  //unit
                  
                  blink_get_data_cntr=1;   
                  get_character_index=strlen(get_value_ch); 
                  get_cursor=0;
                 }   
                if(select_item==ORDER_OF_MANUAL_TARE2)         
                 { 
                  page=17; 
                  previous_page=2;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;  
                  //sprintf(get_value_ch,"%d",scale2_manual_tare); 
                  printf_longnum(get_value_ch,scale2_manual_tare,0);
                  get_value_i1=6;               //number of digits
                  
                  get_value_i3=select_unit_str(NULL,1);  //unit
                  
                  blink_get_data_cntr=1;   
                  get_character_index=strlen(get_value_ch);  
                  get_cursor=0;
                 }                   
                                      
             }                 
             
             
           if(menu_item==ORDER_OF_INTERFACE)    //ORDER_OF_INTERFACE order of interface menu
             {
                 if(select_item==ORDER_OF_COM1PROF)
                  {
                   page=7;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com1_baud_rate; 
                   get_value_i2=com1_databits;                 
                   get_value_i3=com1_parity;  
                   get_value_i4=com1_stop_bits;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;                  
                  } 
                 if(select_item==ORDER_OF_COM2PROF)
                  {
                   page=7;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com2_baud_rate; 
                   get_value_i2=com2_databits;                 
                   get_value_i3=com2_parity;  
                   get_value_i4=com2_stop_bits;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;                  
                  }  
                 if(select_item==ORDER_OF_COM1FUNC)
                  {
                   page=8;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com1_func; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }      
                 if(select_item==ORDER_OF_COM2FUNC)
                  {
                   page=8;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com2_func; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  } 
                 if(select_item==ORDER_OF_COM1PROT)
                  {
                   page=8;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com1_protocol; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }      
                 if(select_item==ORDER_OF_COM2PROT)
                  {
                   page=8;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=com2_protocol; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }                                                                 

             }   
             
           if(menu_item==ORDER_OF_FORMAT)   
             {  
                 if(select_item==ORDER_OF_FIELDNO)
                  {                  
                   page=17;
                   previous_page=2;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   sprintf(get_value_ch,"%d",0); 
                   get_value_i1=3;               //number of digits  
                   get_value_i2=0;   //field no in i2
                   blink_get_data_cntr=1;   
                   get_character_index=0;  
                   get_cursor=0;  
                   sub_select_item=1; 
                  } 
                 if(select_item==ORDER_OF_INPUTX)
                  { 
                   page=17;
                   previous_page=2;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   sprintf(get_value_ch,"%d",0); 
                   get_value_i1=2;               //number of digits  
                   get_value_i2=0;   //field no in i2
                   blink_get_data_cntr=1;   
                   get_character_index=0;  
                   get_cursor=0;                    
                  }   
                 if(select_item==ORDER_OF_FORMAT_DELETE)      
                  { 
                   page=17;
                   previous_page=2;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   sprintf(get_value_ch,"%c",0); 
                   get_value_i1=0;               //number of digits  
                   get_value_i2=0;   //field no in i2
                   blink_get_data_cntr=0;   
                   get_character_index=0;  
                   get_cursor=0;                    
                  }                                                   
            
             }               
                 
             
           if(menu_item==ORDER_OF_GENERAL)    //ORDER_OF_GENERAL order of general menu
             {
                 if(select_item==ORDER_OF_LANGUAGE)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=language; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }  
                 if(select_item==ORDER_OF_SOFTWARE)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=software; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;
                  }   
                 if(select_item==ORDER_OF_BL_TIME)
                  {
                   previous_page=2;
                   page=17;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   sprintf(get_value_ch,"%d",backlight_time); 
                   get_value_i1=2;               //number of digits
                  
                   blink_get_data_cntr=1;   
                   get_character_index=strlen(get_value_ch);  
                   get_cursor=0;                   
                  }                   
                 if(select_item==ORDER_OF_DONGLE)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=dongletype; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;
                  } 
                 if(select_item==ORDER_OF_CALENDAR)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=calendar; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }  
                 if(select_item==ORDER_OF_VOLTAGE)
                  {
                   page=9;
                   countout_display=1;
                   clear_all_graph_en=1;  
                   clear_home_pos_en=1; 
                   display_area_en=1; 
                   display_weight_en=1;  
                   display_home_pos_en=1;                   
                  }                   
                 if(select_item==ORDER_OF_HOMEPOS)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=homepos; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }   
                 if(select_item==ORDER_OF_ALARM)
                  {
                   page=9;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_value_i1=alarm; 
                   blink_get_data_cntr=1;   
                   get_cursor=0;                   
                  }                                                                                                        
             }                   
             
           if(menu_item==ORDER_OF_APPLICATION)    //ORDER_OF_APPLICATION order of application menu
             {
                 if(select_item==ORDER_OF_1STWEIGH)
                  {
                   page=20;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;     
                   sub_select_item=1;
                  }  
                 if(select_item==ORDER_OF_FIXEDTAR)
                  {
                   page=20;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;     
                   sub_select_item=1;
                  }   
                 if(select_item==ORDER_OF_SORTFILE)
                  {
                   page=20;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;     
                   sub_select_item=1;                  
                  }                   
                 if(select_item==ORDER_OF_MEMCHECK)
                  {
                   page=20;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;     
                   sub_select_item=1;
                  }                                                  
             }              
             
             
           if(menu_item==ORDER_OF_BACKUP)    //ORDER_OF_BACKUP order of Backup menu
             {
                 if(select_item==ORDER_OF_READYPC)
                  {
                    page=16;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                  
                  } 
    
             }              
             
           if(menu_item==ORDER_OF_RESTORE)    //ORDER_OF_RESTORE order of interface menu
             {
                 if(select_item==ORDER_OF_FACTORY)
                  {
                    page=15;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                  
                  }      
                 if(select_item==ORDER_OF_LE2PROM)
                  {
                    page=15;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                  
                  }    
                 if(select_item==ORDER_OF_SE2PROM)
                  {
                    page=15;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                  
                  }              
                 if(select_item==ORDER_OF_DELERROR)
                  {
                    page=15;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                  
                  }                                            
                 if(select_item==ORDER_OF_CH_PAS1 || select_item==ORDER_OF_CH_PAS2 ||
                    select_item==ORDER_OF_CH_PAS3 || select_item==ORDER_OF_CH_PAS4)
                  {
                   page=3;   
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   get_pass[0]='*';get_pass[1]='*';
                   get_pass[2]='*';get_pass[3]='*';   
                   get_pass[4]='*';get_pass[5]='*'; 
                   get_cursor=0;
                   blink_get_data_cntr=1;   
                   blink_en=0;
                   get_key_character=0xFF;
                   get_character_index=0;              
                  }  
                       
             }  
             
           if(menu_item==ORDER_OF_SETPOINT)    //ORDER_OF_TARE order of Tare menus
             {
                  page=17; 
                  previous_page=2;
                  display_home_pos_en=1; 
                  clear_home_pos_en=1;  
                  get_value_i1=7;               //number of digits   
                  
                if(select_item==ORDER_OF_S1_SETPOINT1)         
                 { 
                  //sprintf(get_value_ch,"%d",scale1_set_point1); 
                   get_value_l1 = convert_unit(scale1_set_point1,select_unit_str(NULL,1));  
                 }   
                if(select_item==ORDER_OF_S1_SETPOINT2)         
                 { 
                  //sprintf(get_value_ch,"%d",scale1_set_point2); 
                  get_value_l1 = convert_unit(scale1_set_point2,select_unit_str(NULL,1));
                 }     
                if(select_item==ORDER_OF_S2_SETPOINT1)         
                 { 
                  //sprintf(get_value_ch,"%d",scale2_set_point1);  
                  get_value_l1 = convert_unit(scale2_set_point1,select_unit_str(NULL,2));
                 }   
                if(select_item==ORDER_OF_S2_SETPOINT2)         
                 { 
                  //sprintf(get_value_ch,"%d",scale2_set_point2);
                  get_value_l1 = convert_unit(scale2_set_point2,select_unit_str(NULL,2)); 
                 }      
                printf_longnum(get_value_ch,get_value_l1,7);
                blink_get_data_cntr=1;   
                get_character_index=strlen(get_value_ch); 
                get_cursor=0;                                                   
    
             }                  
                                                 
             
           if(menu_item==ORDER_OF_CALIBRATION)    //ORDER_OF_INTERFACE order of interface menu
             {
                 if(select_item==ORDER_OF_SELECT)
                  {
                    page=13;  
                    get_value_i1=scales_func_mode;
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;   
                    blink_get_data_cntr=1;   
                    get_cursor=0;                  
                  }                   
                 if(select_item==ORDER_OF_SCALE1)
                  {
                    page=10;  
                    sub_select_item=1;
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;   
                    blink_get_data_cntr=1;   
                    get_cursor=0;                  
                  }  
                 if(select_item==ORDER_OF_SCALE2)
                  {
                    page=10;  
                    sub_select_item=1;
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;   
                    blink_get_data_cntr=1;   
                    get_cursor=0;                  
                  }  
                 if(select_item==ORDER_OF_CALCAL)
                  {
                     if(scales_func_mode==1 || scales_func_mode==2)
                     {
                        get_value_i4=scales_func_mode;          //No Need to select scale for calibration  
                        page=17; 
                        previous_page=2;       
                     } 
                     else
                     {
                       page=27;  
                       previous_page=2;  
                       get_value_i4=0;
                     } 
                     
                    for(kn1=0;kn1<3;kn1++)get_value_ch[kn1]=0;
                    sprintf(get_value_ch,"%d",geo_value);  
                    get_value_i1=2;               //number of digits
                  
                    blink_get_data_cntr=1;   
                    get_character_index=strlen(get_value_ch); 
                    get_cursor=0;                       

                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                      
                     
                  }                                               
             
             
             }             
        
        

        }          
                     
     
     }   //end page==2     
     
     
    else
    if(page==3)         //Enter Password Page
     {
      if((key_code==UPARROW_KEY)||(key_code==INFO_KEY))
        {
          if(select_item)page=2;     //when in change pasword menu select item has value
          else page=1;

          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        { 
        /*   if(key_code==get_key_character || (get_key_character==0xFF))   //press same key  or if overwrite on previous
             {
               if(get_key_character==0xFF)
                    {get_key_character=key_code;
                     get_character_index=0;}
               else  get_character_index++;   
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);
               if(kn1==0)get_character_index=0;
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);  
               get_pass[get_cursor]=kn1;
               display_home_pos_en=1;  
               
             }    */
           if(get_key_character==0xFF)   //press same key  or if overwrite on previous
             {
               get_key_character=key_code;
               kn1=*(key_character_table_n_address[key_code-'0']);  
               get_pass[get_cursor]=kn1;
               display_home_pos_en=1;  
               
             }                  
           else
             {       
               if(get_cursor<5)get_cursor++;
               kn1=*(key_character_table_n_address[key_code-'0']);  
               get_pass[get_cursor]=kn1;               
               get_key_character=key_code;   
               
               get_character_index=0; 
               display_home_pos_en=1;   
               blink_get_data_cntr=1;  
               
             }  
        }    
      if(key_code==CLEAR_KEY)      //clear a character
        { 
           get_pass[get_cursor]='*'; 
           if(get_cursor>0)
            {              
              get_cursor--; 
              get_key_character=0xFF; // start from NUMBER and overwrite on previous
            }
           display_home_pos_en=1; 
        }          
      if(key_code==ENTER_KEY)      //select a menu
        { 
          get_key_character=0xFF;    // start from NUMBER and overwrite on next
          if(++get_cursor>5)
           {
             kn1=0;                     //TEST correction of password  
             if(select_item==0)         //enter menu in page==0
             {
               if(menu_item==ORDER_OF_INTERFACE || menu_item==ORDER_OF_FORMAT)        
                  {
                   if(get_pass[0]==Pass_1[0] && get_pass[1]==Pass_1[1] &&  
                      get_pass[2]==Pass_1[2] && get_pass[3]==Pass_1[3] &&
                      get_pass[4]==Pass_1[4] && get_pass[5]==Pass_1[5])kn1=0xFF;    
                      
                      #ifdef IGNORE_PASS
                       if(get_pass[0]=='*' && get_pass[1]=='*' && get_pass[2]=='*' && get_pass[3]=='*' && get_pass[4]=='*' && get_pass[5]=='*')kn1=0xFF;
                      #endif                       
                      
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass);     
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);
                  } 
               else
               if(menu_item==ORDER_OF_GENERAL || menu_item==ORDER_OF_APPLICATION)   //general  and application
                  {
                   if(get_pass[0]==Pass_2[0] && get_pass[1]==Pass_2[1] &&
                      get_pass[2]==Pass_2[2] && get_pass[3]==Pass_2[3] &&
                      get_pass[4]==Pass_2[4] && get_pass[5]==Pass_2[5])kn1=0xFF; 
                     
                   #ifdef IGNORE_PASS 
                   if(get_pass[0]=='*' && get_pass[1]=='*' && get_pass[2]=='*' && get_pass[3]=='*' && get_pass[4]=='*' && get_pass[5]=='*')kn1=0xFF;
                   #endif   
                   
                     if(kn1!=0xFF)kn1=check_master_pass(get_pass);  
                     if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);           
                                                             
                  }  
               else
               if(menu_item==ORDER_OF_RESTORE)   //restore
                  {
                   if( get_pass[0]==Pass_3[0] && get_pass[1]==Pass_3[1] &&
                       get_pass[2]==Pass_3[2] && get_pass[3]==Pass_3[3] &&
                       get_pass[4]==Pass_3[4] && get_pass[5]==Pass_3[5])kn1=0xFF; 
                    
                   #ifdef IGNORE_PASS  
                   if(get_pass[0]=='*' && get_pass[1]=='*' && get_pass[2]=='*' && get_pass[3]=='*' && get_pass[4]=='*' && get_pass[5]=='*')kn1=0xFF;
                   #endif               
                   
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass);     
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);
              
                                                  
                  }                   
               else
               if(menu_item==ORDER_OF_CALIBRATION)   //calibration
                  {
                   if(get_pass[0]==Pass_4[0] && get_pass[1]==Pass_4[1] &&
                       get_pass[2]==Pass_4[2] && get_pass[3]==Pass_4[3] &&
                       get_pass[4]==Pass_4[4] && get_pass[5]==Pass_4[5])kn1=0xFF;       
                       
                   if(!WRITE_PROTECT)kn1=0;             //ignore entrance by user password if Write Protection is enabled    

                   #ifdef IGNORE_PASS
                   if(get_pass[0]=='*' && get_pass[1]=='*' && get_pass[2]=='*' && get_pass[3]=='*' && get_pass[4]=='*' && get_pass[5]=='*')kn1=0xFF;
                   #endif     
                   
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass);  
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);                                                     
                       
                  }                       
             } 
             else 
             if(select_item!=0)         //in restore menu and change menu
             {
               if(select_item==ORDER_OF_CH_PAS1)        
                  {
                   if(get_pass[0]==Pass_1[0] && get_pass[1]==Pass_1[1] &&  
                      get_pass[2]==Pass_1[2] && get_pass[3]==Pass_1[3] &&
                      get_pass[4]==Pass_1[4] && get_pass[5]==Pass_1[5])kn1=0xFF;  
                      
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass);      
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);             
                  } 
               else
               if(select_item==ORDER_OF_CH_PAS2)  
                  {
                   if(get_pass[0]==Pass_2[0] && get_pass[1]==Pass_2[1] &&
                      get_pass[2]==Pass_2[2] && get_pass[3]==Pass_2[3] &&
                      get_pass[4]==Pass_2[4] && get_pass[5]==Pass_2[5])kn1=0xFF;  
                      
                     if(kn1!=0xFF) kn1=check_master_pass(get_pass);     
                     if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);                          
                  }  
               else
               if(select_item==ORDER_OF_CH_PAS3)  
                  {
                   if( get_pass[0]==Pass_3[0] && get_pass[1]==Pass_3[1] &&
                       get_pass[2]==Pass_3[2] && get_pass[3]==Pass_3[3] &&
                       get_pass[4]==Pass_3[4] && get_pass[5]==Pass_3[5])kn1=0xFF;      
                       
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass); 
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);                            
                  }                   
               else
               if(select_item==ORDER_OF_CH_PAS4) 
                  {
                   if(get_pass[0]==Pass_4[0] && get_pass[1]==Pass_4[1] &&
                       get_pass[2]==Pass_4[2] && get_pass[3]==Pass_4[3] &&
                       get_pass[4]==Pass_4[4] && get_pass[5]==Pass_4[5])kn1=0xFF;   
                       
                       
                      if(kn1!=0xFF)kn1=check_master_pass(get_pass);
                      if(kn1!=0xFF)kn1=check_one_use_pass(get_pass);                               
                  }                       
             }                 
                                
             if(kn1==0xFF)          //PASSWORD IS CORRECT
               {
                     if(menu_item==ORDER_OF_RESTORE && select_item>0)      //select_item in page==1
                       { 
                           page=14;                                     //GO TO CHANGE NEW PASSWORD
                           get_pass[0]='*';get_pass[1]='*';
                           get_pass[2]='*';get_pass[3]='*';   
                           get_pass[4]='*';get_pass[5]='*'; 
                           get_cursor=0;
                           blink_get_data_cntr=1;   
                           blink_en=0;
                           get_key_character=0xFF;
                           get_character_index=0;   
                       }    
                     else
                       { 
                           page=2;       //Password is correct  
                           select_item=1; 
                           blink_get_data_cntr=0;                                                                                
                       }
                    clear_home_pos_en=1; 
                    display_home_pos_en=1; 
               }
             else           //PASSWORD IS WORONG!!!
               {
                  if(menu_item==ORDER_OF_RESTORE && select_item>0)page=2;      //select_item in page==1
                  else page=1;               //Password is incorrect  
                  
                  display_home_pos_en=1; 
                  blink_get_data_cntr=0; 
                  clear_home_pos_en=1; 
                  all_keys_temp_disable=30;  
                  no_display_change_cntr=30;          //3 second
                  display_home_pos_message_code='i';  //pass incorect mode                     
               } 
               
           }
          display_home_pos_en=1; 
        }          
                     
     
     }   //end page==3
 
    else
    if(page==4)
     {    
      if(key_code=='4')
       { 
         previous_page=0; 
         menu_item=ORDER_OF_APPLICATION;select_item=ORDER_OF_1STWEIGH;sub_select_item=ORDER_OF_1ST_VIEW;  
         page=17; 
         page_4_out_cntr=0;    
                       get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits          
       }    
      else
      if(key_code=='5')            //Manual Tare
       {
         clear_home_pos_en=1;display_home_pos_en=1;   ////***\\\ 
         page_4_out_cntr=0;
         #ifdef KEYBOARD_4x4
         page=0;key_code='2';goto first;   
         #else
         page=0;key_code=PBTARE_KEY;goto first;  
         #endif
       }   
      else
      if(key_code=='6')
       {
         clear_home_pos_en=1;display_home_pos_en=1;   ////***\\\ 
         page_4_out_cntr=0;
         #ifdef KEYBOARD_4x4
         page=0;key_code='1';goto first;
         #else
         page=0;key_code=PBZERO_KEY;goto first;  
         #endif
       }  
      else  
      if(key_code=='8')
       {     
         clear_home_pos_en=1;display_home_pos_en=1;   ////***\\\         
         page_4_out_cntr=0;
         #ifdef KEYBOARD_4x4
         page=0;key_code=INFO_KEY;goto first;
         #else
         page=0;key_code=INFO_KEY;goto first;  
         #endif              
       }       
      else        
      if(key_code>='4' && key_code<='9')      //next menu item here is '6'
        { 
          page_4_out_cntr=0;
          
          page=0;                ////***\\\
          clear_home_pos_en=1;     ////***\\\
          display_home_pos_en=1;   ////***\\\         
        
        }
      else
      if(key_code=='0')   //TenFold 
       {
         if(scales_func_mode==0)                               //both scales are enable
                   {        
                      page_4_out_cntr=50;        //5 second to enter a NUM
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_X10;                           //select item save          
                      using_scale=0;
                   }    
         else      
                   {
                     tenfold_out_cntr=TENFOLD_TIMEOUT;     
                     page=0;           
                     using_scale= scales_func_mode;
                   }    
          clear_home_pos_en=1;  
          display_home_pos_en=1;                      
       }           
      else
      if(key_code=='3')   //          
        {
           page_4_out_cntr=50;        //5 second to enter a NUM
           if(menu_item==1)
            {
                  if(scales_func_mode==0)                               //both scales are enable
                   {        
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_WB;                           //select item save 
                   }    
                  else      
                   {
                        if(scales_func_mode==1)using_scale=1;
                        if(scales_func_mode==2)using_scale=2;
                        first_weighing_state=0;                //1st weighing  
                        second_weighing_state=0;   
                        basic_weighing_state=1;
                        page=21;  
                        menu_item=4;                         //Enter Fixed-Tare page
                        temp_reg_first_weigh=0;
                        temp_reg_second_weigh=0;
                        temp_reg_total_weigh=0;  
                        
                        sprintf(get_value_ch,"0    ",); 
                        get_value_i1=3;             //number of digits  
                        blink_get_data_cntr=1;   
                        get_character_index=0;
                        get_cursor=0;   
                        page_4_out_cntr=0;                              
                   }                      
            }
          
          clear_home_pos_en=1;  
          display_home_pos_en=1;            
        
        }        
      else
      if(key_code=='1')
        {
           page_4_out_cntr=50;        //5 second to enter a NUM
           if(menu_item==1)
            {
                  if(scales_func_mode==0)                               //both scales are enable
                   {        
                      menu_item=2;                 //select scale  
                      select_item=ORDER_OF_W1;                           //select item save 
                   }    
                  else      
                   {
                        if(scales_func_mode==1)using_scale=1;
                        if(scales_func_mode==2)using_scale=2;
                        first_weighing_state=1;                //1st weighing  
                        second_weighing_state=0;   
                        page=21;  menu_item=1;                         //Enter Truck_ID page
                        temp_reg_first_weigh=0;
                        temp_reg_second_weigh=0;
                        temp_reg_total_weigh=0;  
                   }                      
            }
           else 
           if(menu_item==2)                
            {

                  if(select_item==ORDER_OF_X10)
                        {
                           using_scale=1;  
                           tenfold_out_cntr=TENFOLD_TIMEOUT;     
                           page=0;                ////***\\\
                           clear_home_pos_en=1;     ////***\\\
                           display_home_pos_en=1;   ////***\\\     
                           menu_item=0;                                   
                        }  
                   else                      
                  if(select_item==ORDER_OF_W1)
                        {
                          using_scale=1;
                          first_weighing_state=1;    
                          second_weighing_state=0;      
                          page=21; menu_item=1;                  //Enter Truck_ID page to save
                          temp_reg_first_weigh=0;
                          temp_reg_second_weigh=0;
                          temp_reg_total_weigh=0;                                                     
                        }                                       //1st weiging for S1  
                        
                  else
                  if(select_item==ORDER_OF_W2)
                        {
                          using_scale=1;     
                          first_weighing_state=0;
                          second_weighing_state=1;     
                          page=21; menu_item=2;                  //Enter Inbound-Code page  
                          temp_reg_first_weigh=0;
                          temp_reg_second_weigh=0;
                          temp_reg_total_weigh=0;                                
                        }                                       //2nd weiging for S1     
                  else
                  if(select_item==ORDER_OF_WB)
                        {
                         using_scale=1;
                         first_weighing_state=0;                //1st weighing  
                         second_weighing_state=0;   
                         basic_weighing_state=1;
                         page=21;  
                         menu_item=4;                         //Enter Fixed-Tare page
                         temp_reg_first_weigh=0;
                         temp_reg_second_weigh=0;
                         temp_reg_total_weigh=0;  
                          
                        }   
                   else
                  if(select_item==ORDER_OF_MZ)
                        {
                              if(pb_zero_s1_state==0)pb_zero_s1_state = 1;
                              else  if(pb_zero_s1_state) pb_zero_s1_state = 0xFF;  
                              display_weight_en=1;
                              page=0;           
                        }                  
                   else
                  if(select_item==ORDER_OF_MT)
                        {
//                              if((manual_tare_s1_state==0) && (scale1_manual_tare!=0))manual_tare_s1_state = 1;
  //                            else  if(manual_tare_s1_state) manual_tare_s1_state = 0xFF;
                                if( manual_tare_s1_state)  manual_tare_s1_state=0xFF;            
                                if( (manual_tare_s1_state==0) && (pb_tare_s1_state==0)) pb_tare_s1_state= 1;
                                else  if( pb_tare_s1_state )  pb_tare_s1_state= 0xFF;      
                                display_weight_en=1;
                                page=0;           
                        }                                
                                             
                        
            }           
           if(page==21)
            {
                    sprintf(get_value_ch,"0    ",); 
                    if(menu_item==1)get_value_i1=5;               //number of digits  
                       else get_value_i1=3;
                    blink_get_data_cntr=1;   
                    get_character_index=0;
                    get_cursor=0;   
                    page_4_out_cntr=0;            
            }
          
           clear_home_pos_en=1;  
           display_home_pos_en=1;            
        }
      else    
      if(key_code=='2')
        {
          page_4_out_cntr=50;        //5 second to enter a NUM
          if(menu_item==1)
            {
                  if(scales_func_mode==0)                               //both scales are enable
                   {        
                      menu_item=ORDER_OF_FKEY_SEL_SCALE;                 //select scale
                      select_item=ORDER_OF_W2;                           //select item save 
                   } 
                  else      
                   {
                        if(scales_func_mode==1)using_scale=1;
                        if(scales_func_mode==2)using_scale=2;
                        first_weighing_state=0;                         //2nd weighing
                        second_weighing_state=1;                                                                                   
                          temp_reg_first_weigh=0;
                          temp_reg_second_weigh=0;
                          temp_reg_total_weigh=0;      
                        page=21; menu_item=2;                           //Enter Inbound-Code page                                  
                   }     
            }
           else 
          if(menu_item==2)                //select scale 2
            {
                  if(select_item==ORDER_OF_X10)
                        {
                           using_scale=2;  
                           tenfold_out_cntr=TENFOLD_TIMEOUT;     
                           page=0;                ////***\\\
                           clear_home_pos_en=1;     ////***\\\
                           display_home_pos_en=1;   ////***\\\      
                           menu_item=0;                                  
                        }                      
                  if(select_item==ORDER_OF_W1) //1st weiging for S2
                        {
                          using_scale=2;
                          first_weighing_state=1; 
                          second_weighing_state=0;
                          page=21; menu_item=1;                  //Enter Truck_ID page
                          temp_reg_first_weigh=0;
                          temp_reg_second_weigh=0;
                          temp_reg_total_weigh=0;                            
                        }                
                  else
                  if(select_item==ORDER_OF_W2) //2nd weiging for S2
                        {
                          using_scale=2;  
                          first_weighing_state=0;
                          second_weighing_state=1;
                          temp_reg_first_weigh=0;
                          temp_reg_second_weigh=0;
                          temp_reg_total_weigh=0;      
                          page=21; menu_item=2;                          //Enter Inbound-Code page
                        } 
                  else
                  if(select_item==ORDER_OF_WB)  //basic weighing for S2
                        {
                         using_scale=2;
                         first_weighing_state=0;             
                         second_weighing_state=0;   
                         basic_weighing_state=1;
                         page=21;  
                         menu_item=4;                           //Enter Fixed-Tare page
                         temp_reg_first_weigh=0;
                         temp_reg_second_weigh=0;
                         temp_reg_total_weigh=0;  
                        }  
                   else     
                  if(select_item==ORDER_OF_MZ)
                        {
                              if(pb_zero_s2_state==0)pb_zero_s2_state = 1;
                              else  if(pb_zero_s2_state) pb_zero_s2_state = 0xFF;         
                              display_weight_en=1;
                              page=0;           
                        }        
                   else
                  if(select_item==ORDER_OF_MT)
                        {
//                              if((manual_tare_s2_state==0)&&(scale2_manual_tare!=0))manual_tare_s2_state = 1;
  //                            else  if(manual_tare_s2_state) manual_tare_s2_state = 0xFF;
                               if( manual_tare_s2_state)  manual_tare_s2_state=0xFF;            
                               if( (manual_tare_s2_state==0) && (pb_tare_s2_state==0)) pb_tare_s2_state= 1;
                               else  if( pb_tare_s2_state )  pb_tare_s2_state= 0xFF;           
                              display_weight_en=1;
                              page=0;           
                        }                                                                                   
                        
            }        
            
          if(page==21)
            {
                    sprintf(get_value_ch,"0     "); 
                    if(menu_item==1)get_value_i1=5;               //number of digits  
                       else get_value_i1=3;
                    blink_get_data_cntr=1;   
                    get_character_index=0;
                    get_cursor=0;    
                    page_4_out_cntr=0;           
            }
          
          clear_home_pos_en=1;  
          display_home_pos_en=1;                      
          
        }
      else      
      if(key_code==FUNC_KEY || key_code==ENTER_KEY)
       {          
          page=0;
          clear_home_pos_en=1;  
          display_home_pos_en=1;  
          page_4_out_cntr=0;     
       }     
     
     }        //end page==4
     
    else
    if(page==5)
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        {  
          if(get_cursor==0)
          {
             if(get_character_index==0)
              {
               get_value_i1='0';
               get_value_i2=key_code;  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i1=get_value_i2;
               get_value_i2=key_code;  
              }                
              
          }  
          if(get_cursor==1)
          {
             if(get_character_index==0)
              {
               get_value_i3='0';
               get_value_i4=key_code;  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i3=get_value_i4;
               get_value_i4=key_code;  
              }                
              
          }          
         display_home_pos_en=1;  
        
        }    
      if(key_code==CLEAR_KEY)      //clear a character
        {  
          if(get_cursor==0)
          {        
             if(get_character_index==0)
              {
               get_value_i1='0';
               get_value_i2='0';  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i2=get_value_i1;  
               get_value_i1='0';
              }                
              
          }  
          if(get_cursor==1)
          {
             if(get_character_index==0)
              {
               get_value_i3='0';
               get_value_i4='0';  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i4=get_value_i3; 
               get_value_i3='0';
              }                
              
          }          
         display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
          if(get_cursor==0)
           {get_cursor=1;
            get_character_index=0;}
           else 
           {         //save time
             get_value_i1-='0';get_value_i2-='0';get_value_i3-='0';get_value_i4-='0'; 
             get_value_i1*=10;get_value_i1+=get_value_i2;
             get_value_i3*=10;get_value_i3+=get_value_i4;  
             if(get_value_i1<24 && get_value_i3<60)   //time is valid
              {
                 #asm("cli")
                 hour=get_value_i1;minute=get_value_i3;second=0;
                 #asm("sei")
                 rtc_set_time(hour,minute,second);
                 display_home_pos_message_code='T';  //pass incorect mode 
           
              }
             else
              {            //time is invalid
                display_home_pos_message_code='t'; 
              }  
              
             blink_get_data_cntr=0; 
             clear_home_pos_en=1; 
             all_keys_temp_disable=30;  
             no_display_change_cntr=30;          //3 second  
                
             page=2;
             clear_home_pos_en=1;  
             blink_get_data_cntr=0;                             
           }  
           
          display_home_pos_en=1; 
        }            
     
     }          //end page==5    
     
    else
    if(page==6)
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code>='0' && key_code<='9')     
        {  
          if(get_cursor==0)
          {
             if(get_character_index==0)
              {
               get_value_i1='0';
               get_value_i2=key_code;  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i1=get_value_i2;
               get_value_i2=key_code;  
              }                
              
          }  
          if(get_cursor==1)
          {
             if(get_character_index==0)
              {
               get_value_i3='0';
               get_value_i4=key_code;  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i3=get_value_i4;
               get_value_i4=key_code;  
              }                
              
          }    
          if(get_cursor==2)
          {
             if(get_character_index==0)
              {
               get_value_i5='0';
               get_value_i6=key_code;  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i5=get_value_i6;
               get_value_i6=key_code;  
              }                
              
          }                  
         display_home_pos_en=1;  
        
        }    
      else  
      if(key_code==CLEAR_KEY)      //clear a character
        {  
          if(get_cursor==0)
          {        
             if(get_character_index==0)
              {
               get_value_i1='0';
               get_value_i2='1';  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i2=get_value_i1;  
               get_value_i1='0';
              }                
              
          }  
          if(get_cursor==1)
          {
             if(get_character_index==0)
              {
               get_value_i3='0';
               get_value_i4='0';  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i4=get_value_i3; 
               get_value_i3='0';
              }                
              
          }  
          if(get_cursor==2)
          {
             if(get_character_index==0)
              {
               get_value_i5='0';
               get_value_i6='0';  
               get_character_index=1;
              }    
             else
             if(get_character_index==1)
              {
               get_value_i6=get_value_i5; 
               get_value_i5='0';
              }                
              
          }                   
         display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
          if(get_cursor==0)
           {get_cursor=1;
            get_character_index=0;}  
          else  
          if(get_cursor==1)
           {get_cursor=2;
            get_character_index=0;}            
           else 
           {         //save time
             get_value_i1-='0';get_value_i2-='0';get_value_i3-='0';
             get_value_i4-='0'; get_value_i5-='0'; get_value_i6-='0'; 
             get_value_i1*=10;get_value_i1+=get_value_i2;
             get_value_i3*=10;get_value_i3+=get_value_i4;  
             get_value_i5*=10;get_value_i5+=get_value_i6;  
             if(get_value_i3>0 && get_value_i3<13 && get_value_i5>0 && get_value_i5<32)   //date is valid
              {
                 #asm("cli")
                 year=get_value_i1;month=get_value_i3;day=get_value_i5;
                 #asm("sei")
                 rtc_set_date(day,month,year);
                 display_home_pos_message_code='D';  
           
              }
             else
              {            //time is invalid
                display_home_pos_message_code='d'; 
              }  
              
             blink_get_data_cntr=0; 
             clear_home_pos_en=1; 
             all_keys_temp_disable=30;  
             no_display_change_cntr=30;          //3 second  
                
             page=2;
             clear_home_pos_en=1;  
             blink_get_data_cntr=0;                             
           }  
           
          display_home_pos_en=1; 
        }            
     
     }          //end page==6          
 
   else
    if(page==7)        //com1 and com2 profile setting
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code==UP_KEY)     
        {  
          if(get_cursor==0)
          {
               if(++get_value_i1>6)get_value_i1=0;
          }  
          if(get_cursor==1)
          {
               if(++get_value_i2>1)get_value_i2=0;
          }  
          if(get_cursor==2)
          {
               if(++get_value_i3>2)get_value_i3=0;
          }  
          if(get_cursor==3)
          {
               if(++get_value_i4>1)get_value_i4=0;
          }                                      
         display_home_pos_en=1;  
        }    
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(get_cursor==0)
          {
               if(get_value_i1)get_value_i1--;else get_value_i1=6;
          }  
          if(get_cursor==1)
          {
               if(get_value_i2)get_value_i2--;else get_value_i2=1;
          }  
          if(get_cursor==2)
          {
               if(get_value_i3)get_value_i3--;else get_value_i3=2;
          }  
          if(get_cursor==3)
          {
               if(get_value_i4)get_value_i4--;else get_value_i4=1;
          }                                      
         display_home_pos_en=1;  
        }   
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {  
          if(get_cursor==0)
          {
               if(select_item==ORDER_OF_COM1PROF)get_value_i1=com1_baud_rate;
               if(select_item==ORDER_OF_COM2PROF)get_value_i1=com2_baud_rate;
          } 
          else 
          if(get_cursor==1)
          {
               if(select_item==ORDER_OF_COM1PROF)get_value_i2=com1_databits;
               if(select_item==ORDER_OF_COM2PROF)get_value_i2=com2_databits;
          }   
          else
          if(get_cursor==2)
          {
               if(select_item==ORDER_OF_COM1PROF)get_value_i3=com1_parity;
               if(select_item==ORDER_OF_COM2PROF)get_value_i3=com2_parity;
          }   
          else
          if(get_cursor==3)
          {
               if(select_item==ORDER_OF_COM1PROF)get_value_i4=com1_stop_bits;  
               if(select_item==ORDER_OF_COM2PROF)get_value_i4=com2_stop_bits;
          }    
         display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
          if(get_cursor<3)get_cursor++;  
          else 
           {         //save time

             if(select_item==ORDER_OF_COM1PROF)
             {
              com1_baud_rate=get_value_i1;EE_com1_baud_rate=get_value_i1;
              com1_databits=get_value_i2;EE_com1_databits=get_value_i2;
              com1_parity=get_value_i3;EE_com1_parity=get_value_i3;     
              com1_stop_bits=get_value_i4;EE_com1_stop_bits=get_value_i4; 
              config_USART(1);
             }
             if(select_item==ORDER_OF_COM2PROF)
             {
              com2_baud_rate=get_value_i1;EE_com2_baud_rate=get_value_i1;
              com2_databits=get_value_i2;EE_com2_databits=get_value_i2;
              com2_parity=get_value_i3;EE_com2_parity=get_value_i3;    
              com2_stop_bits=get_value_i4;EE_com2_stop_bits=get_value_i4;
              config_USART(2);
             }             

             display_home_pos_message_code='O'; 
             all_keys_temp_disable=30;  
             no_display_change_cntr=30;          //3 second                
              
             blink_get_data_cntr=0; 
             clear_home_pos_en=1; 

             page=2;
             clear_home_pos_en=1;  
             blink_get_data_cntr=0;                             
           }  
           
          display_home_pos_en=1; 
        }            
     
     }          //end page==7   
     
   else
    if(page==8)        //com1 and com2 func setting
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code==UP_KEY)     
        {  
          if(select_item==ORDER_OF_COM1FUNC){if(++get_value_i1>6)get_value_i1=0;}
          if(select_item==ORDER_OF_COM2FUNC){if(++get_value_i1>6)get_value_i1=0;}      
          if(select_item==ORDER_OF_COM1PROT){if(++get_value_i1>5)get_value_i1=0;}
          if(select_item==ORDER_OF_COM2PROT){if(++get_value_i1>5)get_value_i1=0;}          
         // display_home_pos_en=1;  
          blink_get_data_cntr=BLINK_ON_TIME;
        }    
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(select_item==ORDER_OF_COM1FUNC){if(get_value_i1)get_value_i1--;else get_value_i1=6;}
          if(select_item==ORDER_OF_COM2FUNC){if(get_value_i1)get_value_i1--;else get_value_i1=6;}      
          if(select_item==ORDER_OF_COM1PROT){if(get_value_i1)get_value_i1--;else get_value_i1=5;}
          if(select_item==ORDER_OF_COM2PROT){if(get_value_i1)get_value_i1--;else get_value_i1=5;}          
          //display_home_pos_en=1;   
           blink_get_data_cntr=BLINK_ON_TIME;
        }   
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {  
          if(select_item==ORDER_OF_COM1FUNC)get_value_i1=com1_func;
          if(select_item==ORDER_OF_COM2FUNC)get_value_i1=com2_func;      
          if(select_item==ORDER_OF_COM1PROT)get_value_i1=com1_protocol;
          if(select_item==ORDER_OF_COM2PROT)get_value_i1=com2_protocol;
  
          display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
           if(select_item==ORDER_OF_COM1FUNC)
             {
              com1_func=get_value_i1;EE_com1_func=get_value_i1;                           
              config_USART(1);
             }
           if(select_item==ORDER_OF_COM2FUNC)
             {
              com2_func=get_value_i1;EE_com2_func=get_value_i1;      
              config_USART(2);
             } 
           if(select_item==ORDER_OF_COM1PROT)
             {
              com1_protocol=get_value_i1;EE_com1_protocol=get_value_i1;
             }
           if(select_item==ORDER_OF_COM2PROT)
             {
              com2_protocol=get_value_i1;EE_com2_protocol=get_value_i1;
             }                         

           display_home_pos_message_code='O';  
              
           blink_get_data_cntr=0; 
           clear_home_pos_en=1; 
           all_keys_temp_disable=30;  
           no_display_change_cntr=30;          //3 second  
                
           page=2;
           clear_home_pos_en=1;  
           blink_get_data_cntr=0;                             
           
           display_home_pos_en=1; 
        }            
     
     }          //end page==8    
     
 else
    if(page==9)        //general
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;  
          if(select_item==ORDER_OF_VOLTAGE){countout_display=0;clear_all_graph_en=1;display_area_en=1;display_weight_en=1;}   
        }
      else   
      if(key_code==UP_KEY)     
        {  
          if(select_item==ORDER_OF_LANGUAGE){if(++get_value_i1>1)get_value_i1=0;}
          if(select_item==ORDER_OF_SOFTWARE){if(++get_value_i1>5)get_value_i1=0;} 
        //  if(select_item==ORDER_OF_BL_TIME) {if(++get_value_i1>240)get_value_i1=50;} 
          if(select_item==ORDER_OF_DONGLE){if(++get_value_i1>3)get_value_i1=0;}  
          if(select_item==ORDER_OF_CALENDAR){if(++get_value_i1>1)get_value_i1=0;}
          if(select_item==ORDER_OF_ALARM){if(++get_value_i1>1)get_value_i1=0;}     
          if(select_item==ORDER_OF_HOMEPOS){if(++get_value_i1>1)get_value_i1=0;}    
          
          //display_home_pos_en=1;    
           blink_get_data_cntr=BLINK_ON_TIME;
        }    
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(select_item==ORDER_OF_LANGUAGE){if(get_value_i1)get_value_i1--;else get_value_i1=1;}
          if(select_item==ORDER_OF_SOFTWARE){if(get_value_i1)get_value_i1--;else get_value_i1=5;} 
        //  if(select_item==ORDER_OF_BL_TIME) {if(get_value_i1)get_value_i1--;else get_value_i1=0;}          
          if(select_item==ORDER_OF_DONGLE){if(get_value_i1)get_value_i1--;else get_value_i1=3;} 
          if(select_item==ORDER_OF_CALENDAR){if(get_value_i1)get_value_i1--;else get_value_i1=1;}  
          if(select_item==ORDER_OF_ALARM){if(get_value_i1)get_value_i1--;else get_value_i1=1;}  
          if(select_item==ORDER_OF_HOMEPOS){if(get_value_i1)get_value_i1--;else get_value_i1=1;}
          
         // display_home_pos_en=1;   
          blink_get_data_cntr=BLINK_ON_TIME;
        }   
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {      
          if(select_item==ORDER_OF_LANGUAGE)get_value_i1=language;
          if(select_item==ORDER_OF_SOFTWARE)get_value_i1=software;
         // if(select_item==ORDER_OF_BL_TIME) get_value_i1=backlight_time;
          if(select_item==ORDER_OF_DONGLE)get_value_i1=dongletype;   
          if(select_item==ORDER_OF_CALENDAR)get_value_i1=calendar;   
          if(select_item==ORDER_OF_HOMEPOS)get_value_i1=homepos;
          if(select_item==ORDER_OF_ALARM)get_value_i1=alarm;  
  
          display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
             
          if(select_item==ORDER_OF_LANGUAGE){language=get_value_i1;EE_language=get_value_i1;}
          if(select_item==ORDER_OF_SOFTWARE){software=get_value_i1;EE_software=get_value_i1;}
         // if(select_item==ORDER_OF_BL_TIME){ backlight_time=get_value_i1;EE_backlight_time=backlight_time;}
          if(select_item==ORDER_OF_DONGLE){dongletype=get_value_i1;EE_dongletype=get_value_i1;} 
          if(select_item==ORDER_OF_CALENDAR){calendar=get_value_i1;EE_calendar=get_value_i1;}
          if(select_item==ORDER_OF_ALARM){alarm=get_value_i1;EE_alarm=get_value_i1;}        
          if(select_item==ORDER_OF_HOMEPOS){homepos=get_value_i1;EE_homepos=get_value_i1;display_area_en=1;display_weight_en=1;clear_all_graph_en=1;  }
          if(select_item==ORDER_OF_VOLTAGE){countout_display=0;clear_all_graph_en=1;display_area_en=1;display_weight_en=1;}       

           if(display_area_en==0)
             {
               display_home_pos_message_code='O';
               all_keys_temp_disable=30;  
               no_display_change_cntr=30;          //3 second                    display_home_pos_en=1;
             } 
   

           page=2;
           clear_home_pos_en=1;  
           blink_get_data_cntr=0;                             
           
           display_home_pos_en=1;
            
        }            
     
     }          //end page==9
     
    else
    if(page==10)         //internal menus of calibration:
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;       
        }
      else   
      if(key_code==RIGHT_KEY)      //next menu item here is '6'
        { 
          if(++sub_select_item>no_of_items_in_sub_menus[select_item-1])sub_select_item=1;    
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }      
      else   
      if(key_code==LEFT_KEY)      //previous menu item here is '4'
        { 
          if(--sub_select_item<1)sub_select_item=no_of_items_in_sub_menus[select_item-1];
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        } 
      else    
      if(key_code==ENTER_KEY)      //select a menu
        {                                                                
           if(select_item==ORDER_OF_SCALE1)              
                 {   
                   if(sub_select_item==ORDER_OF_SCALE_RANGE)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_ranges; 
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     } 
                        
                   if(    (sub_select_item==ORDER_OF_SCALE_INTERVAL1) || (sub_select_item==ORDER_OF_SCALE_INTERVAL2) ||
                           (sub_select_item==ORDER_OF_SCALE_INTERVAL3) || (sub_select_item==ORDER_OF_SCALE_CAPACITY1) || 
                           (sub_select_item==ORDER_OF_SCALE_CAPACITY2) || (sub_select_item==ORDER_OF_SCALE_CAPACITY3) )
                     {
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL1){get_value_l1 = scale1_interval[0];get_value_l1 = convert_unit(get_value_l1,scale1_interval_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL2){get_value_l1 = scale1_interval[1];get_value_l1 = convert_unit(get_value_l1,scale1_interval_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL3){get_value_l1 = scale1_interval[2];get_value_l1 = convert_unit(get_value_l1,scale1_interval_unit);     }  
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY1){get_value_l1 = scale1_capacity[0];get_value_l1 = convert_unit(get_value_l1,scale1_capacity_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY2){get_value_l1 = scale1_capacity[1];get_value_l1 = convert_unit(get_value_l1,scale1_capacity_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY3){get_value_l1 = scale1_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale1_capacity_unit);     }                          
                         
                                                                             
                          
                          get_value_i1=6;  
                          
                          if(get_value_l1>999999){/*get_value_i1=1;*/sprintf(get_value_ch,"E");}  //overflow
                          else
                          if(get_value_l1>999)sprintf(get_value_ch,"%d%03d",(unsigned int)(get_value_l1/1000),(unsigned int)(get_value_l1%1000));  
                          else  sprintf(get_value_ch,"%d",get_value_l1);                            
                          
                          page=17;          
                          previous_page=10;
                          display_home_pos_en=1; 
                          clear_home_pos_en=1;  

                          blink_get_data_cntr=1;   
                          get_character_index=strlen(get_value_ch); 
                          get_cursor=0;                     
                     
                     }  
                     
                   if(sub_select_item==ORDER_OF_SCALE_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }     
                   if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_interval_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }    
                   if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_capacity_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }                                              
                   if(sub_select_item==ORDER_OF_SCALE_DIGITS)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_digits;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }    
                   if(sub_select_item==ORDER_OF_SCALE_FLOAT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale1_float;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }                       
                   if(sub_select_item==ORDER_OF_ADAPTATION)                   
                     {
                        page=26;
                        sub_select_item=ORDER_OF_ADAPT_MOTION_W;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                     }                                                                                                    
                 }    
                 
           if(select_item==ORDER_OF_SCALE2)              
                 {   
                   if(sub_select_item==ORDER_OF_SCALE_RANGE)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_ranges; 
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }   

                   if(    (sub_select_item==ORDER_OF_SCALE_INTERVAL1) || (sub_select_item==ORDER_OF_SCALE_INTERVAL2) ||
                           (sub_select_item==ORDER_OF_SCALE_INTERVAL3) || (sub_select_item==ORDER_OF_SCALE_CAPACITY1) || 
                           (sub_select_item==ORDER_OF_SCALE_CAPACITY2) || (sub_select_item==ORDER_OF_SCALE_CAPACITY3) )
                     {
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL1){get_value_l1 = scale2_interval[0];get_value_l1 = convert_unit(get_value_l1,scale2_interval_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL2){get_value_l1 = scale2_interval[1];get_value_l1 = convert_unit(get_value_l1,scale2_interval_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_INTERVAL3){get_value_l1 = scale2_interval[2];get_value_l1 = convert_unit(get_value_l1,scale2_interval_unit);     }  
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY1){get_value_l1 = scale2_capacity[0];get_value_l1 = convert_unit(get_value_l1,scale2_capacity_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY2){get_value_l1 = scale2_capacity[1];get_value_l1 = convert_unit(get_value_l1,scale2_capacity_unit);     }
                          if(sub_select_item==ORDER_OF_SCALE_CAPACITY3){get_value_l1 = scale2_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale2_capacity_unit);     }                          
                                                       
                          
                          get_value_i1=6;  
                          
                          if(get_value_l1>999999){/*get_value_i1=1;*/sprintf(get_value_ch,"E");}  //overflow
                          else
                          if(get_value_l1>999)sprintf(get_value_ch,"%d%03d",(unsigned int)(get_value_l1/1000),(unsigned int)(get_value_l1%1000));  
                          else  sprintf(get_value_ch,"%d",get_value_l1);                            
                          
                          page=17;          
                          previous_page=10;
                          display_home_pos_en=1; 
                          clear_home_pos_en=1;  
                          blink_get_data_cntr=1;   
                          get_character_index=strlen(get_value_ch); 
                          get_cursor=0;                     
                     
                     }                           
                   if(sub_select_item==ORDER_OF_SCALE_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }           
                   if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_interval_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }    
                   if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_capacity_unit;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }                       
                   if(sub_select_item==ORDER_OF_SCALE_DIGITS)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_digits;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }  
                   if(sub_select_item==ORDER_OF_SCALE_FLOAT)                   
                     {
                        page=12;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_value_i1=scale2_float;   
                        blink_get_data_cntr=1;   
                        get_cursor=0;                      
                     }                       
                   if(sub_select_item==ORDER_OF_ADAPTATION)                   
                     {
                        page=26;
                        sub_select_item=ORDER_OF_ADAPT_MOTION_W;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                     }                                                                                                       
                 }                     
 
       }
    }    //end of page===10    
    
 
  
    
else
    if(page==12)        
     {
      if(key_code==UPARROW_KEY)
        {
          page=10;  
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;   
          
          if(sub_select_item>=100)page=26;
        }
      else   
      if(key_code==UP_KEY)                                        
        {  
          if(sub_select_item==ORDER_OF_SCALE_RANGE){if(++get_value_i1>2)get_value_i1=0;}
          if(sub_select_item==ORDER_OF_SCALE_UNIT){if(++get_value_i1>2)get_value_i1=0;} 
          if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT){if(++get_value_i1>2)get_value_i1=0;} 
          if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT){if(++get_value_i1>2)get_value_i1=0;} 
          if(sub_select_item==ORDER_OF_SCALE_DIGITS){if(++get_value_i1>2)get_value_i1=0;} 
          if(sub_select_item==ORDER_OF_SCALE_FLOAT){if(++get_value_i1>3)get_value_i1=0;} 
          
          if(sub_select_item==ORDER_OF_ADAPT_MOTION_W){if(++get_value_i1>3)get_value_i1=0;}             
          if(sub_select_item==ORDER_OF_ADAPT_MOTION_C){if(++get_value_i1>20)get_value_i1=1;}  
          if(sub_select_item==ORDER_OF_ADAPT_FILTER_S){if(++get_value_i1>20)get_value_i1=1;}  
          if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R){if(++get_value_i1>3)get_value_i1=0;}  
          if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP){if(++get_value_i1>99)get_value_i1=0;}  
          if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON){if(++get_value_i1>99)get_value_i1=0;}  
          if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO){if(++get_value_i1>2)get_value_i1=0;}  
          if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD){if(++get_value_i1>20)get_value_i1=1;}  
                   
         // display_home_pos_en=1;     
           blink_get_data_cntr=BLINK_ON_TIME;
        }    
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(sub_select_item==ORDER_OF_SCALE_RANGE){if(get_value_i1)get_value_i1--;else get_value_i1=2;}
          if(sub_select_item==ORDER_OF_SCALE_UNIT){if(get_value_i1)get_value_i1--;else get_value_i1=2;} 
          if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT){if(get_value_i1)get_value_i1--;else get_value_i1=2;} 
          if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT){if(get_value_i1)get_value_i1--;else get_value_i1=2;} 
          if(sub_select_item==ORDER_OF_SCALE_DIGITS){if(get_value_i1)get_value_i1--;else get_value_i1=2;}   
          if(sub_select_item==ORDER_OF_SCALE_FLOAT){if(get_value_i1)get_value_i1--;else get_value_i1=3;}   
          
          if(sub_select_item==ORDER_OF_ADAPT_MOTION_W){if(get_value_i1)get_value_i1--;else get_value_i1=3;}
          if(sub_select_item==ORDER_OF_ADAPT_MOTION_C){if(get_value_i1>1)get_value_i1--;else get_value_i1=20;}
          if(sub_select_item==ORDER_OF_ADAPT_FILTER_S){if(get_value_i1>1)get_value_i1--;else get_value_i1=20;}
          if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R){if(get_value_i1)get_value_i1--;else get_value_i1=3;}
          if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP){if(get_value_i1)get_value_i1--;else get_value_i1=99;}
          if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON){if(get_value_i1)get_value_i1--;else get_value_i1=99;}
          if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO){if(get_value_i1)get_value_i1--;else get_value_i1=2;}
          if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD){if(get_value_i1>1)get_value_i1--;else get_value_i1=20;}
          
          //display_home_pos_en=1;  
           blink_get_data_cntr=BLINK_ON_TIME;
        }   
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {      
          if(select_item==ORDER_OF_SCALE1)
           {          
            if(sub_select_item==ORDER_OF_SCALE_RANGE)get_value_i1=scale1_ranges;
            if(sub_select_item==ORDER_OF_SCALE_UNIT)get_value_i1=scale1_unit;
            if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)get_value_i1=scale1_interval_unit;
            if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)get_value_i1=scale1_capacity_unit;
            if(sub_select_item==ORDER_OF_SCALE_DIGITS)get_value_i1=scale1_digits;       
            if(sub_select_item==ORDER_OF_SCALE_FLOAT)get_value_i1=scale1_float;       
           }
          if(select_item==ORDER_OF_SCALE2)
           {          
            if(sub_select_item==ORDER_OF_SCALE_RANGE)get_value_i1=scale2_ranges;
            if(sub_select_item==ORDER_OF_SCALE_UNIT)get_value_i1=scale2_unit;      
            if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT)get_value_i1=scale2_interval_unit;
            if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT)get_value_i1=scale2_capacity_unit;            
            if(sub_select_item==ORDER_OF_SCALE_DIGITS)get_value_i1=scale2_digits;       
            if(sub_select_item==ORDER_OF_SCALE_FLOAT)get_value_i1=scale2_float;  
           }  
          display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
          if(select_item==ORDER_OF_SCALE1)
           {                              
                  if(sub_select_item==ORDER_OF_SCALE_RANGE){scale1_ranges=get_value_i1;if(WRITE_PROTECT)EE_scale1_ranges=scale1_ranges;}
                  if(sub_select_item==ORDER_OF_SCALE_UNIT){scale1_unit=get_value_i1;if(WRITE_PROTECT)EE_scale1_unit=scale1_unit;}  
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT){scale1_interval_unit=get_value_i1;if(WRITE_PROTECT)EE_scale1_interval_unit=scale1_interval_unit;}
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT){scale1_capacity_unit=get_value_i1;if(WRITE_PROTECT)EE_scale1_capacity_unit=scale1_capacity_unit;}
                  if(sub_select_item==ORDER_OF_SCALE_DIGITS){scale1_digits=get_value_i1;if(WRITE_PROTECT)EE_scale1_digits=scale1_digits;}                         
                  if(sub_select_item==ORDER_OF_SCALE_FLOAT){scale1_float=get_value_i1;if(WRITE_PROTECT)EE_scale1_float=scale1_float;}                         
                  
                  if(sub_select_item==ORDER_OF_ADAPT_MOTION_W){scale1_motion_window=get_value_i1;if(WRITE_PROTECT)EE_scale1_motion_window=scale1_motion_window;} 
                  if(sub_select_item==ORDER_OF_ADAPT_MOTION_C){scale1_motion_counter=get_value_i1;if(WRITE_PROTECT)EE_scale1_motion_counter=scale1_motion_counter;} 
                  if(sub_select_item==ORDER_OF_ADAPT_FILTER_S){scale1_filetr_size=get_value_i1;if(WRITE_PROTECT)EE_scale1_filetr_size=scale1_filetr_size;} 
                  if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R){scale1_auto_zero_range=get_value_i1;if(WRITE_PROTECT)EE_scale1_auto_zero_range=scale1_auto_zero_range;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP){scale1_pb_zero_p=get_value_i1;if(WRITE_PROTECT)EE_scale1_pb_zero_p=scale1_pb_zero_p;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON){scale1_pb_zero_n=get_value_i1;if(WRITE_PROTECT)EE_scale1_pb_zero_n=scale1_pb_zero_n;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO){scale1_powerup_zero=get_value_i1;if(WRITE_PROTECT)EE_scale1_powerup_zero=scale1_powerup_zero;} 
                  if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD){scale1_overload=get_value_i1;if(WRITE_PROTECT)EE_scale1_overload=scale1_overload;} 
                  
           }   
          if(select_item==ORDER_OF_SCALE2)
           {              
                  if(sub_select_item==ORDER_OF_SCALE_RANGE){scale2_ranges=get_value_i1;if(WRITE_PROTECT)EE_scale2_ranges=scale2_ranges;}
                  if(sub_select_item==ORDER_OF_SCALE_UNIT){scale2_unit=get_value_i1;if(WRITE_PROTECT)EE_scale2_unit=scale2_unit;}
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL_UNIT){scale2_interval_unit=get_value_i1;if(WRITE_PROTECT)EE_scale2_interval_unit=scale2_interval_unit;}
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY_UNIT){scale2_capacity_unit=get_value_i1;if(WRITE_PROTECT)EE_scale2_capacity_unit=scale2_capacity_unit;}                  
                  if(sub_select_item==ORDER_OF_SCALE_DIGITS){scale2_digits=get_value_i1;if(WRITE_PROTECT)EE_scale2_digits=scale2_digits;}
                  if(sub_select_item==ORDER_OF_SCALE_FLOAT){scale2_float=get_value_i1;if(WRITE_PROTECT)EE_scale2_float=scale2_float;}
                  
                  if(sub_select_item==ORDER_OF_ADAPT_MOTION_W){scale2_motion_window=get_value_i1;if(WRITE_PROTECT)EE_scale2_motion_window=scale2_motion_window;} 
                  if(sub_select_item==ORDER_OF_ADAPT_MOTION_C){scale2_motion_counter=get_value_i1;if(WRITE_PROTECT)EE_scale2_motion_counter=scale2_motion_counter;} 
                  if(sub_select_item==ORDER_OF_ADAPT_FILTER_S){scale2_filetr_size=get_value_i1;if(WRITE_PROTECT)EE_scale2_filetr_size=scale2_filetr_size;} 
                  if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R){scale2_auto_zero_range=get_value_i1;if(WRITE_PROTECT)EE_scale2_auto_zero_range=scale2_auto_zero_range;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP){scale2_pb_zero_p=get_value_i1;if(WRITE_PROTECT)EE_scale2_pb_zero_p=scale2_pb_zero_p;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON){scale2_pb_zero_n=get_value_i1;if(WRITE_PROTECT)EE_scale2_pb_zero_n=scale2_pb_zero_n;} 
                  if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO){scale2_powerup_zero=get_value_i1;if(WRITE_PROTECT)EE_scale2_powerup_zero=scale2_powerup_zero;} 
                  if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD){scale2_overload=get_value_i1;if(WRITE_PROTECT)EE_scale2_overload=scale2_overload;}                                           
           }           

           if(!WRITE_PROTECT)
            {
              display_home_pos_message_code='W';  
              all_keys_temp_disable=30;  
              no_display_change_cntr=30;          //1 second  no need to display message cause of applying online            
            }
             
           if(sub_select_item>=100)page=26;
           else  page=10;
           
           clear_home_pos_en=1;  
           blink_get_data_cntr=0;                             
           
           display_home_pos_en=1; 
           display_area_en=1;           //for apply changes to AREA  
           display_weight_en=1;           //for apply changes to AREA
           clear_all_graph_en=1;  
        }            
     
     }          //end page==9    
     
     
   else
    if(page==11)        //scale1 and scale2 Interval and capacity
     {
     
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;   
        }   
      else  
      if((key_code==UP_KEY)||(key_code==RIGHT_KEY))     
        {  
         
          if(get_value_i2)
             {
               if(get_value_i4)get_value_i4--;
               else get_value_i4=SIZE_OF_ERROR_LIST;
             }
          else    
             {
               if(get_value_i4)get_value_i4--;
               else get_value_i4=get_value_i1;
             }          

           clear_home_pos_en=1;  
           display_home_pos_en=1;  
        }     
      else  
      if((key_code==DOWN_KEY)||(key_code==LEFT_KEY))       
        {  

          get_value_i4++;
          
          if(get_value_i2)       //get_value_i2==1 when error list overflowed by errormsg()
             {
               if(get_value_i4>SIZE_OF_ERROR_LIST)get_value_i4=0;
             }
          else    
             {
               if(get_value_i4>get_value_i1)get_value_i4=0;
             }           
         clear_home_pos_en=1;  
         display_home_pos_en=1;  
        }                      

     }          //end page==11
     
     
   else
    if(page==13)        //select scale1/2 in calibration
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code==UP_KEY)     
        {  
          if(++get_value_i1>(NUMBER_OF_SELECT_SCALE_TABLE-1))get_value_i1=0;   
         // display_home_pos_en=1;             
          blink_get_data_cntr=BLINK_ON_TIME;
        }    
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(get_value_i1)get_value_i1--;else get_value_i1=(NUMBER_OF_SELECT_SCALE_TABLE-1);  
          //display_home_pos_en=1;
           blink_get_data_cntr=BLINK_ON_TIME;  
        }   
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {  
          get_value_i1=scales_func_mode;
          display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
           scales_func_mode=get_value_i1;     
           if(WRITE_PROTECT)EE_scales_func_mode=scales_func_mode;
           blink_get_data_cntr=0; 
                
           page=2;
           blink_get_data_cntr=0;                             
           
           clear_all_graph_en=1;       //no need to display message cause of applying online
           clear_home_pos_en=1; 
           display_area_en=1; 
           display_weight_en=1; 
           display_home_pos_en=1;           
        }            
     
     }          //end page==13      
     
     

    else
    if(page==14)         //change Password Page
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
      else   
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        { 
        /*   if(key_code==get_key_character || (get_key_character==0xFF))   //press same key  or if overwrite on previous
             {
               if(get_key_character==0xFF)
                    {get_key_character=key_code;
                     get_character_index=0;}
               else  get_character_index++;   
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);
               if(kn1==0)get_character_index=0;
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);  
               get_pass[get_cursor]=kn1;
               display_home_pos_en=1;  
               
             }     */     
           if(get_key_character==0xFF)   //press same key  or if overwrite on previous
             {
               get_key_character=key_code;
               kn1=*(key_character_table_n_address[key_code-'0']);  
               get_pass[get_cursor]=kn1;
               display_home_pos_en=1;  
               
             }              
           else
             {
               if(get_cursor<5)get_cursor++;
               kn1=*(key_character_table_n_address[key_code-'0']);  
               get_pass[get_cursor]=kn1;               
               get_key_character=key_code;   
               
               display_home_pos_en=1;   
               blink_get_data_cntr=1;  
               
             }  
        }    
      if(key_code==CLEAR_KEY)      //clear a character
        { 
           get_pass[get_cursor]='*'; 
           if(get_cursor>0)
            {              
              get_cursor--; 
              get_key_character=0xFF; // start from NUMBER and overwrite on previous
            }
           display_home_pos_en=1; 
        }          
      if(key_code==ENTER_KEY)      //select a menu
        { 
          get_key_character=0xFF;    // start from NUMBER and overwrite on next
          if(++get_cursor>5)
           {
                                                 //SAVE PASSWORDs HERE
             if(select_item==ORDER_OF_CH_PAS1)        
                  {
                      Pass_1[0]=get_pass[0];Pass_1[1]=get_pass[1];
                      Pass_1[2]=get_pass[2];Pass_1[3]=get_pass[3];
                      Pass_1[4]=get_pass[4];Pass_1[5]=get_pass[5];
                      
                      EE_Pass_1[0]=get_pass[0];EE_Pass_1[1]=get_pass[1];
                      EE_Pass_1[2]=get_pass[2];EE_Pass_1[3]=get_pass[3];
                      EE_Pass_1[4]=get_pass[4];EE_Pass_1[5]=get_pass[5];
                      
                  } 
             else
             if(select_item==ORDER_OF_CH_PAS2)   
                  {
                      Pass_2[0]=get_pass[0];Pass_2[1]=get_pass[1];
                      Pass_2[2]=get_pass[2];Pass_2[3]=get_pass[3];
                      Pass_2[4]=get_pass[4];Pass_2[5]=get_pass[5]; 
                      
                      EE_Pass_2[0]=get_pass[0];EE_Pass_2[1]=get_pass[1];
                      EE_Pass_2[2]=get_pass[2];EE_Pass_2[3]=get_pass[3];
                      EE_Pass_2[4]=get_pass[4];EE_Pass_2[5]=get_pass[5];
                      
                      
                  }  
             else
             if(select_item==ORDER_OF_CH_PAS3)   
                  {
                      Pass_3[0]=get_pass[0];Pass_3[1]=get_pass[1];
                      Pass_3[2]=get_pass[2];Pass_3[3]=get_pass[3];
                      Pass_3[4]=get_pass[4];Pass_3[5]=get_pass[5];  
                      
                      EE_Pass_3[0]=get_pass[0];EE_Pass_3[1]=get_pass[1];
                      EE_Pass_3[2]=get_pass[2];EE_Pass_3[3]=get_pass[3];
                      EE_Pass_3[4]=get_pass[4];EE_Pass_3[5]=get_pass[5];
                  }                   
             else
             if(select_item==ORDER_OF_CH_PAS4)   
                  {
                      Pass_4[0]=get_pass[0];Pass_4[1]=get_pass[1];
                      Pass_4[2]=get_pass[2];Pass_4[3]=get_pass[3];
                      Pass_4[4]=get_pass[4];Pass_4[5]=get_pass[5];   
                      
                      EE_Pass_4[0]=get_pass[0];EE_Pass_4[1]=get_pass[1];
                      EE_Pass_4[2]=get_pass[2];EE_Pass_4[3]=get_pass[3];
                      EE_Pass_4[4]=get_pass[4];EE_Pass_4[5]=get_pass[5];
                      
                  }                       
                  
                                
             page=2;               //Password saved
             display_home_pos_en=1; 
             blink_get_data_cntr=0; 
             clear_home_pos_en=1; 
             all_keys_temp_disable=30;  
             display_home_pos_message_code='p';  //pass save mode              
             no_display_change_cntr=30;          //3 second
               
           }
          display_home_pos_en=1; 
        }          
                     
     
     }   //end page==14                
     
   else
    if(page==15)        //RESTORE ALL DATA 
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
        }                    
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

          if(menu_item==ORDER_OF_RESTORE)    //ORDER_OF_RESTORE order of interface menu
          {
          if(select_item==ORDER_OF_FACTORY)
          {           
           //****** RESET FACTORY SETTING HERE **********\\
            EE_General_pointer = &EE_scales_func_mode;

             do
              {
                *(EE_General_pointer) = 0xFF;
  
                 EE_General_pointer++;         
                 delay_ms(2);
                 #asm("wdr")
              }while(EE_General_pointer < &EE_END_OF_SETTINGS);

             load_initial_values();     
                    
             write_e2prom_byte(0,CONSEQ1_L_SAVE_INDEX);                     
             write_e2prom_byte(0,CONSEQ1_H_SAVE_INDEX); 
             write_e2prom_byte(0,WEIGHING_DATA_SAVE_INDEX);       
             
             
             errormsg(LOAD_FACTORY_ALARM,3); //s22
                                                                           

           //****** RESET FACTORY SETTING HERE **********\\
                
           page=2;   
           clear_all_graph_en=1;
           blink_get_data_cntr=0;                             
           display_home_pos_en=1; 
           clear_home_pos_en=1; 
           display_area_en=1; 
           display_weight_en=1;  
          }     
          else
          if(select_item==ORDER_OF_LE2PROM)
          {           
            EE_General_pointer = &EE_START_OF_SETTINGS;
            kn4=START_OF_EEPROM_SAVING_PARAMETERS_ADDRESS;    
            kn2=0;
             do
              {
                
                kn1=read_e2prom_byte(kn4++);
                *(++EE_General_pointer)=kn1;

                 delay_ms(2);
                 #asm("wdr")    
                 if(++kn2>SIZE_OF_EEPROM_SAVING_PARAMETERS_ADDRESS)break;
              }while(EE_General_pointer < (&EE_END_OF_SETTINGS));

            
            load_initial_values();     
            
            
            errormsg(LOAD_EEPROM_PARAMETER_ALARM,3); //s23
                
           page=2;   
           clear_all_graph_en=1;
           blink_get_data_cntr=0;                             
           display_home_pos_en=1; 
           clear_home_pos_en=1; 
           display_area_en=1; 
           display_weight_en=1;  
          }     
          else
          if(select_item==ORDER_OF_SE2PROM)
          {           
            EE_General_pointer = &EE_START_OF_SETTINGS;
            kn4=START_OF_EEPROM_SAVING_PARAMETERS_ADDRESS;    
            kn2=0;
             do
              {
                kn1=*(++EE_General_pointer);
                write_e2prom_byte(kn1,kn4++);

                 delay_ms(2);
                 #asm("wdr")    
                 if(++kn2>SIZE_OF_EEPROM_SAVING_PARAMETERS_ADDRESS)break;
              }while(EE_General_pointer < (&EE_END_OF_SETTINGS));

              errormsg(SAVE_EEPROM_PARAMETER_ALARM,3); //s24
                
            page=2;   
            clear_all_graph_en=1;
            blink_get_data_cntr=0;                             
            display_home_pos_en=1; 
            clear_home_pos_en=1; 
            display_area_en=1; 
            display_weight_en=1;  
          }  
          else
          if(select_item==ORDER_OF_DELERROR)
          {           

             kn1=0;
             for(kn1=0;kn1<SIZE_OF_ERROR_LIST;kn1++)
              {
                ErrorHistory[kn1].ErrorSec=0xFF;
                ErrorHistory[kn1].ErrorMin=0xFF;
                ErrorHistory[kn1].ErrorHour=0xFF;                    
                ErrorHistory[kn1].ErrorDate=0xFF;
                ErrorHistory[kn1].ErrorMonth=0xFF;
                ErrorHistory[kn1].ErrorYear=0xFF;
                ErrorHistory[kn1].ErrorQty=0xFF;
                ErrorHistory[kn1].ErrorNum=0xFF;
                delay_ms(1);
                #asm("wdr")
              }

                   
             write_e2prom_byte(0,WRITE_ERROR_INDEX);                     
             write_e2prom_byte(0,WRITE_ERROR_OV); 
             write_e2prom_byte(0,NO_OF_ERRORS_H);
             write_e2prom_byte(0,NO_OF_ERRORS_L);  

             page=2;   
             clear_all_graph_en=1;
             blink_get_data_cntr=0;                             
             display_home_pos_en=1; 
             clear_home_pos_en=1; 
             display_area_en=1; 
             display_weight_en=1;  
          }                              
          
          
          
          
          
          
          } 
        }            
     
     }          //end page==13      
          

   else
    if(page==16)        //BackUp ALL DATA 
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
        }                    
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

           all_keys_temp_disable=30;  
           display_home_pos_message_code='w';  //waiting       
           no_display_change_cntr=30;             
           
           page=2;
           blink_get_data_cntr=0;                             
           display_home_pos_en=1; 
           clear_home_pos_en=1; 
           display_area_en=1; 
           display_weight_en=1; 
        }            
     
     }          //end page==16
     
     
   else
    if(page==17)        //ENTER a number        
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
          if(menu_item==ORDER_OF_SETPOINT)select_item=1;    //ORDER_OF_TARE order of Tare menus
          page=previous_page;
          display_home_pos_en=1; 
          clear_home_pos_en=1;   
          
          if(sub_select_item>100)sub_select_item-=100;   
          
          first_weighing_state=0;
          second_weighing_state=0;
          basic_weighing_state=0; 
          calibration_state=0;              
          
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
           sprintf(get_value_ch,"0        ");  
           get_character_index=0;  
           display_home_pos_en=1; 
          // get_value_i2=0;              //reset place of point 
        }                    
      else   
      if(key_code>='0' && key_code<='9')
        { 
           if(get_character_index==get_value_i1)get_character_index=get_value_i1-1;
           get_value_ch[get_character_index]=key_code;  
           get_value_ch[get_character_index+1]=0; 
           if(get_character_index<get_value_i1)get_character_index++;
           display_home_pos_en=1;            
        }
      else          
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

           for(kn1=0;kn1<get_value_i1+1;kn1++)
              {
               if(get_value_ch[kn1]==' ')get_value_ch[kn1]=0;    
              }
           get_value_ch[get_value_i1]=0;  
           
           kn2=strlen(get_value_ch);
           get_value_l2=1;  
           get_value_l1=0;
           
           for(kn1=1;kn1<=kn2;kn1++)
            {
              get_value_l3  = get_value_ch[kn2-kn1]-'0';
              get_value_l3 *= get_value_l2;
              get_value_l1 += get_value_l3;
              get_value_l2 *= 10;
            }
            
            
           if(menu_item==ORDER_OF_TARE)
            {
             if(select_item==ORDER_OF_MANUAL_TARE1)   
               {
                 scale1_manual_tare = invert_unit(get_value_l1,get_value_i3);
                 if( (manual_tare_s1_state==0) && (scale1_manual_tare!=0) && (pb_tare_s1_state==0)) manual_tare_s1_state= 1;
                 display_weight_en=1; 
               }  
             if(select_item==ORDER_OF_MANUAL_TARE2)   
               {
                  scale2_manual_tare = invert_unit(get_value_l1,get_value_i3);
                 if( (manual_tare_s2_state==0) && (scale2_manual_tare!=0) && (pb_tare_s2_state==0)) manual_tare_s2_state= 1;
                 display_weight_en=1;                   
               }  
             page=previous_page;  
             blink_get_data_cntr=0;                             
             display_home_pos_en=1; 
             clear_home_pos_en=1;                               
            }   
            
           if(menu_item==ORDER_OF_GENERAL)
            {
             if(select_item==ORDER_OF_BL_TIME)   
               {
                 if(get_value_l1<=240)
                         {
                          backlight_time=(unsigned int)(get_value_l1);  
                          EE_backlight_time=backlight_time;
                         }
                         
               } 
             page=previous_page; 
             blink_get_data_cntr=0;                             
             display_home_pos_en=1; 
             clear_home_pos_en=1;                           
            }                   
            
           if(menu_item==ORDER_OF_SETPOINT)    //ORDER_OF_TARE order of Tare menus
             {

                if(select_item==ORDER_OF_S1_SETPOINT1)         
                 {   
                    scale1_set_point1=invert_unit(get_value_l1,select_unit_str(NULL,1));
                    EE_scale1_set_point1=scale1_set_point1;
                 } 
                if(select_item==ORDER_OF_S1_SETPOINT2)         
                 {   
                    scale1_set_point2=invert_unit(get_value_l1,select_unit_str(NULL,1));
                    EE_scale1_set_point2=scale1_set_point2;
                 } 
                if(select_item==ORDER_OF_S2_SETPOINT1)         
                 {   
                    scale2_set_point1=invert_unit(get_value_l1,select_unit_str(NULL,2));
                    EE_scale2_set_point1=scale2_set_point1;
                 } 
                if(select_item==ORDER_OF_S2_SETPOINT2)         
                 {   
                    scale2_set_point2=invert_unit(get_value_l1,select_unit_str(NULL,2));
                    EE_scale2_set_point2=scale2_set_point2;
                 }  
               page=previous_page;  
               blink_get_data_cntr=0;                             
               display_home_pos_en=1; 
               clear_home_pos_en=1;                                                   
            }  
           
           if(menu_item==ORDER_OF_FORMAT)
            {             
             if(select_item==ORDER_OF_FORMAT_DELETE)         
               { 
                  for(kn1=0;kn1<=NUMBER_OF_FIELDS;kn1++)
                   { 
                      field[kn1].block=0;                                         //save all data here
                      field[kn1].line=0;
                      field[kn1].column=0;
                      field[kn1].attribute=0;
                      field[kn1].type=0;  
                      field[kn1].data_index=0;   
                      delay_ms(3);
                      #asm("wdr")
                    }      
                  sort_print_data();          

                  display_home_pos_message_code='O';  
              
                  blink_get_data_cntr=0; 
                  clear_home_pos_en=1; 
                  all_keys_temp_disable=10;  
                  no_display_change_cntr=10;          //3 second  
                
                  page=2;
                  clear_home_pos_en=1;  
                  blink_get_data_cntr=0;                             
           
                  display_home_pos_en=1; 
               }   
             else                                                                                                                               
             if(select_item==ORDER_OF_FIELDNO)         
               {               
                if(sub_select_item==ORDER_OF_FIELDNO)         
                 {   
                    get_value_i2=(unsigned int)(get_value_l1);    //get_value_i2 is field no    
                    if(get_value_i2<=NUMBER_OF_FIELDS && get_value_i2>0) 
                     {
                       sub_select_item=ORDER_OF_BLOCKNO;   
                       page=17;
                       sprintf(get_value_ch,"%d",field[get_value_i2].block); 
                       get_value_i1=1;               //number of digits  
                       blink_get_data_cntr=1;   
                       get_character_index=0;  
                       get_cursor=0;         
                     }       
                    else page=previous_page; 
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;                       

                 } 
                else  
                if(sub_select_item==ORDER_OF_BLOCKNO)         
                 {   
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i3 is block no
                    sub_select_item=ORDER_OF_LINENO;   
                    page=17;
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;  
                    sprintf(get_value_ch,"%d",field[get_value_i2].line); 
                    get_value_i1=3;               //number of digits  
                    blink_get_data_cntr=1;   
                    get_character_index=strlen(get_value_ch); 
                    if(get_character_index==1)get_character_index=0;  
                    get_cursor=0;                                                    
                 } 
                else     
                if(sub_select_item==ORDER_OF_LINENO)         
                 {   
                    get_value_i4=(unsigned int)(get_value_l1);    //get_value_i4 is line no
                    sub_select_item=ORDER_OF_COLUNO;  
                    page=17;
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;  
                    sprintf(get_value_ch,"%d",field[get_value_i2].column); 
                    get_value_i1=3;               //number of digits  
                    blink_get_data_cntr=1;   
                    get_character_index=strlen(get_value_ch);  
                    if(get_character_index==1)get_character_index=0;
                    get_cursor=0;                                                      
                 } 
                else    
                if(sub_select_item==ORDER_OF_COLUNO)         
                 {   
                    get_value_i5=(unsigned int)(get_value_l1);    //get_value_i5 is column no
                    get_value_i1=field[get_value_i2].attribute;     //get_value_i1 is attribute  
                    sub_select_item=ORDER_OF_ATTR;
                    
                    page=18;     
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;  
                                 
                    blink_get_data_cntr=1;   
                    get_character_index=0;  
                    get_cursor=0;    
                    blink_get_data_cntr=BLINK_ON_TIME;                                                  
                 } 
               }   //if(select_item==ORDER_OF_FIELDNO)
             if(select_item==ORDER_OF_INPUTX)                                      
              {
                    get_value_i1=(unsigned int)(get_value_l1);    //get_value_i1 is order of text
                    page=19;  
                    display_home_pos_en=1; 
                    clear_home_pos_en=1; 
                  //  previous_page=17;  
                  previous_page=2;
                      
                    get_character_index=1;  
                    blink_get_data_cntr=1;  
                    kn1=0;
                    kn2=*(user_defines_texts_table[get_value_i1]);
                    while(1)
                      {
                        get_value_ch[kn1]=kn2;
                        if(++kn1>USER_TEXTS_LENGHT)
                          {
                           get_value_ch[kn1-1]=0;
                           break;
                          } 
                        if(kn2!=0)kn2=*(user_defines_texts_table[get_value_i1]+kn1);  
                      }   
                   get_cursor=strlen(get_value_ch);                    
                                    
              }
                            
            
            
            }    
           if(menu_item==ORDER_OF_CALIBRATION)
            {             
             if(select_item==ORDER_OF_SCALE1)         
               { 
                                                                                         
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) 
                      {
                          scale1_interval[0]=   invert_unit(get_value_l1,scale1_interval_unit);     //change units to gram
                          if(WRITE_PROTECT)EE_scale1_interval[0]=  scale1_interval[0];  
                      }
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) 
                      {
                          scale1_interval[1]=  invert_unit(get_value_l1,scale1_interval_unit);    //change units to gram
                          if(WRITE_PROTECT)EE_scale1_interval[1]=  scale1_interval[1];  
                      } 
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) 
                      {
                          scale1_interval[2]=  invert_unit(get_value_l1,scale1_interval_unit);      //change units to gram
                          if(WRITE_PROTECT)EE_scale1_interval[2]=  scale1_interval[2];  
                      }       
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) 
                      {
                          scale1_capacity[0]=  invert_unit(get_value_l1,scale1_capacity_unit);    //change units to gram
                          if(WRITE_PROTECT)EE_scale1_capacity[0]=  scale1_capacity[0];  
                      }
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) 
                      {
                          scale1_capacity[1]=  invert_unit(get_value_l1,scale1_capacity_unit);    //change units to gram
                          if(WRITE_PROTECT)EE_scale1_capacity[1]=  scale1_capacity[1];  
                      } 
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) 
                      {
                          scale1_capacity[2]=  invert_unit(get_value_l1,scale1_capacity_unit);      //change units to gram
                          if(WRITE_PROTECT)EE_scale1_capacity[2]=  scale1_capacity[2];  
                      }                          
                      
                        page=previous_page;  
                        blink_get_data_cntr=0;                             
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;    
                        display_area_en=1;  
                        clear_all_graph_en=1;   
                        
                                                                                   
               
               }       
               
             if(select_item==ORDER_OF_SCALE2)         
               { 
                                                                                  
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL1) 
                      {
                          scale2_interval[0]=  invert_unit(get_value_l1,scale2_interval_unit);   //change units to gram
                          if(WRITE_PROTECT)EE_scale2_interval[0]=  scale2_interval[0];  
                      }
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL2) 
                      {
                          scale2_interval[1]=  invert_unit(get_value_l1,scale2_interval_unit);     //change units to gram
                          if(WRITE_PROTECT)EE_scale2_interval[1]=  scale2_interval[1];  
                      } 
                  if(sub_select_item==ORDER_OF_SCALE_INTERVAL3) 
                      {
                          scale2_interval[2]=  invert_unit(get_value_l1,scale2_interval_unit);    //change units to gram
                          if(WRITE_PROTECT)EE_scale2_interval[2]=  scale2_interval[2];  
                      }  
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY1) 
                      {
                          scale2_capacity[0]=  invert_unit(get_value_l1,scale2_capacity_unit);    //change units to gram
                          if(WRITE_PROTECT)EE_scale2_capacity[0]=  scale2_capacity[0];  
                      }
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY2) 
                      {
                          scale2_capacity[1]=  invert_unit(get_value_l1,scale2_capacity_unit);     //change units to gram
                          if(WRITE_PROTECT)EE_scale2_capacity[1]=  scale2_capacity[1];  
                      } 
                  if(sub_select_item==ORDER_OF_SCALE_CAPACITY3) 
                      {
                          scale2_capacity[2]=  invert_unit(get_value_l1,scale2_capacity_unit);      //change units to gram
                          if(WRITE_PROTECT)EE_scale2_capacity[2]=  scale2_capacity[2];  
                      }                          
                        page=previous_page;  
                        blink_get_data_cntr=0;                             
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;       
                        display_area_en=1;      
                        clear_all_graph_en=1;                                                            
               
               }   
                                  
             if(select_item==ORDER_OF_CALCAL)         
               { 
                 if(calibration_state==0)
                 { 
                   if(get_value_l1<100)
                    {
                      geo_value=get_value_l1; 
                      if(WRITE_PROTECT)EE_geo_value=geo_value;  
                      page=22;   
                      first_weighing_state=0;
                      second_weighing_state=0;
                      basic_weighing_state=0;   
                      calibration_state=1; 
                      
                      blink_get_data_cntr=0;                             
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;
                                                                
                      if(get_value_i4==1)pb_zero_s1_state=0xFF;  
                        else pb_zero_s2_state=0xFF;      
                        
                      if(get_value_i4==1)manual_tare_s1_state=0xFF;  
                        else manual_tare_s2_state=0xFF;  
                        
                      if(get_value_i4==1)pb_tare_s1_state=0xFF;  
                        else pb_tare_s2_state=0xFF;                                                  
                                             
                    } 
                   else
                    {
                      page=2;
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                     
                    }
                  }
                else     
                 if(calibration_state==5)    //enter calibrateing weigh value
                 { 

                      calibration_state=6;
                      calibration_state_counter=30;                       
                      if(get_value_i4==1)
                         {
                            knl1=invert_unit(get_value_l1,select_unit_str(NULL,1));    //-->grams
                            if(scale1_default_calib_weight!=knl1)
                             {
                               scale1_default_calib_weight = knl1;  
                               if(WRITE_PROTECT)EE_scale1_default_calib_weight=scale1_default_calib_weight;
                             }
                         }
                        else 
                         {
                            knl1=invert_unit(get_value_l1,select_unit_str(NULL,2));        
                            if(scale2_default_calib_weight!=knl1)
                             {
                               scale2_default_calib_weight = knl1;  
                               if(WRITE_PROTECT)EE_scale2_default_calib_weight=scale2_default_calib_weight;
                             }
                         }
                      page=22;

                     
                       sum_tempf=0;
                       average_index=0;                   
                     
                      blink_get_data_cntr=0;                             
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                      
                 } 
                else
                 if(calibration_state==9)
                 { 
                   if(get_value_l1<9)
                    {
                      if(get_value_i4==1)
                       {
                             scale2_loadcells_no=get_value_l1; 
                            if(WRITE_PROTECT)EE_scale1_loadcells_no=scale1_loadcells_no;
                       }  
                      else 
                       {
                             scale2_loadcells_no=get_value_l1; 
                            if(WRITE_PROTECT)EE_scale1_loadcells_no=scale1_loadcells_no;
                       }                      
                      
                     if(get_value_i4==1)
                            {get_value_l1 = scale1_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale1_capacity_unit);}
                     else  {get_value_l1 = scale2_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale2_capacity_unit);}                          
                                                       
                          
                     get_value_i1=6;  
                          
                     if(get_value_l1>999999){/*get_value_i1=1;*/sprintf(get_value_ch,"E");}  //overflow
                     else
                     if(get_value_l1>999)sprintf(get_value_ch,"%d%03d",(unsigned int)(get_value_l1/1000),(unsigned int)(get_value_l1%1000));  
                     else  sprintf(get_value_ch,"%d",get_value_l1);                            
                          
                     page=17;          
                     previous_page=10;
                     display_home_pos_en=1; 
                     clear_home_pos_en=1;  

                     blink_get_data_cntr=1;   
                     get_character_index=strlen(get_value_ch); 
                     get_cursor=0;                     
                                             
                       
                     calibration_state=10; 
                     
  //                   blink_get_data_cntr=0;                             
    //                 display_home_pos_en=1; 
      //               clear_home_pos_en=1;                      
                    } 
                   else
                    {
                      page=2;
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;                     
                    }
                  }
                else
                 if(calibration_state==10)
                 { 
                   
                      if(get_value_i4==1)
                       {
                          scale1_capacity[2]=  invert_unit(get_value_l1,scale1_capacity_unit);      //change units to gram
                          if(WRITE_PROTECT)EE_scale1_capacity[2]=  scale1_capacity[2]; 
                       }  
                      else 
                       {
                          scale2_capacity[2]=  invert_unit(get_value_l1,scale2_capacity_unit);      //change units to gram
                          if(WRITE_PROTECT)EE_scale2_capacity[2]=  scale2_capacity[2];                        
                       }                      

                      calibration_state=0;   
                      page=2; 


                      display_home_pos_message_code='O';  
                      all_keys_temp_disable=30;  
                      no_display_change_cntr=30;          //3 second        
                                                          
                      blink_get_data_cntr=0;                             
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;    
                      display_area_en=1;  
             
                           
                  }                    
                  
                }
            }     
           if(menu_item==ORDER_OF_APPLICATION)
            {   
             if(select_item==ORDER_OF_1STWEIGH)         
               { 
                if(sub_select_item==ORDER_OF_1ST_PRINT || sub_select_item==ORDER_OF_1ST_VIEW  )         
                 {
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i2 is 1st weigh file code no  
                    
                                      
                    if(get_value_i3>0 && get_value_i3<=NUM_OF_FIRST_WEIGHING_MEMORY) 
                     {
 //                        sub_select_item=ORDER_OF_1ST_PRINT_GET;    //  
   //                      get_value_i1=7; 
     //                    page=17;
                                               
                         read_weighing_data_eeprom(&weighing_data,get_value_i3-1);
                         
                         get_value_l1=0;      
                         get_value_l1 = weighing_data.first_weighing_value_msb;        
                         get_value_l1<<=8;
                         get_value_l1 |= weighing_data.first_weighing_value_mid;
                         get_value_l1<<=8;
                         get_value_l1 |= weighing_data.first_weighing_value_lsb;     
                         
                         temp_reg_first_weigh = get_value_l1;
    
                         get_value_l1=0;      
                         get_value_l1 = weighing_data.vehicle_no_msb;        
                         get_value_l1<<=8;
                         get_value_l1 |= weighing_data.vehicle_no_mid;
                         get_value_l1<<=8;
                         get_value_l1 |= weighing_data.vehicle_no_lsb;     
                         temp_truck_id = get_value_l1;
                         
                         year_print =  weighing_data.date_year;
                         month_print = weighing_data.date_month;
                         day_print = weighing_data.date_day;
                         hour_print = weighing_data.time_hour;
                         minute_print = weighing_data.time_minute;
                         second_print = weighing_data.time_second;    
                         
                         using_scale =  weighing_data.scale_no;
                                 
                         temp_sort_code=  weighing_data.sort_code; 
                         
                         temp_inbound_code= get_value_i3-1;                         
                                                                               
                         sprintf(temp_customer_name,"%s",weighing_data.customer_name);   
                         
                         if(sub_select_item==ORDER_OF_1ST_PRINT)  
                           {
                             print_data_block(1);
                             page=20;  
                     
                             display_home_pos_message_code='P';  
                             all_keys_temp_disable=30;  
                             no_display_change_cntr=30;          //3 second                        
                           } 
                          if(sub_select_item==ORDER_OF_1ST_VIEW)
                           {
                             page=22;
                             view_1st_weighing=1;     
                           }
                     
                    
                         display_home_pos_en=1; 
                         clear_home_pos_en=1;  
                         blink_get_data_cntr=0;                    
                     }
                    else
                     {
                        display_home_pos_message_code='N';   
                        page=20;
                        all_keys_temp_disable=30;  
                        no_display_change_cntr=30;          //3 second                                                
                        blink_get_data_cntr=0;   
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;                                    
                     }  
                  
                 
                 
                 }
                else
                if(sub_select_item==ORDER_OF_1ST_DELETE)         
                 {   
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i2 is field no
                    
                    if(get_value_i3>0 && get_value_i3<=NUM_OF_FIRST_WEIGHING_MEMORY) 
                     {
                                 weighing_data.vehicle_no_lsb = 0;
                                 weighing_data.vehicle_no_mid = 0;
                                 weighing_data.vehicle_no_msb = 0;
                                 weighing_data.first_weighing_value_lsb = 0;
                                 weighing_data.first_weighing_value_mid = 0;
                                 weighing_data.first_weighing_value_msb = 0;
                                 weighing_data.date_year=0;
                                 weighing_data.date_month=0;
                                 weighing_data.date_day=0;
                                 weighing_data.time_hour=0;
                                 weighing_data.time_minute=0;
                                 weighing_data.time_second= 0;  
                                 weighing_data.scale_no = 0;
                                 weighing_data.sort_code= 0;  
                                 weighing_data.conseq1_serial=0; 
                                 sprintf(weighing_data.customer_name,"\0");   

                                 write_weighing_data_eeprom(weighing_data,get_value_i3-1);   
                        
                                 display_home_pos_message_code='w';           //DELETE

                     }
                    else
                     {
                                 display_home_pos_message_code='N';      //CAN'T delete
                     }  
                    
                    page=20;
                    all_keys_temp_disable=10;  
                    no_display_change_cntr=10;          //3 second                                                
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                      
                 }          
                else
                if(sub_select_item==ORDER_OF_1ST_DELETEA)         
                 {   
                    
                                 weighing_data.vehicle_no_lsb = 0;
                                 weighing_data.vehicle_no_mid = 0;
                                 weighing_data.vehicle_no_msb = 0;
                                 weighing_data.first_weighing_value_lsb = 0;
                                 weighing_data.first_weighing_value_mid = 0;
                                 weighing_data.first_weighing_value_msb = 0;
                                 weighing_data.date_year=0;
                                 weighing_data.date_month=0;
                                 weighing_data.date_day=0;
                                 weighing_data.time_hour=0;
                                 weighing_data.time_minute=0;
                                 weighing_data.time_second= 0;  
                                 weighing_data.scale_no = 0;
                                 weighing_data.sort_code= 0;  
                                 weighing_data.conseq1_serial=0; 
                                 sprintf(weighing_data.customer_name,"\0");   
                                 
                                 for(kn1=0;kn1<NUM_OF_FIRST_WEIGHING_MEMORY;kn1++)
                                 {
                                    write_weighing_data_eeprom(weighing_data,kn1);  
                                    delay_ms(1);
                                    #asm("wdr")
                                 }                    
                                    
                                 display_home_pos_message_code='w';           //DELETE
                    
                   
                    page=20;
                    all_keys_temp_disable=20;  
                    no_display_change_cntr=20;          //3 second    
                    
                    write_e2prom_byte(0,WEIGHING_DATA_SAVE_INDEX);
                    write_e2prom_byte(0,CONSEQ1_H_SAVE_INDEX);  
                    write_e2prom_byte(0,CONSEQ1_L_SAVE_INDEX);                                              
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                      
                 }                                 
               }            
             else
             if(select_item==ORDER_OF_FIXEDTAR)         
               { 
                
                if(sub_select_item==ORDER_OF_FT_DELETE)         
                 {   
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i2 is field no
                    
                    if(get_value_i3>0 && get_value_i3<=FIXED_TARE_INBOUND_CODES) 
                     {
                        get_value_i3--;

                        fixed_tare[get_value_i3].fixed_tare_msb=0;        
                        fixed_tare[get_value_i3].fixed_tare_mid=0;
                        fixed_tare[get_value_i3].fixed_tare_lsb=0;    
                        
                        display_home_pos_message_code='O';           //DELETE Fixed Tare
                 
                     }
                    else
                     {
                        display_home_pos_message_code='N';      //CAN'T delete
                     }  
                    
                    page=20;
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                                                
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                         
                     
                 } 
                else    
                if(sub_select_item==ORDER_OF_FT_DELETEA)         
                 {   
                    
                    for(kn1=0;kn1<FIXED_TARE_INBOUND_CODES;kn1++)
                     {
                        fixed_tare[kn1].fixed_tare_msb=0;        
                        fixed_tare[kn1].fixed_tare_mid=0;
                        fixed_tare[kn1].fixed_tare_lsb=0;   
                        delay_ms(1);
                        #asm("WDR")                      
                     }

                    EE_fixed_tare_save_index=0xFF;
                        
                    display_home_pos_message_code='O';           //DELETE Fixed Tare
                 

                    page=20;
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                                                
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                         
                     
                 } 
                else                                            
                if(sub_select_item==ORDER_OF_FT_EDIT)         
                 {   
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i2 is fixed tare no
                    
                    if(get_value_i3>0 && get_value_i3<=FIXED_TARE_INBOUND_CODES) 
                     {
                         sub_select_item=ORDER_OF_FT_EDIT_GET;  
                         get_value_i1=7; 
                         page=17;

                         get_value_l1=0;      
                         get_value_l1 = fixed_tare[get_value_i3-1].fixed_tare_msb;        
                         get_value_l1<<=8;
                         get_value_l1 |= fixed_tare[get_value_i3-1].fixed_tare_mid;
                         get_value_l1<<=8;
                         get_value_l1 |= fixed_tare[get_value_i3-1].fixed_tare_lsb;  
                         
                         if(scales_func_mode==2)get_value_l1 = convert_unit( get_value_l1 , select_unit_str(NULL,2));                       
                         else get_value_l1 = convert_unit( get_value_l1 , select_unit_str(NULL,1));                       
                         
                      
                         printf_longnum(get_value_ch,get_value_l1,7);
                         //sprintf(get_value_ch,"%d",get_value_l1); 
                         blink_get_data_cntr=1;   
                         get_character_index=strlen(get_value_ch); 
                         get_cursor=0;       
                         display_home_pos_en=1; 
                         clear_home_pos_en=1;                                                           
                     }
                    else
                     {
                        display_home_pos_message_code='N';   
                        page=20;
                        all_keys_temp_disable=30;  
                        no_display_change_cntr=30;          //3 second                                                
                        blink_get_data_cntr=0;   
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;                                    
                     }  

                     
                 } 
                else 
                if(sub_select_item==ORDER_OF_FT_EDIT_GET)         
                 {   
                    
                    if(scales_func_mode==2)get_value_l1 = invert_unit( get_value_l1 , select_unit_str(NULL,2));  
                    else get_value_l1 = invert_unit( get_value_l1 , select_unit_str(NULL,1));  
                    
                    fixed_tare[get_value_i3-1].fixed_tare_lsb = get_value_l1;
                    fixed_tare[get_value_i3-1].fixed_tare_mid = (unsigned char)(get_value_l1>>8);
                    fixed_tare[get_value_i3-1].fixed_tare_msb = (unsigned char)(get_value_l1>>16);                      
                     
                    page=20;  
                    sub_select_item=ORDER_OF_FT_EDIT;
                    
                    display_home_pos_message_code='O';  
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                        
                    
                    
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;  
                    blink_get_data_cntr=0;   
                   
                 } 
                else
                if(sub_select_item==ORDER_OF_FT_CREATN)         
                 {   
                    
                    if(scales_func_mode==2)knl1 = invert_unit(get_value_l1,select_unit_str(NULL,2)); 
                    else knl1 = invert_unit(get_value_l1,select_unit_str(NULL,1));    
     
                    fixed_tare[get_value_i3-1].fixed_tare_lsb = knl1;
                    fixed_tare[get_value_i3-1].fixed_tare_mid = (unsigned char)(knl1>>8);
                    fixed_tare[get_value_i3-1].fixed_tare_msb = (unsigned char)(knl1>>16);    
                    
                    EE_fixed_tare_save_index=get_value_i3;
                    
                    page=20;  
                    display_home_pos_message_code='O';  
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                                                  

                    display_home_pos_en=1; 
                    clear_home_pos_en=1; 

                    blink_get_data_cntr=0;   
                   
                 }    
                             
               }
               
             else
             if(select_item==ORDER_OF_SORTFILE)         
               {                   

                
                if(sub_select_item==ORDER_OF_SF_DELETE)         
                 {     
                    get_value_i3=(unsigned int)(get_value_l1);    //get_value_i1 is field no
                    
                    if(get_value_i3>0 && get_value_i3<=NUM_OF_SORT_NAMES) 
                     {
                        get_value_i3--;    
                                                                                     
                        sprintf(get_value_ch,"%c               ",0xFF);
                        
                        write_sort_name(get_value_ch,get_value_i3);
                        
                        display_home_pos_message_code='O';          
                 
                     }
                    else display_home_pos_message_code='N';      //CAN'T delete
                   
                    page=20;
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                                                
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                      
                 }     
                else     
                if(sub_select_item==ORDER_OF_SF_DELETEA)         
                 {     
                    get_value_i3=(unsigned int)(get_value_l1);    //DELETE ALL SORT FILES 
                    sprintf(get_value_ch,"%c               ",0xFF);
                    for(kn1=0;kn1<NUM_OF_SORT_NAMES;kn1++)
                     { 
                        write_sort_name(get_value_ch,kn1);      
                        delay_ms(1);
                        #asm("wdr")
                     }                         
                    display_home_pos_message_code='O';   
                   
                    page=20;
                    all_keys_temp_disable=30;  
                    no_display_change_cntr=30;          //3 second                                                
                        
                    blink_get_data_cntr=0;   
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                      
                 }     
                else                                             
                if(sub_select_item==ORDER_OF_SF_EDIT)         
                 {   
                    get_value_i1=(unsigned int)(get_value_l1);    //get_value_i2 is fixed tare no      
                    if(get_value_i1>0 && get_value_i1<=NUM_OF_SORT_NAMES) 
                     {                    
                            //sub_select_item=ORDER_OF_SF_EDIT; 
                            page=19;  
                            display_home_pos_en=1; 
                            clear_home_pos_en=1; 
                            previous_page=20;  
                      
                            get_character_index=1;  
                            blink_get_data_cntr=1;  
                            
                            read_sort_name(get_value_ch,get_value_i1-1);    
                            

                            get_cursor=strlen(get_value_ch);                        

                     }
                   else
                    {
                        display_home_pos_message_code='N';   
                        page=20;
                        all_keys_temp_disable=30;  
                        no_display_change_cntr=30;          //3 second                                                
                        blink_get_data_cntr=0;   
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;                        
                    
                    } 
                    
                 }                  
                
                
                
                
                }             
               
            }   
           

        //   display_area_en=1; 
        //   display_weight_en=1; 
        }            
                                                        
     }          //end page==17          
     
     else
    if(page==18)      
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;  
          select_item=1;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }  
      else   
      if(key_code==DOWN_KEY)     
        {  
          if(sub_select_item==ORDER_OF_ATTR){if(get_value_i1)get_value_i1--; else get_value_i1=7;}
          if(sub_select_item==ORDER_OF_TYPE){if(get_value_i6)get_value_i6--; else get_value_i6=3;}
          if(sub_select_item==ORDER_OF_TEXT)
            {
              if(get_value_i6==0)     //fetch
                {if(get_value_i7)get_value_i7--; else get_value_i7=NUMBER_OF_FETCH_TEXTS-1;}
              if(get_value_i6==3)     //text
                {if(get_value_i7)get_value_i7--; else get_value_i7=NUMBER_OF_EMBED_TEXTS+NUMBER_OF_USER_TEXTS-1;}
            }        
         // display_home_pos_en=1;          
         blink_get_data_cntr=BLINK_ON_TIME;
        }           
      else 
      if(key_code==UP_KEY)     
        {  
          if(sub_select_item==ORDER_OF_ATTR){if(++get_value_i1>7)get_value_i1=0;}
          if(sub_select_item==ORDER_OF_TYPE){if(++get_value_i6>3)get_value_i6=0;} 
          if(sub_select_item==ORDER_OF_TEXT)
            {
              if(get_value_i6==0)     //fetch
                {if(++get_value_i7>=NUMBER_OF_FETCH_TEXTS)get_value_i7=0;}
              if(get_value_i6==3)     //text
                {if(++get_value_i7>=(NUMBER_OF_EMBED_TEXTS+NUMBER_OF_USER_TEXTS))get_value_i7=0;}
            } 
         // display_home_pos_en=1;   
         blink_get_data_cntr=BLINK_ON_TIME;
        }    
      else             
      if(key_code==CLEAR_KEY)      //clear a character
        {      

          display_home_pos_en=1;  
        }          
      if(key_code==ENTER_KEY)     
        { 
             
          if(sub_select_item==ORDER_OF_ATTR)
            {
             sub_select_item=ORDER_OF_TYPE;    
             get_value_i6 = field[get_value_i2].type;  
             clear_home_pos_en=1; 
             display_home_pos_en=1;      
             blink_get_data_cntr=BLINK_ON_TIME;    
            }   
          else  
          if(sub_select_item==ORDER_OF_TYPE)
            { 
             if(get_value_i6==0)   //fetch is selected
               {
                 get_value_i7 = field[get_value_i2].data_index; 
                 if(get_value_i7>=NUMBER_OF_FETCH_TEXTS)get_value_i7=0;
               }
             if(get_value_i6==3)   //text entry
               {
                 get_value_i7 = field[get_value_i2].data_index; 
                 if(get_value_i7>= (NUMBER_OF_EMBED_TEXTS+NUMBER_OF_USER_TEXTS))get_value_i7=0;
               }               
             
             sub_select_item=ORDER_OF_TEXT;    
             clear_home_pos_en=1; 
             display_home_pos_en=1;   
            // blink_get_data_cntr=0;  //no blink
            blink_get_data_cntr=BLINK_ON_TIME;          
            } 
          else  
          if(sub_select_item==ORDER_OF_TEXT)
            {               
              field[get_value_i2].block=get_value_i3;                                         //save all data here
              field[get_value_i2].line=get_value_i4;
              field[get_value_i2].column=get_value_i5;
              field[get_value_i2].attribute=get_value_i1;
              field[get_value_i2].type=get_value_i6;  
              field[get_value_i2].data_index=get_value_i7;   
              
              sort_print_data();          
            
              display_home_pos_message_code='O';  
              
              blink_get_data_cntr=0; 
              clear_home_pos_en=1; 
              all_keys_temp_disable=10;  
              no_display_change_cntr=10;          //3 second  
                
              page=2;
              select_item=1;
              clear_home_pos_en=1;  
              blink_get_data_cntr=0;                             
           
              display_home_pos_en=1; 
             
           }
        }            
     
     }          //end page==18
     
   else
    if(page==19)         //Enter text here
     {
      if(key_code==UPARROW_KEY)
        {
          page=previous_page;
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
     
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        { 
           if(key_code==get_key_character || (get_key_character==0xFF))   //press same key  or if overwrite on previous
             {
               if(get_key_character==0xFF)
                    {get_key_character=key_code;
                     get_character_index=1;}
               else  get_character_index++;   
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);
               if(kn1==0)get_character_index=0;
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);  
               get_value_ch[get_cursor]=kn1;
               get_value_ch[get_cursor+1]=0;///////*******
               display_home_pos_en=1;  
               page_19_next_letter_cntr=30;
               
             } 
           else
             {
               if(get_cursor<(USER_TEXTS_LENGHT-2))get_cursor++;
               kn1=*(key_character_table_n_address[key_code-'0']+1);  
               get_value_ch[get_cursor]=kn1;               
               get_value_ch[get_cursor+1]=0;
               get_key_character=key_code;   
               
               get_character_index=1; 
               display_home_pos_en=1;   
               blink_get_data_cntr=1;  
               
             }  
        }    
      if(key_code==CLEAR_KEY)      //clear a character
        { 
           get_value_ch[get_cursor]=0; 
           if(get_cursor>0)
            {              
              get_cursor--; 
              get_key_character=0xFF; // start from NUMBER and overwrite on previous
            } 
           get_value_ch[get_cursor]=' ';  
           display_home_pos_en=1;   
           page_19_next_letter_cntr=0;
        }     
             
      if(key_code==ENTER_KEY)      //accept letter and go to receive next letter
        { 
          if(get_value_ch[get_cursor]==' ')get_value_ch[get_cursor]=0; 
          page_19_next_letter_cntr=0; 

          blink_get_data_cntr=0; 
          
          get_character_index=1; 
          display_home_pos_en=1;   
          clear_home_pos_en=1;     
          
          if(menu_item==ORDER_OF_FORMAT)
             {
               if(select_item==ORDER_OF_INPUTX)          
                {
                     kn2=get_value_ch[0];  
                     for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++)
                      {  
                         *(user_defines_texts_table[get_value_i1]+kn1)=kn2;
                         if(kn2!=0)kn2=get_value_ch[kn1+1];   
                      } 
                     *(user_defines_texts_table[get_value_i1]+USER_TEXTS_LENGHT)=0;
                  
                       for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++)get_value_ch[kn1]=0;                   
                            
                      page=2;  
                }
             }   
          else
          if(menu_item==ORDER_OF_APPLICATION)
              {
                if(select_item==ORDER_OF_SORTFILE)  
                   {
                   
                     write_sort_name(get_value_ch,get_value_i1-1);
                     
                     page=previous_page;
                   }
              
              
              }
          
          
              
           
        }                    
                     
     
     }   //end page==19
     
  else
    if(page==20)         //internal menus of application:
     {
      if(key_code==UPARROW_KEY)
        {
          page=2;
          display_home_pos_en=1; 
          clear_home_pos_en=1;       
        }
      else   
      if(key_code==RIGHT_KEY)      //next menu item here is '6'
        { 
          if(++sub_select_item>no_of_items_in_app_sub_menus[select_item-1])sub_select_item=1;    
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }      
      else   
      if(key_code==LEFT_KEY)      //previous menu item here is '4'
        { 
          if(--sub_select_item<1)sub_select_item=no_of_items_in_app_sub_menus[select_item-1];
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        } 
      else    
      if(key_code==ENTER_KEY)      //select a menu
        {                                                                
           if(select_item==ORDER_OF_1STWEIGH)              
                 {   
                   if(sub_select_item==ORDER_OF_1ST_PRINT)                   
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                      
                     }                 
                   else
                   if(sub_select_item==ORDER_OF_1ST_VIEW)                   
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                  
                     }
                   else
                   if(sub_select_item==ORDER_OF_1ST_DELETE)                   
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                                          
                           
                     } 
                   else
                   if(sub_select_item==ORDER_OF_1ST_DELETEA)                   
                     {
                        page=17;
                        previous_page=20;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        sprintf(get_value_ch,"%c",0); 
                        get_value_i1=0;               //number of digits  
                        get_value_i2=0;   //field no in i2
                        blink_get_data_cntr=0;   
                        get_character_index=0;  
                        get_cursor=0;                                         
                     }                          
                                                     
                 } 
           else      
           if(select_item==ORDER_OF_FIXEDTAR)              
                 {   
                   if(sub_select_item==ORDER_OF_FT_PRINT)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;     
                        print_fixed_tare();
                     }                 
                   else
                   if(sub_select_item==ORDER_OF_FT_EDIT)                  
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits

                     }
                   else
                   if(sub_select_item==ORDER_OF_FT_DELETE)                   
                     {

                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                                          
         
                     }  
                   else
                   if(sub_select_item==ORDER_OF_FT_CREATN)                   
                     {
                        
                        page=17;             
                        previous_page=20;
                        
                        kn1=EE_fixed_tare_save_index;  
                        
                        if(++kn1>=FIXED_TARE_INBOUND_CODES)kn1=1; 

                        get_value_i3=kn1;
                        
                        get_value_l1=0;      
                        sprintf(get_value_ch,"0        "); 
                                    
                        
                        blink_get_data_cntr=1;   
                        get_character_index=0;  
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;   
                        get_value_i1=7;             //7 digits               
                     }   
                   else
                   if(sub_select_item==ORDER_OF_FT_DELETEA)                   
                     {
                        page=17;
                        previous_page=20;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        sprintf(get_value_ch,"%c",0); 
                        get_value_i1=0;               //number of digits  
                        get_value_i2=0;   //field no in i2
                        blink_get_data_cntr=0;   
                        get_character_index=0;  
                        get_cursor=0;                              
                     }                                             
                                                     
                 }      
           else
           if(select_item==ORDER_OF_MEMCHECK)              
                 {   
                   if(sub_select_item==ORDER_OF_MC_PRINTO)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;                      
                     }                 
                   else
                   if(sub_select_item==ORDER_OF_MC_PRINTA)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;                     
                     }
                   else
                   if(sub_select_item==ORDER_OF_MC_DELETEO)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;                  
                     }  
                   else
                   if(sub_select_item==ORDER_OF_MC_DELETEA)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;                  
                     }                         
                                                     
                 }   
           else
           if(select_item==ORDER_OF_SORTFILE)              
                 {   
                   if(sub_select_item==ORDER_OF_SF_PRINT)                   
                     {
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;               
                        print_sort_file();       
                     }                 
                   else
                   if(sub_select_item==ORDER_OF_SF_EDIT)                   
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                          
                     }
                   else
                   if(sub_select_item==ORDER_OF_SF_DELETE)                   
                     {
                        previous_page=20;
                        page=17;

                        get_value_i2=0;     
                        sprintf(get_value_ch,"0        ");  
                                                            
                        get_character_index=0;  
                        blink_get_data_cntr=1;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        get_cursor=0;    
                        get_value_i1=3;   //3 digits                 
                     }  
                   else
                   if(sub_select_item==ORDER_OF_SF_DELETEA)                   
                     {
                        page=17;
                        previous_page=20;
                        display_home_pos_en=1; 
                        clear_home_pos_en=1;  
                        sprintf(get_value_ch,"%c",0); 
                        get_value_i1=0;               //number of digits  
                        get_value_i2=0;   //field no in i2
                        blink_get_data_cntr=0;   
                        get_character_index=0;  
                        get_cursor=0;                              
                     }                                               
                                                     
                 }                                                
              

       }
    }    //end of page===20    
    
  else
    if(page==21)        //ENTER a 5 digit number as Truck ID  or  a 3 digit number as sortcode  or inbound code or fixed tare code
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
         // if(menu_item==ORDER_OF_SETPOINT)select_item=1;    //ORDER_OF_TARE order of Tare menus
          page=0;
          display_home_pos_en=1; 
          clear_home_pos_en=1;   
          first_weighing_state=0;
          second_weighing_state=0;  
          basic_weighing_state=0;
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
           sprintf(get_value_ch,"0     ");  
           get_character_index=0;  
           display_home_pos_en=1; 
        }                    
      else   
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        { 
           if(get_character_index==get_value_i1)get_character_index=get_value_i1-1;
           get_value_ch[get_character_index]=key_code;  
           if(get_character_index<get_value_i1)get_character_index++;
           display_home_pos_en=1;            
        
        }
      else          
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

           for(kn1=0;kn1<get_value_i1+1;kn1++)if(get_value_ch[kn1]==' ')get_value_ch[kn1]=0;    
           get_value_ch[get_value_i1]=0;  
           
           kn2=strlen(get_value_ch);
           get_value_l2=1;  
           get_value_l1=0;
           
           for(kn1=1;kn1<=kn2;kn1++)            //convert string to a long int NUMBER
            {
              get_value_l3  = get_value_ch[kn2-kn1]-'0';
              get_value_l3 *= get_value_l2;
              get_value_l1 += get_value_l3;
              get_value_l2 *= 10;
            }
            
            
           if(menu_item==1)   //enter truck ID is now completed         
            {

             if(get_value_l1==0)    //enter an invalid value and must exit completely
                {
                   first_weighing_state=0;
                   second_weighing_state=0;  
                   basic_weighing_state=0;
                   page=0; 
                }
             else
                {
                     
                     if(first_weighing_state==1)
                       {
                                temp_truck_id=  get_value_l1;       //save value
                                first_weighing_state=2;     //2 means that WAIT to complete weighing then CONTINUE  
                                                                //**** must be continue with inbound code show, save , print and etc. 
                                                                 //when weighing completed 
                                page=23;                     //Get customer name
                               
                                get_character_index=1;  
                                blink_get_data_cntr=1;  

                                for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++){get_value_ch[kn1]=0;temp_customer_name[kn1]=0;}
                                get_value_ch[0]='0';
                                temp_customer_name[0]='0'; 
                                get_key_character=0xFF;  
                                get_cursor=0;
                     
                       }   
                     else
                     if(second_weighing_state==1)
                       {
                                 //here entered TruckID must be checked in memory
                                kn1=check_ID_truck_in_memory(get_value_l1,read_e2prom_byte(WEIGHING_DATA_SAVE_INDEX));
                                 if(kn1!=0xFF)          //Find 
                                  {
                                         second_weighing_state=2;     //>1 means that WAIT to complete weighing then CONTINUE    
                                         temp_inbound_code=kn1; 
                                         temp_truck_id =get_value_l1;       //save value                                                            
                                         page=22;   
                                    
                                         read_weighing_data_eeprom(&weighing_data,kn1);    
                 
                                         temp_reg_first_weigh=0;      
                                         temp_reg_first_weigh = weighing_data.first_weighing_value_msb;        
                                         temp_reg_first_weigh<<=8;
                                         temp_reg_first_weigh |= weighing_data.first_weighing_value_mid;
                                         temp_reg_first_weigh<<=8;
                                         temp_reg_first_weigh |= weighing_data.first_weighing_value_lsb;       
                         
                                         temp_sort_code = weighing_data.sort_code;
                         
                                         sprintf(temp_customer_name,"%s",weighing_data.customer_name);                                    
                                  }
                                 else
                                  {
                                    page=0;  
                                    all_keys_temp_disable=30;  
                                    display_home_pos_message_code='f';  //woring TruckID message is Displayed     
                                    no_display_change_cntr=30;          //3 second                                    
                                  } 

                       }  
                                       
                }

             blink_get_data_cntr=0;                             
             display_home_pos_en=1; 
             clear_home_pos_en=1;                               
            }     
           else
           if(menu_item==2)   //enter Inbound_Code is now completed         
            {

             if(get_value_l1==0)    //inbound code is not available 
               {                                                                  //entered number is inavalid
                 menu_item=1;           //Enter TruckID
                 sprintf(get_value_ch,"0    ",); 
                 get_value_i1=5;               //number of digits  
                 blink_get_data_cntr=1;   
                 get_character_index=0;
                 get_cursor=0;   
               }
             else                             
               {
                 blink_get_data_cntr=0;   
                 
                 get_value_l1--;                                        //conv to index of saved items   
                 
                 read_weighing_data_eeprom(&weighing_data,get_value_l1);    

                 kn1=weighing_data.vehicle_no_msb;
                 kn2=weighing_data.vehicle_no_mid;
                 kn3=weighing_data.vehicle_no_lsb;
                 
                 
                 if((kn1==0xFF && kn2==0xFF && kn3==0xFF) || get_value_l1>NUM_OF_FIRST_WEIGHING_MEMORY)
                     {                                                  //empty field in memory is invalid
                              page=0;   
                              blink_get_data_cntr=0;
                              all_keys_temp_disable=30;  
                              display_home_pos_message_code='b';  //woring InboundCode message is Displayed     
                              no_display_change_cntr=30;          //3 second  
                              first_weighing_state=0;
                              second_weighing_state=0;  
                              basic_weighing_state=0;                                                 
                     }
                 else
                     {          //valid inbound code  and load data in temp_ variable
                 
                         temp_inbound_code= get_value_l1;
                         page=22;      
                         second_weighing_state=2;     //>1 means that WAIT to complete weighing then CONTINUE 
                 
                         temp_truck_id=0;       //extract data from EEPROM and make them ready to Display and be confirmed HERE
                         temp_truck_id = kn1;        
                         temp_truck_id<<=8;
                         temp_truck_id |= kn2;
                         temp_truck_id<<=8;
                         temp_truck_id |= kn3;        
                 
                         temp_reg_first_weigh=0;      
                         temp_reg_first_weigh = weighing_data.first_weighing_value_msb;        
                         temp_reg_first_weigh<<=8;
                         temp_reg_first_weigh |= weighing_data.first_weighing_value_mid;
                         temp_reg_first_weigh<<=8;
                         temp_reg_first_weigh |= weighing_data.first_weighing_value_lsb;       
                         
                         temp_sort_code = weighing_data.sort_code;
                         
                         sprintf(temp_customer_name,"%s",weighing_data.customer_name);
                                   
                     }
                          
               }    

             display_home_pos_en=1; 
             clear_home_pos_en=1;                 
               
            }    
           else
           if(menu_item==3)   //enter Sort Code is now completed         
            {
              
             if(first_weighing_state==3)  
              {
                first_weighing_state=4;          
                second_weighing_state=0;  
                basic_weighing_state=0;
                temp_sort_code = get_value_l1; 
                page=22;    
              }   
            /*  
             if(second_weighing_state==4)  
              {
                first_weighing_state=0;          
                second_weighing_state=5;   
                basic_weighing_state=0;
                temp_sort_code = get_value_l1; 
                page=22;    
              }    */
                
             if(basic_weighing_state==3)                          
              {
                first_weighing_state=0;          
                second_weighing_state=0;   
                basic_weighing_state=4;              
                temp_sort_code = get_value_l1; 
                page=22;
              } 
              
                display_home_pos_en=1; 
                clear_home_pos_en=1;                 
               
            }  
           else
           if(menu_item==4)   //enter Fixed-Tare is now completed         
            {

             if(get_value_l1==0)    //inbound code is not available 
               {                                                                  //entered number is inavalid
                 menu_item=5;           //Enter 1st Weigh MANUALLY
                 sprintf(get_value_ch,"0      "); 
                 get_value_i1=7;                 //number of digits  
                 blink_get_data_cntr=1;   
                 get_character_index=0;
                 get_cursor=0;  
                 temp_inbound_code=0; 
               }
             else                             
               {
                 blink_get_data_cntr=0;   
                 
                 get_value_l1--;                                        //conv to index of saved items         
                 
                 kn1=fixed_tare[get_value_l1].fixed_tare_msb;
                 kn2=fixed_tare[get_value_l1].fixed_tare_mid;
                 kn3=fixed_tare[get_value_l1].fixed_tare_lsb;
                 
                 
                 if((kn1==0xFF && kn2==0xFF && kn3==0xFF) || get_value_l1>FIXED_TARE_INBOUND_CODES)
                     {                                                  //empty field in memory is invalid
                              page=0;   
                              blink_get_data_cntr=0;
                              all_keys_temp_disable=30;  
                              display_home_pos_message_code='x';  //woring FixedTare Code message is Displayed     
                              no_display_change_cntr=30;          //3 second                     
                              first_weighing_state=0;
                              second_weighing_state=0;  
                              basic_weighing_state=0;   
                     }
                 else
                     {         
                 
                         page=22;      
                         basic_weighing_state=2;         //>1 means that WAIT to complete weighing then CONTINUE 
                         
                         temp_inbound_code= get_value_l1+1;
                         
                         temp_reg_first_weigh=0;      
                         temp_reg_first_weigh = fixed_tare[get_value_l1].fixed_tare_msb;        
                         temp_reg_first_weigh<<=8;
                         temp_reg_first_weigh |= fixed_tare[get_value_l1].fixed_tare_mid;
                         temp_reg_first_weigh<<=8;
                         temp_reg_first_weigh |= fixed_tare[get_value_l1].fixed_tare_lsb;                     
                     }
                          
               }    

             display_home_pos_en=1; 
             clear_home_pos_en=1;                 
               
            }       
           else
           if(menu_item==5)   //enter Manual 1st Weighing is now completed         
            {
                page=23;     
                
                get_character_index=1;  
                blink_get_data_cntr=1;  

                for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++){get_value_ch[kn1]=0;temp_customer_name[kn1]=0;}
                get_value_ch[0]='0';
                temp_customer_name[0]='0'; 
                get_key_character=0xFF;  
                get_cursor=0;
                                                 
                basic_weighing_state=2;         //>1 means that WAIT to complete weighing then CONTINUE 

//                if(using_scale==1)temp_reg_first_weigh = invert_unit(get_value_l1,scale1_unit);
  //              if(using_scale==2)temp_reg_first_weigh = invert_unit(get_value_l1,scale2_unit);
                
                temp_reg_first_weigh = invert_unit(get_value_l1,get_value_i3);    //get_value_i2 is using unit

                display_home_pos_en=1; 
                clear_home_pos_en=1;                 
               
            }                                                               
     
       }
     
    }   //page==21
     
   else
    if(page==22)        //SHOW W1 DATA and waiting to verify it by OPERATOR
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
         if(calibration_state) 
           {
              page=previous_page;
              kn3=0;
              for(kn1=0;kn1<10;kn1++)
                     {
                       if(get_value_i4==1)kn2=EE_scale1_load_mv_v_str[kn1]; 
                         else kn2=EE_scale2_load_mv_v_str[kn1]; 
                       get_value_ch[kn1]=kn2;  
                       if(kn3==0xFF)get_value_ch[kn1]=0; else get_value_ch[kn1]=kn2;  
                       if(kn2==0)kn3=0xFF;
                       if(kn2=='.' && get_value_i2==0)get_value_i2=kn1;
                     }                
             if(get_value_i4==1)scale1_load_mv_v = convert_string_to_float(get_value_ch);  
              else scale2_load_mv_v = convert_string_to_float(get_value_ch); 
           }
         else   
         if(view_1st_weighing) 
           {
             page=17;
           
           }         
         else
         page=0;     
         
          second_weighing_state=0;
          first_weighing_state=0;
          basic_weighing_state=0;
          calibration_state=0; 
          
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;    
          
        }
      else  
      if(key_code==CLEAR_KEY)     //QUIT
        {
          if(view_1st_weighing)
           {
             view_1st_weighing=0;
             page=previous_page;
           }
           
          if(calibration_state==1)
           { 
              calibration_state=3;      //skip calculate zero_mv_v  
              page=25; 
              previous_page=2;
              display_home_pos_en=1; 
              clear_home_pos_en=1;    
              blink_get_data_cntr=1; 
              
              get_value_i2=0;   
              kn3=0;
              for(kn1=0;kn1<10;kn1++)
                 {
                   if(get_value_i4==1)kn2=EE_scale1_zero_mv_v_str[kn1]; 
                     else  kn2=EE_scale2_zero_mv_v_str[kn1]; 
                   if(kn3==0xFF)get_value_ch[kn1]=0; else get_value_ch[kn1]=kn2;  
                   if(kn2==0)kn3=0xFF;
                   if(kn2=='.' && get_value_i2==0)get_value_i2=kn1;
                 }
              get_value_i1=7;
              get_value_i3=0;               //use for edited_value flag in page 25
                  
              blink_get_data_cntr=1;   
              get_character_index=strlen(get_value_ch); 
              get_cursor=0;                
    
           }  
          if(calibration_state==4)       
           {         
              calibration_state=8;
              page=25;   
            
//            calibration_state=7;
              
              previous_page=2;
              previous_cal_state=4;
              display_home_pos_en=1; 
              clear_home_pos_en=1;     
              
                  if(get_value_i4==1)load_float_in_string(scale1_load_mv_v+0.000005,get_value_ch);
                  else load_float_in_string(scale2_load_mv_v+0.000005,get_value_ch);              
              
              get_value_i1=7;
              get_value_i2=0;  
              get_value_i3=0;               //use for edited_value flag in page 25
                  
              blink_get_data_cntr=1;   
              get_character_index=strlen(get_value_ch); 
              get_cursor=0;                             
              
           }    
          else
          if(calibration_state==7) 
            {
                       calibration_state=6;
                       calibration_state_counter=30;                       
                       sum_tempf=0;
                       average_index=0;                   
                       blink_get_data_cntr=0;                             
                       display_home_pos_en=1; 
                       clear_home_pos_en=1;
                       if(get_value_i4==1)
                          {
                            for(kn1=0;kn1<10;kn1++)get_value_ch[kn1]=EE_scale1_load_mv_v_str[kn1];
                            scale1_load_mv_v = convert_string_to_float(get_value_ch);  
                          }
                       else
                          {
                            for(kn1=0;kn1<10;kn1++)get_value_ch[kn1]=EE_scale2_load_mv_v_str[kn1];
                            scale2_load_mv_v = convert_string_to_float(get_value_ch);  
                          }                       
            
            }   
          else  
          if(second_weighing_state==0 && first_weighing_state==5)
           { 
              first_weighing_state=6;
              second_weighing_state=0;     
              
              display_home_pos_en=1; 
              clear_home_pos_en=1;              
              
                                 /**** save all value and give an INBOUND CODE*****/    
                                 
                                /* kn1 = read_e2prom_byte(WEIGHING_DATA_SAVE_INDEX);  
                                 kn1++;                    ///must be saved later originally       
                                 if(kn1>=NUM_OF_FIRST_WEIGHING_MEMORY)kn1=0;
                                 write_e2prom_byte(kn1,WEIGHING_DATA_SAVE_INDEX);   
                                 temp_inbound_code= kn1;  */
                                 
                                 kn1 = read_e2prom_byte(WEIGHING_DATA_SAVE_INDEX);     
                                 temp_inbound_code= kn1;   
                                 kn1++;                    ///must be saved later originally       
                                 if(kn1>=NUM_OF_FIRST_WEIGHING_MEMORY)kn1=0;
                                 write_e2prom_byte(kn1,WEIGHING_DATA_SAVE_INDEX);   
                                    
                                 
                                 
                                 get_value_l1 = temp_truck_id;
                                 weighing_data.vehicle_no_lsb = get_value_l1;
                                 weighing_data.vehicle_no_mid = (unsigned char)(get_value_l1>>8);
                                 weighing_data.vehicle_no_msb = (unsigned char)(get_value_l1>>16);  
                                 
                                 
                                 //1st weight is saved here
                                 weighing_data.first_weighing_value_lsb = temp_reg_first_weigh;
                                 weighing_data.first_weighing_value_mid = (unsigned char)(temp_reg_first_weigh>>8);
                                 weighing_data.first_weighing_value_msb = (unsigned char)(temp_reg_first_weigh>>16);                                   
                                 
                                 weighing_data.date_year=year;
                                 weighing_data.date_month=month;
                                 weighing_data.date_day=day;
                                 weighing_data.time_hour=hour;
                                 weighing_data.time_minute=minute;
                                 weighing_data.time_second= second;  
                                 weighing_data.scale_no = using_scale;
                                   
                                 
                                 weighing_data.sort_code= temp_sort_code;   
                                 
                                 sprintf(weighing_data.customer_name,"%s",temp_customer_name);   
                                 
                                 knl1=read_e2prom_byte(CONSEQ1_H_SAVE_INDEX); 
                                 knl1 *= 0x100;
                                 knl1+=read_e2prom_byte(CONSEQ1_L_SAVE_INDEX);        
                                 knl1++;
                                 
                                 weighing_data.conseq1_serial = knl1;    
                                 
                                 write_e2prom_byte((unsigned char)(knl1),CONSEQ1_L_SAVE_INDEX);                     
                                 write_e2prom_byte((unsigned char)(knl1>>8),CONSEQ1_H_SAVE_INDEX);                     
                                 
                                 
                                 write_weighing_data_eeprom(weighing_data,temp_inbound_code);   
                                 
                                 //print All saved data in Block1 Here   
//                                 year_print=year;month_print=month;day=day_print;
  //                               hour=hour_print;minute=minute_print;second=second_print;
    //                             print_data_block(1);
                                 
   
           }              
          
                         
        }
      else                 
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

          if(calibration_state==1)
           { 
             calibration_state=2;            //calibration_state->2 means calcuating ZERO mV/V parameters  
             calibration_state_counter=30;   //3 second for complete calibration
             
             sum_tempf=0;
             average_index=0;
           } 
          else 
          if(calibration_state==4)
           {
             calibration_state=5;  
                    
             page=17; 
             previous_page=2;
             display_home_pos_en=1; 
             clear_home_pos_en=1;              

             if(get_value_i4==1)get_value_l1=convert_unit(scale1_default_calib_weight,select_unit_str(NULL,1));
              else get_value_l1=convert_unit(scale2_default_calib_weight,select_unit_str(NULL,2));
              
             printf_longnum(get_value_ch,get_value_l1,7);    
              
             get_value_i1=7;               //number of digits
                  
             blink_get_data_cntr=1;   
             get_character_index=strlen(get_value_ch); 
             get_cursor=0;                 
           } 
          else     
          if(calibration_state==7)
            { 
              calibration_state=8;
              page=25;    
              previous_page=2;
              previous_cal_state=7;
              display_home_pos_en=1; 
              clear_home_pos_en=1;    
              
              if(get_value_i4==1)load_float_in_string(scale1_load_mv_v+0.000005,get_value_ch);
              else load_float_in_string(scale2_load_mv_v+0.000005,get_value_ch);    
                  
              get_value_i1=7;//strlen(get_value_ch);               //number of digits
              get_value_i2=0;  
              get_value_i3=0;               //use for edited_value flag in page 25
                  
              blink_get_data_cntr=1;   
              get_character_index=strlen(get_value_ch); 
              get_cursor=0;                           
             }     
                           
        
          if(second_weighing_state==0 && first_weighing_state==4)
           { 
              first_weighing_state=5; 
              second_weighing_state=0;
           } 
          else
          if(second_weighing_state==0 && first_weighing_state==5)
           { 
              first_weighing_state=6;
              second_weighing_state=0; 
              
                                 /**** save all value and give an INBOUND CODE*****/    
                                 /*
                                 kn1 = read_e2prom_byte(WEIGHING_DATA_SAVE_INDEX);  
                                 kn1++;                    ///must be saved later originally       
                                 if(kn1>=NUM_OF_FIRST_WEIGHING_MEMORY)kn1=0;
                                 write_e2prom_byte(kn1,WEIGHING_DATA_SAVE_INDEX); 
                                 temp_inbound_code = kn1;  */     
                                 
                                 kn1 = read_e2prom_byte(WEIGHING_DATA_SAVE_INDEX);  
                                 temp_inbound_code=kn1;
                                 kn1++;                    ///must be saved later originally       
                                 if(kn1>=NUM_OF_FIRST_WEIGHING_MEMORY)kn1=0;
                                 write_e2prom_byte(kn1,WEIGHING_DATA_SAVE_INDEX); 
                                 
                                 
                                 
                                 get_value_l1 = temp_truck_id;
                                 weighing_data.vehicle_no_lsb = get_value_l1;
                                 weighing_data.vehicle_no_mid = (unsigned char)(get_value_l1>>8);
                                 weighing_data.vehicle_no_msb = (unsigned char)(get_value_l1>>16);  
                                 
                                 
                                 //1st weight is saved here
                                 weighing_data.first_weighing_value_lsb = temp_reg_first_weigh;
                                 weighing_data.first_weighing_value_mid = (unsigned char)(temp_reg_first_weigh>>8);
                                 weighing_data.first_weighing_value_msb = (unsigned char)(temp_reg_first_weigh>>16);                                   
                                 
                                 weighing_data.date_year=year;
                                 weighing_data.date_month=month;
                                 weighing_data.date_day=day;
                                 weighing_data.time_hour=hour;
                                 weighing_data.time_minute=minute;
                                 weighing_data.time_second= second;  
                                 weighing_data.scale_no = using_scale;  
                                 
                                 weighing_data.sort_code= temp_sort_code;   
                                 
                                 sprintf(weighing_data.customer_name,"%s",temp_customer_name);   
                                 
                                 knl1=read_e2prom_byte(CONSEQ1_H_SAVE_INDEX); 
                                 knl1 *= 0x100;
                                 knl1+=read_e2prom_byte(CONSEQ1_L_SAVE_INDEX);        
                                 knl1++;
                                 
                                 weighing_data.conseq1_serial = knl1;    
                                 
                                 write_e2prom_byte((unsigned char)(knl1),CONSEQ1_L_SAVE_INDEX);                     
                                 write_e2prom_byte((unsigned char)(knl1>>8),CONSEQ1_H_SAVE_INDEX);  
                                 
                                 
                                 write_weighing_data_eeprom(weighing_data,temp_inbound_code);   
                                 year_print=year;month_print=month;day_print=day;
                                 hour_print=hour;minute_print=minute;second_print=second;                                 
                                 print_data_block(1); 
   
           }  
          else
          if(second_weighing_state==0 && first_weighing_state==6)
           { 
              first_weighing_state=0;
              second_weighing_state=0;  
              basic_weighing_state=0;
              page=0;
           }             
           
           
          if(first_weighing_state==0 && second_weighing_state==3)           //second_weighing_state==5
           { 
              first_weighing_state=0; 
              second_weighing_state=4;                 //second_weighing_state=6
              basic_weighing_state=0;
           } 
          else 
          if(first_weighing_state==0 && second_weighing_state==4)     //second_weighing_state=6
           { 
              first_weighing_state=0;
              second_weighing_state=0;   
              basic_weighing_state=0;
              page=0;     
              year_print=year;month_print=month;day_print=day;
              hour_print=hour;minute_print=minute;second_print=second;              
              print_data_block(2);
           }                                       
           
          if(second_weighing_state==2 && first_weighing_state==0)
           { 
                  //  page=23;     
                    page=22;  
                  
                    second_weighing_state=3;
                    first_weighing_state=0; 
                    basic_weighing_state=0;
                    
//                    get_character_index=1;  
   //                 blink_get_data_cntr=1;  
      /*              
                                for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++){get_value_ch[kn1]=0;temp_customer_name[kn1]=0;}
                                get_value_ch[0]='0';
                                temp_customer_name[0]='0'; 
                                get_key_character=0xFF;  
                                get_cursor=0;     */              
           }   
           
         if(basic_weighing_state==2)
           {
             page=23;
             
             for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++){get_value_ch[kn1]=0;temp_customer_name[kn1]=0;}
             get_value_ch[0]='0';
             temp_customer_name[0]='0'; 
             get_key_character=0xFF;  
             get_cursor=0;             
           }  
           
          if(basic_weighing_state==4)
           { 
              first_weighing_state=0;
              second_weighing_state=0;  
              basic_weighing_state=5;
           } 
          else 
          if(basic_weighing_state==5)
           { 
              first_weighing_state=0;
              second_weighing_state=0;  
              basic_weighing_state=0;
              page=0;      
              year_print=year;month_print=month;day_print=day;
              hour_print=hour;minute_print=minute;second_print=second;              
              print_data_block(3);
           }   
           
           if(view_1st_weighing==1)
           {
              first_weighing_state=0;
              second_weighing_state=0;  
              basic_weighing_state=0;    
              view_1st_weighing=2;       
           }                     
           else
           if(view_1st_weighing==2)
           {
              first_weighing_state=0;
              second_weighing_state=0;  
              basic_weighing_state=0;    
              view_1st_weighing=0; 
              page=previous_page; 
              if(page==0){menu_item=0;select_item=0;sub_select_item=0;}     
           }                                
                          
          display_home_pos_en=1; 
          clear_home_pos_en=1;            
        }            
     
     }          //end page==22  
     
   else
    if(page==23)         //Enter Customer Name
     {
      if(key_code==UPARROW_KEY)
        {
          page=0;
          second_weighing_state=0;
          first_weighing_state=0;
          
          display_home_pos_en=1; 
          clear_home_pos_en=1;  
          blink_get_data_cntr=0;     
        }
     
      if(key_code>='0' && key_code<='9')      
        { 
           if(key_code==get_key_character || (get_key_character==0xFF))   //press same key  or if overwrite on previous
             {
               if(get_key_character==0xFF)
                    {get_key_character=key_code;
                     get_character_index=1;}
               else  get_character_index++;   
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);
               if(kn1==0)get_character_index=0;
               kn1=*(key_character_table_n_address[key_code-'0']+get_character_index);  
               get_value_ch[get_cursor]=kn1;
               display_home_pos_en=1;  
               page_19_next_letter_cntr=30;
               
             } 
           else
             {
               if(get_cursor<(USER_TEXTS_LENGHT-2))get_cursor++;
               kn1=*(key_character_table_n_address[key_code-'0']+1);  
               get_value_ch[get_cursor]=kn1;               
               get_key_character=key_code;   
               
               get_character_index=1; 
               display_home_pos_en=1;   
               blink_get_data_cntr=1;  
               
             }  
        }    
      if(key_code==CLEAR_KEY)      //clear a character
        { 
           get_value_ch[get_cursor]=0; 
           if(get_cursor>0)
            {              
              get_cursor--; 
              get_key_character=0xFF; // start from NUMBER and overwrite on previous
            } 
           get_value_ch[get_cursor]=' ';  
           display_home_pos_en=1;   
           page_19_next_letter_cntr=0;
        }     
             
      if(key_code==ENTER_KEY)      //accept letter and go to receive next letter
        { 
          if(get_value_ch[get_cursor]==' ')get_value_ch[get_cursor]=0; 
          page_19_next_letter_cntr=0; 
          
          kn2=get_value_ch[0];  
          for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++)
           {  
              temp_customer_name[kn1]=kn2;
              if(kn2!=0)kn2=get_value_ch[kn1+1];   
           } 
          temp_customer_name[USER_TEXTS_LENGHT]=0;
          
          for(kn1=0;kn1<USER_TEXTS_LENGHT;kn1++)get_value_ch[kn1]=0;    
          
          blink_get_data_cntr=0; 
          
          get_character_index=1; 
          display_home_pos_en=1;   
          clear_home_pos_en=1;     
          
         // page=0;     //***\\ 
          
          if(first_weighing_state==2)
                {
                  first_weighing_state=3; 
                  second_weighing_state=0; 
                  basic_weighing_state=0;
                  page=21;  
                  menu_item=3;          //3 means Enter Sort Code   
                  
                  sprintf(get_value_ch,"0    ",); 
                  get_value_i1=3;               //number of digits  
                  blink_get_data_cntr=1;   
                  get_character_index=0;
                  get_cursor=0;   
                }   
                /*       
          if(second_weighing_state==3)
                {
                  first_weighing_state=0;
                  second_weighing_state=4;    
                  basic_weighing_state=0;
                  page=21;  
                  menu_item=3;          //3 means Enter Sort Code   
                  
                  sprintf(get_value_ch,"0    ",); 
                  get_value_i1=3;               //number of digits  
                  blink_get_data_cntr=1;   
                  get_character_index=0;
                  get_cursor=0;                     
                }*/
          if(basic_weighing_state==2)
                {
                  first_weighing_state=0;
                  second_weighing_state=0; 
                  basic_weighing_state=3;
                  page=21;  
                  menu_item=3;          //3 means Enter Sort Code   
                  
                  sprintf(get_value_ch,"0    ",); 
                  get_value_i1=3;               //number of digits  
                  blink_get_data_cntr=1;   
                  get_character_index=0;
                  get_cursor=0;                 
                }      
               
        }                    
                     
     
     }   //end page==23  
     
  else
    if(page==24)      
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
         // if(menu_item==ORDER_OF_SETPOINT)select_item=1;    //ORDER_OF_TARE order of Tare menus
          page=0;
          display_home_pos_en=1; 
          clear_home_pos_en=1;   
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
           sprintf(get_value_ch,"0     ");  
           get_character_index=0;  
           display_home_pos_en=1; 
        }                    
      else   
      if(key_code>='0' && key_code<='9')      //next menu item here is '6'
        { 
           if(get_character_index==get_value_i1)get_character_index=get_value_i1-1;
           get_value_ch[get_character_index]=key_code;  
           if(get_character_index<get_value_i1)get_character_index++;
           display_home_pos_en=1;            
        
        }
      else          
      if(key_code==ENTER_KEY)     //ACCEPT
        { 

           for(kn1=0;kn1<get_value_i1+1;kn1++)if(get_value_ch[kn1]==' ')get_value_ch[kn1]=0;    
           get_value_ch[get_value_i1]=0;  
           
           kn2=strlen(get_value_ch);
           get_value_l2=1;  
           get_value_l1=0;
           
           for(kn1=1;kn1<=kn2;kn1++)            //convert string to a long int NUMBER
            {
              get_value_l3  = get_value_ch[kn2-kn1]-'0';
              get_value_l3 *= get_value_l2;
              get_value_l1 += get_value_l3;
              get_value_l2 *= 10;
            }  
            
            //get_value_l1 entry value
            
         }
         
     }        //end page==24  

   else
    if(page==25)        //ENTER a float number        
     {
      if(key_code==UPARROW_KEY)     //QUIT
        {
          
          page=previous_page;
          display_home_pos_en=1; 
          clear_home_pos_en=1;   
          if(calibration_state==3)
            {
              for(kn1=0;kn1<10;kn1++)                 //restore original value in zero_mv_v in case of quitting from menu
                     {
                       if(get_value_i4==1)kn2=EE_scale1_zero_mv_v_str[kn1]; 
                         else kn2=EE_scale2_zero_mv_v_str[kn1]; 
                       get_value_ch[kn1]=kn2;  
                       if(kn3==0xFF)get_value_ch[kn1]=0; else get_value_ch[kn1]=kn2;  
                       if(kn2==0)kn3=0xFF;
                       if(kn2=='.' && get_value_i2==0)get_value_i2=kn1;
                     }                
              if(get_value_i4==1)scale1_zero_mv_v = convert_string_to_float(get_value_ch);  
              else scale2_zero_mv_v = convert_string_to_float(get_value_ch);                
            }
          else
           {  
              for(kn1=0;kn1<10;kn1++)                 //restore original value in scale_mv_v in case of quitting from menu
                     {
                       if(get_value_i4==1)kn2=EE_scale1_load_mv_v_str[kn1]; 
                         else kn2=EE_scale2_load_mv_v_str[kn1]; 
                       get_value_ch[kn1]=kn2;  
                       if(kn3==0xFF)get_value_ch[kn1]=0; else get_value_ch[kn1]=kn2;  
                       if(kn2==0)kn3=0xFF;
                       if(kn2=='.' && get_value_i2==0)get_value_i2=kn1;
                     }                
              if(get_value_i4==1)scale1_load_mv_v = convert_string_to_float(get_value_ch);  
              else scale2_load_mv_v = convert_string_to_float(get_value_ch);                
           }
          calibration_state=0;
        }
      else             
      if(key_code==CLEAR_KEY)      //DENY
        {  
           sprintf(get_value_ch,"0        ");  
           get_character_index=0;  
           display_home_pos_en=1; 
           get_value_i2=0;              //reset place of point 
           get_value_i3=0xFF;           //it means value was edited
        }     
      else             
      if(key_code==MINUS_KEY)      //'-'    minus sign
       {
           if(get_character_index!=0)return;
           get_value_ch[0]='-'; 
           get_character_index=1;
           display_home_pos_en=1;   
       }                       
      else   
      if((key_code>='0' && key_code<='9') || key_code=='.')
        { 
           if(get_value_i3!=0xFF)
            {
                   sprintf(get_value_ch,"0        ");  
                   get_character_index=0;  
                   display_home_pos_en=1; 
                   get_value_i2=0;              //reset place of point 
                   get_value_i3=0xFF;           //it means value was edited            
            }           

           if(key_code=='.')
             {
              if(get_character_index==0)return;
              if(get_value_i2==0)get_value_i2=get_character_index; 
              else return;
             }
           if(get_character_index==get_value_i1)get_character_index=get_value_i1-1;
           get_value_ch[get_character_index]=key_code;  
           if(get_character_index<get_value_i1)get_character_index++;
           display_home_pos_en=1;            
        }
      else          
      if(key_code==ENTER_KEY)     //ACCEPT
        { 
                                 
           for(kn1=0;kn1<get_value_i1+1;kn1++)
              {
               if(get_value_ch[kn1]==' ')get_value_ch[kn1]=0;    
              }
           get_value_ch[get_value_i1]=0;   
           
           if(menu_item==ORDER_OF_CALIBRATION)
            {   
             if(select_item==ORDER_OF_CALCAL)         
               { 
                 if(calibration_state==3)       //enter zero_mv_v value
                 { 
                    calibration_state=4;
                    page=22;
                    if(get_value_i4==1)
                     {
                        scale1_zero_mv_v = convert_string_to_float(get_value_ch);   
                        if(WRITE_PROTECT)
                           for(kn1=0;kn1<10;kn1++)EE_scale1_zero_mv_v_str[kn1]=get_value_ch[kn1];  
                        
                     }
                    else
                     {
                        scale2_zero_mv_v = convert_string_to_float(get_value_ch);   
                        if(WRITE_PROTECT)
                           for(kn1=0;kn1<10;kn1++)EE_scale2_zero_mv_v_str[kn1]=get_value_ch[kn1];                       
                        
                     } 
                   
                    blink_get_data_cntr=0;                             
                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                       
                 } 
                else     
                 if(calibration_state==8)    //enter calibrating weigh value
                 { 

                   if(get_value_i4==1)
                         scale1_load_mv_v = convert_string_to_float(get_value_ch); 
                   else                                                   
                         scale2_load_mv_v = convert_string_to_float(get_value_ch); 
                         
                    if(get_value_i4==1)
                         { 
                           if(WRITE_PROTECT)
                              for(kn1=0;kn1<10;kn1++)EE_scale1_load_mv_v_str[kn1]=get_value_ch[kn1];   
                         //  if(scale1_powerup_zero==0)poweron_zero_s1_state=0; else poweron_zero_s1_state=POWER_UP_ZERO_TIME;  
                         }
                    else
                         {
                           if(WRITE_PROTECT)
                           for(kn1=0;kn1<10;kn1++)EE_scale2_load_mv_v_str[kn1]=get_value_ch[kn1];                          
                        //   if(scale2_powerup_zero==0)poweron_zero_s2_state=0; else poweron_zero_s2_state=POWER_UP_ZERO_TIME;                             
                         }
                  
                   if(previous_cal_state==7)   //7 means Auto-calibrate 
                    {                                    //and no need to enter the No of loadcell(LC=1)
                      
                      if(get_value_i4==1)
                       {
                             scale2_loadcells_no=1; 
                            if(WRITE_PROTECT)EE_scale1_loadcells_no=1;
                       }  
                      else 
                       {
                             scale2_loadcells_no=1; 
                            if(WRITE_PROTECT)EE_scale1_loadcells_no=1;
                       }                      
                      
                    /* if(get_value_i4==1)
                            {get_value_l1 = scale1_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale1_capacity_unit);}
                     else  {get_value_l1 = scale2_capacity[2];get_value_l1 = convert_unit(get_value_l1,scale2_capacity_unit);}                          
                                                       
                          
                     get_value_i1=6;  
                          
                     if(get_value_l1>999999){sprintf(get_value_ch,"E");}  //overflow
                     else
                     if(get_value_l1>999)sprintf(get_value_ch,"%d%03d",(unsigned int)(get_value_l1/1000),(unsigned int)(get_value_l1%1000));  
                     else  sprintf(get_value_ch,"%d",get_value_l1);                            
                          
                     page=17;          
                     previous_page=10;
                     display_home_pos_en=1; 
                     clear_home_pos_en=1;  

                     blink_get_data_cntr=1;   
                     get_character_index=strlen(get_value_ch); 
                     get_cursor=0;                     
                                             
                       
                     calibration_state=10;   */   
                                                                
                     
                      calibration_state=0;   
                      page=2;                      
                     
                      display_home_pos_message_code='O';  
                      all_keys_temp_disable=30;  
                      no_display_change_cntr=30;          //3 second        
                                                          
                      blink_get_data_cntr=0;                             
                      display_home_pos_en=1; 
                      clear_home_pos_en=1;    
                      display_area_en=1;   
                      clear_all_graph_en=1;                    
                     
                       
                    }        
                    else
                    {

                    calibration_state=9;          
                    page=17;          
                    
                    for(kn1=0;kn1<3;kn1++)get_value_ch[kn1]=0;
                    if(get_value_i4==1)sprintf(get_value_ch,"%d",scale1_loadcells_no);
                    else  sprintf(get_value_ch,"%d",scale2_loadcells_no); 
                    get_value_i1=1;               //number of digits
                  
                    blink_get_data_cntr=1;   
                    get_character_index=strlen(get_value_ch); 
                    get_cursor=0;                       

                    display_home_pos_en=1; 
                    clear_home_pos_en=1;                    
                             
                     /*   
                    if(get_value_i4==1)
                         { 
                           if(WRITE_PROTECT)
                              for(kn1=0;kn1<10;kn1++)EE_scale1_load_mv_v_str[kn1]=get_value_ch[kn1];   
                           if(scale1_powerup_zero==0)poweron_zero_s1_state=0; else poweron_zero_s1_state=POWER_UP_ZERO_TIME;  
                         }
                    else
                         {
                           if(WRITE_PROTECT)
                           for(kn1=0;kn1<10;kn1++)EE_scale2_load_mv_v_str[kn1]=get_value_ch[kn1];                          
                           if(scale2_powerup_zero==0)poweron_zero_s2_state=0; else poweron_zero_s2_state=POWER_UP_ZERO_TIME;                             
                         }
                       */
                    }                                                     
                 }                  
                
               }
            }             
             
            
            
        }  //if(key_code==ENTER_KEY)    
                         
     }  //if(page==25)   

   else
    if(page==26)         //internal menus of calibration_scale#_adaptation
     {
      if(key_code==UPARROW_KEY)
        {
          page=10;
          display_home_pos_en=1; 
          clear_home_pos_en=1;
          sub_select_item=ORDER_OF_ADAPTATION;
        }
      else   
      if(key_code==RIGHT_KEY)      //next menu item here is '6'
        { 
          if(++sub_select_item>ORDER_OF_ADAPT_OVERLOAD)sub_select_item=ORDER_OF_ADAPT_MOTION_W;    
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        }      
      else   
      if(key_code==LEFT_KEY)      //previous menu item here is '4'
        { 
          if(--sub_select_item<ORDER_OF_ADAPT_MOTION_W)sub_select_item=ORDER_OF_ADAPT_OVERLOAD;
          display_home_pos_en=1; 
          clear_home_pos_en=1;         
        } 
      else    
      if(key_code==ENTER_KEY)      //select a menu
           {                                                                
                if(select_item==ORDER_OF_SCALE1)
                 { 
                   if(sub_select_item==ORDER_OF_ADAPT_MOTION_W)get_value_i1=scale1_motion_window; 
                   else if(sub_select_item==ORDER_OF_ADAPT_MOTION_C)get_value_i1=scale1_motion_counter; 
                   else if(sub_select_item==ORDER_OF_ADAPT_FILTER_S)get_value_i1=scale1_filetr_size; 
                   else if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R)get_value_i1=scale1_auto_zero_range; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP)get_value_i1=scale1_pb_zero_p; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON)get_value_i1=scale1_pb_zero_n; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO)get_value_i1=scale1_powerup_zero; 
                   else if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD)get_value_i1=scale1_overload; 
                   else return;     
                 }
                else 
                if(select_item==ORDER_OF_SCALE2)
                 { 
                   if(sub_select_item==ORDER_OF_ADAPT_MOTION_W)get_value_i1=scale2_motion_window; 
                   else if(sub_select_item==ORDER_OF_ADAPT_MOTION_C)get_value_i1=scale2_motion_counter; 
                   else if(sub_select_item==ORDER_OF_ADAPT_FILTER_S)get_value_i1=scale2_filetr_size; 
                   else if(sub_select_item==ORDER_OF_ADAPT_AUTO_Z_R)get_value_i1=scale2_auto_zero_range; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PB_ZEROP)get_value_i1=scale2_pb_zero_p; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PB_ZERON)get_value_i1=scale2_pb_zero_n; 
                   else if(sub_select_item==ORDER_OF_ADAPT_PUP_ZERO)get_value_i1=scale2_powerup_zero; 
                   else if(sub_select_item==ORDER_OF_ADAPT_OVERLOAD)get_value_i1=scale2_overload; 
                   else return;     
                 }                  
                   page=12;
                   display_home_pos_en=1; 
                   clear_home_pos_en=1;  
                   blink_get_data_cntr=1;   
                   get_cursor=0;                                                                                    
            }
        
     }  //if(page==26) 
     
   else
    if(page==27)         
     {
      if(key_code==UPARROW_KEY)
        {
          page=previous_page;
          display_home_pos_en=1; 
          clear_home_pos_en=1;
        }
      else   
      if(key_code=='1')      //next menu item here is '6'
        { 
          get_value_i4=1; 
          display_home_pos_en=1; 
          clear_home_pos_en=1; 
          page=17; 
          previous_page=2;       
        }      
      else   
      if(key_code=='2')      //previous menu item here is '4'
        { 
          get_value_i4=2; 
          display_home_pos_en=1; 
          clear_home_pos_en=1; 
          page=17; 
          previous_page=2;             
        } 
              
     }
     
     
       
 }  
 
 
unsigned char insert_delete_key_in_buf(unsigned char input_key,unsigned char action)
 { 
 unsigned char idkib1;
   
   if(action=='i')
    {
     if(apply_key==0)         //New key
      {
        key_buffer[0]=input_key;
        apply_key=input_key;
      }
     else
      {
        idkib1=0;
        while(key_buffer[idkib1]){if(++idkib1>3)return 0;} 
        key_buffer[idkib1]=input_key;
      } 
     } 
   
   if(action=='d')
    {           
       for(idkib1=0;idkib1<3;idkib1--)key_buffer[idkib1]=key_buffer[idkib1+1];
       key_buffer[3]=0;
       apply_key=key_buffer[3];
    }
   return 1;
 }
 
  
#ifdef KEYBOARD_4x4 
unsigned char get_key_4x4(void)
{ 
  unsigned char tempgk=0,tempdb=0;
  ROW1=0;
  ROW2=1;
  ROW3=1;
  ROW4=1;
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[0];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[1];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[2];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[3];      
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  
  //delay_ms(5);
  delay_ms(1);  
  
  ROW1=1;
  ROW2=0;
  ROW3=1;
  ROW4=1;
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[4];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[5];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[6];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[7];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  
  //delay_ms(5);
  delay_ms(1);
  
  ROW1=1;
  ROW2=1;
  ROW3=0;
  ROW4=1;
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[8];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[9];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[10];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[11];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  
  //delay_ms(5);
  delay_ms(1);
  
  ROW1=1;
  ROW2=1;
  ROW3=1;
  ROW4=0;
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[12];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else     
  if(!COL2)
     {
      tempgk= KEYS[13];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[14];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[15];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  
  //delay_ms(5);
  delay_ms(1); 
  
  ROW1=0;
  ROW2=0;
  ROW3=0;
  ROW4=0;        
     
  return tempgk;

}  

#else


unsigned char get_key_5x4(void)
{ 
  unsigned char tempgk=0,tempdb=0;
  PORTG&=(~ROW1);
  ROW2=1;
  ROW3=1;
  ROW4=1;
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[0];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[1];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[2];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[3];      
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }  
  else   
  if(!COL5)
     {
      tempgk= KEYS[4];      
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL5)tempgk= 0;delay_ms(1);}
     }        
  
  //delay_ms(5);
  delay_ms(1);  
  
  PORTG|=ROW1;
  ROW2=0;
  ROW3=1;
  ROW4=1; 
  
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[5];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[6];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[7];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[8];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  else   
  if(!COL5)
     {
      tempgk= KEYS[9];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL5)tempgk= 0;delay_ms(1);}
     }     
  //delay_ms(5);
  delay_ms(1);
  
  PORTG|=ROW1;
  ROW2=1;
  ROW3=0;
  ROW4=1;
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[10];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL2)
     {
      tempgk= KEYS[11];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[12];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[13];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }  
  else   
  if(!COL5)
     {
      tempgk= KEYS[14];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL5)tempgk= 0;delay_ms(1);}
     }        
  
  //delay_ms(5);
  delay_ms(1);
  
  PORTG|=ROW1;
  ROW2=1;
  ROW3=1;
  ROW4=0;
  
  //delay_ms(5);
  delay_ms(1);
  
  if(!COL1)
     {
      tempgk= KEYS[15];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL1)tempgk= 0;delay_ms(1);}
     }
  else     
  if(!COL2)
     {
      tempgk= KEYS[16];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL2)tempgk= 0;delay_ms(1);}
     }
  else   
  if(!COL3)
     {
      tempgk= KEYS[17];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL3)tempgk= 0;delay_ms(1);}
     }          
  else   
  if(!COL4)
     {
      tempgk= KEYS[18];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL4)tempgk= 0;delay_ms(1);}
     }   
  else   
  if(!COL5)
     {
      tempgk= KEYS[19];
      for(tempdb=0;tempdb<PB_DEBOUNCE;tempdb++){if(COL5)tempgk= 0;delay_ms(1);}
     }      
  
  //delay_ms(5);
  delay_ms(1); 
  
  PORTG&=(~ROW1);
  ROW2=0;
  ROW3=0;
  ROW4=0;        
     
  return tempgk;

}   

#endif


void display_logo(void)
{

  unsigned char temp_dw1[40],dl1;  


  LCD_clear_graph();
  
  LCD_line(0,63,239,63,1);
  LCD_line(0,0,239,0,1);
  LCD_line(0,0,0,63,1);  
  LCD_line(239,0,239,63,1);    
  
  
  
  LCD_setxy((239-113)/2+10,(48-33)/2);
  LCD_draw_icon(IT3000,112,32);    
        

  for(dl1=0;dl1<70;dl1++)
   {
     delay_ms(100);
     #asm("wdr") 
     if(dl1==1)
      { 
            sprintf(temp_dw1,TEXT1_LOGO_ENG);     
            LCD_setxy(2,5); 
            LCD_printstring2(temp_dw1,2);
      } 
     if(dl1==6)
      { 
            sprintf(temp_dw1,TEXT2_LOGO_ENG);     
            LCD_setxy(2,20); 
            LCD_printstring2(temp_dw1,2);
      }      
     if(dl1==11)
      { 
            sprintf(temp_dw1,TEXT3_LOGO_ENG);     
            LCD_setxy(2,35);                                            
            LCD_printstring2(temp_dw1,2);
      }              
     if(dl1==1)
      { 
            sprintf(temp_dw1,TEXT1_LOGO_FA);     
            LCD_setxy(240-LCD_getstringlength2(temp_dw1)-2,5); 
            LCD_printstring2(temp_dw1,2);
      }     
     if(dl1==6)
      { 
            sprintf(temp_dw1,TEXT2_LOGO_FA);     
            LCD_setxy(240-LCD_getstringlength2(temp_dw1)-2,20); 
            LCD_printstring2(temp_dw1,2);
      } 
     if(dl1==11)
      { 
            sprintf(temp_dw1,TEXT3_LOGO_FA);     
            LCD_setxy(240-LCD_getstringlength2(temp_dw1)-2,35); 
            LCD_printstring2(temp_dw1,2);
      }  
      
     if(dl1==15)
      { 
            sprintf(temp_dw1,TEXT_TEL_ENG);     
            LCD_setxy((240-LCD_getstringlength2(temp_dw1))/2,48); 
            LCD_printstring2(temp_dw1,2);
      }                   
      
     insert_delete_key_in_buf_inuse=1; 
     if(apply_key)
       {
          insert_delete_key_in_buf(0,'d');   
          break;
       } 
     insert_delete_key_in_buf_inuse=0;       
      
     
   }

return;

}



void jalali(unsigned char day_i,unsigned char month_i,unsigned char year_i,
            unsigned char *day_o ,unsigned char *month_o,unsigned char *year_o)
{
   int Myear; // As Integer ëëinput Miladi(gregorian) Year
   int Mday; // As Integer ëinput Miladi Day
   int Mmonth; // As Integer ëinput Miladi Month
   int Mdayofyear;
   int Myear_; // As Integer

   int Jday; //output Jalali(shamsi) Day 
   int Jmonth; //output Jalali Month
   int Jyear;  //output Jalali Year
   int Jdayofyear;

   int Gnumdayofyear; // As Integer
   char Isleapyear; // As Bit
   char Isleapyearp; // As Bit

   char Iloop; // As Byte
   int Temp1; // As Integer
   int Temp2; // As Integer
   int Temp3; // As Integer
   int Temp4; // As Integer

   char Jmonth_a[13];// As Byte
   char Mmonth_a[13];// As Byte

   Mmonth_a[1] = 31;
/*28 Ya 29 Agar Sall Kabiseh Bashad In Mah 29 Roz Mibashad*/
   Mmonth_a[2] = 28;
   Mmonth_a[3] = 31;

Mmonth_a[4] = 30;
Mmonth_a[5] = 31;
Mmonth_a[6] = 30;

Mmonth_a[7] = 31;
Mmonth_a[8] = 31;
Mmonth_a[9] = 30;

Mmonth_a[10] = 31;
Mmonth_a[11] = 30;
Mmonth_a[12] = 31;

Gnumdayofyear = 365;

Myear = year_i + 2000;//sample Miladi(gregorian) Year
Mday = day_i ; //ësample Miladi Day
Mmonth = month_i;// ësample Miladi Month
 


/*shart Kabiseh Bodane Sall*/
Temp1 = Myear % 4;
Temp2 = Myear % 100;
Temp3 = Myear % 400;
Temp4 = Temp1 & Temp2;

if ((Temp4!=0)||(Temp3==0))Mmonth_a[2] = 28;
else Mmonth_a[2] = 29;


Myear_ = Myear - 1;
Temp1 = Myear_ % 4;
Temp2 = Myear_ % 100;
Temp3 = Myear_ % 400;
Temp4 = Temp1 & Temp2;

if((Temp4!=0)||(Temp3==0))Isleapyearp=0;
else Isleapyearp=1;


Temp1 = Mmonth - 1;
Mdayofyear=0;
for(Iloop = 1;Iloop<=Temp1;Iloop++)
   Mdayofyear = Mmonth_a[Iloop] + Mdayofyear;

Mdayofyear = Mday + Mdayofyear;

if(Mdayofyear > 79)
{

 Temp1 = Mdayofyear - 79;
 Jyear = Myear - 621;

 if (Temp1 < 186)
 {

  Temp2 = Temp1 % 31;
  Temp3 = Temp1 / 31;

  if(Temp2== 0)
  {Jmonth = Temp3;
   Jday = 31;}
  else  
  {Jmonth = Temp3 + 1;
   Jday = Temp2;} 
   
 } 
 else 
 {
   Temp2 = Temp1 - 186;
   Temp3 = Temp2 % 30;
   Temp4 = Temp2 / 30;

   if(Temp3 == 0)
   {Jmonth = Temp4 + 6;
    Jday = 30;}
   else
   {Jmonth = Temp4 + 7;
    Jday = Temp3;}
  }

 
}
else
{
Jyear = Myear - 622;

if(Isleapyearp == 1)Mdayofyear = Mdayofyear + 11;
else Mdayofyear = Mdayofyear + 10;


Temp2 = Mdayofyear % 30;
Temp3 = Mdayofyear / 30;

if(Temp2 == 0)
{Jmonth = Temp3 + 9;
 Jday = 30;}
else
 {Jmonth = Temp3 + 10;
  Jday = Temp2;}
  
}   
 Jyear = Jyear - 1300;  
 *year_o = Jyear;
 *month_o = Jmonth;
 *day_o = Jday; 
 
}


void  load_initial_values(void)
{
   unsigned char liv1=0,liv2=0;
   unsigned long int liv3=0;     
 
 
 //default value for Parameters must be set here
 /*  
 scales_func_mode=0;
 geo_value=10;  //0 for both -  1 only scale1  -  2 only scale2  -  3 for sum 
 scale1_ranges=0;scale2_ranges=0;                 //0 single range   1 double range    2 triple range
 scale1_capacity[0]=300000;scale1_capacity[1]=400000;scale1_capacity[2]=500000;
 scale2_capacity[0]= 4000000;scale2_capacity[1]= 5500000;scale2_capacity[2]= 10000000;
 scale1_interval[0]=10;scale1_interval[1]=20;scale1_interval[2]=40;
 scale2_interval[0]=10000;scale2_interval[1]=15000;scale2_interval[2]=30000;
 scale1_digits=2;scale2_digits=2;   //0->4 1->5 2->6
 scale1_float=0;scale2_float=0;   //0->0 1->1 2->2  3->3
 scale1_unit=0;scale2_unit=01;       //0->g 1->kg 2->t        
 scale1_loadcells_no=1;scale2_loadcells_no=1;
 scale1_interval_unit=0;scale2_interval_unit=0;       //0->g 1->kg 2->t 
 scale1_capacity_unit=1;scale2_capacity_unit=1;       //0->g 1->kg 2->t 
 scale1_set_point1=1000000;scale1_set_point2=1500000;    
 scale2_set_point1=1000000;scale2_set_point2=1500000;

 scale1_motion_window=0;scale2_motion_window=0;       //3->3.0D 2->2.0D 1->1.0D 0->0.5D
 scale1_motion_counter=20;scale2_motion_counter=10;   //1..20
 scale1_filetr_size=11;scale2_filetr_size=11;
 scale1_auto_zero_range=3;scale2_auto_zero_range=1;   //3->3.0D  2->1.0D 1->0.5D  0->OFF
 scale1_pb_zero_p=2;scale2_pb_zero_p=2;   //0..99
 scale1_pb_zero_n=2;scale2_pb_zero_n=2;   //0..99
 scale1_powerup_zero=0;scale2_powerup_zero=0;   //0-> OFF  1->2%  2->10%
 scale1_overload=9;scale2_overload=9;   //1..20

 scale1_default_calib_weight=1000;scale2_default_calib_weight=1000;    
 
 scale1_zero_mv_v=0.24064;scale1_load_mv_v=2.0001; 
 scale2_zero_mv_v=0.00012;scale2_load_mv_v=2.0002;

 com1_baud_rate=5;com2_baud_rate=3;   //300 600 1200 2400 4800 9600 19200
 com1_databits=1;com2_databits=1;     // 7  8
 com1_parity=0;com2_parity=0;         // no even odd
 com1_stop_bits=0;com2_stop_bits=0;   //1 2      

 com1_func=NOTUSE;com2_func=PRINTER;   //0 1 2 3 4 5
 com1_protocol=0;com2_protocol=0;   //0 1 2 3
 language=0; 
 software=0;
 backlight_time =0;
 dongletype=0; 
 homepos=1;        
 calendar=1;   */
 
 
scales_func_mode=DEFAULT_scales_func_mode;
 geo_value=DEFAULT_geo_value;  //0 for both -  1 only scale1  -  2 only scale2  -  3 for sum 
 scale1_ranges=DEFAULT_scale1_ranges;
 scale2_ranges=DEFAULT_scale2_ranges;                 //0 single range   1 double range    2 triple range
 scale1_capacity[0]=DEFAULT_scale1_capacity0;
 scale1_capacity[1]=DEFAULT_scale1_capacity1;
 scale1_capacity[2]=DEFAULT_scale1_capacity2;
 scale2_capacity[0]= DEFAULT_scale2_capacity0;
 scale2_capacity[1]= DEFAULT_scale2_capacity1;
 scale2_capacity[2]= DEFAULT_scale2_capacity2;
 scale1_interval[0]=DEFAULT_scale1_interval0;
 scale1_interval[1]=DEFAULT_scale1_interval1;
 scale1_interval[2]=DEFAULT_scale1_interval2;
 scale2_interval[0]=DEFAULT_scale2_interval0;
 scale2_interval[1]=DEFAULT_scale2_interval1;
 scale2_interval[2]=DEFAULT_scale2_interval2;
 scale1_digits=DEFAULT_scale1_digits;
 scale2_digits=DEFAULT_scale2_digits;   //0->4 1->5 2->6
 scale1_float=DEFAULT_scale1_float;
 scale2_float=DEFAULT_scale2_float;   //0->0 1->1 2->2  3->3
 scale1_unit=DEFAULT_scale1_unit;  
 scale2_unit=DEFAULT_scale2_unit;       //0->g 1->kg 2->t
 scale1_interval_unit=DEFAULT_scale1_interval_unit;
 scale2_interval_unit=DEFAULT_scale2_interval_unit;       //0->g 1->kg 2->t
 scale1_capacity_unit=DEFAULT_scale1_capacity_unit;
 scale2_capacity_unit=DEFAULT_scale2_capacity_unit;       //0->g 1->kg 2->t
 scale1_set_point1=DEFAULT_scale1_set_point1;
 scale1_set_point2=DEFAULT_scale1_set_point2;
 scale2_set_point1=DEFAULT_scale2_set_point1;
 scale2_set_point2=DEFAULT_scale2_set_point2;
 scale1_loadcells_no=DEFAULT_scale1_loadcells_no;
 scale2_loadcells_no=DEFAULT_scale2_loadcells_no; 
 scale1_motion_window=DEFAULT_scale1_motion_window;
 scale2_motion_window=DEFAULT_scale2_motion_window;       //3->3.0D 2->2.0D 1->1.0D 0->0.5D
 scale1_motion_counter=DEFAULT_scale1_motion_counter;
 scale2_motion_counter=DEFAULT_scale2_motion_counter;   //1..20
 scale1_filetr_size=DEFAULT_scale1_filetr_size;
 scale2_filetr_size=DEFAULT_scale2_filetr_size;
 scale1_auto_zero_range=DEFAULT_scale1_auto_zero_range;
 scale2_auto_zero_range=DEFAULT_scale2_auto_zero_range;   //3->3.0D  2->1.0D 1->0.5D  0->OFF
 scale1_pb_zero_p=DEFAULT_scale1_pb_zero_p;
 scale2_pb_zero_p=DEFAULT_scale2_pb_zero_p;   //0..99
 scale1_pb_zero_n=DEFAULT_scale1_pb_zero_n;
 scale2_pb_zero_n=DEFAULT_scale2_pb_zero_n;   //0..99
 scale1_powerup_zero=DEFAULT_scale1_powerup_zero;
 scale2_powerup_zero=DEFAULT_scale2_powerup_zero;   //0-> OFF  1->2%  2->10%
 scale1_overload=DEFAULT_scale1_overload;
 scale2_overload=DEFAULT_scale2_overload;   //1..20
 scale1_default_calib_weight=DEFAULT_scale1_default_calib_weight;    
 scale2_default_calib_weight=DEFAULT_scale2_default_calib_weight;
 scale1_zero_mv_v=DEFAULT_scale1_zero_mv_v;
 scale1_load_mv_v=DEFAULT_scale1_load_mv_v;
 scale2_zero_mv_v=DEFAULT_scale2_zero_mv_v;
 scale2_load_mv_v=DEFAULT_scale2_load_mv_v;
 com1_baud_rate=DEFAULT_com1_baud_rate;   
 com2_baud_rate=DEFAULT_com2_baud_rate;   
 com1_databits=DEFAULT_com1_databits;
 com2_databits=DEFAULT_com2_databits;    
 com1_parity=DEFAULT_com1_parity;
 com2_parity=DEFAULT_com2_parity;         
 com1_stop_bits=DEFAULT_com1_stop_bits;
 com2_stop_bits=DEFAULT_com2_stop_bits;  
 com1_func=DEFAULT_com1_func;     
 com2_func=DEFAULT_com2_func;   
 com1_protocol=DEFAULT_com1_protocol;
 com2_protocol=DEFAULT_com2_protocol; 
 language=DEFAULT_language;     
 software=DEFAULT_software;     
 backlight_time =DEFAULT_backlight_time;
 dongletype=DEFAULT_dongletype;
 homepos=DEFAULT_homepos;
 calendar=DEFAULT_calendar;
 alarm=DEFAULT_alarm;             
   
  
   
 liv1=EE_scales_func_mode;
 if(liv1>3)EE_scales_func_mode = scales_func_mode;
 else  scales_func_mode = liv1;  
 
 liv1=EE_scale1_ranges;
 if(liv1>2)EE_scale1_ranges = scale1_ranges;
 else  scale1_ranges = liv1;  
 
 liv1=EE_scale2_ranges;
 if(liv1>2)EE_scale2_ranges = scale2_ranges;
 else  scale2_ranges = liv1;  
 
 
 
 
 liv3=EE_scale1_capacity[2];
 if(liv3>1000000000)EE_scale1_capacity[2] = scale1_capacity[2];
 else  scale1_capacity[2] = liv3;  
 
 liv3=EE_scale1_capacity[1];
 if(liv3>scale1_capacity[2]){EE_scale1_capacity[1] = scale1_capacity[2];scale1_capacity[1] = scale1_capacity[2];}
 else  scale1_capacity[1] = liv3;
 
 liv3=EE_scale1_capacity[0];
 if(liv3>scale1_capacity[1]){EE_scale1_capacity[0] = scale1_capacity[1];scale1_capacity[0] = scale1_capacity[1];}
 else  scale1_capacity[0] = liv3;
 
 
 liv3=EE_scale2_capacity[2];
 if(liv3>1000000000)EE_scale2_capacity[2] = scale2_capacity[2];
 else  scale2_capacity[2] = liv3;  
 
 liv3=EE_scale2_capacity[1];
 if(liv3>scale2_capacity[2]){EE_scale2_capacity[1] = scale2_capacity[2];scale2_capacity[1] = scale2_capacity[2];}
 else  scale2_capacity[1] = liv3;
 
 liv3=EE_scale2_capacity[0];
 if(liv3>scale2_capacity[1]){EE_scale2_capacity[0] = scale2_capacity[1];scale2_capacity[0] = scale2_capacity[1];}
 else  scale2_capacity[0] = liv3;
 
           
           
                    
       
 liv3=EE_scale1_interval[2];
 if(liv3>1000000000)EE_scale1_interval[2] = scale1_interval[2];
 else  scale1_interval[2] = liv3;  
 
 liv3=EE_scale1_interval[1];
 if(liv3>scale1_interval[2]){EE_scale1_interval[1] = scale1_interval[2];scale1_interval[1] = scale1_interval[2];}
 else  scale1_interval[1] = liv3;  
 
 liv3=EE_scale1_interval[0];
 if(liv3>scale1_interval[1]){EE_scale1_interval[0] = scale1_interval[1];scale1_interval[0] = scale1_interval[1];}
 else  scale1_interval[0] = liv3;   
  

 liv3=EE_scale2_interval[2];
 if(liv3>1000000000)EE_scale2_interval[2] = scale2_interval[2];
 else  scale2_interval[2] = liv3;  
 
 liv3=EE_scale2_interval[1];
 if(liv3>scale2_interval[2]){EE_scale2_interval[1] = scale2_interval[2];scale2_interval[1] = scale2_interval[2];}
 else  scale2_interval[1] = liv3;  
 
 liv3=EE_scale2_interval[0];
 if(liv3>scale2_interval[1]){EE_scale2_interval[0] = scale2_interval[1];scale2_interval[0] = scale2_interval[1];}
 else  scale2_interval[0] = liv3; 
                                
 
                    
                                              
 
 
 liv1=EE_scale1_digits;
 if(liv1>2)EE_scale1_digits = scale1_digits;
 else  scale1_digits = liv1;  
 
 liv1=EE_scale2_digits;
 if(liv1>2)EE_scale2_digits = scale2_digits;
 else  scale2_digits = liv1;  
                                          
 
 liv1=EE_scale1_float;
 if(liv1>3)EE_scale1_float = scale1_float;
 else  scale1_float = liv1;  
 
 liv1=EE_scale2_float;
 if(liv1>3)EE_scale2_float = scale2_float;
 else  scale2_float = liv1;   
                   
 
 liv1=EE_scale1_unit;
 if(liv1>2)EE_scale1_unit = scale1_unit;
 else  scale1_unit = liv1;    
 
 liv1=EE_scale1_loadcells_no;
 if(liv1>8 || liv1==0)EE_scale1_loadcells_no = scale1_loadcells_no;
 else  scale1_loadcells_no = liv1; 
         
 
 liv1=EE_scale1_interval_unit;
 if(liv1>2)EE_scale1_interval_unit = scale1_interval_unit;
 else  scale1_interval_unit = liv1;  
 
 liv1=EE_scale1_capacity_unit;
 if(liv1>2)EE_scale1_capacity_unit = scale1_capacity_unit;
 else  scale1_capacity_unit = liv1;    
 
 liv1=EE_scale2_unit;
 if(liv1>2)EE_scale2_unit = scale2_unit;
 else  scale2_unit = liv1;   
 
 liv1=EE_scale2_loadcells_no;
 if(liv1>8 || liv1==0)EE_scale2_loadcells_no = scale2_loadcells_no;
 else  scale2_loadcells_no = liv1; 
 

 liv1=EE_scale2_interval_unit;
 if(liv1>2)EE_scale2_interval_unit = scale2_interval_unit;
 else  scale2_interval_unit = liv1;  
 
 liv1=EE_scale2_capacity_unit;
 if(liv1>2)EE_scale2_capacity_unit = scale2_capacity_unit;
 else  scale2_capacity_unit = liv1;    
    

 
  liv1=EE_scale1_motion_window;
 if(liv1>3)EE_scale1_motion_window = scale1_motion_window;
 else  scale1_motion_window = liv1;  
 
  liv1=EE_scale2_motion_window;
 if(liv1>3)EE_scale2_motion_window = scale2_motion_window;
 else  scale2_motion_window = liv1;  
 
 
  liv1=EE_scale1_motion_counter;
 if(liv1>20 || liv1<1)EE_scale1_motion_counter = scale1_motion_counter;
 else  scale1_motion_counter = liv1;   
 
  liv1=EE_scale2_motion_counter;
 if(liv1>20 || liv1<1)EE_scale2_motion_counter = scale2_motion_counter;
 else  scale2_motion_counter = liv1;   
 

  liv1=EE_scale1_filetr_size;
 if(liv1>20 || liv1<1)EE_scale1_filetr_size = scale1_filetr_size;
 else  scale1_filetr_size = liv1;     
 
  liv1=EE_scale2_filetr_size;
 if(liv1>20 || liv1<1)EE_scale2_filetr_size = scale2_filetr_size;
 else  scale2_filetr_size = liv1; 
                                        
   
  liv1=EE_scale1_auto_zero_range;
 if(liv1>3)EE_scale1_auto_zero_range = scale1_auto_zero_range;
 else  scale1_auto_zero_range = liv1;   
 
  liv1=EE_scale2_auto_zero_range;
 if(liv1>3)EE_scale2_auto_zero_range = scale2_auto_zero_range;
 else  scale2_auto_zero_range = liv1;     
 

  liv1=EE_scale1_pb_zero_p;
 if(liv1>99)EE_scale1_pb_zero_p = scale1_pb_zero_p;
 else  scale1_pb_zero_p = liv1;  
 
  liv1=EE_scale2_pb_zero_p;
 if(liv1>99)EE_scale2_pb_zero_p = scale2_pb_zero_p;
 else  scale2_pb_zero_p = liv1; 
 
 
  liv1=EE_scale1_pb_zero_n;
 if(liv1>99)EE_scale1_pb_zero_n = scale1_pb_zero_n;
 else  scale1_pb_zero_n = liv1;  
 
  liv1=EE_scale2_pb_zero_n;
 if(liv1>99)EE_scale2_pb_zero_n = scale2_pb_zero_n;
 else  scale2_pb_zero_n = liv1;    
            
 
  liv1=EE_scale1_powerup_zero;
 if(liv1>2)EE_scale1_powerup_zero = scale1_powerup_zero;
 else  scale1_powerup_zero = liv1;  
 
  liv1=EE_scale2_powerup_zero;
 if(liv1>2)EE_scale2_powerup_zero = scale2_powerup_zero;
 else  scale2_powerup_zero = liv1;      
 
 
    
  liv1=EE_scale1_overload;
 if(liv1>20 || liv1<1)EE_scale1_overload = scale1_overload;
 else  scale1_overload = liv1;         

  liv1=EE_scale2_overload;
 if(liv1>20 || liv1<1)EE_scale2_overload = scale2_overload;
 else  scale2_overload = liv1;  
 
                              
 
 
 liv1=EE_com1_baud_rate;
 if(liv1>6)EE_com1_baud_rate = com1_baud_rate;
 else  com1_baud_rate = liv1;  
 
 liv1=EE_com2_baud_rate;
 if(liv1>6)EE_com2_baud_rate = com2_baud_rate;
 else  com2_baud_rate = liv1;           
 
 liv1=EE_com1_databits;
 if(liv1>2)EE_com1_databits = com1_databits;
 else  com1_databits = liv1;  
 
 liv1=EE_com2_databits;
 if(liv1>2)EE_com2_databits = com2_databits;
 else  com2_databits = liv1;      
 
 liv1=EE_com1_parity;
 if(liv1>2)EE_com1_parity = com1_parity;
 else  com1_parity = liv1;  
 
 liv1=EE_com2_parity;
 if(liv1>2)EE_com2_parity = com2_parity;
 else  com2_parity = liv1;              
 
 
  liv1=EE_com1_stop_bits;
 if(liv1>2)EE_com1_stop_bits = com1_stop_bits;
 else  com1_stop_bits = liv1;    
 
 
  liv1=EE_com2_stop_bits;
 if(liv1>2)EE_com2_stop_bits = com2_stop_bits;
 else  com2_stop_bits = liv1;   
                                               
    
  liv1=EE_com1_func;
 if(liv1>NOTUSE)EE_com1_func = com1_func;
 else  com1_func = liv1;        
 
  liv1=EE_com2_func;
 if(liv1>NOTUSE)EE_com2_func = com2_func;
 else  com2_func = liv1;
 
  liv1=EE_com1_protocol;
 if(liv1>5)EE_com1_protocol = com1_protocol;
 else  com1_protocol = liv1;        
 
  liv1=EE_com2_protocol;
 if(liv1>5)EE_com2_protocol = com2_protocol;
 else  com2_protocol = liv1;   
       
config_USART(1);   
config_USART(2);    
    
  liv1=EE_language;
 if(liv1>1)EE_language = language;
 else  language = liv1;      
 
 
  liv1=EE_homepos;
 if(liv1>1)EE_homepos = homepos;
 else  homepos = liv1;             
 
  liv1=EE_software;
 if(liv1>5)EE_software = software;
 else  software = liv1;           
 
 
  liv1=EE_backlight_time;
 if(liv1>240)EE_backlight_time = backlight_time;
 else  backlight_time = liv1;       
 
 
  liv1=EE_dongletype;
 if(liv1>3)EE_dongletype = dongletype;
 else  dongletype = liv1;             
 
  liv1=EE_calendar;
 if(liv1>1)EE_calendar = calendar;
 else  calendar = liv1;      
 
  liv1=EE_alarm;
 if(liv1>1)EE_alarm = alarm;
 else  alarm = liv1;  
  
  liv3=EE_scale1_default_calib_weight;
 if(liv3==0xFFFFFFFF)EE_scale1_default_calib_weight = scale1_default_calib_weight;
 else  scale1_default_calib_weight = liv3;  
 
  liv3=EE_scale2_default_calib_weight;
 if(liv3==0xFFFFFFFF)EE_scale2_default_calib_weight = scale2_default_calib_weight;
 else  scale2_default_calib_weight = liv3;  


   for(liv1=0;liv1<10;liv1++)
      {
        liv2=EE_scale1_zero_mv_v_str[liv1];
        get_value_ch[liv1]=liv2;  
        if(liv2==0)break;
        if((liv2>'9' || liv2<'0') && liv2!='.'){liv1=0xFF;break;}
      }
   if(liv1==0xFF)                                    
     {
        EE_scale1_zero_mv_v_str[0]='0';get_value_ch[0]='0';
        EE_scale1_zero_mv_v_str[1]='.';get_value_ch[1]='.';
        EE_scale1_zero_mv_v_str[2]='0';get_value_ch[2]='0';
        EE_scale1_zero_mv_v_str[3]='0';get_value_ch[3]='0';
        EE_scale1_zero_mv_v_str[4]='0';  get_value_ch[4]='0';
        EE_scale1_zero_mv_v_str[5]='0';  get_value_ch[5]='0'; 
        EE_scale1_zero_mv_v_str[6]='0';  get_value_ch[6]='0';
        EE_scale1_zero_mv_v_str[7]=0;  get_value_ch[7]=0;
        EE_scale1_zero_mv_v_str[8]=0;  get_value_ch[8]=0;   
        EE_scale1_zero_mv_v_str[9]=0;  get_value_ch[9]=0;
     }
   scale1_zero_mv_v = convert_string_to_float(get_value_ch);  
                 
   
   for(liv1=0;liv1<10;liv1++)
      {
        liv2=EE_scale2_zero_mv_v_str[liv1];
        get_value_ch[liv1]=liv2;  
        if(liv2==0)break;
        if((liv2>'9' || liv2<'0') && liv2!='.' && liv2!='-'){liv1=0xFF;break;}
      }
   if(liv1==0xFF)
     {
        EE_scale2_zero_mv_v_str[0]='0';get_value_ch[0]='0';
        EE_scale2_zero_mv_v_str[1]='.';get_value_ch[1]='.';
        EE_scale2_zero_mv_v_str[2]='0';get_value_ch[2]='0';
        EE_scale2_zero_mv_v_str[3]='0';get_value_ch[3]='0';
        EE_scale2_zero_mv_v_str[4]=0;  get_value_ch[4]=0;
        EE_scale2_zero_mv_v_str[5]=0;  get_value_ch[5]=0; 
        EE_scale2_zero_mv_v_str[6]=0;  get_value_ch[6]=0;
        EE_scale2_zero_mv_v_str[7]=0;  get_value_ch[7]=0;
        EE_scale2_zero_mv_v_str[8]=0;  get_value_ch[8]=0;   
        EE_scale2_zero_mv_v_str[9]=0;  get_value_ch[9]=0;
     }
   scale2_zero_mv_v = convert_string_to_float(get_value_ch);   
   
         
           
   for(liv1=0;liv1<10;liv1++)
      {
        liv2=EE_scale1_load_mv_v_str[liv1];
        get_value_ch[liv1]=liv2;  
        if(liv2==0)break;        
        if((liv2>'9' || liv2<'0') && liv2!='.'){liv1=0xFF;break;}
      }
   if(liv1==0xFF)
      {
        EE_scale1_load_mv_v_str[0]='2';get_value_ch[0]='2';
        EE_scale1_load_mv_v_str[1]='.';get_value_ch[1]='.';
        EE_scale1_load_mv_v_str[2]='0';get_value_ch[2]='0';
        EE_scale1_load_mv_v_str[3]='0';get_value_ch[3]='0';
        EE_scale1_load_mv_v_str[4]=0;  get_value_ch[4]=0;
        EE_scale1_load_mv_v_str[5]=0;  get_value_ch[5]=0; 
        EE_scale1_load_mv_v_str[6]=0;  get_value_ch[6]=0;
        EE_scale1_load_mv_v_str[7]=0;  get_value_ch[7]=0;
        EE_scale1_load_mv_v_str[8]=0;  get_value_ch[8]=0;   
        EE_scale1_load_mv_v_str[9]=0;  get_value_ch[9]=0;
      }
   scale1_load_mv_v = convert_string_to_float(get_value_ch);   
   
           
   for(liv1=0;liv1<10;liv1++)
      {
        liv2=EE_scale2_load_mv_v_str[liv1];
        get_value_ch[liv1]=liv2;  
        if(liv2==0)break;
        if((liv2>'9' || liv2<'0') && liv2!='.' && liv2!='-'){liv1=0xFF;break;}
      }
   if(liv1==0xFF)
      {
        EE_scale2_load_mv_v_str[0]='2';get_value_ch[0]='2';
        EE_scale2_load_mv_v_str[1]='.';get_value_ch[1]='.';
        EE_scale2_load_mv_v_str[2]='0';get_value_ch[2]='0';
        EE_scale2_load_mv_v_str[3]='0';get_value_ch[3]='0';
        EE_scale2_load_mv_v_str[4]=0;  get_value_ch[4]=0;
        EE_scale2_load_mv_v_str[5]=0;  get_value_ch[5]=0; 
        EE_scale2_load_mv_v_str[6]=0;  get_value_ch[6]=0;
        EE_scale2_load_mv_v_str[7]=0;  get_value_ch[7]=0;
        EE_scale2_load_mv_v_str[8]=0;  get_value_ch[8]=0;   
        EE_scale2_load_mv_v_str[9]=0;  get_value_ch[9]=0;
      }
   scale2_load_mv_v = convert_string_to_float(get_value_ch);      
   
   
   sort_print_data();           //sorted_field_table[] is created   
   
   
                   
   for(liv1=0;liv1<6;liv1++)
    {
        liv2=EE_Pass_1[liv1];                
        Pass_1[liv1]=liv2;  
        if(liv2<'0' || liv2>'9'){liv1=0xFF;break;}
    }
   if(liv1==0xFF)
    {
       Pass_1[0]='1';EE_Pass_1[0]=Pass_1[0];
       Pass_1[1]='4';EE_Pass_1[1]=Pass_1[1];
       Pass_1[2]='0';EE_Pass_1[2]=Pass_1[2];
       Pass_1[3]='7';EE_Pass_1[3]=Pass_1[3];
       Pass_1[4]='8';EE_Pass_1[4]=Pass_1[4];
       Pass_1[5]='9';EE_Pass_1[5]=Pass_1[5];
    }  
    
   for(liv1=0;liv1<6;liv1++)
    {
        liv2=EE_Pass_2[liv1];                
        Pass_2[liv1]=liv2;  
        if(liv2<'0' || liv2>'9'){liv1=0xFF;break;}
    }
   if(liv1==0xFF)
    {
       Pass_2[0]='6';EE_Pass_2[0]=Pass_2[0];
       Pass_2[1]='1';EE_Pass_2[1]=Pass_2[1];
       Pass_2[2]='8';EE_Pass_2[2]=Pass_2[2];
       Pass_2[3]='3';EE_Pass_2[3]=Pass_2[3];
       Pass_2[4]='4';EE_Pass_2[4]=Pass_2[4];
       Pass_2[5]='9';EE_Pass_2[5]=Pass_2[5];
    }    
    
   for(liv1=0;liv1<6;liv1++)
    {
        liv2=EE_Pass_3[liv1];                
        Pass_3[liv1]=liv2;  
        if(liv2<'0' || liv2>'9'){liv1=0xFF;break;}
    }
   if(liv1==0xFF)
    {
       Pass_3[0]='5';EE_Pass_3[0]=Pass_3[0];
       Pass_3[1]='0';EE_Pass_3[1]=Pass_3[1];
       Pass_3[2]='5';EE_Pass_3[2]=Pass_3[2];
       Pass_3[3]='4';EE_Pass_3[3]=Pass_3[3];
       Pass_3[4]='4';EE_Pass_3[4]=Pass_3[4];
       Pass_3[5]='2';EE_Pass_3[5]=Pass_3[5];
    }   
    
   for(liv1=0;liv1<6;liv1++)
    {
        liv2=EE_Pass_4[liv1];                
        Pass_4[liv1]=liv2;  
        if(liv2<'0' || liv2>'9'){liv1=0xFF;break;}
    }
   if(liv1==0xFF)
    {
       Pass_4[0]='1';EE_Pass_4[0]=Pass_4[0];
       Pass_4[1]='2';EE_Pass_4[1]=Pass_4[1];
       Pass_4[2]='2';EE_Pass_4[2]=Pass_4[2];
       Pass_4[3]='3';EE_Pass_4[3]=Pass_4[3];
       Pass_4[4]='4';EE_Pass_4[4]=Pass_4[4];
       Pass_4[5]='5';EE_Pass_4[5]=Pass_4[5];
    }           
    
}

void  initialize(void)
{       

            /*
// Watchdog Timer initialization
// Watchdog Timer Prescaler: OSC/2048k
#pragma optsize-
WDTCR=0x1F;
WDTCR=0x0F;
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
                   */
                   
// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
//PORTA=0x00;

 LC1_SCK=1;
 LC1_SCK_DDR=1; 
 
 LC1_DI_OUT=1;
 LC1_DI_DDR=0;

 LC2_SCK=1;
 LC2_SCK_DDR=1;
 
 LC2_DI_OUT=1;
 LC2_DI_DDR=0;


// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTB=0x00;
DDRB=0x00;

// Port c initialization
// Func7=In Func6=In Func5=In Func4=In Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=P State6=P State5=P State4=P State3=1 State2=1 State1=1 State0=1 
PORTC=0xFF;

#ifdef KEYBOARD_4x4
ROW1_DDR=1;
#else
DDRG|=ROW1_DDR;
#endif
ROW2_DDR=1;
ROW3_DDR=1;
ROW4_DDR=1;

COL1_DDR=0;
COL2_DDR=0;
COL3_DDR=0;
COL4_DDR=0; 
               
#ifdef KEYBOARD_4x4
ROW1=0;
#else
PORTG&=(~ROW1);
#endif
ROW2=0;
ROW3=0;
ROW4=0;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTD=0x00;
DDRD=0x00;

// Port E initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTE=0x00;
DDRE=0x00;

// Port F initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=T State0=T 
PORTF=0x00;
DDRF=0x00;

// Port G initialization
// Func4=In Func3=In Func2=In Func1=In Func0=In 
// State4=T State3=T State2=T State1=T State0=T 
PORTG|=WRITE_PROTECT_OUT;
DDRG&=WRITE_PROTECT_DDR;   


OUT1=0;
OUT1_DDR=1;

OUT2=0;
OUT2_DDR=1;


OUT3=0;
OUT3_DDR=1;

OUT4=0;
OUT4_DDR=1;



BUZ_DDR=1;
ALARM_DDR=1;
BL_DDR=1; 

BUZ=0;
ALARM=1;
BL=1;

SEC_IN_DDR=0;
sec_in_c = SEC_IN;     


DIR=0;
DIR_DDR=1;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=FFh
// OC0 output: Disconnected
ASSR=0x00;
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: 62.500 kHz
// Mode: Normal top=FFFFh
// OC1A output: Discon.
// OC1B output: Discon.
// OC1C output: Discon.
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer 1 Overflow Interrupt: On
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR1A=0x00;
TCCR1B=0x04;
TCNT1H=0xF3;//0xE7;
TCNT1L=0xCB;//0x96;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
OCR1CH=0x00;
OCR1CL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer 2 Stopped
// Mode: Normal top=FFh
// OC2 output: Disconnected
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;

// Timer/Counter 3 initialization
// Clock source: System Clock
// Clock value: Timer 3 Stopped
// Mode: Normal top=FFFFh
// Noise Canceler: Off
// Input Capture on Falling Edge
// OC3A output: Discon.
// OC3B output: Discon.
// OC3C output: Discon.
// Timer 3 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
// Compare C Match Interrupt: Off
TCCR3A=0x00;
TCCR3B=0x00;
TCNT3H=0x00;
TCNT3L=0x00;
ICR3H=0x00;
ICR3L=0x00;
OCR3AH=0x00;
OCR3AL=0x00;
OCR3BH=0x00;
OCR3BL=0x00;
OCR3CH=0x00;
OCR3CL=0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
// INT3: Off
// INT4: Off
// INT5: Off
// INT6: Off
// INT7: Off
EICRA=0x00;
EICRB=0x00;
EIMSK=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x04;
ETIMSK=0x00;

// USART0 initialization
// Communication Parameters: 8 Data 1 Stop, No Parity
// USART0 Receiver: Off
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud rate: 19200
UCSR0A=0x00;
UCSR0B=0x08;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x33;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
ACSR=0x80;
SFIOR=0x00;

   
// I2C Bus initialization
i2c_init();

// DS1307 Real Time Clock initialization
// Square wave output on pin SQW/OUT: On
// Square wave frequency: 1Hz
rtc_init(0,1,0);
   

delay_ms(100);                                                                    

   rtc_get_time(&hour,&minute,&second);     
   if(second&0x80)
      {second&=(~0x80);   
       rtc_set_time(hour,minute,second);}
   if((hour>23)||(minute>59)||(second>59)){hour=0;minute=0;second=0;rtc_set_time(hour,minute,second);} 
   
   rtc_get_date(&day,&month,&year);        
   
   
           
#asm("sei") 

delay_ms(1000);   
  
    
LCD_setup();	
LCD_init();  
	

LCD_clear_graph(); 
LCD_clear_text();     


}   
