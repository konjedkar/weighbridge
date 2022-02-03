
#define   LINE_FEED  10
#define   FORM_FEED  12
#define   RETURN     13
#define   ESC        27



#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7

#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)

// USART0 Transmitter buffer
#define TX_BUFFER_SIZE0 128
char tx_buffer0[TX_BUFFER_SIZE0];


unsigned int tx_wr_index0,tx_rd_index0,tx_counter0;


// USART0 Transmitter interrupt service routine
interrupt [USART0_TXC] void usart0_tx_isr(void)
{
if (tx_counter0)
   {
   --tx_counter0;
   UDR0=tx_buffer0[tx_rd_index0];
   if (++tx_rd_index0 == TX_BUFFER_SIZE0) tx_rd_index0=0;
   }    
else DIR=0;  //make transciever RECEIVER   
}


void putchar_usart0(char ch_data)
{
if (tx_counter0 == TX_BUFFER_SIZE0)return;

if (tx_counter0 || ((UCSR0A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer0[tx_wr_index0]=ch_data;
   if (++tx_wr_index0 == TX_BUFFER_SIZE0) tx_wr_index0=0;
   ++tx_counter0;
   }
else
   {
    DIR=1;
    UDR0=ch_data;
   }

}


void sprintf_usart0(unsigned char *str_data)
{
  unsigned char str_index=0;
  while(*(str_data+str_index))
   {
     putchar_usart0(*(str_data+str_index));
     str_index++;
   }
}


// USART1 Transmitter buffer
#define TX_BUFFER_SIZE1 128
char tx_buffer1[TX_BUFFER_SIZE1];

unsigned int tx_wr_index1,tx_rd_index1,tx_counter1;


// USART1 Transmitter interrupt service routine
interrupt [USART1_TXC] void usart1_tx_isr(void)
{
if (tx_counter1)
   {
   --tx_counter1;
   UDR1=tx_buffer1[tx_rd_index1];
   if (++tx_rd_index1 == TX_BUFFER_SIZE1) tx_rd_index1=0;
   }
 
}

void putchar_usart1(char ch_data)
{
 if(tx_counter1 == TX_BUFFER_SIZE1)return;

if (tx_counter1 || ((UCSR1A & DATA_REGISTER_EMPTY)==0))
   {
   tx_buffer1[tx_wr_index1]=ch_data;
   if (++tx_wr_index1 == TX_BUFFER_SIZE1) tx_wr_index1=0;
   ++tx_counter1;
   }
else
   {
    UDR1=ch_data;
   }
} 



void sprintf_usart1(unsigned char *str_data)
{
  unsigned char str_index=0;
  while(*(str_data+str_index))
   {
     putchar_usart1(*(str_data+str_index));
     str_index++;
   }
}



void config_USART(unsigned char port_no)
{
  if(port_no==1)
   {
      UCSR0A=0x00;
      UCSR0B=0x00;
      UCSR0C=0x00;
      UBRR0H=0x00;
      UBRR0L=0x00;      
      switch(com1_baud_rate)  
       {
         case 0:     //baud rate=300
           UBRR0H=0x0D;
           UBRR0L=0x04;
         break;
         case 1:     //baud rate=600
           UBRR0H=0x06;
           UBRR0L=0x82;
         break;
         case 2:     //baud rate=1200
           UBRR0H=0x03;
           UBRR0L=0x40;
         break;
         case 3:     //baud rate=2400
           UBRR0H=0x01;
           UBRR0L=0xA0;
         break;
         case 4:     //baud rate=4800
           UBRR0H=0x00;
           UBRR0L=0xCF;
         break;
         case 5:     //baud rate=9600
           UBRR0H=0x00;
           UBRR0L=0x67;
         break;
         case 6:     //baud rate=19200
           UBRR0H=0x00;
           UBRR0L=0x33;
         break;
       }
      switch(com1_parity)  
       {
         case 0:     //no parity
           UCSR0C |=0x00;
         break;   
         case 1:     //even parity
           UCSR0C |=0x20;
         break;   
         case 2:     //odd parity
           UCSR0C |=0x30;
         break;   
       }       
      switch(com1_stop_bits)  
       {
         case 0:     //1 stop
           UCSR0C |=0x00;
         break;   
         case 1:     //2 stop
           UCSR0C |=0x08;
         break;   
       }    
      switch(com1_databits)  
       {
         case 0:     //7 bit Char Size
           UCSR0C |=0x02;
         break;   
         case 1:     //8 bit Char Size
           UCSR0C |=0x06;
         break;   
       }  
       
      if(com1_func==PRINTER)UCSR0B=0x48; //printer                     
      if(com1_func==REMOTE_S1)UCSR0B=0x48; //remote
      if(com1_func==PCWORK_S1)UCSR0B=0x48; //pc work out  
      if(com1_func==REMOTE_S2)UCSR0B=0x48; //remote
      if(com1_func==PCWORK_S2)UCSR0B=0x48; //pc work out      
      if(com1_func==NOTUSE)UCSR0B=0x00; //not used
      
      if(com1_func!=NOTUSE)
       {
         #asm("sei")
       } 
                            
   }

 if(port_no==2)
   {
      UCSR1A=0x00;
      UCSR1B=0x00;
      UCSR1C=0x00;
      UBRR1H=0x00;
      UBRR1L=0x00;      
      switch(com2_baud_rate)  
       {
         case 0:     //baud rate=300
           UBRR1H=0x0D;
           UBRR1L=0x04;
         break;
         case 1:     //baud rate=600
           UBRR1H=0x06;
           UBRR1L=0x82;
         break;
         case 2:     //baud rate=1200
           UBRR1H=0x03;
           UBRR1L=0x40;
         break;
         case 3:     //baud rate=2400
           UBRR1H=0x01;
           UBRR1L=0xA0;
         break;
         case 4:     //baud rate=4800
           UBRR1H=0x00;
           UBRR1L=0xCF;
         break;
         case 5:     //baud rate=9600
           UBRR1H=0x00;
           UBRR1L=0x67;
         break;
         case 6:     //baud rate=19200
           UBRR1H=0x00;
           UBRR1L=0x33;
         break;
       }
      switch(com2_parity)  
       {
         case 0:     //no parity
           UCSR1C |=0x00;
         break;   
         case 1:     //even parity
           UCSR1C |=0x20;
         break;   
         case 2:     //odd parity
           UCSR1C |=0x30;
         break;   
       }       
      switch(com2_stop_bits)  
       {
         case 0:     //1 stop
           UCSR1C |=0x00;
         break;   
         case 1:     //2 stop
           UCSR1C |=0x08;
         break;   
       }    
      switch(com2_databits)  
       {
         case 0:     //7 bit Char Size
           UCSR1C |=0x02;
         break;   
         case 1:     //8 bit Char Size
           UCSR1C |=0x06;
         break;                                                          
       }  
       
      if(com2_func==PRINTER)UCSR1B=0x48; //printer
      if(com2_func==REMOTE_S1)UCSR1B=0x48; //remote
      if(com2_func==PCWORK_S1)UCSR1B=0x48; //pc work out  
      if(com2_func==REMOTE_S2)UCSR1B=0x48; //remote
      if(com2_func==PCWORK_S2)UCSR1B=0x48; //pc work out      
      if(com2_func==NOTUSE)UCSR1B=0x00; //not used
      
      if(com2_func!=NOTUSE)
       {
         #asm("sei")
       } 
                            
   }


}


 
void send_byte_to_printer(char ch_data)
{
   
    if(com2_func==PRINTER)putchar_usart1(ch_data);
    else if(com1_func==PRINTER)putchar_usart0(ch_data);  
   
}




void send_str_to_printer(unsigned char *str_data)
{
   if(com2_func==PRINTER)sprintf_usart1(str_data);
   else if(com1_func==PRINTER)sprintf_usart0(str_data);  
}

          
            /*
void sort_print_data(void)
{
  unsigned char field_index;  
  unsigned int location,abs_loc[NUMBER_OF_FIELDS+1],spd3,spd4,spd2;
  
  for(field_index=1;field_index<=NUMBER_OF_FIELDS;field_index++)
   {
     location = field[field_index].line;     
     if(location>200 || location==0)
        {abs_loc[field_index-1]=0xFFFF;continue;}
     location--;
     location *=80;
     spd4 = field[field_index].column;
     if(spd4>80 || spd4==0)
        {abs_loc[field_index-1]=0xFFFF;continue;} 
     location += spd4;    
     abs_loc[field_index-1] = location;       //put the absolute location of all fields in abs_loc[] -- 0xffff means N/A
   }
  
  spd4=0;
  spd2 = 0xFFFF;              //spd2 is minimum
  while(spd4<NUMBER_OF_FIELDS)
  {
    spd2 = 0xFFFF; 
    spd3=0xFF;
    for(field_index=0;field_index<NUMBER_OF_FIELDS;field_index++)
     {
       if(abs_loc[field_index]<spd2){spd3=field_index;spd2=abs_loc[field_index];}
     }    
    if(spd3!=0xFF)
     {
       abs_loc[spd3]=0xFFFF;
       sorted_field_table[spd4]=spd3;
     } 
    else sorted_field_table[spd4]=0xFF; 
    spd4++;
  }
}                       */

                  
void sort_print_data(void)
{
  unsigned char field_index;  
  unsigned int location,abs_loc[NUMBER_OF_FIELDS+1],spd3,spd4,spd2;
  
  for(field_index=0;field_index<NUMBER_OF_FIELDS;field_index++)
   {
     location = field[field_index].line;     
     if(location>200 || location==0)
        {abs_loc[field_index]=0xFFFF;continue;}
     location--;
     location *=80;
     spd4 = field[field_index].column;
     if(spd4>80 || spd4==0)
        {abs_loc[field_index]=0xFFFF;continue;} 
     location += spd4;    
     abs_loc[field_index] = location;       //put the absolute location of all fields in abs_loc[] -- 0xffff means N/A
   }
  
  spd4=0;
  spd2 = 0xFFFF;              //spd2 is minimum
  while(spd4<NUMBER_OF_FIELDS)
  {
    spd2 = 0xFFFF; 
    spd3=0xFF;
    for(field_index=0;field_index<NUMBER_OF_FIELDS;field_index++)
     {
       if(abs_loc[field_index]<spd2){spd3=field_index;spd2=abs_loc[field_index];}
     }    
    if(spd3!=0xFF)
     {
       abs_loc[spd3]=0xFFFF;
       sorted_field_table[spd4]=spd3;
     } 
    else sorted_field_table[spd4]=0xFF; 
    spd4++;
  }
}      
                

   
     
void print_sort_file(void)
{
    unsigned char temp_str_psf2[8],temp_str_psf[32],temp_psf;   
    
    
    sprintf(temp_str_psf,"\n\n\n\n\n\r     Saved Sort File Report\r\n");    
    send_str_to_printer(temp_str_psf);   

    sprintf(temp_str_psf,"   ----------------------------\r\n");    
    send_str_to_printer(temp_str_psf);            
      
    
    for(temp_psf=1;temp_psf<=NUM_OF_SORT_NAMES;temp_psf++)
     {
      
      read_sort_name(temp_str_psf,temp_psf-1);  
      if(*(temp_str_psf)=='0')continue;
      sprintf(temp_str_psf2,"\r\n   %3d  ->  ",temp_psf);    
      send_str_to_printer(temp_str_psf2);
      send_str_to_printer(temp_str_psf);  
      delay_ms(100);
      #asm("wdr")
      
     }

  send_byte_to_printer(FORM_FEED);
}  
  
  

void print_fixed_tare(void)
{
    unsigned char temp_str_psf[8],temp_str_psf2[32],temp_psf;    
    
    unsigned long int temp_lft;
    
    
    sprintf(temp_str_psf2,"\n\n\n\n\n\r   Fixed Tares File Report\r\n");    
    send_str_to_printer(temp_str_psf2);   

    sprintf(temp_str_psf2,"   ----------------------------\r\n");    
    send_str_to_printer(temp_str_psf2);            
      
    
    for(temp_psf=1;temp_psf<=FIXED_TARE_INBOUND_CODES;temp_psf++)
     {


                         temp_lft=0;      
                         temp_lft = fixed_tare[temp_psf-1].fixed_tare_msb;        
                         temp_lft<<=8;
                         temp_lft |= fixed_tare[temp_psf-1].fixed_tare_mid;
                         temp_lft<<=8;
                         temp_lft |= fixed_tare[temp_psf-1].fixed_tare_lsb;       
                         
                         if(temp_lft==0)continue;

      sprintf(temp_str_psf2,"\r\n    FixedTare No. %3d  ->  ",temp_psf);    
      send_str_to_printer(temp_str_psf2);    
         
      if(scales_func_mode==2)
         {
           strcpyf(temp_str_psf,scale_unit_table[scale2_unit]);
           convert_num_point_str(temp_str_psf2,temp_lft,2,0,0); 
         }
      else
         {
           strcpyf(temp_str_psf,scale_unit_table[scale1_unit]);
           convert_num_point_str(temp_str_psf2,temp_lft,1,0,0);        
         }      
      
      send_str_to_printer(temp_str_psf2);   
      send_byte_to_printer(' ');
      send_str_to_printer(temp_str_psf);  
      delay_ms(100);
      #asm("wdr")
      
     }

  send_byte_to_printer(FORM_FEED);

}




void print_data_block(unsigned char block_no)
{
   unsigned int  row_cntr=0;
   unsigned char field_index=0,current_field_no,pdb4,column_cntr; 
                                               
   unsigned char dhp1,dhp2,temp_str_pdb[48],dhp3,dhp4,dhp5,temp_str_pdb2[8],temp_str_pdb3[8];   
   


   row_cntr=1;
   send_byte_to_printer(RETURN);   
   column_cntr=1;   //column_cntr column
   while(1)
       {
          current_field_no = sorted_field_table[field_index];   //field index
          if(current_field_no==0xff)break;
          
     //     if(row_cntr>10)break;
          
          if(field[current_field_no].block!=block_no)
           {
              field_index++;
              continue;
           }
          else 
          if(field[current_field_no].line==row_cntr)
           {
             //print field according to attr. 
             
             pdb4 = field[current_field_no].column;
             while(column_cntr<pdb4)
              {
               send_byte_to_printer(' ');
               column_cntr++;
              }
             column_cntr = pdb4;
             
             if(field[current_field_no].attribute==0)sprintf(temp_str_pdb,"%cE",ESC);  //bold
             if(field[current_field_no].attribute==1)sprintf(temp_str_pdb,"%c-1",ESC);  //underlined
             if(field[current_field_no].attribute==3)sprintf(temp_str_pdb,"%cG",ESC);  //expand
             if(field[current_field_no].attribute==2)sprintf(temp_str_pdb,"%cW1",ESC);   //condensed
             if(field[current_field_no].attribute==4)sprintf(temp_str_pdb,"%c4",ESC);  //italic
             if(field[current_field_no].attribute==5)sprintf(temp_str_pdb,"%cE",ESC);  //special1
             if(field[current_field_no].attribute==6)sprintf(temp_str_pdb,"%cE",ESC);  //special2

             if(field[current_field_no].attribute<7)send_str_to_printer(temp_str_pdb);
                 
             if(field[current_field_no].type==0)
              {
                 //fetch
                 dhp1 = field[current_field_no].data_index;   
                 if(dhp1==0)  //hijri data value
                   {
                          if(day_print==0 && month_print==0 && year_print==0){day_print=day;month_print=month;year_print=year;}
                          jalali(day_print,month_print,year_print,&dhp3,&dhp4,&dhp5);
                          if(dhp5<50)sprintf(temp_str_pdb2,"14");
                            else sprintf(temp_str_pdb2,"13");
                          sprintf(temp_str_pdb,"%s%02d/%02d/%02d",temp_str_pdb2,dhp5,dhp4,dhp3);  
                          day_print=0;month_print=0; year_print=0;                
                   }  
                  else 
                  if(dhp1==1)    //date
                   {
                         if(day_print==0 && month_print==0 && year_print==0){day_print=day;month_print=month;year_print=year;}
                         sprintf(temp_str_pdb,"20%02d/%02d/%02d",year_print,month_print,day_print); 
                         day_print=0;month_print=0; year_print=0;   
                   
                   }
                  else
                  if(dhp1==2)  //time
                   {
                        sprintf(temp_str_pdb,"%02d:%02d:%02d",hour_print,minute_print,second_print);
                   
                   }   
                  else
                  if(dhp1==3)  //conseq1_serial
                   {
                        printf_longnum(temp_str_pdb,weighing_data.conseq1_serial,5); 
                   }                       
                  else             
                  if(dhp1==6)  //Tare   
                   {                                                         
                     if(basic_weighing_state) 
                     {
                       if(using_scale==1)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale1_unit]);   
                             if(temp_inbound_code)strcatf(temp_str_pdb3," FT");
                             else strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_first_weigh,1,0,0);      
                           } 
                       else      
                       if(using_scale==2)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale2_unit]);   
                             if(temp_inbound_code)strcatf(temp_str_pdb3," FT");
                             else strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_first_weigh,2,0,0);                             
                           }
                       else
                          {temp_str_pdb2[0]=0;temp_str_pdb3[0]=0;}
                           sprintf(temp_str_pdb,"%s %s",temp_str_pdb2,temp_str_pdb3);                     
                     }
                   }                    
                  else                     
                  if(dhp1==7)  //Net   
                   {                                                         
                     if(basic_weighing_state || second_weighing_state) 
                     {
                       if(using_scale==1)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale1_unit]);   
                             strcatf(temp_str_pdb3," C");
                             convert_num_point_str(temp_str_pdb2,temp_reg_total_weigh,1,0,0);
                           } 
                       else      
                       if(using_scale==2)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale2_unit]);   
                             strcatf(temp_str_pdb3," C");
                             convert_num_point_str(temp_str_pdb2,temp_reg_total_weigh,2,0,0);   
                           } 
                       else
                          {temp_str_pdb2[0]=0;temp_str_pdb3[0]=0;}
                           sprintf(temp_str_pdb,"%s %s",temp_str_pdb2,temp_str_pdb3);                     
                     }
                   }                    
                  else    
                  if(dhp1==8)  //Unit   
                   { 
                       if(using_scale==1)
                           {
                             sprintf(temp_str_pdb,"%s",scale_unit_table[scale1_unit]);
                           } 
                       else      
                       if(using_scale==2)
                           {
                             sprintf(temp_str_pdb,"%s",scale_unit_table[scale2_unit]);
                           } 
                       else
                          {temp_str_pdb[0]=0;}

                   
                   }
                  else                                 
                  if(dhp1==10)  //1st weight   
                   {                                                         
                     if(using_scale==1)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale1_unit]);   
                             if(manual_tare_s1_state || pb_tare_s1_state)strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_first_weigh,1,0,0);

                           } 
                     else      
                     if(using_scale==2)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale2_unit]);   
                             if(manual_tare_s2_state || pb_tare_s2_state)strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_first_weigh,2,0,0);
                            } 
                     else
                        {temp_str_pdb2[0]=0;temp_str_pdb3[0]=0;}
                     sprintf(temp_str_pdb,"%s %s",temp_str_pdb2,temp_str_pdb3);                     
                   }     
                  else 
                  if(dhp1==11)  //2nd weight   
                   {                                                         
                     if(using_scale==1)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale1_unit]);   
                             if(manual_tare_s1_state || pb_tare_s1_state)strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_second_weigh,1,0,0);
                           } 
                     else      
                     if(using_scale==2)
                           {
                             strcpyf(temp_str_pdb3,scale_unit_table[scale2_unit]);   
                             if(manual_tare_s2_state || pb_tare_s2_state)strcatf(temp_str_pdb3," PT");
                             convert_num_point_str(temp_str_pdb2,temp_reg_second_weigh,2,0,0);
                            } 
                     else
                        {temp_str_pdb2[0]=0;temp_str_pdb3[0]=0;}
                     sprintf(temp_str_pdb,"%s %s",temp_str_pdb2,temp_str_pdb3);                     
                   }     
                  else                  
                  if(dhp1==12)  // temp_truck_id
                   {                                          
                      printf_longnum(temp_str_pdb,temp_truck_id,5); 
                   }  
                  else
                  if(dhp1==13)  //  inbound
                   {                     
                     sprintf(temp_str_pdb,"%d",temp_inbound_code+1);
                   }  
                  else
                  if(dhp1==14)  //  customer
                   {                     
                     sprintf(temp_str_pdb,"%s",temp_customer_name);
                   }                 
                  else
                  if(dhp1==15)  //sort                   
                   {
                     sprintf(temp_str_pdb2," (%d)",temp_sort_code);  
                     read_sort_name(temp_str_pdb,temp_sort_code-1); 
                     strcat(temp_str_pdb,temp_str_pdb2); 
                   } 
                  else
                  if(dhp1==16)  //scale#                   
                   {
                             sprintf(temp_str_pdb,"%d",using_scale);
                   }                   
                   
                   
                  if(dhp1<NUMBER_OF_FETCH_TEXTS)
                   {
                     send_str_to_printer(temp_str_pdb);   
                     column_cntr +=strlen(temp_str_pdb);                   
                   }
              } 
             else
             if(field[current_field_no].type==1)
              {
                 //input
              }
             else
             if(field[current_field_no].type==2)
              {
                 //calc
              }    
             else
             if(field[current_field_no].type==3)
              {
                 //text   
                 if(field[current_field_no].data_index<NUMBER_OF_EMBED_TEXTS)strcpyf(temp_str_pdb,texts_table[field[current_field_no].data_index]);
                 else
                   {
                     dhp1=0;dhp2=0;
                     while(1)
                      {
                        dhp2=*(user_defines_texts_table[field[current_field_no].data_index-NUMBER_OF_EMBED_TEXTS]+dhp1);
                        temp_str_pdb[dhp1]=dhp2;
                        if(dhp2==0)break;
                        if(++dhp1>USER_TEXTS_LENGHT){temp_str_pdb[dhp1]=0;break;}
                      }
                   }
                 send_str_to_printer(temp_str_pdb);   
                 column_cntr +=strlen(temp_str_pdb);
                 
              }                         
                              
                           
             if(field[current_field_no].attribute==0)sprintf(temp_str_pdb,"%cF",ESC);  //bold
             if(field[current_field_no].attribute==1)sprintf(temp_str_pdb,"%c-0",ESC);  //underlined
             if(field[current_field_no].attribute==3)sprintf(temp_str_pdb,"%cH",ESC);  //expand
             if(field[current_field_no].attribute==2)sprintf(temp_str_pdb,"%cW0",ESC);   //condensed
             if(field[current_field_no].attribute==4)sprintf(temp_str_pdb,"%c5",ESC);  //italic
             if(field[current_field_no].attribute==5)sprintf(temp_str_pdb,"%cF",ESC);  //special1
             if(field[current_field_no].attribute==6)sprintf(temp_str_pdb,"%cF",ESC);  //special2

             if(field[current_field_no].attribute<7)send_str_to_printer(temp_str_pdb);

             
             field_index++;  
             continue;
           } 
          else
           {
             column_cntr=1; //column cntr
             row_cntr++;
             send_byte_to_printer(RETURN);
             send_byte_to_printer(LINE_FEED);     
             delay_ms(100); 
             #asm("wdr")
           }



       }   //while(1)
        
        send_byte_to_printer(FORM_FEED);
        return;

}


void sendout_pcwork_data_s1(unsigned char usartnum)
{
   unsigned char  temp_dw1[30],temp_dw2[20],temp_dw3[20],spd1,spd2;

     //*******************scale 1 ***********************//
     
     sprintf(temp_dw1,"");
     
     if( (usartnum==0 && com1_protocol==OFF) || (usartnum==1 && com2_protocol==OFF) )sprintf(temp_dw1,"");        
     
     if( (usartnum==0 && com1_protocol==SYSTEC) || (usartnum==1 && com2_protocol==SYSTEC) ) 
      {
       convert_num_point_str(temp_dw2,scale1_value,1,0,scale1_sign);   
       sprintf(temp_dw3,"          "); 
       spd2=strlen(temp_dw2);  
       for(spd1=9;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[10]=0;  
       if(scale1_mode==0)spd1='D';else spd1=' ';  
       if(scale1_unit==0)sprintf(temp_dw2,"g "); 
       else if(scale1_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale1_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"S%c %s%c%c",spd1,temp_dw3,0x67,0x0D);
      }       

     if( (usartnum==0 && com1_protocol==SCHENCK) || (usartnum==1 && com2_protocol==SCHENCK) ) 
      {
       convert_num_point_str(temp_dw2,scale1_value,1,0,scale1_sign);   
       sprintf(temp_dw3,"         "); 
       spd2=strlen(temp_dw2);  
       for(spd1=8;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[9]=0;  
       if(scale1_mode=='z')spd1='9';else spd1='1';  
       if(scale1_unit==0)sprintf(temp_dw2,"g "); 
       else if(scale1_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale1_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"01%s%s8%c37%c%c%c",temp_dw3,temp_dw2,spd1,0x09,0x0A,0x0D);
      }  
      
     if( (usartnum==0 && com1_protocol==DDP8785) || (usartnum==1 && com2_protocol==DDP8785) ) 
      {
       convert_num_point_str(temp_dw2,scale1_value,1,0,scale1_sign);   
       sprintf(temp_dw3,"         "); 
       spd2=strlen(temp_dw2);  
       for(spd1=8;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[9]=0;  
       if(scale1_mode=='z')spd1='9';else spd1='1';  
       if(scale1_unit==0)sprintf(temp_dw2,"g ");                        
       else if(scale1_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale1_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"01%s%s8%c3;%c%c%c",temp_dw3,temp_dw2,spd1,0x09,0x0A,0x0D);
      }              

      
     if( (usartnum==0 && com1_protocol==FILINTEC) ||  (usartnum==1 && com2_protocol==FILINTEC) ) 
      {
       convert_num_point_str(temp_dw2,scale1_value,1,0,scale1_sign);   
       sprintf(temp_dw3,"       "); 
       spd2=strlen(temp_dw2);  
       for(spd1=6;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[7]=0;  
       sprintf(temp_dw1,"%c%s%c",0x40,temp_dw3,0x0D);
      }      
   
     if( (usartnum==0 && com1_protocol==TOLIDO) ||  (usartnum==1 && com2_protocol==TOLIDO) ) 
      {
       convert_num_point_str(temp_dw2,scale1_value,1,0,scale1_sign);   
       sprintf(temp_dw3,"       "); 
       spd2=strlen(temp_dw2);  
       for(spd1=6;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[7]=0;  
       sprintf(temp_dw1,"%c%c%c%c%s 00000%c",0x4F,0x02,0x32,0x30,temp_dw3,0x0D);
       
      }     

     if(usartnum==0)sprintf_usart0(temp_dw1);   
     if(usartnum==1)sprintf_usart1(temp_dw1);
     
}



void sendout_pcwork_data_s2(unsigned char usartnum)
{
   unsigned char  temp_dw1[30],temp_dw2[20],temp_dw3[20],spd1,spd2;

     //*******************scale 2 ***********************//
     
     sprintf(temp_dw1,"");
     
     if( (usartnum==0 && com1_protocol==OFF) || (usartnum==1 && com2_protocol==OFF) )sprintf(temp_dw1,"");
     if( (usartnum==0 && com1_protocol==SYSTEC) || (usartnum==1 && com2_protocol==SYSTEC) ) 
      {
       convert_num_point_str(temp_dw2,scale2_value,2,0,scale2_sign);   
       sprintf(temp_dw3,"          "); 
       spd2=strlen(temp_dw2);  
       for(spd1=9;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[10]=0;  
       if(scale2_mode==0)spd1='D';else spd1=' ';  
       if(scale2_unit==0)sprintf(temp_dw2,"g "); 
       else if(scale2_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale2_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"S%c %s%c%c",spd1,temp_dw3,0x67,0x0D);
      }     
      
     if( (usartnum==0 && com1_protocol==SCHENCK) || (usartnum==1 && com2_protocol==SCHENCK) ) 
      {
       convert_num_point_str(temp_dw2,scale2_value,2,0,scale2_sign);   
       sprintf(temp_dw3,"         "); 
       spd2=strlen(temp_dw2);  
       for(spd1=8;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[9]=0;  
       if(scale2_mode=='z')spd1='9';else spd1='1';  
       if(scale2_unit==0)sprintf(temp_dw2,"g "); 
       else if(scale2_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale2_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"02%s%s8%c37%c%c%c",temp_dw3,temp_dw2,spd1,0x09,0x0A,0x0D);
      }  
      
     if( (usartnum==0 && com1_protocol==DDP8785) || (usartnum==1 && com2_protocol==DDP8785) ) 
      {
       convert_num_point_str(temp_dw2,scale2_value,2,0,scale2_sign);   
       sprintf(temp_dw3,"         "); 
       spd2=strlen(temp_dw2);  
       for(spd1=8;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[9]=0;  
       if(scale2_mode=='z')spd1='9';else spd1='1';  
       if(scale2_unit==0)sprintf(temp_dw2,"g ");                        
       else if(scale2_unit==1)sprintf(temp_dw2,"Kg");
       else if(scale2_unit==2)sprintf(temp_dw2,"t ");        
       sprintf(temp_dw1,"02%s%s8%c3;%c%c%c",temp_dw3,temp_dw2,spd1,0x09,0x0A,0x0D);
      }              
      
     if( (usartnum==0 && com1_protocol==FILINTEC) ||  (usartnum==1 && com2_protocol==FILINTEC) ) 
      {
       convert_num_point_str(temp_dw2,scale2_value,2,0,scale2_sign);      
       sprintf(temp_dw3,"       "); 
       spd2=strlen(temp_dw2);  
       for(spd1=6;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[7]=0;  
       sprintf(temp_dw1,"%c%s%c",0x40,temp_dw3,0x0D);
      }      
   
     if( (usartnum==0 && com1_protocol==TOLIDO) ||  (usartnum==1 && com2_protocol==TOLIDO) ) 
      {
       convert_num_point_str(temp_dw2,scale2_value,2,0,scale2_sign);   
       sprintf(temp_dw3,"       "); 
       spd2=strlen(temp_dw2);  
       for(spd1=6;;spd1--){temp_dw3[spd1]= temp_dw2[--spd2];if(spd2==0)break;}
       temp_dw3[7]=0;  
       sprintf(temp_dw1,"%c%c%c%c%s 00000%c",0x4F,0x02,0x32,0x30,temp_dw3,0x0D);
      }     

     if(usartnum==0)sprintf_usart0(temp_dw1);   
     if(usartnum==1)sprintf_usart1(temp_dw1);
     
}