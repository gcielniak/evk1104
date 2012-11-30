/*
 * Simple examples demonstrating the use of LEDs and switches not using ASF.
 *
 * Created: 30/11/2012 14:55:14
 * Author: Grzegorz Cielniak
 */ 

//include microcontroller IO definitions
#include <avr32/io.h>

//LED section
#define LED0 1
#define LED1 2
#define LED2 4
#define LED3 8

//calculate ports and pins for each LED
#define LED0_PORT AVR32_PIN_PX16/32 //LED0, pin PX16
#define LED0_PIN (1 << (AVR32_PIN_PX16%32))
#define LED1_PORT AVR32_PIN_PX50/32 //LED1, pin PX50
#define LED1_PIN (1 << (AVR32_PIN_PX50%32))
#define LED2_PORT AVR32_PIN_PX54/32 //LED2, pin PX54
#define LED2_PIN (1 << (AVR32_PIN_PX54%32))
#define LED3_PORT AVR32_PIN_PX51/32 //LED3, pin PX51
#define LED3_PIN (1 << (AVR32_PIN_PX51%32))

//Initialise the LED ports
void LED_Init()
{
	AVR32_GPIO.port[LED0_PORT].oders = LED0_PIN; // The GPIO output driver is enabled for that pin.
	AVR32_GPIO.port[LED0_PORT].gpers = LED0_PIN; // The GPIO module controls that pin.
	AVR32_GPIO.port[LED1_PORT].oders = LED1_PIN;
	AVR32_GPIO.port[LED1_PORT].gpers = LED1_PIN;
	AVR32_GPIO.port[LED2_PORT].oders = LED2_PIN;
	AVR32_GPIO.port[LED2_PORT].gpers = LED2_PIN;
	AVR32_GPIO.port[LED3_PORT].oders = LED3_PIN;
	AVR32_GPIO.port[LED3_PORT].gpers = LED3_PIN;
}

//Switch on the specified LED
void LED_On(unsigned char led_flags)
{
	if (led_flags & LED0)
	AVR32_GPIO.port[LED0_PORT].ovrc = LED0_PIN;
	if (led_flags & LED1)
	AVR32_GPIO.port[LED1_PORT].ovrc = LED1_PIN;
	if (led_flags & LED2)
	AVR32_GPIO.port[LED2_PORT].ovrc = LED2_PIN;
	if (led_flags & LED3)
	AVR32_GPIO.port[LED3_PORT].ovrc = LED3_PIN;
}

//Switch off the specified LED
void LED_Off(unsigned char led_flags)
{
	if (led_flags & LED0)
	AVR32_GPIO.port[LED0_PORT].ovrs = LED0_PIN;
	if (led_flags & LED1)
	AVR32_GPIO.port[LED1_PORT].ovrs = LED1_PIN;
	if (led_flags & LED2)
	AVR32_GPIO.port[LED2_PORT].ovrs = LED2_PIN;
	if (led_flags & LED3)
	AVR32_GPIO.port[LED3_PORT].ovrs = LED3_PIN;
}

//Toggle the specified LED
void LED_Toggle(unsigned char led_flags)
{
	if (led_flags & LED0)
	AVR32_GPIO.port[LED0_PORT].ovrt = LED0_PIN;
	if (led_flags & LED1)
	AVR32_GPIO.port[LED1_PORT].ovrt = LED1_PIN;
	if (led_flags & LED2)
	AVR32_GPIO.port[LED2_PORT].ovrt = LED2_PIN;
	if (led_flags & LED3)
	AVR32_GPIO.port[LED3_PORT].ovrt = LED3_PIN;
}

//SWITCH section
#define SWITCH2_PORT AVR32_PIN_PB10/32  //SW2, pin PX16
#define SWITCH2_PIN (1 << (AVR32_PIN_PB10%32))
#define SWITCH2 1

//Initialise the switch IO port
void Switch_Init()
{
	AVR32_GPIO.port[SWITCH2_PORT].gpers = SWITCH2_PIN; // The GPIO module controls that pin.
}

//Check if the specified switch is on
unsigned char Switch_On(unsigned char switch_flags)
{
	if ((switch_flags & SWITCH2) && !(AVR32_GPIO.port[SWITCH2_PORT].pvr & SWITCH2_PIN))
	return 1;
	else
	return 0;
}

//CPU clock defines
#define FOSCRC	115000
#define FOSC32	32768
#define FOSC0	12000000

//delay in ms
void cpu_delay_ms(unsigned int delay, unsigned int clock_freq)
{
	unsigned int delay_cycles = delay*clock_freq/1000;
	__builtin_mtsr(AVR32_COUNT, 0);
	while (__builtin_mfsr(AVR32_COUNT) < delay_cycles)
	;
}

//delay in us
void cpu_delay_us(unsigned int delay, unsigned int clock_freq)
{
	unsigned int delay_cycles = delay*clock_freq/1000000;
	__builtin_mtsr(AVR32_COUNT, 0);
	while (__builtin_mfsr(AVR32_COUNT) < delay_cycles)
	;
}

//the main function
int main(void) {

	//change the clock to osc0 = 12MHz
	AVR32_PM.oscctrl0 = AVR32_PM_OSCCTRL0_MODE_CRYSTAL_G3<<AVR32_PM_OSCCTRL0_MODE_OFFSET | 3<<AVR32_PM_OSCCTRL0_STARTUP_OFFSET;
	AVR32_PM.mcctrl |= AVR32_PM_MCCTRL_OSC0EN_MASK;
	while (!(AVR32_PM.poscsr & AVR32_PM_POSCSR_OSC0RDY_MASK));
	AVR32_PM.mcctrl |= AVR32_PM_MCCTRL_MCSEL_OSC0;

	LED_Init();
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
	LED_Init();

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
	LED_Init();

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
	LED_Init();

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
	LED_Init();

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
	LED_Init();

	LED_Off(LED0|LED1|LED2|LED3);

	while (1)
	{
		LED_Toggle(LED0|LED1|LED2|LED3);
		cpu_delay_ms(1000, FOSCRC);
	}
}

void SwitchLEDExample()
{
	LED_Init();
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
