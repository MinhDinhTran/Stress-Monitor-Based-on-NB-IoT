# ��Ŀ����

����NB-IoTխ�����������ɼ�ѹ�����ݣ������͵�����������ƽ̨���������FreeRTOSʵʱ����ϵͳ��

�����������Եķ������ݣ����������ģʽ���ڴ�ͬʱ������Tickless�����ڿ���ʱ����˯��ģʽ��

�汾��2018-4-22

# ��Ӫ���

NB��Ӫ�̣�����

������ƽ̨������

>�����Ҫ�ĳ���������Ӫ�̺�������ƽ̨��ֻ��Ҫ�޸ģ�nband��CDP��ַ���ɡ�

# ��Դ����

* 1��USART3

STML1��PB10��TX����PB11��RX��

BC95��RX��һ��������TXֱ��ͨ��50R����

��������PB10��PB11Ϊ�������������ģʽʱ������Ϊ�͵�ƽ

ע�����Ҫ��USARTx����Ҫ�ĵ����ݣ�GPIOx��GPIO_PINx��������������ã���GPIOʱ�ӡ��ж���USARTx_IRQHandler��

* 2��OLED

PB6��SCL����PB7��SDA��

* 3��LED

PD2��PC10

* 4��KEY1��KEY2��KEY3

PC13��PC12��PC11

* 5��ʱ��

ϵͳʱ�ӣ�MSI  -��   2.097MHz����ѡ��

ADCʱ�ӣ�HSI  -��    16Mhz

LSE��32.768kHz

* 6��RTC

����RTC��������Ƶ����Ƶ�õ���Ƶ����ΪԴ����һ������Ϊ1Hz�������Զ���װ�ؼĴ�������ֵ�����趨u16����������ʱ���жϣ��ﵽ18Hours���ϣ����������17bits���Դﵽ36Hours���ϡ�

* 7.�źŵ�����·��Դ����

LM_CS��PC1

* 8.ADC1

PC0
�����ȵ���GetAdcValue�����������źŵ�����·��Դ����ʼADCת�����ر�ADCת�����ر��źŵ�����·��Դ�ȹ��̣�

* 9.NVIC

��ֲFreeRTOS��ʱ���ж����ȼ�����Ϊ4bits��0bits��FreeRTOS�����������ȼ���

ע���ڶ�ʱ���ж��и���OLEDʱ��Ϊ�˷�ֹ���ֺͷ��жϸ���OLED��ͻ����Ҫ���ñ�־λ��1�����ڸ��£�0��δ���£���

* 10.Tickless

Tickless�������Ҫ�ر�ĳһЩ�͹���ʱ����Ҫ��ʱ�ӣ�����LED�����ŵ�ʱ�ӡ�

�ڱ������У�ADC��ʱ���ڶ�ȡ��ʱ��Ŵ򿪣����Բ���Ҫ��Tickless����ǰ��PreSleepProcessing�����д��������Ҷ�ȡ����ǰ���ܹر�ʱ�ӡ�

* 11.IWDG

����SysTick_Handler�У�IWDG����ʵ�ֻ���Standby������ڴ���ģʽ��������Ҳ����ʵ�ֻ��ѡ���������ʱ��̣ܶ���������볤ʱ���������û�п�����������û�п����������Ź���

* 12.WWDG

�����뿪���˴��ڿ��Ź���

# ����

�����������ʲô���⣬��ϵ�ҡ�

Email: sonechaochao@gmail.com