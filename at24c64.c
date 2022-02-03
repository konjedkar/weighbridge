#include <i2c.h>

void read_e2prom_page(unsigned char *datablock,unsigned int address)
{
unsigned char address_H,address_L,reph;   

address_H =  (unsigned char)(address>>8);
address_L =  (unsigned char)(address);

i2c_start();
i2c_write(0xA0);
i2c_write(address_H);
i2c_write(address_L);     
i2c_start();
i2c_write(0xA1);
for(reph=0;reph<32;reph++)
     {*(datablock+reph)=i2c_read(1);}
*(datablock+31)=i2c_read(0);     
i2c_stop();

delay_ms(5);
}

void write_e2prom_page(unsigned char *datablock,unsigned int address,unsigned char len)
{
unsigned char address_H,address_L,reph;   

address_H =  (unsigned char)(address>>8);
address_L =  (unsigned char)(address);

i2c_start();
i2c_write(0xA0);
i2c_write(address_H);
i2c_write(address_L);
for(reph=0;reph<len;reph++)i2c_write(*(datablock+reph));
i2c_stop();     

delay_ms(5);
}

unsigned char read_e2prom_byte(unsigned int address)
{
unsigned char address_H,address_L,reph;   

address_H =  (unsigned char)(address>>8);
address_L =  (unsigned char)(address);

i2c_start();
i2c_write(0xA0);
i2c_write(address_H);
i2c_write(address_L);
i2c_start();
i2c_write(0xA1);
reph=i2c_read(0);     
i2c_stop();  

delay_ms(2);

return reph;
}

void write_e2prom_byte(unsigned char databyte,unsigned int address)
{
unsigned char address_H,address_L;   

address_H =  (unsigned char)(address>>8);
address_L =  (unsigned char)(address);

i2c_start();
i2c_write(0xA0);
i2c_write(address_H);
i2c_write(address_L);
i2c_write(databyte);      
i2c_stop();     

delay_ms(2);

}


