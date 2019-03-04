#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "variables.h"

/* Interrupt Service Routine */
void user_isr( void )
{
  if (gamemode > 19 && gamemode < 28) {
    // Controls
    display_clear();
    display_controls();
    display_cursor();
    stickX = ADC1BUF0;
    if (stickX <= 0x2ff) {
      gamemode += stickX_gate();
    }
    if (gamemode == 19) {
      gamemode = 20;
    } else if (gamemode == 28) {
      gamemode = 27;
    }
    buttonj = !((PORTF & 8) >> 3);
    if (buttonj) {
      while (buttonj) {
        buttonj = !((PORTF & 8) >> 3);
      }
      gamemode = 1;
    }
    display_update_frame();
  }
  if (gamemode == 12) {
    display_ingame_screen(35,9,"GAME OVER$");
    display_ingame_screen(35,17,"Press to return$");
    display_update_frame();
    buttonj = !((PORTF & 8) >> 3);
    if (buttonj) {
      while (buttonj) {
        buttonj = !((PORTF & 8) >> 3);
      }
      gamemode = 1;
      reset_asteroid_array();
      reset_bullet_array();
      xpos = 10;
      ypos = 10;
      score = 0;
      playerEnergy = 8;
    }
  }
  if (gamemode == 11) {
    // Pause screen
    display_ingame_screen(35,9,"Paused$");
    display_update_frame();
    stickX = ADC1BUF0;
    stickY = ADC1BUF1;
    buttonj = !((PORTF & 8) >> 3);
  }
  if (gamemode == 10) {
    // Main game
    play_game();
  } else if (gamemode > 0 && gamemode < 10) {
    // Main menu
    display_clear();
    display_main_menu();
    display_cursor();
    stickY = ADC1BUF1;
    gamemode += stickY_gate();
    if (gamemode == 0) {
      gamemode = 1;
    } else if (gamemode == 4) {
      gamemode = 3;
    }
    buttonj = !((PORTF & 8) >> 3);
    if (buttonj) {
      while (buttonj) {
        buttonj = !((PORTF & 8) >> 3);
      }
      gamemode *= 10;
    }
    display_update_frame();
  } else if (!gamemode) {
    // Splash screen
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
  stickPull = 0;
  xpos = 10;
  ypos = 10;
  score = 0;
  playerEnergy = 8;
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  if (gamemode == 10) {
    while (1) {
      if (buttonj) {
        while (buttonj);
        gamemode++;
        return;
      }
    }
  }
  if (gamemode == 11) {
    while (1) {
      if (stickX != 0x2ff || stickY != 0x2ff) {
        display_clear();
        gamemode--;
        return;
      }
      if (buttonj) {
        while (buttonj);
        gamemode = 1;
        return;
      }
    }
  }
}
