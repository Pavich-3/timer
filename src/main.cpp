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