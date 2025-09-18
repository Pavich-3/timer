#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

int main(void)
{
    while(1);
    
    return 0;
}

void PWM_Init(void)
{
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = 624;   
}

void TIMER_Init(void)
{
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS02) | (1 << CS00);
    OCR0A = 155;
    TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER0_COMPA_vect)
{
    
}