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
  PORTE += 1; // Add 1 to LED counter
  stickX = ADC1BUF0;
  button = !((PORTF & 8) >> 3);
  AD1CON1SET = 0x2; // Start sampling
  display_update();
  IFSCLR(1) = 0x2;  // Clear interrupt flag
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Digital pin 1 to button
  TRISFSET = 0x4;
  // Timer to LEDs
  TRISECLR = 0xff;
  PORTE = 0;
  // Initialize Timer3
  T3CON = 0x70;
  TMR3 = 0;
  PR3 = 0xffff;
  T3CONSET = 0x8000;
  // Initialize PORTB bits 10 and 8 to 1
  TRISB |= 0x500;
  // Initialize ADC
  AD1PCFG = ~0x100;  // Analog pin 2 to analog input
  AD1CON1CLR = 0x8000;  // ADC off
  AD1CON1 = 0x0240; // FORM: 16 bit fraction, timer 3 control
  AD1CON2 = 0;  // AVdd and AVss voltage, do not scan, interrupt every sample, 16 word buffer, always MUX A
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
  display_debug(&button);
}
