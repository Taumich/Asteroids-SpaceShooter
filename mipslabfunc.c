/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

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

#define AST_INACTIVE -7

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

void display_update_frame(uint8_t* framebuffer) {
	int i, j;
	for (i = 0; i < 4; i++) {
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;
		for (j = 0; j < 128; j++) {
			spi_send_recv(*framebuffer);
			framebuffer++;
		}
	}
}

void display_insert_data(uint8_t* framebuffer, int x, int y, int* sprite, int sprite_size) {
	// Conditions for rendering inside the frame
	if (sprite_size * -1 > x || x > 127) {
		return;	// if out of bounds on x-axis, exit code from here
	}
	if (-8 > y || y > 31) {
		return; // if out of bounds on y-axis, exit code from here
	}
	// Position on the current page byte segment
	//int ypos = y % 8;
	// Page of the display
	//int ypag = y / 8;
	// Variable to hold the root byte segment of the display
	uint8_t* root = framebuffer;
	// Chooses start position
	//framebuffer += x + 128 * (y/8);
	// Creates an offset variable that is overridden when the sprite is
	// out of bounds on the left side of the display (x-axis)

	//int offset = 0;
	//if (framebuffer < root) {
	//	offset = (root-framebuffer) >> 3;
	//}

	// framebuffer is now functional for x and y axis, i am not completely
	// sure how this code works but after intensive testing, this solved it:
	int i;	//used for width of the ship
	for (i = 0; i < sprite_size; i++) //column
	{
		//int xpx;
		//xpx = i+x;
		/*for(j = 0; j < sprite_size; j++) //row
		{
			//pixel to work with
			//int xpx, ypx, column;
			//ypx = j+y;

			//framebuffer++;
			//sprite++;
		}*/
		if(0 < i+x && i+x < 127)
		{
			framebuffer[i+x+ 128*(y/8)] |= (sprite[i] << (y%8));
			framebuffer[i+x+ 128*((y/8)+1)] |= (sprite[i] >> 8-(y%8));
		}
	}
	/*for (i = 0; i < sprite_size - offset; i++) {
		*framebuffer |= sprite[offset] << (y % 8);
		*(framebuffer+128) |= sprite[offset] >> 8-(y % 8);
		framebuffer++;
		sprite++;
	}*/
}

void display_clear(uint8_t* framebuffer) {
	int i;
	for (i = 0; i < 512; i++) {
		*framebuffer++ = 0;
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
void reset_asteroid_array(int* location, int max)
{
	int i;
	for (i=0; i<max; i+=2)
	{
		location[i] = AST_INACTIVE;
	}
}

void reset_bullet_array(int* location, int max)
{
	int i;
	for (i=0; i<max; i+=2)
	{
		location[i] = 128;
	}
}

void spawn_asteroid (int *location, int quantity, int* asthp, int max)
{
	int i;
	for (i=0; i<max; i+=2)
	{
		if (location[i] <= AST_INACTIVE)
		{
			location[i] = 128;
			int randVal = randomNumberGenerator(location[0]);
			//randomized y-location:
			//int newLoc = location[0] % 11 + location[2] % 5 + location[4] % 7;
			int newLoc = 2*randomNumberGenerator(randVal) + randVal/2;
			location[i+1] = (newLoc > 25 || newLoc < 1)? 12 : newLoc;
			asthp[i/2] = 10;
			return;
		}
	}
}

void spawn_bullet (int x, int y, int* location, int max)
{
	int i;
	for (i=0; i<max; i+=2)
	{
		if (location[i] > 127)
		{
			location[i] = x+7;
			location[i+1] = y+2;
			return;
		}
	}
}

void display_all_asteroids(uint8_t* framebuffer, int* location, int* asthp, int* sprite, int max)
{
	int i;
	for (i=0; i<max; i+=2)
  	{
		if (location[i] > AST_INACTIVE) //checking only x-values for active state (not -7)
		{
			display_insert_data(framebuffer, location[i], location[i+1], sprite[asthp[i/2]/3], 7);
			location[i]--;
		}
		else if (location[i] != AST_INACTIVE)
		{
			location[i] = AST_INACTIVE;
		}
  	}
}

void display_all_bullets(uint8_t* framebuffer, int* location, int* asteroids, int* asthp, int* sprite, int maxbul, int maxast)
{
	int i;
	for (i=0; i<maxbul; i+=2)
 	{
		if (location[i] < 127) //checking only x-values for active state (not 127)
		{
			// check which asteroid collides with bullet
			int j;
			for (j=0; j<maxast; j+=2)
			{	// asteroids[j+1] < location[i+1] && location[i+1] < asteroids[j+1]+7
				if(	asteroids[j] != AST_INACTIVE &&
					asteroids[j]+7 >= location[i] && location[i]+3 >= asteroids[j] &&
					asteroids[j+1] <= location[i+1]+1 && location[i+1]+1 <= asteroids[j+1]+7)
				{
					asthp[j/2]--;
					if(asthp[j/2] < 1)
					{
						location[i] = 128;
						asteroids[j] = AST_INACTIVE;
					}
				}
			}
			display_insert_data(framebuffer, location[i], location[i+1], sprite, 3);
			location[i]+=4;

		}
		else if (location[i] != 128)
		{
			location[i] = 128;
		}
  }
}
// Collission calculation functions:

int collission_check (uint8_t* framebuffer, int x, int y, int* sprite)
{
	int i;
	for(i=x; i < x+7; i++) //i will check framebuffer locations where ship will render. Columns.
	{
		int j;
		for (j=y; j < y+7; j++) //j will check each pixel in y-axis.
		{
			//if ( (framebuffer[i+ 127*(j/8)] >> j%8) & 0x1 == 1 && (sprite[i-x] >> j-y) & 0x1 == 1)
			if(0 < i+x && i+x < 127)
			{
				if ( (framebuffer[i+ 127*(j/8)] >> j%8) & 0x1 == 1)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
