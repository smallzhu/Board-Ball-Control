

#include "common.h"
#include "include.h"

#define S3010_FTM   FTM0
#define S3010_CH    FTM_CH3
#define S3010_HZ    (100)
#define S3010_PIN   FTM0_CH3_PIN


#define S3010_FTM_1   FTM0
#define S3010_CH_1    FTM_CH5
#define S3010_HZ_1    (100)
#define S3010_PIN_1   FTM0_CH5_PIN

//��������
void PORTA_IRQHandler();
void DMA0_IRQHandler();
extern void LPTMR_IRQHandler();
void get_ball();      //��ȡ���λ��
void set_area();


uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 img[CAMERA_H][CAMERA_W];                           //����洢��ѹ���ͼ�����飨���ڶ�ȡ����ֵ��ͼ����


Site_t up,down,left,right,central;
Site_t time_site;

char second[]={'0','0'},millisecond='0';
int count_time=0;//��ʱ
int i,j;
//int up_flag=0,down_flag=0,left_flag=0,right_flag=0;


/*!
 *  @brief      main����
 *  @since      v5.3
 *  @note       ɽ������ͷ LCD ����ʵ��
 */
  void  main(void)
{
    Site_t line_site_V1[60];
    Site_t line_site_V2[60];
    Site_t line_site_H1[80];
    Site_t line_site_H2[80];
    
    Site_t site     = {0,0};                           //��ʾͼ�����Ͻ�λ��
    Size_t imgsize  = {CAMERA_W, CAMERA_H};             //ͼ���С
    Size_t size;                   //��ʾ����ͼ���С
    
    lcd_init(GBLUE);
    
 
   // size.H = LCD_H;
    //size.W = LCD_W;
    size.H=CAMERA_H;
    size.W=CAMERA_W;

    camera_init(imgbuff);
    ftm_pwm_init(S3010_FTM, S3010_CH,S3010_HZ,20,S3010_PIN);//�����ʼ��
    ftm_pwm_init(S3010_FTM_1, S3010_CH_1,S3010_HZ_1,17,S3010_PIN_1);
    
    
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_DMA16_VECTORn , DMA0_IRQHandler);     //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    
    lptmr_timing_ms(100);                                  //��ʼ��LPTMR����ʱʱ��Ϊ�� 100ms
    set_vector_handler(LPTMR_VECTORn ,LPTMR_IRQHandler);    //����LPTMR���жϷ�����Ϊ LPTMR_IRQHandler
    enable_irq (LPTMR_IRQn);                                //ʹ��LPTMR�ж�
    
    for(i=0;i<=79;i++)//���ߵ㸳ֵ
    {
      if(i<60)
      {  
      line_site_V1[i].x=26;
      line_site_V2[i].x=52;
      line_site_V1[i].y=i;
      line_site_V2[i].y=i;
      }
      line_site_H1[i].y=20;
      line_site_H2[i].y=40;
      line_site_H1[i].x=i;
      line_site_H2[i].x=i;
    }

    while(1)
    {
        camera_get_img();                                   //����ͷ��ȡͼ��

        //�ڰ�����ͷ
      lcd_img_binary_z(site, size, imgbuff, imgsize,BLACK,WHITE);
      
      lcd_points(line_site_V1,60, RED);
      lcd_points(line_site_V2,60,RED);
      lcd_points(line_site_H1,80,RED);
      lcd_points(line_site_H2,80,RED);
      
      img_extract(img,imgbuff,CAMERA_SIZE);//��ѹΪ��ά����
      get_ball();
      set_area();
    //ftm_pwm_duty(S3010_FTM, S3010_CH,1000-i);
    }
}

void get_ball()
{
  int point_flag=1;
  for(i=10;i<50;i++)
      {
        for(j=10;j<70;j++)
          {
            if(img[i][j]==0x00&&img[i][j+1]==0xFF)//�ϡ��¡���
            {
              if(point_flag)//�����ֵ��ֻ��һ��
              {
                up.x=down.x=left.x=right.x=j;
                up.y=down.y=left.y=right.y=i;
                  point_flag=0;
               }
              if(up.y>=i)
              {
                up.y = i;
                up.x = j;
              }
         if(left.x>j)
         {
           left.x=j;
           left.y=i;
         }
         if(down.y<i)
         {
           down.x=j;
           down.y=i;
         }
         if(right.x<j)
         {
           right.x=j;
           right.y=i;
         }
            }
      }
    }
    central.x=(up.x + down.x)/2;
    central.y=(up.y+down.y+right.y)/3;
    lcd_char(central,'c',RED,BRED);
}

void set_area()
{
  float temp_x,temp_y;
  
  if(central.x>40)
  {
    temp_x = 17-(central.x-40)/8;
    ftm_pwm_duty(S3010_FTM_1, S3010_CH_1,temp_x);
  }
  else
  {
    temp_x = 17+(40-central.x)/8;
    ftm_pwm_duty(S3010_FTM_1, S3010_CH_1,temp_x);
  }
  if(central.y>40)
  {
    temp_y = 20+(central.y-40)/8;
    ftm_pwm_duty(S3010_FTM, S3010_CH,temp_y);
  }
  else
  {
    temp_y = 20-(40-central.y)/8;
    ftm_pwm_duty(S3010_FTM, S3010_CH,temp_y);
  }
  
}


/*!
 *  @brief      PORTA�жϷ�����
 *  @since      v5.0
 */
void PORTA_IRQHandler()
{
    uint8  n;    //���ź�
    uint32 flag;

    while(!PORTA_ISFR);
    flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if ( CAMERA_USE_HREF == 1 )                            //ʹ�����ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0�жϷ�����
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    camera_dma();
}

//��ʱ�жϷ�����
void LPTMR_IRQHandler(void)
{
  if(second[0]=='9'&& second[1]=='9')
  {
    second[0]='0';
    second[1]='0';
  }
      if(millisecond<'9')
    {
      millisecond++;
    }
    else
    {
      if(second[0]<'9')
          second[0]++;
      else
      {
        second[1]++;
        second[0]='0';
      }
      millisecond='0';
    }
    time_site.x=0;time_site.y=70;
    lcd_str(time_site,"step:1",RED,GBLUE);
    time_site.y=90;time_site.x=0;
    lcd_char(time_site,second[1],RED,GBLUE);
    time_site.x=10;
    lcd_char(time_site,second[0],RED,GBLUE);
    time_site.x=20;
    lcd_char(time_site,'s',RED,GBLUE);
    time_site.x=30;
    lcd_char(time_site,millisecond,RED,GBLUE);
    time_site.x=40;
    lcd_str(time_site,"ms",RED,GBLUE);
    
    LPTMR_Flag_Clear();       //���жϱ�־λ
}
