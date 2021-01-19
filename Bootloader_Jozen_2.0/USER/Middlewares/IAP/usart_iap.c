#include "usart_iap.h"

/*-----------------------------------------------内部函数------------------------------------------------------*/

//接收程序函数
static uint32_t Progress_Receive(void)
{
	/*--------------------------指令集-------------------------*/
	char rec[] = "数据接收完成\r\n";
	char trans[] = "请使用串口发送.bin文件!\r\n";	
	char error[] = "发送数据有误,请选择发送.bin文件\r\n";
	/*--------------------------------------------------------*/
	uint16_t oldcount=0;							//老的串口接收数据值
	uint32_t applenth=0;				    	//接收到的app代码长度
	HAL_UART_Transmit(&huart1,(uint8_t*)trans,sizeof(trans),MAX_WAIT_TIME);
	while(1)
	{
	 	if(USART_RX_CNT)
		{
			if(oldcount==USART_RX_CNT)	//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				if(USART_RX_CNT > 50)	//防止误操作
				{
					applenth=USART_RX_CNT;
					oldcount=0;
					USART_RX_CNT=0;
					HAL_UART_Transmit(&huart1,(uint8_t*)rec,sizeof(rec),MAX_WAIT_TIME);	//传输完成提示
					return applenth;
				}
				else
				{
					USART_RX_CNT=0;
					HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//错误提示
				}
			}
			else 
			{
				oldcount=USART_RX_CNT;			
			}
		}
		HAL_Delay(10);
	}	
}

//执行程序函数
void Usart_Execute_Progress(void)
{
	char start[] = "开始执行程序!\r\n";
	char error[] = "程序有误,无法执行!\r\n";
	
	if(((*(__IO uint32_t*)(FLASH_APP1_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
	{	 
		HAL_UART_Transmit(&huart1,(uint8_t*)start,sizeof(start),MAX_WAIT_TIME);	//执行程序提示
		iap_load_app(FLASH_APP1_ADDR);//执行FLASH APP代码
	}
	else
	{
		HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);	//执行程序提示
	}
}

//固件更新
void Usart_Hardware_Update(void)
{
	/*--------------------------指令集-------------------------*/
	char update[] = "固件更新完成!\r\n";
	/*--------------------------------------------------------*/
	uint32_t appsize;
	appsize = Progress_Receive();																								//接收程序
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)							//判断是否为0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,Progress_Buff,appsize);									//更新内部FLASH代码   
		HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//发送更新完成提示	
	}
}

/*------------------------------------------------API函数-----------------------------------------------------*/

//参数: 0 固件更新		1 执行程序
void Usart_Interaction(uint8_t flag)
{
	if(flag == 0) Usart_Hardware_Update();
	else Usart_Execute_Progress();
}
/*-------------------------------------------------------------------------------------------------------------*/
