#include "interaction.h"

/*----------------------------重新定义延迟函数--------------------------*/
void HAL_Delay(uint32_t Delay)
{
	int i, j;
	for(i = 0; i < Delay; i++)
	{
		for(j = 0; j < 168000; j++);
	}
}

/*-----------------------交互命令-------------------------*/




//询问命令函数
//返回值: 0 更新固件 1 启动程序
static uint8_t Inquiry_Usart(void)
{
	char inquiry[] = "请输入命令\r\n0:更新固件\r\n1.启动程序\r\n";
	char error[] = "输入命令有误,请重新输入\r\n0:更新固件\r\n1.启动程序\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)inquiry,sizeof(inquiry),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT)
		{
			USART_RX_CNT = 0;	//清零计数器
			if(USART_RX_BUF[0] == '0') return 0;																				//更新固件
			else if(USART_RX_BUF[0] == '1') return 1;																		//启动程序
			else HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);//错误提示
		}
		HAL_Delay(10);		
	}
}

//固件更新
static void Hardware_Update(uint8_t *appbuf,uint32_t appsize)
{
	char update[] = "固件更新完成!\r\n";
	if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)							//判断是否为0X08XXXXXX.
	{	 
		iap_write_appbin(FLASH_APP1_ADDR,appbuf,appsize);										//更新FLASH代码   
		HAL_UART_Transmit(&huart1,(uint8_t*)update,sizeof(update),MAX_WAIT_TIME);	//发送更新完成提示	
	}
}

//程序接收函数
//返回值: 接收的程序的大小
static uint32_t Progress_Receive(void)
{
	char rec[] = "用户接收完成\r\n";
	
	/*-----------------串口接收------------------*/
	#ifdef USART_MODE
	uint16_t oldcount=0;						//老的串口接收数据值
	uint32_t applenth=0;				    //接收到的app代码长度
	char trans[] = "请使用串口发送.bin文件!\r\n";	
	char error[] = "发送数据有误,请选择发送.bin文件\r\n";
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
	#endif
	
//	#ifdef SDCARD_MODE
//	
//	#endif
//	
//	#ifdef EXTERNAL_FLASH_MODE
//	
//	#endif
}

//执行程序函数
static void Execute_Progress(void)
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

#ifdef USART_MODE
void Interaction_Usart(void)
{
	uint8_t flag;
	while(1)
	{
		//接收命令
		flag = Inquiry_Usart();
		
		//执行操作
		if(flag == 0) Hardware_Update(USART_RX_BUF, Progress_Receive());
		else Execute_Progress();
	}
}
#endif

