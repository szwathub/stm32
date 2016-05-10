#include "stm32f10x.h"

/*******************************************************************************

����оƬ����

*******************************************************************************/

#define TP_CS()  GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define TP_DCS() GPIO_SetBits(GPIOB,GPIO_Pin_12)

u16 TPReadX(void);
u16 TPReadY(void);

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: SPI_Config 
** ��������: ������SPI�ӿ�����
** ������������
** ��  ����: ��һ�ؿƼ�  
** �ա�  ��: 2015��3��20��
** �Ա����̣�http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void SPI_Config(void) 	  //������SPI�ӿ�
{ 
		  GPIO_InitTypeDef  GPIO_InitStructure; 
		  SPI_InitTypeDef   SPI_InitStructure; 
		
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
		  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE) ;
		
		  //Configure SPI2 pins: SCK, MISO and MOSI 
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15; 
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�������
		  GPIO_Init(GPIOB,&GPIO_InitStructure);  
		
		  //Configure SPI2 pins: SCK, MISO and MOSI 
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //����
		  GPIO_Init(GPIOB,&GPIO_InitStructure);  

		  //Configure PB2 pin: TP_CS pin 
		  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
		  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
		  GPIO_Init(GPIOB,&GPIO_InitStructure); 

		  // SPI2 Config
		  SPI_Cmd(SPI2, DISABLE);     
		  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
		  SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
		  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
		  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
		  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
		  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
		  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; 
		  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
		  SPI_InitStructure.SPI_CRCPolynomial = 7; 
		  SPI_Init(SPI2,&SPI_InitStructure); 
		
		  // SPI2 enable  
		  SPI_Cmd(SPI2,ENABLE);  
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: SPI_WriteByte 
** ��������: 
** ������������
** ��  ����: ��һ�ؿƼ�  
** �ա�  ��: 2015��3��20��
** �Ա����̣�http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
unsigned char SPI_WriteByte(unsigned char data) 
{ 
		unsigned char Data = 0; 
		
		//Wait until the transmit buffer is empty 
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); 
		// Send the byte  
		SPI_I2S_SendData(SPI2,data); 
		
		 //Wait until a data is received 
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET); 
		// Get the received data 
		Data = SPI_I2S_ReceiveData(SPI2); 
		
		// Return the shifted data 
		return Data; 
} 
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: SpiDelay(unsigned int DelayCnt) 
** ��������: 
** ������������
** ��  ����: ��һ�ؿƼ�  
** �ա�  ��: 2015��3��20��
** �Ա����̣�http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/ 
void SpiDelay(unsigned int DelayCnt)
{
		unsigned int i;
		for(i=0;i<DelayCnt;i++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: SpiDelay(unsigned int DelayCnt) 
** ��������: ��X��
** ������������
** ��  ����: ��һ�ؿƼ�  
** �ա�  ��: 2015��3��20��
** �Ա����̣�http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/ 
u16 TPReadX(void)
{ 
	   u16 x=0;
	   TP_CS();
	   SpiDelay(10);
	   SPI_WriteByte(0x90);
	   SpiDelay(10);
	   x=SPI_WriteByte(0x00);
	   x<<=8;
	   x+=SPI_WriteByte(0x00);
	   SpiDelay(10);
	   TP_DCS(); 
	   x = x>>3;
	   return (x);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: SpiDelay(unsigned int DelayCnt) 
** ��������: ��Y��
** ������������
** ��  ����: ��һ�ؿƼ�  
** �ա�  ��: 2015��3��20��
** �Ա����̣�http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/ 
u16 TPReadY(void)
{
	  u16 y=0;
	  TP_CS();
	  SpiDelay(10);
	  SPI_WriteByte(0xD0);
	  SpiDelay(10);
	  y=SPI_WriteByte(0x00);
	  y<<=8;
	  y+=SPI_WriteByte(0x00);
	  SpiDelay(10);
	  TP_DCS();
	  y = y>>3; 
	  return (y);
}