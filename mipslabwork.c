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

int mytime = 0x5957;
int timeoutcount = 0;
int spaceX = 0;
int spaceY = 0;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */
void user_isr( void ) {
  IFSCLR(0) = 0x100;  // Clear interrupt flag;
}


/* Lab-specific initialization goes here */
void labinit( void )
{
  // Initialize TRISE 7:0 to 0
  volatile int* ledoutput = (volatile int*) 0xbf886100;
  *ledoutput = *ledoutput & 0xffffff00;
  // Initialize PORTE 7:0 to 0
  volatile int* ledvalue = (volatile int*) 0xbf886110;
  *ledvalue = *ledvalue & ~0xff;
  // Initialize TRISD 11:4 to 1
  TRISD |= 0xff0;
  // Basic initialization of timer 2
  T2CON = 0x70; // Stop timer and clear control register, set prescaler at 1:256
  TMR2 = 0x0; // Clear timer register
  PR2 = 0x7a12; // Load period register
  IPC(2) = 0xd; // Set interrupt priority
  IFSCLR(0) = 0x100;  // Clear interrupt flag;
  IEC(0) = 0x100;  // Enable interrupts
  T2CONSET = 0x8070; // Start timer
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  int pressed = getbtns();
  if (pressed & 2) {
    spaceY++;
  }
  if (pressed & 4) {
    spaceY--;
  }
  // display_debug(&spaceY);
  delay(10000);
  display_update();
}
