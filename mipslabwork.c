#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_BULLETS 30
#define BULLET_VELOCITY 2
#define MAX_ASTEROIDS 30

int stickX = 0;
int button = 0;

/* Interrupt Service Routine */
void user_isr( void ) {
  stickX = ADC1BUF0;
  button = !((PORTF & 8) >> 3);
  AD1CON1SET = 0x2; // Start sampling
  display_debug(&stickX);
  display_update();
  IFSCLR(1) = 0x2;  // Clear interrupt flag
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Digital pin 1 to button
  TRISFSET = 0x0004;
  // Initialize Timer3
  T3CON = 0x0070; // 256x prescaling
  TMR3 = 0; // Clear timer register
  PR3 = 0xafff; // Set timer period
  T3CONSET = 0x8000;  // Turn on Timer3
  // Initialize PORTB bits 10 and 8 to 1
  TRISB |= 0x0500;
  // Initialize ADC
  AD1CON1CLR = 0x8000;  // ADC off
  AD1CHS = 0x0a880000; // Select input pins to ADC
  AD1PCFG = 0xfeff;  // AN8 to analog input
  AD1CON1 = 0x0440; // FORM: 16 bit fraction, timer 3 control
  AD1CON2 = 0x4000;  // External voltage, do not scan, interrupt every sample, 16 word buffer, always MUX A
  AD1CON3 = 0x0101; // Peripheral bus clock, 4x prescaling
  AD1CON1SET = 0x8000;  // Enable ADC
  AD1CON1SET = 0x2; // Start sampling
  IPCSET(6) = 0x17000000; // Set priority 5, subpriority 3
  IFSCLR(1) = 0x2;  // Clear interrupt flag
  IECSET(1) = 0x2;  // Enable ADC interrupts
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
}
