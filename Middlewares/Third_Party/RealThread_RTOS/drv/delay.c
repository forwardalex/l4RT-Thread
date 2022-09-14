#include "delay.h"
#include "main.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

static u32 fac_us = 0;							//us��ʱ������

#if SYSTEM_SUPPORT_OS
static u16 fac_ms = 0;				      //ms��ʱ������,��os��,����ÿ�����ĵ�ms��

#if SYSTEM_SUPPORT_OS							//���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
//��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
//������3���궨��:
//delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
//delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
//Ȼ����3������:
//delay_osschedlock:��������OS�������,��ֹ����
//delay_osschedunlock:���ڽ���OS�������,���¿�������
//delay_ostimedly:����OS��ʱ,���������������.

//�����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif


/**
 * @brief	us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
 *
 * @param   SYSCLK	ϵͳʱ��Ƶ��
 *
 * @return  void
 */
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
    OS_ERR err;
    OSSchedLock(&err);						//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else										//����UCOSII
    OSSchedLock();							//UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}
/**
 * @brief	us����ʱʱ,�ָ��������
 *
 * @param   SYSCLK	ϵͳʱ��Ƶ��
 *
 * @return  void
 */
void delay_osschedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
    OS_ERR err;
    OSSchedUnlock(&err);					//UCOSIII�ķ�ʽ,�ָ�����
#else										//����UCOSII
    OSSchedUnlock();						//UCOSII�ķ�ʽ,�ָ�����
#endif
}
/**
 * @brief	����OS�Դ�����ʱ������ʱ
 *
 * @param   ticks	��ʱ�Ľ�����
 *
 * @return  void
 */
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
    OS_ERR err;
    OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err); //UCOSIII��ʱ��������ģʽ
#else
    OSTimeDly(ticks);						    //UCOSII��ʱ
#endif
}
/**
* @brief	systick�жϷ�����,ʹ��OSʱ�õ�
*
* @param   ticks	��ʱ�Ľ�����
*
* @return  void
*/
void SysTick_Handler(void)
{
    HAL_IncTick();

    if(delay_osrunning == 1)					//OS��ʼ����,��ִ�������ĵ��ȴ���
    {
        OSIntEnter();						//�����ж�
        OSTimeTick();       				//����ucos��ʱ�ӷ������
        OSIntExit();       	 				//���������л����ж�
    }
}
#endif



#endif

void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
    u32 reload;
#endif
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTickƵ��ΪHCLK
    fac_us = SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��

#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
    reload = SYSCLK;					  //ÿ���ӵļ������� ��λΪK
    reload *= 1000000 / delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
    //reloadΪ24λ�Ĵ���,���ֵ:16777216,��80M�£�Լ209.7ms����
    fac_ms = 1000 / delay_ostickspersec;		//����OS������ʱ�����ٵ�λ
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //����SYSTICK�ж�
    SysTick->LOAD = reload; 					//ÿ1/OS_TICKS_PER_SEC���ж�һ��
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //����SYSTICK
#else
#endif
}


/**
 * @brief	��ʱ΢��(us)����
 *
 * @remark	nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)
 *
 * @param   nus		��Ҫ��ʱ����΢��
 *
 * @return  void
 */
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD;				//LOAD��ֵ
    ticks = nus * fac_us; 						//��Ҫ�Ľ�����
    told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ

    while(1)
    {
        tnow = SysTick->VAL;

        if(tnow != told)
        {
            if(tnow < told)tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else tcnt += reload - tnow + told;
			
            told = tnow;
            if(tcnt >= ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }
    }
}


/**
 * @brief	��ʱ����(ms)����
 *
 * @param   nms		��Ҫ��ʱ���ٺ���
 *
 * @return  void
 */
void delay_ms(u16 nms)
{
    u32 i;

    for(i = 0; i < nms; i++) delay_us(1000);
}







