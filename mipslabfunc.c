/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include "variables.h"

/* Declare a helper function which is local to this file */
static void num32asc( char * s, int );

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tick( unsigned int * timep )
{
  /* Get current value, store locally */
  register unsigned int t = * timep;
  t += 1; /* Increment local copy */

  /* If result was not a valid BCD-coded time, adjust now */

  if( (t & 0x0000000f) >= 0x0000000a ) t += 0x00000006;
  if( (t & 0x000000f0) >= 0x00000060 ) t += 0x000000a0;
  /* Seconds are now OK */

  if( (t & 0x00000f00) >= 0x00000a00 ) t += 0x00000600;
  if( (t & 0x0000f000) >= 0x00006000 ) t += 0x0000a000;
  /* Minutes are now OK */

  if( (t & 0x000f0000) >= 0x000a0000 ) t += 0x00060000;
  if( (t & 0x00ff0000) >= 0x00240000 ) t += 0x00dc0000;
  /* Hours are now OK */

  if( (t & 0x0f000000) >= 0x0a000000 ) t += 0x06000000;
  if( (t & 0xf0000000) >= 0xa0000000 ) t = 0;
  /* Days are now OK */

  * timep = t; /* Store new value */
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr )
{
  display_string( 1, "Addr" );
  display_string( 2, "Data" );
  num32asc( &textbuffer[1][6], (int) addr );
  num32asc( &textbuffer[2][6], *addr );
  display_update();
}

uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);

	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);

	spi_send_recv(0x8D);
	spi_send_recv(0x14);

	spi_send_recv(0xD9);
	spi_send_recv(0xF1);

	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);

	spi_send_recv(0xA1);
	spi_send_recv(0xC8);

	spi_send_recv(0xDA);
	spi_send_recv(0x20);

	spi_send_recv(0xAF);
}

void display_string(int line, char *s) {
	int i;
	if(line < 0 || line >= 4)
		return;
	if(!s)
		return;

	for(i = 0; i < 16; i++)
		if(*s) {
			textbuffer[line][i] = *s;
			s++;
		} else
			textbuffer[line][i] = ' ';
}

#define ITOA_BUFSIZ ( 24 )
char* itoaconv( int num )
{
  register int i, sign;
  static char itoa_buffer[ ITOA_BUFSIZ ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ ITOA_BUFSIZ - 1 ] = 0;   /* Insert the end-of-string marker. */
  sign = num;                           /* Save sign. */
  if( num < 0 && num - 1 > 0 )          /* Check for most negative integer */
  {
    for( i = 0; i < sizeof( maxneg ); i += 1 )
    itoa_buffer[ i + 1 ] = maxneg[ i ];
    i = 0;
  }
  else
  {
    if( num < 0 ) num = -num;           /* Make number positive. */
    i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
    do {
      itoa_buffer[ i ] = num % 10 + '0';/* Insert next digit. */
      num = num / 10;                   /* Remove digit from number. */
      i -= 1;                           /* Move index to next empty position. */
    } while( num > 0 );
    if( sign < 0 )
    {
      itoa_buffer[ i ] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return( &itoa_buffer[ i + 1 ] );
}

void display_image(int x, const uint8_t *data) {
	int i, j;

	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;

		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(x & 0xF);
		spi_send_recv(0x10 | ((x >> 4) & 0xF));

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 32; j++)
			spi_send_recv(~data[i*32 + j]);
	}
}

void display_update(void) {
	int i, j, k;
	int c;
	for(i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;

		for(j = 0; j < 16; j++) {
			c = textbuffer[i][j];
			if(c & 0x80)
				continue;

			for(k = 0; k < 8; k++)
				spi_send_recv(font[c*8 + k]);
		}
	}
}

void display_update_frame(void) {
	int i, j;
	for (i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;
		for (j = 0; j < 128; j++) {
			spi_send_recv(displaybuffer[j + i*128]);
		}
	}
}

void display_insert_data(int x, int y, int* sprite, int sprite_size)
{
	// Conditions for rendering inside the frame, skipping the drawing of any object that won't be visible.
	if (sprite_size * -1 > x || x > 127 || -8 > y || y > 31)
	{
		return;	// If out of bounds on x- or y-axis, exit code from here.
	}

	int i;	//used for width and height of the art. All art is rendered in an i-by-i square of pixels.
	for (i = 0; i < sprite_size; i++) // Column
	{
		// These checks will only allow any pixels of any art to be drawn if the pixel is located inside of the screen.
		if(0 < i+x && i+x < 127 && -1 < i+y && i+y < 31)
		{
			// All pixels are ORed into the 512 element wide uint array. the target pixel on the Chipkit display
			// is decided by (x+i) (x-coordinate to place the art + specific pixel of art to draw) the y-coordinate of
			// the pixel to draw is decided by "y%8" for shifting the art's pixels back to the start of a display column.
			// and "128*(y/8)" for shifting the art to the next display segment which is located exactly 128 elements away
			//for seamless drawing between pages.
			displaybuffer[i+x+ 128*(y/8)] |= (sprite[i] << (y%8));
			displaybuffer[i+x+ 128*((y/8)+1)] |= (sprite[i] >> 8-(y%8));

			// Only bothers rendering art with a third segment if the art can span over three segments (has to be 10 pixels wide or more).
			if(sprite_size > 9)
			{
//				1. Code that mathematically should work flawlessly for 3-segment-spanning:
				//displaybuffer[i+x+ 128*((y/8)+2)] |= (sprite[i] >> 12-(y%8));

//				2. A test that doesn't crash but still only repeats the second segment's values:
//					(PS: you can try changing the "8-(y%8)" to "9-(y%8)" and other higher values)
				displaybuffer[i+x+ 128*((y/8)+2)] |= (sprite[i] >> 8-(y%8));

//				3. A test that shouldn't work but might work anyway? (it is technically equal to the 1st option):
				//displaybuffer[i+x+ 128*((y/8)+2)] |= (sprite[i] << (y%8)-8);
			}
		}
	}
}

void display_score(void) {
	int i = 0;
	int n = 0;
	if (score < 0) {
		score = 0;
	}
	char* p = itoaconv(score);
	while (p[i] >= 48) {
		n++;
		i++;
	}
	for (i = 0; i < n*5+1; i++) {
		displaybuffer[(127-(n*5+1))+i] &= ~SCOREPANEL;
	}
	int* f;
	for (i = 0; i < 6; i++) {
		f = numbers+(p[i]-48)*4;
		if (!(f < numbers)) {
			display_insert_data(97+(6-n)*5+i*5,0,f,4);
		}
	}
}

void display_text(int x, int y, char* string) {
	int i, currentx = x;
	int* charcode;
	for (i = 0; string[i] != 36; i++) {
		if (string[i] == 32) {
			// Space
			currentx += 3;
		} else if (string[i] == 45) {
			// -
			display_insert_data(currentx, y, numbers+40, 4);
			currentx += 5;
		} else if (string[i] == 46) {
			// .
			display_insert_data(currentx, y, numbers+60, 4);
			currentx += 5;
		} else if (string[i] == 47) {
			// /
			display_insert_data(currentx, y, numbers+44, 4);
			currentx += 5;
		} else if (string[i] > 47 && string[i] < 58) {
			// 0-9
			charcode = numbers + (string[i]-48)*4;
			display_insert_data(currentx, y, charcode, 4);
			currentx += 5;
		} else if (string[i] == 58) {
			// :
			display_insert_data(currentx, y, numbers+48, 4);
			currentx += 5;
		} else if (string[i] == 63) {
			// ?
			display_insert_data(currentx, y, numbers+52, 4);
			currentx += 5;
		} else if (string[i] > 64 && string[i] < 91) {
			// A-Z
			charcode = letters + (string[i]-65)*5;
			if (*charcode == 0) {
				display_insert_data(currentx, y, charcode, 5);
				currentx += 5;
			} else {
				display_insert_data(currentx, y, charcode, 5);
				currentx += 6;
			}
		} else if (string[i] > 96 && string[i] < 123) {
			// a-z
			charcode = letters + (string[i]-71)*5;
			if (*charcode == 0) {
				if (*(charcode+1) == 0) {
					currentx--;
					display_insert_data(currentx, y, charcode, 5);
					currentx += 4;
				} else {
					display_insert_data(currentx, y, charcode, 5);
					currentx += 5;
				}
			} else {
				display_insert_data(currentx, y, charcode, 5);
				currentx += 6;
			}
		}
	}
}

void display_clear(void) {
	int i;
	for (i = 0; i < 512; i++) {
		displaybuffer[i] = 0;
	}
}

/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc( char * s, int n )
{
  int i;
  for( i = 28; i >= 0; i -= 4 )
    *s++ = "0123456789ABCDEF"[ (n >> i) & 15 ];
}

//random number generator function
int previous;
int seed[] = {8,3,5,2,9,6,4,1,7,0};
int randomNumberGenerator (int param)
{
	previous = seed[seed[(previous + param) % 10]];
	return previous;
}

//Extra commands for tasks such as spawning and collission calculation

//Asteroid Spawn:
void reset_asteroid_array(void)
{
	int i;
	for (i=0; i<MAX_ASTEROIDS*2; i+=2)
	{
		asteroidPositions[i] = AST_INACTIVE;
	}
}

void reset_bullet_array(void)
{
	int i;
	for (i=0; i<MAX_BULLETS*2; i+=2)
	{
		bulletPositions[i] = 128;
	}
}

void spawn_asteroid (int giant)
{
	int i;
	for (i=0; i<MAX_ASTEROIDS*2; i+=2)
	{
		if (asteroidPositions[i] <= AST_INACTIVE)
		{
			asteroidPositions[i] = 128;
			int randVal = randomNumberGenerator(asteroidPositions[0]);
			//randomized y-location:
			//int newLoc = location[0] % 11 + location[2] % 5 + location[4] % 7;
			int newLoc = (giant? 2:1)*randomNumberGenerator(randVal) + randVal/2;
			asteroidPositions[i+1] = ( newLoc > (giant? 15:25) || newLoc < (giant? 5:1) )? 12 : newLoc;
			// location[i+1] = (location == asteroidPositions)? 1 : 20;
			asteroidHealth[i/2] = (giant? 30:10);
			return;
		}
	}
}

void spawn_bullet (int set_bullet_level)
{
	int i;
	for (i=0; i<MAX_BULLETS*2; i+=2)
	{
		if (bulletPositions[i] > 127)
		{
			bullets_level[i/2] = set_bullet_level;
			bulletPositions[i] = xpos+7;
			bulletPositions[i+1] = ypos+2;
			return;
		}
	}
}

void display_all_asteroids(void)
{
	int i;
	for (i=0; i<MAX_ASTEROIDS*2; i+=2)
  	{
		if (asteroidPositions[i] > AST_INACTIVE) //checking only x-values for active state (not -7)
		{
			display_insert_data(asteroidPositions[i], asteroidPositions[i+1], asteroid[asteroidHealth[i/2]/3], 7);
			asteroidPositions[i]--;
		}
		else if (asteroidPositions[i] != AST_INACTIVE)
		{
			asteroidPositions[i] = AST_INACTIVE;
		}
  	}
}

void display_all_bullets
(int* location, int* asteroids, int* asthp) //replace all parameters with variables.h
{
	int i;
	for (i=0; i<MAX_BULLETS*2; i+=2)
 	{
		if (location[i] < 127) //checking only x-values for active state (not 127)
		{
			// check which asteroid collides with bullet
			int j;
			for (j=0; j<MAX_ASTEROIDS*2; j+=2)
			{	// asteroids[j+1] < location[i+1] && location[i+1] < asteroids[j+1]+7
				if(	asteroids[j] != AST_INACTIVE &&
					asteroids[j]+7 >= location[i] && location[i]+3 >= asteroids[j] &&
					asteroids[j+1] <= location[i+1]+1 && location[i+1]+1 <= asteroids[j+1]+7)
				{
					asthp[j/2]-=(bullets_level[i/2] + 1);
					score++;
					location[i] = 128;
					if(asthp[j/2] < 1)
					{
						score += 5;
						asteroids[j] = AST_INACTIVE;
					}
				}
			}
			display_insert_data(location[i], location[i+1], bullet[bullets_level[i/2]], 3);
			location[i]+=4;

		}
		else if (location[i] != 128)
		{
			location[i] = 128;
		}
  }
}

// Collission calculation functions:
int collission_check (int* sprite) //TODO:replace sprite with variables.h def
{
	int i;
	for(i=xpos; i < xpos+7; i++) //i will check framebuffer locations where ship will render. Columns.
	{
		int j;
		for (j=ypos; j < ypos+7; j++) //j will check each pixel in y-axis.
		{
			//if ( (framebuffer[i+ 127*(j/8)] >> j%8) & 0x1 == 1 && (sprite[i-x] >> j-y) & 0x1 == 1)
			if(1 < i && i < 127)
			{
				if ( (displaybuffer[i+ 127*(j/8)] >> j%8) & 0x1 == 1)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

void stick_actions(void) {
	if (stickX == 0) {
      if(xpos < 120)
        xpos++;
    }
    else if (stickX == 0x3ff) {
      if(xpos > 1)
        xpos--;
    }
    if (stickY == 0) {
      if(ypos > 0)
        ypos--;
    }
    if (stickY == 0x3ff) {
      if(ypos < 24)
        ypos++;
    }
}

int stickX_gate(void) {
	// Functions for X direction
	if (stickX == 0 && stickPull == 0) {
		stickPull = 1;
		return 1;
	}
	if (stickX == 0x3ff && stickPull == 0) {
		stickPull = 1;
		return -1;
	}
	if (stickX == 0x2ff && stickPull == 1) {
		stickPull = 0;
		return 0;
	}
	return 0;
}

int stickY_gate(void) {
	// Fucntions for Y direction
	if (stickY == 0 && stickPull == 0) {
		stickPull = 1;
		return -1;
	}
	if (stickY == 0x3ff && stickPull == 0) {
		stickPull = 1;
		return 1;
	}
	if (stickY == 0x2ff && stickPull == 1) {
		stickPull = 0;
		return 0;
	}
	return 0;
}

// Chooses an appropriate button to select bullet power
static int power = 0;
int pickAmmo(void)
{
	if (PORTD & 0x70)
	{
		if ((button4)) {
			power = 2;
		} else if (button3) {
			power = 1;
		} else if (button2) {
			power = 0;
		}
	}
	return power;
}

void display_energy(void) {
	if (playerEnergy > 8) {
		playerEnergy = 8;
	}
	if (playerEnergy == 0) {
		playerEnergy = 1;
	}
	if (playerEnergy < 0) {
		PORTE = 0;
	}
	if (playerEnergy == 8) {
		PORTE = 0xff;
	} else if (playerEnergy == 7) {
		PORTE = 0x7f;
	} else if (playerEnergy == 6) {
		PORTE = 0x3f;
	} else if (playerEnergy == 5) {
		PORTE = 0x1f;
	} else if (playerEnergy == 4) {
		PORTE = 0xf;
	} else if (playerEnergy == 3) {
		PORTE = 0x7;
	} else if (playerEnergy == 2) {
		PORTE = 0x3;
	} else if (playerEnergy == 1) {
		if (pickAmmo()) {
			power = 0;
		}
		PORTE = 0x1;
	}
}

void display_startup_screen(void) {
	display_text(1,0,"Welcome to A-STEROID-z$");
	display_text(1,16,"Made by CINTE-Soft Studios$");
	display_update_frame();
}

void display_main_menu(void) {
	// Title bar
	display_text(1,0,"Main Menu$");
	display_text(70,0,"A-STEROID-z$");
	// Options
	display_text(8,8,"Play game$");
	display_text(8,16,"Controls$");
	display_text(77,8,"Highscores$");
	// Control 1
	display_insert_data(1,24,symbols+12,6);
	display_text(8,25,"/$");
	display_insert_data(12,24,symbols+24,6);
	display_text(19,25," - Up/Down$");
	// Control 2
	display_insert_data(77,24,symbols+30,6);
	display_text(84,25," - Select$");
}

void display_controls(void) {
	// Title bar
	display_text(1,0,"Controls$");
	// Content
	display_text(8,8,"Use the joystick to move.$");
	display_text(8,16,"Destroy asteroids to gain$");
	// Control 1
	display_insert_data(1,24,symbols+18,6);
	display_text(8,25," - Next page$");
	// Control 2
	display_insert_data(77,24,symbols+30,6);
	display_text(84,25," - Back$");
}

void display_cursor(void) {
	if (gamemode == 1) {
		display_insert_data(1, 8, numbers+56, 4);
	} else if (gamemode == 2) {
		display_insert_data(1, 16, numbers+56, 4);
	} else if (gamemode == 3) {
		display_insert_data(70, 8, numbers+56, 4);
	}
}

void display_pause_screen(void) {
	int i;
	for (i = 0; i < 30; i++) {
		displaybuffer[159+i] &= 0;
	}
	display_text(34, 8, "Paused$");
}

void play_game(void) {
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
  if (collission_check(active_ship[1]))
  {
    score -= 20;
    playerEnergy -= 4;
    xpos = 0;
  }

  //spawning all active bullets
  display_all_bullets(bulletPositions, asteroidPositions, asteroidHealth);

  display_insert_data(xpos, ypos, active_ship[pickAmmo()], 7);
  display_score();
  display_energy();
  display_update_frame();
}
