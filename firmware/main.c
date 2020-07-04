#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdbool.h>

#define LED_PIN		PB0
#define SENSOR_PIN 	PB2
#define MOSFET_PIN 	PB1

ISR(PCINT0_vect)
{
	cli();
	PCMSK &= ~(1 << PCINT2);
}

/* if mosfet is turned on enable led */
void set_led(void)
{
	if (PINB & (1 << MOSFET_PIN)) {
		PORTB |= (1 << LED_PIN);
	} else {
		PORTB &= ~(1 << LED_PIN);
	}
}

int main(void)
{
	/* LED Output, Mosfet Output, Sensor Input */
	DDRB &= ~(1 << SENSOR_PIN);
	DDRB |= (1 << LED_PIN) | (1 << MOSFET_PIN);

	/* Set LED and Mosfet low */
	PORTB &= ~((1 << LED_PIN) | (1 << MOSFET_PIN));

	/* Enable pin change interrupt */
	PCICR |= (1 << PCIE0);
	PCMSK |= (1 << PCINT2);

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	while(1) {
		set_led();
		sleep_enable();
		sei();
		PCMSK |= (1 << PCINT2);
		sleep_cpu();
		sleep_disable();

		uint32_t timer = 0;
		bool sensor;
		while (timer < 1000) {
			sensor = PINB & (1 << SENSOR_PIN);
			if (sensor) {
				break;
			}
			timer++;
			if (timer % 50 == 0) {
				PORTB ^= (1 << LED_PIN);
			}
			_delay_ms(1);
		}

		/* if last sensor value indicates an inative sensor, the user removed the
		 * magnetic field before the time threshold. In this case continue without
		 * changing the switch.
		 */
		if (sensor) {
			continue;
		}

		PORTB ^= (1 << MOSFET_PIN);
	}
}
