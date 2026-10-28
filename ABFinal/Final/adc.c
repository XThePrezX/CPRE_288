#include "Timer.h"
#include "lcd.h"


void adc_init (void) {
    SYSCTL_RCGCGPIO_R |= 0b10;
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};

    SYSCTL_RCGCADC_R |= 0b1;
    while ((SYSCTL_PRADC_R & 0b1) == 0) {};


    GPIO_PORTB_DIR_R &= ~0x10; // 4) make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x10; // 5) enable alternate function on PB4
    GPIO_PORTB_DEN_R &= ~0x10; // 6) disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x10; // 7) enable analog functionality on PB4

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1; // 8) configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123; // 9) Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008; // 10) disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000; // 11) seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10; // 12) set channel
    ADC0_SSCTL3_R = 0x0006; // 13) no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008; // 14) disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008; // 15) enable sample sequencer 3
}

uint32_t adc_read (void) {
    uint32_t result;
    ADC0_PSSI_R = 0x0008; // 1) initiate SS3
    while((ADC0_RIS_R&0x08)==0){}; // 2) wait for conversion done
    result = ADC0_SSFIFO3_R&0xFFF; // 3) read result
    ADC0_ISC_R = 0x0008; // 4)acknowledge completion
    return result;
}
