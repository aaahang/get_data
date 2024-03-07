#ifndef DHT11_H
#define DHT11_H


//PA11



u8 DHT11_Init(void);//³õÊ¼»¯DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//¶ÁÈ¡Êý¾Ý
u8 DHT11_Read_Byte(void);//¶ÁÈ¡Ò»¸ö×Ö½Ú
u8 DHT11_Read_Bit(void);//¶ÁÈ¡Ò»Î»
u8 DHT11_Check(void);//¼ì²âDHT11
void DHT11_Rst(void);//¸´Î»DHT11   

#endif
