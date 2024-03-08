/***************STM32F103C8T6**********************
 * ÎÄ¼þÃû  £ºDHT11.c
 * ÃèÊö    £ºDHT11´«¸ÐÆ÷
 * ±¸×¢    : DHT11ÎÂ¶ÈÊª¶È´«¸ÐÆ÷
 * ½Ó¿Ú    £ºPA11-DATA

********************LIGEN*************************/
#include "dht11.h"
#include "linux/printk.h"

extern int tem1;
#define DHT11_IO_IN()   gpio_direction_input(tem1)
#define DHT11_IO_OUT()  gpio_direction_output(tem1,0)

#define	DHT11_DQ_OUT(n)  gpio_set_value(tem1,n)
#define	DHT11_DQ_IN  	 gpio_get_value(tem1)

#define Delay_ms(n) 	mdelay(n)
#define Delay_us(n) 	udelay(n)

//¸´Î»DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
	DHT11_DQ_OUT(0); 	//À­µÍDQ
	Delay_ms(20);    	//À­µÍÖÁÉÙ18ms
	DHT11_DQ_OUT(1); 	//DQ=1 
	Delay_us(30);     	//Ö÷»úÀ­¸ß20~40us
}

//µÈ´ýDHT11µÄ»ØÓ¦
//·µ»Ø1:Î´¼ì²âµ½DHT11µÄ´æÔÚ
//·µ»Ø0:´æÔÚ
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11»áÀ­µÍ40~80us
	{
		retry++;
		Delay_us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11À­µÍºó»áÔÙ´ÎÀ­¸ß40~80us
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//µÈ´ý±äÎªµÍµçÆ½
	{
		retry++;
		Delay_us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//µÈ´ý±ä¸ßµçÆ½
	{
		retry++;
		Delay_us(1);
	}
	Delay_us(40);//µÈ´ý40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//´ÓDHT11¶ÁÈ¡Ò»¸ö×Ö½Ú
//·µ»ØÖµ£º¶Áµ½µÄÊý¾Ý
u8 DHT11_Read_Byte(void)    
{        
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++) 
	{
		dat<<=1; 
		dat|=DHT11_Read_Bit();
	}						    
	return dat;
}

u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//¶ÁÈ¡40Î»Êý¾Ý
		{
			buf[i]=DHT11_Read_Byte();
			
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
			printk("buf[0] is %d buf[1] is %d buf[2] is %d buf[3] is %d \n",buf[0],buf[1],buf[2],buf[3]);
		}
	}
	else return 1;
	return 0;	    
}

