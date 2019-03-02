#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "variables.h"

/* Interrupt Service Routine */
void user_isr( void )
{
  if (gamemode == 2) {
    play_game();
  } else if (gamemode == 1) {
    display_clear();
    display_main_menu();
    display_update_frame();
    while (!button4) {
  		button4 = (PORTD & 0x80) >> 7;
  	}
  	gamemode = 2;
  } else if (!gamemode) {
    display_clear();
    if (!rep) {
      display_startup_screen();
    }
    rep++;
    if (rep == 120) {
      gamemode++;
      PORTE = 0;
      rep = 0;
    }
  }
  IFSCLR(1) = 0x2;  // Clear interrupt flag
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Initialize LED array
  TRISECLR = 0xff;
  PORTE = 0xff;
  // Digital pin 1 to button
  TRISFSET = 0x0004;
  // Initialize Timer3
  T3CON = 0x0070; // 256x prescaling
  TMR3 = 0; // Clear timer register
  PR3 = 0x100f; // Set timer period
  T3CONSET = 0x8000;  // Turn on Timer3
  // Initialize PORTB bits 10 and 8 to 1
  TRISB |= 0x0500;
  // BEGIN Initialize ADC
  AD1CON1CLR = 0x8000;  // ADC off
  AD1CHS = 0x0a080000; // Select input pins to ADC
  AD1PCFG = 0xfaff;  // AN8 and AN10 to analog input
  AD1CSSL = 0x0500; // Select AN8 and AN10 for scan sequence
  AD1CON1 = 0x0444; // FORM: 16 bit fraction, timer 3 control, sample auto-start
  AD1CON2 = 0x4404;  // Internal voltage +, external voltage -, scan, interrupt every two samples, 16 word buffer, always MUX A
  AD1CON3 = 0x8408; // Internal clock, 4x prescaling
  AD1CON1SET = 0x8000;  // Enable ADC
  AD1CON1SET = 0x2; // Start sampling
  IPCSET(6) = 0x17000000; // Set priority 5, subpriority 3
  IFSCLR(1) = 0x2;  // Clear interrupt flag
  IECSET(1) = 0x2;  // Enable ADC interrupts
  // END Initialize ADC
  // Initialize asteroids and bullets
  reset_asteroid_array();
  reset_bullet_array();
  gamemode = 0;
  xpos = 10;
  ypos = 10;
  score = 0;
  playerEnergy = 8;
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  if (gamemode) {
    while (1) {
      if (buttonj) {
        while (buttonj);
        display_clear();
        gamemode = 1;
        return;
      }
    }
  }
}
