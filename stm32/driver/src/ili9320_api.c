/*******************************************************************************

GUI��ʾ����

*******************************************************************************/

#include "stm32f10x.h"
#include "ili9320.h"
#include "HzLib_65k.h"  //�ֿ�

/****************************************************************************
* ��    �ƣ�GUI_CmpColor()
* ��    �ܣ��ж���ɫֵ�Ƿ�һ�¡�
* ��ڲ�����color1		��ɫֵ1
*		        color2		��ɫֵ2
* ���ڲ���������1��ʾ��ͬ������0��ʾ����ͬ��
* ˵    ����
****************************************************************************/
#define  GUI_CmpColor(color1, color2)	( (color1&0x01) == (color2&0x01) )
/****************************************************************************
* ��    �ƣ�u16 GUI_Color565(u32 RGB)
* ��    �ܣ���RGB��ɫת��Ϊ16λ��ɫ��
* ��ڲ�����RGB  ��ɫֵ
* ���ڲ���������16λRGB��ɫֵ��
* ˵    ����
* ���÷�����i=GUI_Color565(0xafafaf);
****************************************************************************/
u16 GUI_Color565(u32 RGB)
{
  u8  r, g, b;

  b = ( RGB >> (0+3) ) & 0x1f;		// ȡBɫ�ĸ�5λ
  g = ( RGB >> (8+2) ) & 0x3f;		// ȡGɫ�ĸ�6λ
  r = ( RGB >> (16+3)) & 0x1f;		// ȡRɫ�ĸ�5λ
   
  return( (r<<11) + (g<<5) + (b<<0) );		
}

/****************************************************************************
* ��    �ƣ�void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
* ��    �ܣ���ָ��������ʾ�ַ���
* ��ڲ�����x      ������
*           y      ������
*           *str   �ַ���
*           len    �ַ�������
*           Color  �ַ���ɫ
*           bkColor�ַ�������ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Text(0,0,"0123456789",10,0x0000,0xffff);
****************************************************************************/
void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
{
  u8 i;
  
  for (i=0;i<len;i++)
  {
    ili9320_PutChar((x+8*i),y,*str++,Color,bkColor);
  }
}

/****************************************************************************
* ��    �ƣ�void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor)
* ��    �ܣ���ָ��������ʾ�����ı�
* ��ڲ�����x      ������
*           y      ������
*           *str   ���ִ�
*           len    �ִ�����
*           Color  �ַ���ɫ
*           bkColor�ַ�������ɫ
* ���ڲ�������
* ˵    ����һ�����������ַ����ǵ�Ӵ��5������len����10
* ���÷�����GUI_Text(0,0,"�ɶ���һ��",10,0x0000,0xffff);
****************************************************************************/
void GUI_Chinese_Text(u16 x,u16 y, u8 str[],u8 len,u16 charColor,u16 bkColor)
{
    u16 i=0,b;
    u16 j=0;
	u16 x_add,y_add;
    u16 tmp_char=0,index=0;
	x_add=x;
	y_add=y;
	
	for(b=0;b<len/2;b++)
	{
			index=(94*(str[b*2] - 0xa1)+(str[b*2+1] - 0xa1));	    //������λ
			for (i=0;i<16;i++)
		    {
			    for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2];					//���ֿ�
			        if ( (tmp_char << j) & 0x80)
			        {
			          ili9320_SetPoint(x_add+j,y_add+i,charColor);  // �ַ���ɫ
			        }
			        else
			        {
			          ili9320_SetPoint(x_add+j,y_add+i,bkColor);   // ������ɫ
			        }
			    }
			   	for (j=0;j<8;j++)
			    {
				    tmp_char=HzLib[index*32+i*2+1];				    //���ֿ�
			        if ( (tmp_char << j) & 0x80)
			        {
			          ili9320_SetPoint(x_add+j+8,y_add+i,charColor); // �ַ���ɫ
			        }
			        else
			        {
			          ili9320_SetPoint(x_add+j+8,y_add+i,bkColor);   // ������ɫ
			        }
			    }
		     }
			 x_add=x_add+17;										 //��ַ�ۼ�
	}
}
/*******************************************************************************
������     �� DispWord
����       �� ��ʾ9λ�������֣������ʾ999999999
����       �� x     : ����X
              y     : ����Y
			  maxFb : Ҫ��ʾ��λ����ע��С��Ҫ��ʾ��λ��ǰ�油0��
			  Da    : Ҫ��ʾ������
			  PosFb : ����λ��  ���㵽maxFb		
			  Point : С�����λ��  
����ֵ     �� ��
*******************************************************************************///
void GUI_Word(u16  x, u16  y,u16  maxXS, u32  Da,  u16 Point, u16 Color, u16 bkColor)
{
 	u8  i;//,Fb;
    
	u32 NUM;
	NUM=1;
 	for(i=0; i<(maxXS-1); i++)  NUM=NUM*10;//����Ҫ��ʾ��λ��ȡλ��  
	if(Point!=0)ili9320_PutChar((x+(maxXS -Point)*8),y,0x2e,Color,bkColor);//���С����λ��������ʾС����
	
	for(i=0; i<(maxXS-0); i++)  //�����λ������ʾҪ��ʾ��λ��
	{   
		if(i<=(maxXS-Point-1))  //�����λ��С�������ڵ�λ��ǰ��λ������С�������ڵ�λ����������ʾ
		{
		   ili9320_PutChar((x+i*8),y,((Da/NUM)%10+0x30),Color,bkColor);
		   NUM=NUM/10;
                   
		}
		else                    //������ʾ������λһλ����������ʾС����
		{
		   ili9320_PutChar((x+i*8+8),y,((Da/NUM)%10+0x30),Color,bkColor);
		   NUM=NUM/10;
		} 
	}	
	return;
}


/****************************************************************************
* ��    �ƣ�void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
* ��    �ܣ���ָ�����껭ֱ��
* ��ڲ�����x0     A��������
*           y0     A��������
*           x1     B��������
*           y1     B��������
*           color  ����ɫ
* ���ڲ�������
* ˵    ����
* ���÷�����GUI_Line(0,0,240,320,0x0000);
****************************************************************************/
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color)
{
 	u16 x,y;
 	u16 dx;// = abs(x1 - x0);
 	u16 dy;// = abs(y1 - y0);

	if(y0==y1)
	{
		if(x0<=x1)
		{
			x=x0;
		}
		else
		{
			x=x1;
			x1=x0;
		}
  		while(x <= x1)
  		{
   			ili9320_SetPoint(x,y0,color);
   			x++;
  		}
  		return;
	}
	else if(y0>y1)
	{
		dy=y0-y1;
	}
	else
	{
		dy=y1-y0;
	}
 
 	if(x0==x1)
	{
		if(y0<=y1)
		{
			y=y0;
		}
		else
		{
			y=y1;
			y1=y0;
		}
  		while(y <= y1)
  		{
   			ili9320_SetPoint(x0,y,color);
   			y++;
  		}
  		return;
	}
	else if(x0 > x1)
 	{
		dx=x0-x1;
  		x = x1;
  		x1 = x0;
  		y = y1;
  		y1 = y0;
 	}
 	else
 	{
		dx=x1-x0;
  		x = x0;
  		y = y0;
 	}

 	if(dx == dy)
 	{
  		while(x <= x1)
  		{

   			x++;
			if(y>y1)
			{
				y--;
			}
			else
			{
   				y++;
			}
   			ili9320_SetPoint(x,y,color);
  		}
 	}
 	else
 	{
 		ili9320_SetPoint(x, y, color);
  		if(y < y1)
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
    			s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
     				{
      					p += twoDy;
     				}
     				else
     				{
      					y++;
      					p += twoDyMinusDx;
     				}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
   			else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
    			s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y < y1)
    			{
     				y++;
     				if(p < 0)
     				{
      					p += twoDx;
     				}
     				else
     				{
      					x++;
      					p+= twoDxMinusDy;
     				}
     				ili9320_SetPoint(x, y, color);
    			}
   			}
  		}
  		else
  		{
   			if(dx > dy)
   			{
    			s16 p = dy * 2 - dx;
    			s16 twoDy = 2 * dy;
	    		s16 twoDyMinusDx = 2 * (dy - dx);
    			while(x < x1)
    			{
     				x++;
     				if(p < 0)
	     			{
    	  				p += twoDy;
     				}
     				else
     				{
      					y--;
	      				p += twoDyMinusDx;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
	   		else
   			{
    			s16 p = dx * 2 - dy;
    			s16 twoDx = 2 * dx;
	    		s16 twoDxMinusDy = 2 * (dx - dy);
    			while(y1 < y)
    			{
     				y--;
     				if(p < 0)
	     			{
    	  				p += twoDx;
     				}
     				else
     				{
      					x++;
	      				p+= twoDxMinusDy;
    	 			}
     				ili9320_SetPoint(x, y,color);
    			}
   			}
  		}
 	}
}

/****************************************************************************
* ��    �ƣ�void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
* ��    �ܣ���ָ�����껭Բ�������
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill)
{
	u16 x,y;
	s16 delta,tmp;
	x=0;
	y=r;
	delta=3-(r<<1);

	while(y>x)
	{
		if(fill)
		{
			GUI_Line(cx+x,cy+y,cx-x,cy+y,color);
			GUI_Line(cx+x,cy-y,cx-x,cy-y,color);
			GUI_Line(cx+y,cy+x,cx-y,cy+x,color);
			GUI_Line(cx+y,cy-x,cx-y,cy-x,color);
		}
		else
		{
			ili9320_SetPoint(cx+x,cy+y,color);
			ili9320_SetPoint(cx-x,cy+y,color);
			ili9320_SetPoint(cx+x,cy-y,color);
			ili9320_SetPoint(cx-x,cy-y,color);
			ili9320_SetPoint(cx+y,cy+x,color);
			ili9320_SetPoint(cx-y,cy+x,color);
			ili9320_SetPoint(cx+y,cy-x,color);
			ili9320_SetPoint(cx-y,cy-x,color);
		}
		x++;
		if(delta>=0)
		{
			y--;
			tmp=(x<<2);
			tmp-=(y<<2);
			delta+=(tmp+10);
		}
		else
		{
			delta+=((x<<2)+6);
		}
	}
}

/****************************************************************************
* ��    �ƣ�void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
* ��    �ܣ���ָ�����򻭾��Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill)
{
	if(fill)
	{
		u16 i;
		if(x0>x1)
		{
			i=x1;
			x1=x0;
		}
		else
		{
			i=x0;
		}
		for(;i<=x1;i++)
		{
			GUI_Line(i,y0,i,y1,color);
		}
		return;
	}
	GUI_Line(x0,y0,x0,y1,color);
	GUI_Line(x0,y1,x1,y1,color);
	GUI_Line(x1,y1,x1,y0,color);
	GUI_Line(x1,y0,x0,y0,color);
}

/****************************************************************************
* ��    �ƣ�void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
* ��    �ܣ���ָ�����������Σ��������ɫ
* ��ڲ�����
* ���ڲ�����
* ˵    ����
* ���÷�����
****************************************************************************/
void  GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill)
{
	GUI_Rectangle(x0, y0, x0+with, y0+with, color,fill);
}
