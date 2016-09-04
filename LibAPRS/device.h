#include "constants.h"

#ifndef DEVICE_CONFIGURATION
#define DEVICE_CONFIGURATION

// CPU settings
#ifndef TARGET_CPU
    #define TARGET_CPU m32u4
#endif

#ifndef F_CPU
    #define F_CPU 16000000
#endif

#ifndef FREQUENCY_CORRECTION
    #define FREQUENCY_CORRECTION 0
#endif

// Sampling & timer setup
#define CONFIG_AFSK_DAC_SAMPLERATE 9600

// Port settings
#if TARGET_CPU == m328p
    #define DAC_PORT PORTD
    #define DAC_DDR  DDRD
    #define LED_PORT PORTB
    #define LED_DDR  DDRB
    #define LED_TX_BIT 2
    #define ADC_PORT PORTC
    #define ADC_DDR  DDRC
#error upposi
#else if TARGET_CPU == m32u4 
    #define DAC_PORT PORTB
    #define DAC_DDR  DDRB
    #define LED_PORT PORTC
    #define LED_DDR  DDRC
    #define LED_TX_BIT 7
    #define ADC_PORT PORTF
    #define ADC_DDR  DDRF
#endif

#endif
