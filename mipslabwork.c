#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

#define MAX_BULLETS 20
#define BULLET_VELOCITY 2
#define MAX_ASTEROIDS 15
#define SPAWN_FREQUENCY 10

uint8_t displaybuffer[512];
// Define list of sprites
int ship_v1[] = {73,85,85,42,20,28,8};
int ship_v2[] = {85,127,107,85,85,28,8};
int ship_v3[] = {54,73,85,107,99,54,20};
int active_ship[3] = {ship_v1, ship_v2, ship_v3};

int asteroid_1[] = {126,255,255,255,255,255,126};
int asteroid_2[] = {126,255,207,195,207,255,126};
int asteroid_3[] = {126,207,195,129,195,207,126};
int asteroid_4[] = {126,129,129,129,129,129,126};
int asteroid[4] = {asteroid_1, asteroid_2, asteroid_3, asteroid_4};

int bullet_level1[] = {2,2,0};
int bullet_level2[] = {2,7,2};
int bullet_level3[] = {7,7,2};
int bullet[3] = {bullet_level1, bullet_level2, bullet_level3};

//int pixel[1] = {1};

int xpos = 10;
int ypos = 10;
int stickX = 0;
int stickY = 0;
int button = 0;
int button3 = 0;
int rep = 0;
int bulletPositions[MAX_BULLETS*2];
int bulletCount = 0;
int asteroidPositions[MAX_ASTEROIDS*2];
int asteroidHealth[MAX_ASTEROIDS];
int asteroidCount = 0;
int playerEnergy = 8; //health and energy is the same (shields)

/* Interrupt Service Routine */
void user_isr( void )
{
  display_clear(&displaybuffer);
  stickX = ADC1BUF0;
  stickY = ADC1BUF1;
  button = !((PORTF & 8) >> 3);
  button3 = (PORTD & 0x40) >> 6;
  AD1CON1SET = 0x2; // Start sampling

  //checking inputs and timers for spawning of new entities
  rep++;
    if (rep / SPAWN_INTERWAL) {
        if(randomNumberGenerator(xpos + ypos + bulletPositions[0] + asteroidPositions[0]) >= 5)
        {
            spawn_asteroid(asteroidPositions, &asteroidCount, MAX_ASTEROIDS*2);
        }
        rep = 0;
    }
    if (rep % 2) {
      if (stickX == 0) {
        xpos++;
      }
      if (stickX == 0x3ff) {
        xpos--;
      }
      if (stickY == 0) {
        ypos--;
      }
      if (stickY == 0x3ff) {
        ypos++;
      }
    }

    if ( rep % 4 )
    {
        spawn_bullet(xpos, ypos, bulletPositions, MAX_BULLETS*2);
    }

//rendering all active asteroids
    display_all_asteroids(displaybuffer, asteroidPositions, asteroid[0], MAX_ASTEROIDS*2);

//checking for asteroid collission with ship
    if (collission_check(displaybuffer, xpos, ypos, active_ship[1]))
    {
      xpos = 0;
      ypos = 13;
    }

//spawning all active bullets
    display_all_bullets(displaybuffer, bulletPositions, asteroidPositions, asteroidHealth, bullet[0], MAX_BULLETS*2, MAX_ASTEROIDS*2);

    display_insert_data(displaybuffer, xpos, ypos, active_ship[0], 7);
    display_update_frame(displaybuffer);
    IFSCLR(1) = 0x2;  // Clear interrupt flag
}


/* Lab-specific initialization goes here */
void labinit( void ) {
  // Digital pin 1 to button
  TRISFSET = 0x0004;
  // Initialize Timer3
  T3CON = 0x0070; // 256x prescaling
  TMR3 = 0; // Clear timer register
  PR3 = 0x0fff; // Set timer period
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
  reset_asteroid_array(asteroidPositions, MAX_ASTEROIDS*2);
  reset_bullet_array(bulletPositions, MAX_BULLETS*2);
  // Enable global interrupts
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
}
