/*
 * Stress Monitor Based on NB-IoT
 * ��Ŀ���ƣ�����NB-IoT��Ӧ�����ϵͳ
 * ���ߣ��γ���
 *
 * �ô����Ȩ���γ������У�����������ҵ��;��
 * https://neyzoter.github.io/
 */

#include "mytask.h"
#include "common.h"

TaskHandle_t GetDataTask_Handler;
TaskHandle_t IndicatorTask_Handler;
TaskHandle_t AttachNetworkTask_Handler;
TaskHandle_t StartTask_Handler;
////////////////////////////////////////////////////////////
//���ӵ�״̬

//��һ��������BRD_STA
//BRD_SYS_INITING:���ڳ�ʼ��
//BRD_ATTACKNETWORKING����������
//BRD_LOOPING:��������ѭ������

//�ڶ�������IMEI��

//������������OLED_REFRESHING����ǰOLED�ǲ�����д
//�����������ط�ͬʱд�����綨ʱ����ѭ������ͬʱ���¡�

MAIN_FLAG flag = {0,"863703035480214",0};

/////////////////////////////////////////////////////////////

char Adc_Str[9] = "+000000N\0";

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
	
    //����getdata����
    xTaskCreate((TaskFunction_t )getdata_task,     	
                (const char*    )"getdata_task",   	
                (uint16_t       )GET_DATA_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )GET_DATA_TASK_PRIO,	
                (TaskHandle_t*  )&GetDataTask_Handler);  				
    //����ָʾ������
    xTaskCreate((TaskFunction_t )indicator_task,     	
                (const char*    )"indicator_task",   	
                (uint16_t       )INDICATOR_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )INDICATOR_DATA_TASK_PRIO,	
                (TaskHandle_t*  )&IndicatorTask_Handler); 				
    //����LED1����
    xTaskCreate((TaskFunction_t )attach_network_task,     
                (const char*    )"attach_network_task",   
                (uint16_t       )ATTACH_NETWORK_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ATTACH_NETWORK_TASK_PRIO,
                (TaskHandle_t*  )&AttachNetworkTask_Handler);        

    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//�ɼ�����������
void getdata_task(void *pvParameters)
{
	static u8 Data_Num = 0;//���ݵ�����
	static u32 Adc_Val = 0;
    while(1)
    {
		if(Data_Num<DATA_NUM_NEEDED)//�����û������Ҫ�����
		{
			Adc_Val += GetAdcValue();	//�ɼ����ݲ���Ӻ���ȡƽ��
			Data_Num++;
		}
		else if(flag.BRD_STA == BRD_ATTACKED||HAL_RTCEx_BKUPRead(&hrtc,BACKUP_AREA))//������ݲɼ����ˣ������Ѿ������˻��ߴӴ�������
		{
			Num2Str(Adc_Str,Adc_Val / Data_Num);//ȡƽ����ת��Ϊstr
			flag.BRD_STA = BRD_ATTACKED;//�Ӵ������ѵĻ���ֱ�Ӹ�����ֵ
			if(!MSG_Send(Adc_Str,"8"))//����5��ʧ�ܣ���ô��ʾerror
			{
				MsgSentDisp("ERROR");//������Ҫ���
				HAL_PWR_EnableBkUpAccess(); //���������ʹ��,CDP��
				HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_AREA,NEED_REATTACH_NETWORK);//�´���Ҫ��������
				HAL_PWR_DisableBkUpAccess(); //��������ʳ���,CDP��
			}
			else
			{
				HAL_PWR_EnableBkUpAccess(); //���������ʹ��,CDP��
				HAL_RTCEx_BKUPWrite(&hrtc, BACKUP_AREA,RESET_FROM_STANDBY);//�´��������ģʽ����������Ҫ��������
				HAL_PWR_DisableBkUpAccess(); //��������ʳ���,CDP��
			}
			MsgSentDisp(" Entering Standby...");
			vTaskDelay(2000);
			Sys_Entry_Standby();//�������ģʽ			
		}
		vTaskDelay(5000);//����̬��ÿ5��ɼ�һ��
    }
}

 void indicator_task(void *pvParameters)
 {
	while(1)
	{
//		if(flag.BRD_STA == BRD_SYS_INITING)//�����ڳ�ʼ��
//		{	LED1_ON();	LED2_OFF();	}
//		else if(flag.BRD_STA == BRD_ATTACKNETWORKING)//����������������
//		{	LED2_ON();	LED1_OFF();	}
//		else if(flag.BRD_STA == BRD_ATTACKED)//�����Ѿ�����
//		{	LED2_ON();	LED1_ON();	}
//		vTaskDelay(100);
//		if(flag.BRD_STA == BRD_SYS_INITING)//�����ڳ�ʼ��
//		{	LED1_OFF();	LED2_OFF();	}
//		else if(flag.BRD_STA == BRD_ATTACKNETWORKING)
//		{	LED2_OFF();	LED1_OFF();	}		
//		vTaskDelay(500);
		if(flag.BRD_STA == BRD_SYS_INITING)//�����ڳ�ʼ��
		{	LED1_ON();	LED2_OFF();	}
		else if(flag.BRD_STA == BRD_ATTACKNETWORKING)//����������������
		{	LED2_ON();	LED1_OFF();	}
		else if(flag.BRD_STA == BRD_ATTACKED)//�����Ѿ�����
		{	LED2_ON();	LED1_ON();	}	
		vTaskDelay(1000);
	}
 }

//����������
void attach_network_task(void *pvParameters)
{
	while(1)
	{
		if(!HAL_RTCEx_BKUPRead(&hrtc,BACKUP_AREA)&&flag.BRD_STA != BRD_ATTACKED)//����ǵ�һ�ο�������Ҫ����
		{
			while(AttachNetwork()==0)//������Ӳ��ɹ���ϵͳ���³�ʼ��
			{
				vTaskDelay(2000);
				SysInit();
			}
		}
		vTaskDelete(NULL);//ɾ������
		vTaskDelay(4000);
	}
}

//**************************************************************************************
//�������ƣ�void SysInit(void)
//�����������
//�������ã���ʼ�����򣬰�����ʱ��ʼ�����жϳ�ʼ����IO��LED��������OLED����ʱ��������BC95
//���ز�������
//****************************************************************************************
void SysInit(void)
{
	flag.BRD_STA = BRD_SYS_INITING;//�������ڳ�ʼ��
	HAL_Init();
	SystemClock_Config();
	I2C_OLED_Init();//��ʼ��IIC
	OLED_Init();//��ʼ��OLED
	
	OLED_CLS();//����	
	OLED_ShowStr(0,0,"FL",1);	//offline
	OLED_ShowStr(0,1,"CMD:",1);	
	OLED_ShowStr(0,2,"Rcv:",1);
	
	OLED_ShowStr(CMD_START_X,CMD_START_Y,"Sys Init...",1);
	HAL_Delay(1000);
	
//	//����Ҫ�����жϵ�ʱ���������ж�
//	TIM2_Init(99,2096);//100*2097/2097000  =  100ms
	uart_init(9600);              //��ʼ��USART
	BSP_LED_Init();//PD2C10
	ADC_Init();
	
	//iwdg�ڿ������޷��ڹرգ���������������ʱ�䳤�Ļ�����ô�Ͳ�Ҫ����iwdg��
	//�����ڽ���standby֮ǰ����λһ��	
	//���Ҫ��IWDG����ôҪ��һ��LSI����Ȼ���ó�����
//	IWDG_Init(IWDG_PRESCALER_32,4095);//��Լ4s�ӣ�û��ι����Ҫ����

	WWDG_Init(65,127, WWDG_PRESCALER_8);//��Լ1s�н���һ�δ��ڿ��Ź��ж�
	
//	HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_PIN);
//	KEY_Init();	//������ʼ��
//	BSP_JOY_Init();
	
	OLED_ShowStr(CMD_START_X,CMD_START_Y,"Sys Inited   ",1);
	
	HAL_Delay(1000);
		
	IO_Init();
	RTC_Init(60);//60�뿪��һ��
}
