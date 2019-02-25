/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_BULLETS 30
#define BULLET_VELOCITY 2
#define MAX_ASTEROIDS 30

int mytime = 0x5957;
int timeoutcount = 0;
int spaceX = 0;
int spaceY = 0;
int button2down = 0;
int button2up = 0;
int button3down = 0;
int button3up = 0;
int x = 0;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
  x = ADC1BUF0;
  IFSCLR(1) = 0x2;  // Clear interrupt flag;
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Initialize TRISD 11:4 to 1
  TRISD |= 0xff0;

  // Initialize timer 3 for use with ADC event trigger
  T3CON = 0;  // Disable timer
  T3CONSET = 0x72;  // 256x prescaling, external clock source
  TMR3 = 0; // Clear timer register
  PR3 = 0x7a12; // Set timer period
  T3CONSET = 0x8000;  // Start timer

  // Initialize PORTB bits 10 and 8 to 1
  TRISB |= 0x100;
  // Initialize ADC
  AD1PCFG = 0;  // All inputs are analog
  AD1CON1CLR = 0x8000;  // ADC off
  AD1CON1 = 0x0240; // FORM: 16 bit fraction, timer 3 control
  AD1CON2 = 0;  // AVdd and AVss voltage, do not scan, interrupt every sample, 16 word buffer, always MUX A
  AD1CON3 = 0x0101; // Peripheral bus clock, 4x prescaling
  AD1CON1SET = 0x8000;  // Enable ADC
  IPCSET(6) = 0x17000000; // Set priority 5, subpriority 3
  IFSCLR(1) = 0x2;  // Clear interrupt flag
  IECSET(1) = 0x2;  // Enable ADC interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  /*  // Button gates
  button2down = pressed(2);
  button3down = pressed(4);
  if (button2down && (button2up == 0)) {
    spaceY++;
    button2up = 1;
  }
  if (button2up && (button2down == 0)) {
    button2up = 0;
  }
  if (button3down && (button3up == 0)) {
    spaceY--;
    button3up = 1;
  }
  if (button3up && (button3down == 0)) {
    button3up = 0;
  }
  //*/

  /*  // Move mockup spaceship
  switch (spaceY & 0xf) {
    case 0x0: display_string(0,"-");break;
    case 0x1: display_string(0," -");break;
    case 0x2: display_string(0,"  -");break;
    case 0x3: display_string(0,"   -");break;
    case 0x4: display_string(0,"    -");break;
    case 0x5: display_string(0,"     -");break;
    case 0x6: display_string(0,"      -");break;
    case 0x7: display_string(0,"       -");break;
    case 0x8: display_string(0,"        -");break;
    case 0x9: display_string(0,"         -");break;
    case 0xa: display_string(0,"          -");break;
    case 0xb: display_string(0,"           -");break;
    case 0xc: display_string(0,"            -");break;
    case 0xd: display_string(0,"             -");break;
    case 0xe: display_string(0,"              -");break;
    case 0xf: display_string(0,"               -");break;
  }
  //*/

  display_debug(&x);

  display_update();
}
