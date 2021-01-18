#include "interaction.h"
#include "stdio.h"

//询问命令函数
//返回值: 0 更新固件 1 启动程序
static uint8_t Inquiry(void)
{
	char inquiry[] = "请输入命令\r\n0:更新固件\r\n1.启动程序\r\n";
	char error[] = "输入命令有误,请重新输入\r\n0:更新固件\r\n1.启动程序\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)inquiry,sizeof(inquiry),MAX_WAIT_TIME);
	while(1)
	{
		if(USART_RX_CNT)
		{
			USART_RX_CNT = 0;	//清零计数器
			if(Progress_Buff[0] == '0') return 0;																				//更新固件
			else if(Progress_Buff[0] == '1') return 1;																		//启动程序
			else HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);//错误提示
		}
		HAL_Delay(10);		
	}
}

//配置错误提示函数
static void Confige_Error(void)
{
	char error[] = "Bootloader配置参数有误,请检查源程序!\r\n退出程序!\r\n";
	HAL_UART_Transmit(&huart1,(uint8_t*)error,sizeof(error),MAX_WAIT_TIME);
}

//通过串口进行交互
void Interaction(void)
{
	uint8_t flag;
	while(1)
	{
		//接收命令
		flag = Inquiry();
		
		//执行操作
		if(USART_MODE)			Usart_Interaction(flag);
		else if(FLASH_MODE) Flash_Interaction(flag);
//		else if(SDCARD_MODE) SDcard_Interaction(flag);
		else 
		{
			Confige_Error();	
			break;
		}
	}
}


