unsigned char read_LC1(void)
{
  unsigned long int adc_val=0;  
  unsigned long int adc_temp; 
  unsigned char rlc1=0,rlc2=0; 
  unsigned char adc_temp_ch0;
  
//  LC1_DI_OUT=1;
//  LC1_DI_DDR = 0;             //make DOUR/rdy output

  adc_temp=0x80000000;      
  
  delay_us(5);
  
  rlc2=0;
  if(LC1_DI)return 'F';     //Not Ready yet
  
  
  LC1_SCK=1;
  delay_us(10);
   

  adc_val=0; 
  adc_temp=0x80000000;         
  
  for(rlc1=0;rlc1<32;rlc1++)
  {
   LC1_SCK=0;
   delay_us(10);
   if(LC1_DI)adc_val = adc_val | adc_temp;  
   adc_temp = adc_temp>>1;
   adc_temp &= 0x7FFFFFFF;
   LC1_SCK=1;
   delay_us(10);
   }

 LC1_status =  (unsigned char)(adc_val);     
 
 if(LC1_status&0x80)return 'E';         //RDY=1 and it means no convertion available
 
 if(LC1_status&0x20)return 'E';        //ERR=1 and it means an error occured 
 
 if((LC1_status&0x08)==0)return 'D';        //ID0 must be 1
 
 if(LC1_status&0x10)return 'D';           //ID1 must be 0                                      
 
//
//  (21 - filetr)*PrevoiusADCValue + (filetr)*(ADCVALUE)
//  ------------------------------------------------
//                               21
//
 
 
 adc_temp = adc_val>>12;   
 adc_temp &= 0x000FFFFF;    
 
  
 adc_temp_ch0 = 21 - scale1_filetr_size;     
 adc_val = scale1_raw_value;
 adc_val = adc_val * adc_temp_ch0;
 
 scale1_raw_value = adc_temp;
 scale1_raw_value = scale1_raw_value * scale1_filetr_size;
 
 scale1_raw_value = scale1_raw_value +  adc_val;
 
 scale1_raw_value = scale1_raw_value / 21;
              
                 /*
 adc_temp = adc_temp * 9;
 scale1_raw_value = scale1_raw_value + adc_temp;                    //bn = (1/5)an-1  + (4/5)an
 scale1_raw_value = scale1_raw_value / 10;       */
 
// if(LC1_status&0x04)

 

  
// LC1_DI_OUT=1;
// LC1_DI_DDR = 0;             //make DOUR/rdy input
 
 return 0;       //0xFF means failed
  
}  



unsigned char read_LC2(void)
{
  unsigned long int adc_val=0;  
  unsigned long int adc_temp; 
  unsigned char rlc1=0,rlc2=0; 
  unsigned char adc_temp_ch0;
  
//  LC2_DI_OUT=1;
//  LC2_DI_DDR = 0;             //make DOUR/rdy output

  adc_temp=0x80000000;      
  
  delay_us(5);
  
  rlc2=0;
  if(LC2_DI)return 'F';     //Not Ready yet
  
  
  LC2_SCK=1;
  delay_us(10);
   

  adc_val=0; 
  adc_temp=0x80000000;         
  
  for(rlc1=0;rlc1<32;rlc1++)
  {
   LC2_SCK=0;
   delay_us(10);
   if(LC2_DI)adc_val = adc_val | adc_temp;  
   adc_temp = adc_temp>>1;
   adc_temp &= 0x7FFFFFFF;
   LC2_SCK=1;
   delay_us(10);
   }

 LC2_status =  (unsigned char)(adc_val);     
 
 if(LC2_status&0x80)return 'E';         //RDY=1 and it means no convertion available
 
 if(LC2_status&0x20)return 'E';        //ERR=1 and it means an error occured 
 
 if((LC2_status&0x08)==0)return 'D';        //ID0 must be 1
 
 if(LC2_status&0x10)return 'D';           //ID1 must be 0                                      
 
//
//  (21 - filetr)*PrevoiusADCValue + (filetr)*(ADCVALUE)
//  ----------------------------------------------------
//                               21
//
 
 
 adc_temp = adc_val>>12;   
 adc_temp &= 0x000FFFFF;
 
 
 
  
 adc_temp_ch0 = 21 - scale2_filetr_size;     
 adc_val = scale2_raw_value;
 adc_val = adc_val * adc_temp_ch0;
 
 scale2_raw_value = adc_temp;
 scale2_raw_value = scale2_raw_value * scale2_filetr_size;
 
 scale2_raw_value = scale2_raw_value +  adc_val;
 
 scale2_raw_value = scale2_raw_value / 21;
              
                 /*
 adc_temp = adc_temp * 9;
 scale2_raw_value = scale2_raw_value + adc_temp;                    //bn = (1/5)an-1  + (4/5)an
 scale2_raw_value = scale2_raw_value / 10;       */
 
// if(LC2_status&0x04)

 

  
// LC2_DI_OUT=1;
// LC2_DI_DDR = 0;             //make DOUR/rdy input
 
 return 0;       //0xFF means failed
  
}   
 


