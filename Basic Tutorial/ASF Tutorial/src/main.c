/*
* Simple examples demonstrating the use of LEDs and switches using ASF.
*
* Created: 30/11/2012 14:55:14
* Author: Grzegorz Cielniak
*/

/*
* Include header files for all drivers that have been imported from
* Atmel Software Framework (ASF).
*/
#include <asf.h>

#define Switch_Init() (gpio_enable_gpio_pin(GPIO_PUSH_BUTTON_SW2))
#define SWITCH2 1
#define FOSCRC 115000

unsigned char Switch_On(unsigned char switch_flags)
{
	if ((switch_flags & SWITCH2) && !gpio_get_pin_value(GPIO_PUSH_BUTTON_SW2))
	return 1;
	else
	return 0;
}

int main(void) {

	// Switch the main clock to the external oscillator 0
	pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);
	
	board_init();

	Switch_Init();

	LED_Off(LED0|LED1|LED2|LED3);

	unsigned int counter = 0;
	unsigned int div = 32;
	unsigned int cycle = 0, ccycle = 0;
	unsigned int cycle_counter = 0;

	while (1)
	{
		if (cycle)
		{
			if (counter == 0)
			LED_On(LED0|LED1|LED2|LED3);
			else if (counter == cycle)
			LED_Off(LED0|LED1|LED2|LED3);
		}

		counter = (counter + 1) % div;
		cycle_counter = (cycle_counter + 1) % (2048/div);

		if (!cycle_counter)
		{
			ccycle = (ccycle + 1) % (2*div);
			if (ccycle < div)
			cycle = ccycle;
			else
			cycle = 2*div - 1 - ccycle;
		}
		cpu_delay_us(2000/div, FOSC0);
	}

	return 0;
}

void LEDPulsating()
{
	LED_Off(LED0|LED1|LED2|LED3);

	unsigned int counter = 0;
	unsigned int div = 8;
	unsigned int cycle = 0, ccycle = 0;
	unsigned int cycle_counter = 0;

	while (1)
	{
		if (cycle)
		{
			if (counter == 0)
			LED_On(LED0|LED1|LED2|LED3);
			else if (counter == cycle)
			LED_Off(LED0|LED1|LED2|LED3);
		}

		counter = (counter + 1) % div;
		cycle_counter = (cycle_counter + 1) % (256/div);

		if (!cycle_counter)
		{
			ccycle = (ccycle + 1) % (2*div);
			if (ccycle < div)
			cycle = ccycle;
			else
			cycle = 2*div - 1 - ccycle;
		}
		cpu_delay_us(2000/div, FOSCRC);
	}
}

void LEDPWM()
{
	LED_Off(LED0|LED1|LED2|LED3);

	unsigned int counter = 0;

	LED_On(LED0);

	while (1)
	{
		if (counter == 0)
		LED_On(LED1|LED2|LED3);
		if (counter == 3)
		LED_Off(LED1);
		if (counter == 2)
		LED_Off(LED2);
		if (counter == 1)
		LED_Off(LED3);

		counter = (counter + 1) % 4;
		cpu_delay_us(500, FOSCRC);
	}
}

void FastBlink()
{
	LED_Off(LED0|LED1|LED2|LED3);

	unsigned int counter = 0;

	while (1)
	{
		//if (counter % 1)
		LED_Toggle(LED0);

		if (counter % 2)
		LED_Toggle(LED1);

		if (counter % 4)
		LED_Toggle(LED2);

		if (counter % 8)
		LED_Toggle(LED3);

		counter++;
		cpu_delay_ms(2, FOSCRC);
	}
}

void LEDSequencingExample()
{
	int counter = 0;

	while (1)
	{
		LED_Off(LED0|LED1|LED2|LED3);

		switch (counter)
		{
			case 0:
			LED_On(LED0);
			break;
			case 1:
			LED_On(LED1);
			break;
			case 2:
			LED_On(LED2);
			break;
			case 3:
			LED_On(LED3);
			break;
			case 4:
			LED_On(LED2);
			break;
			case 5:
			LED_On(LED1);
			break;
		}

		counter = (counter + 1) % 6;
		cpu_delay_ms(200, FOSCRC);
	}
}

void BlinkLEDExample()
{
	LED_Off(LED0|LED1|LED2|LED3);

	while (1)
	{
		LED_Toggle(LED0|LED1|LED2|LED3);
		cpu_delay_ms(1000, FOSCRC);
	}
}

void SwitchLEDExample()
{
	Switch_Init();

	unsigned char switch_state = 0;
	unsigned char switch_on;

	LED_Off(LED0|LED1|LED2|LED3);

	while (1)
	{
		switch_on = Switch_On(SWITCH2);

		if (!switch_state && switch_on)
		{
			LED_On(LED0|LED1|LED2|LED3);
			switch_state = 1;
		}
		else if (switch_state && ~switch_on)
		{
			LED_Off(LED0|LED1|LED2|LED3);
			switch_state = 0;
		}
	}
}
