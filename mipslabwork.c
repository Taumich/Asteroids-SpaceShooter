#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "variables.h"

/* Interrupt Service Routine */
void user_isr( void )
{
  display_clear();
  stickX = ADC1BUF0;
  stickY = ADC1BUF1;
  buttonj = !((PORTF & 8) >> 3);
  button4 = (PORTD & 0x80) >> 7;
  button3 = (PORTD & 0x40) >> 6;
  button2 = (PORTD & 0x20) >> 5;
  AD1CON1SET = 0x2; // Start sampling

  //checking inputs and timers for spawning of new entities
  rep++;
  if (rep > 100000) {
    rep = 0;
  }
  if (!(rep % SPAWN_INTERVAL) ) {
    if(randomNumberGenerator(rep + bulletPositions[0] + asteroidPositions[0]) >= 5)
    {
      spawn_asteroid(0);
    }
  }
  if (pickAmmo() == 1) {
    if (!(rep % 20)) {
      playerEnergy--;
    }
  } else if (pickAmmo() == 2) {
    if (!(rep % 15)) {
      playerEnergy--;
    }
  } else if (!(rep % 40)) {
    playerEnergy++;
  }
  //Movement
  if (rep % 2) {
    stick_actions();
  }

  if ( !(rep % BULLET_INTERVAL) )
  {
    spawn_bullet(pickAmmo());
  }

  //rendering all active asteroids
  display_all_asteroids();

  //checking for asteroid collission with ship
  if (collission_check(xpos, ypos, active_ship[1]))
  {
    score -= 20;
    playerEnergy -= 4;
    xpos = 0;
  }

  //spawning all active bullets
  display_all_bullets(bulletPositions, asteroidPositions, asteroidHealth, bullet);

  display_insert_data(xpos, ypos, active_ship[pickAmmo()], 7);
  display_score();
  display_energy();
  display_update_frame();
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
  xpos = 10;
  ypos = 10;
  score = 0;
  playerEnergy = 8;
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  gamemode = 0;
  display_main_menu();
  if (gamemode) {
    labinit();
    while (1) {
      if (buttonj) {
        T3CONCLR = 0x8000; // Timer3 off
        PORTE = 0x00;
        IFSCLR(1) = 0x2;  // Clear interrupt flag
        AD1CON1CLR = 0x8000;  // ADC off
        gamemode = 0;
        return;
      }
    }
  }
}
