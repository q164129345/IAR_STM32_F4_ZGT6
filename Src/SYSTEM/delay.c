#include "delay.h"
#include "core_cm4.h"

static uint32_t fac_us;

void delay_init(uint8_t SYSCLK) {
	//HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);   //STM32CubeMX�Ѿ������˸�ѡ��
	fac_us = SYSCLK;
}

void delay_us(uint32_t nus) {
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;      //��ȡLOAD��ֵ
	ticks = nus * fac_us;				  //��Ҫ�Ľ�����
	told = SysTick->VAL;				  //�ս���ʱ�ļ�����ֵ
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow;  //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ����ˡ�
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break;              //ʱ�䳬��/����Ҫ�ӳٵ�ʱ�䣬���˳���
		}
	}
}

void delay_ms(uint16_t nms) {
	uint32_t i;
	for (i = 0; i < nms; i++)
		delay_us(1000);
}

