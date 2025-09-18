# AVR "Breathing" LED with Hardware PWM

This project demonstrates a smooth, non-blocking "breathing" (fading in and out) effect for an LED using two hardware timers on an AVR microcontroller (e.g., ATmega328P).

The animation logic is entirely interrupt-driven, leaving the main CPU loop free for other tasks.
