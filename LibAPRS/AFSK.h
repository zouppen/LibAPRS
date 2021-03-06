#ifndef AFSK_H
#define AFSK_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "FIFO.h"
#include "HDLC.h"

#define SIN_LEN 512
static const uint8_t sin_table[] PROGMEM =
{
    128, 129, 131, 132, 134, 135, 137, 138, 140, 142, 143, 145, 146, 148, 149, 151,
    152, 154, 155, 157, 158, 160, 162, 163, 165, 166, 167, 169, 170, 172, 173, 175,
    176, 178, 179, 181, 182, 183, 185, 186, 188, 189, 190, 192, 193, 194, 196, 197,
    198, 200, 201, 202, 203, 205, 206, 207, 208, 210, 211, 212, 213, 214, 215, 217,
    218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 234, 235, 236, 237, 238, 238, 239, 240, 241, 241, 242, 243, 243, 244, 245,
    245, 246, 246, 247, 248, 248, 249, 249, 250, 250, 250, 251, 251, 252, 252, 252,
    253, 253, 253, 253, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255,
};

inline static uint8_t sinSample(uint16_t i) {
    uint16_t newI = i % (SIN_LEN/2);
    newI = (newI >= (SIN_LEN/4)) ? (SIN_LEN/2 - newI -1) : newI;
    uint8_t sine = pgm_read_byte(&sin_table[newI]);
    return (i >= (SIN_LEN/2)) ? (255 - sine) : sine;
}


#define SWITCH_TONE(inc)  (((inc) == MARK_INC) ? SPACE_INC : MARK_INC)

#define CPU_FREQ F_CPU

#define CONFIG_AFSK_TX_BUFLEN 64   
#define SAMPLERATE ((CPU_FREQ+FREQUENCY_CORRECTION) / 1024)
#define BITRATE    1200
#define SAMPLESPERBIT (SAMPLERATE / BITRATE)
#define BIT_STUFF_LEN 5
#define MARK_FREQ  1200
#define SPACE_FREQ 2200

typedef struct Afsk
{
    // General values
    uint16_t preambleLength;                // Length of sync preamble
    uint16_t tailLength;                    // Length of transmission tail

    // Modulation values
    uint8_t sampleIndex;                    // Current sample index for outgoing bit 
    uint8_t currentOutputByte;              // Current byte to be modulated
    uint8_t txBit;                          // Mask of current modulated bit
    bool bitStuff;                          // Whether bitstuffing is allowed

    uint8_t bitstuffCount;                  // Counter for bit-stuffing

    uint16_t phaseAcc;                      // Phase accumulator
    uint16_t phaseInc;                      // Phase increment per sample

    FIFOBuffer txFifo;                      // FIFO for transmit data
    uint8_t txBuf[CONFIG_AFSK_TX_BUFLEN];   // Actial data storage for said FIFO

    volatile bool sending;                  // Set when modem is sending
    int en_pin;                             // TX enable pin on HX1
} Afsk;

#define DIV_ROUND(dividend, divisor)  (((dividend) + (divisor) / 2) / (divisor))
#define MARK_INC   (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)MARK_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))
#define SPACE_INC  (uint16_t)(DIV_ROUND(SIN_LEN * (uint32_t)SPACE_FREQ, CONFIG_AFSK_DAC_SAMPLERATE))

#define AFSK_DAC_IRQ_START()   do { extern bool hw_afsk_dac_isr; hw_afsk_dac_isr = true; } while (0)
#define AFSK_DAC_IRQ_STOP()    do { extern bool hw_afsk_dac_isr; hw_afsk_dac_isr = false; } while (0)
#define AFSK_DAC_INIT()        do { DAC_DDR |= 0xF0; } while (0)

void AFSK_init(Afsk *afsk, int en_pin);
void AFSK_transmit(char *buffer, size_t size);

void afsk_putchar(char c);

void tx_on();
void tx_off();

#endif
