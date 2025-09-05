#include "Drv_Servo.h"
#include "include.h"
#include "Ano_RC.h"
#include "Ano_Math.h"
/*初始化高电平时间1000us（4000份）*/
#define INIT_DUTY 1500 //舵机信号宽度在1ms~2ms之间
/*频率50hz*/
#define HZ  50
/*精度10000，每份0.25us*/
#define ACCURACY 20000 //
/*舵机PWM信号极值和中立值*/
#define Servo_PWM_MAX 2000
#define Servo_PWM_MIN 1000
#define Servo_PWM_Medium ((Servo_PWM_MAX+Servo_PWM_MIN)/2)
void ServoInit(){
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    uint16_t PrescalerValue = 0;
    u32 hz_set = ACCURACY * HZ;

    GPIO_StructInit(&GPIO_InitStructure);
    TIM_TimeBaseStructInit ( &TIM_TimeBaseStructure );
    TIM_OCStructInit ( &TIM_OCInitStructure );

    RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM5, ENABLE );
    RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOA , ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init ( GPIOA, &GPIO_InitStructure );

    GPIO_PinAFConfig ( GPIOA, GPIO_PinSource3, GPIO_AF_TIM5 );
    /* Compute the prescaler value */
    PrescalerValue = ( uint16_t ) ( ( SystemCoreClock / 2 ) / hz_set ) - 1;
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = ACCURACY-1;
    TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit ( TIM5, &TIM_TimeBaseStructure );
    
    //配置PWM1模式：CNT<CRR时为高电平，反之为低电平
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    /* PWM1 Mode configuration: Channel3 */
    //PA_3对应Tim5的ch4
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = INIT_DUTY;
    TIM_OC4Init ( TIM5, &TIM_OCInitStructure );
    TIM_OC4PreloadConfig ( TIM5, TIM_OCPreload_Enable );

    TIM_ARRPreloadConfig ( TIM5, ENABLE );
    TIM_Cmd ( TIM5, ENABLE );

}
void ServoTask_20ms(){
    uint16_t pwm=CH_N[5]+Servo_PWM_Medium;
    TIM5->CCR4=LIMIT(pwm,Servo_PWM_MIN,Servo_PWM_MAX);
}
