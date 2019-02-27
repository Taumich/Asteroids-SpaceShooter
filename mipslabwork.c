#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_BULLETS 30
#define BULLET_VELOCITY 2
#define MAX_ASTEROIDS 30

uint8_t displaybuffer[512];
// Define list of sprites
int ship_v1[] = {73,85,85,42,20,28,8};
int ship_v2[] = {85,127,107,85,85,28,8};
int ship_v3[] = {54,73,85,107,99,54,20};
int active_ship[3] = {ship_v1, ship_v2, ship_v3};

int asteroid[7] = {126,255,255,255,255,255,126};
int shot_level1[3] = {2,2,0};
int shot_level2[3] = {2,7,2};
int pixel[1] = {1};

int xpos = 0;
int ypos = 0;
int stickX = 0;
int stickY = 0;
int button = 0;
int rep = 0;
int asteroidPositions[20];
int asteroidCount = 0;

/* Interrupt Service Routine */
void user_isr( void )
{
  display_clear(&displaybuffer);
  stickX = ADC1BUF0;
  stickY = ADC1BUF1;
  button = !((PORTF & 8) >> 3);
  AD1CON1SET = 0x2; // Start sampling
  xpos++;
  rep++;

  if (rep=2)
  {
    rep = 0;
    ypos++;
    if (ypos > 16)
    {
      spawn_asteroid(asteroidPositions, &asteroidCount);
      ypos = 0;
      xpos = 0;
    }
  }

  //command for spawning a new asteroid
  //display_insert_data(&displaybuffer, asteroidPositions[0], asteroidPositions[1], asteroid, 7);
  //loop for displaying all active asteroids
  display_all_asteroids(displaybuffer, asteroidPositions, asteroid);

<<<<<<< HEAD
  if (collission_check(displaybuffer, xpos, ypos, active_ship[0]) == 1)
=======
  if (collission_check(displaybuffer, xpos, ypos, ship_right))
>>>>>>> d73fc1333e837c5252f8fd493116cc1a6ebfa4d6
  {
    xpos = 30;
    ypos = 0;
  }
  // bullet here

  //bullet
  display_insert_data(&displaybuffer, xpos+9, ypos, shot_level1, 3);

  display_insert_data(&displaybuffer, xpos, ypos, active_ship[0], 7);
  display_update_frame(&displaybuffer);
  IFSCLR(1) = 0x2;  // Clear interrupt flag
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Digital pin 1 to button
  TRISFSET = 0x0004;
  // Initialize Timer3
  T3CON = 0x0070; // 256x prescaling
  TMR3 = 0; // Clear timer register
  PR3 = 0xffff; // Set timer period
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
  // Initialize asteroids
  reset_asteroid_array(asteroidPositions);
  spawn_asteroid(asteroidPositions, &asteroidCount);
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
}
