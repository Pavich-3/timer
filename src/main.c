#include <avr/io.h>
#include <avr/interrupt.h>>
#include <stdbool.h>

#define F_CPU 8000000UL
#define PWM_STEP 10

const uint16_t maxDutyCycle = 624;
volatile uint16_t targetDutyCycle = 0;
volatile bool isIncreasing = true;

void PWM_Init(void);
void TIMER_Init(void);

int main(void)
{
    PWM_Init();
    TIMER_Init();
    sei();

    while(1);
    
    return 0;
}

void PWM_Init(void)
{
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    ICR1 = maxDutyCycle;   
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
    if (isIncreasing)
    {
        if (targetDutyCycle <= maxDutyCycle - PWM_STEP) {
            targetDutyCycle += PWM_STEP;
        } else {
            targetDutyCycle = maxDutyCycle;
            isIncreasing = false;
        }
    }
    else 
    {
        if (targetDutyCycle >= PWM_STEP) {
            targetDutyCycle -= PWM_STEP;
        } else {
            targetDutyCycle = 0;
            isIncreasing = true;
        }
    }
    OCR1A = targetDutyCycle;
}