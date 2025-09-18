#include <avr/io.h>         // Standard AVR Input/Output library
#include <avr/interrupt.h>  // Library for interrupt handling (ISR)
#include <stdbool.h>        // Library for boolean type (true/false)

// --- Configuration ---
#define F_CPU 8000000UL     // Define the microcontroller's clock speed (8 MHz)
#define PWM_STEP 10         // The value to increase/decrease the duty cycle by in each step

// --- Global Variables ---
const uint16_t maxDutyCycle = 624; // Corresponds to the TOP value for 200 Hz PWM frequency
volatile uint16_t targetDutyCycle = 0; // Holds the current duty cycle value. 'volatile' is essential as it's shared with an ISR.
volatile bool isIncreasing = true;     // Animation direction flag. 'volatile' is also required here.

// --- Function Prototypes ---
void PWM_Init(void);     // Initializes Timer1 for PWM signal generation
void TIMER_Init(void);   // Initializes Timer0 for periodic animation updates

// --- Main Program Entry ---
int main(void)
{
    // --- Initialization ---
    PWM_Init();   // Set up the PWM hardware
    TIMER_Init(); // Set up the interrupt timer
    sei();        // Enable global interrupts

    // --- Main Loop ---
    // The main loop is empty because all the work (the breathing animation)
    // is handled by the Timer0 interrupt in the background.
    while(1);
    
    return 0; // This line is never reached
}

/**
 * @brief Configures Timer1 to generate a Fast PWM signal on the OC1A pin.
 */
void PWM_Init(void)
{
    // Configure Timer1 Control Register A
    // COM1A1=1, COM1A0=0: Non-inverting mode for OC1A output.
    // WGM11=1: Part of the Mode 14 (Fast PWM, TOP=ICR1) setting.
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);

    // Configure Timer1 Control Register B
    // WGM13=1, WGM12=1: Completes the Mode 14 setting.
    // CS11=1, CS10=1: Sets the prescaler to 64. (8MHz / 64 = 125kHz timer clock)
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
    
    // Set the TOP value for the timer counter.
    // This value, with a prescaler of 64, results in a 200 Hz PWM frequency.
    // F_PWM = F_CPU / (Prescaler * (1 + ICR1)) = 8,000,000 / (64 * (1 + 624)) = 200 Hz
    ICR1 = maxDutyCycle;   
}

/**
 * @brief Configures Timer0 to trigger a periodic interrupt.
 * This interrupt serves as the engine for the breathing animation.
 */
void TIMER_Init(void)
{
    // Configure Timer0 Control Register A
    // WGM01=1: Sets CTC (Clear Timer on Compare Match) mode.
    TCCR0A |= (1 << WGM01);

    // Configure Timer0 Control Register B
    // CS02=1, CS00=1: Sets the prescaler to 1024. (8MHz / 1024 = 7812.5 Hz timer clock)
    TCCR0B |= (1 << CS02) | (1 << CS00);
    
    // Set the compare value for the interrupt.
    // This results in an interrupt frequency of about 50 Hz.
    // F_INT = 7812.5 Hz / (1 + 155) = ~50.08 Hz
    OCR0A = 155;
    
    // Enable the Timer0 Compare Match A interrupt.
    TIMSK0 |= (1 << OCIE0A);
}

/**
 * @brief Interrupt Service Routine for Timer0 Compare Match A.
 * This function is executed automatically at ~50 Hz and updates the LED brightness.
 */
ISR(TIMER0_COMPA_vect)
{
    // Check the current direction of the animation
    if (isIncreasing)
    {
        // If brightness is increasing, add the step value
        if (targetDutyCycle <= maxDutyCycle - PWM_STEP) {
            targetDutyCycle += PWM_STEP;
        } else {
            // If the max value is reached, cap it and reverse direction
            targetDutyCycle = maxDutyCycle;
            isIncreasing = false;
        }
    }
    else // If brightness is decreasing
    {
        // Subtract the step value
        if (targetDutyCycle >= PWM_STEP) {
            targetDutyCycle -= PWM_STEP;
        } else {
            // If the min value is reached, cap it and reverse direction
            targetDutyCycle = 0;
            isIncreasing = true;
        }
    }
    
    // Update the actual PWM hardware register with the new duty cycle value.
    OCR1A = targetDutyCycle;
}