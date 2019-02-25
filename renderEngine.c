
#include <stdio.h> //importing standard io
#include "consoleLaunch.c"

#define HEIGHT 32
#define WIDTH 128


unsigned int display[128];

void asteroidRender ()
{
    display[30] = 28;
    display[31] = 20;
    display[32] = 28;
}

// rendering-function for game testing in command line interface or terminal
void shipRender (int x, int y, int *art, int length)
{
    int size = sizeof(art)/sizeof(int);
    printf("\n");
    for (int i = 0; i < HEIGHT; i++ )
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (x <= j && j < length + x)
            {
                int compare = art[j-x];
                compare = compare >> i-y;
                if (y <= i && i < length +y && compare & 0x1 == 1)
                {
                    display[j] = (art[j-x]<<y);
                }
            }
        }
    }
    consoleRender (display);
}

void fullRender (int x, int y, int *ship, int length)
{
    for (int i=0; i < 128; i++)
    {
        display[i] = 0;
    }
    asteroidRender();
    shipRender(x,y,ship,length);
}

// draw-function used to convert arrays with 1 and 0 into images with length x length dimensions.
void draw (int art[], int length, int x, int y)
{
    int sprite[length];

    for (int i = 0; i < length*length; i++) //for each element
    {
        int column = i % length;
        int row = i / length;
        // if using a new element in ret, set this new element to zero.
        (row == 0)? sprite[column] = 0 : 0;

        if (art[i] == 1) {
            // When a white pixel has been found, do the following:
            if (row == 0)
            {   // if found a pixel in the beginning, set ret to 1.
                sprite[column] = 1;
            }
            else
            {    // if found a pixel further down the row,
                // find which value the pixel has and add value to ret.
                int mult = 1;
                for (int j = 0; j < row; j++)
                    mult = mult * 2;

                sprite[column] += mult;
            }
        }
    }

    // Writing the sprite code and printing the sprite using the code
    printf("\n Drawing sprite with the code: ");
    for (int j = 0; j < length; j++)
    {
        (j < length -1)? printf("%d,",sprite[j]) : printf("%d",sprite[j]);
    }
    printf("\n Rendering:\n");
    shipRender(x,y,sprite,length);
}

// the main function of the console engine
int main (void)
{
    printf("managed to launch game:\n");

    int ship_up[] = {112,8,54,75,54,8,112};
    int ship_right[] = {73,85,85,42,20,28,8};
    int ship_v2[] = {85,127,107,85,85,28,8};
    int ship_v3[] = {54,73,85,107,99,54,20};
    int heart[] = {2,5,10,5,2};

    char c;
    printf( "Enter a value :\n");
    c = getchar( );
    printf( "Wrote : %c\n",c);

    int x = 0;
    int y = 0;
    char move[] = {'w','s','a','d'};
    int moveX[] = {0,0,-1,1};
    int moveY[] = {-1,1,0,0};

    for (int i=0; i<30; i)
    {
        c = getchar( );
        for(int j=0; j < 4; j += 1)
        {
            if(c == move[j])
            {
                x += moveX[j];
                y += moveY[j];
                printf("should draw now:");
                fullRender(x,y,ship_v3,7);
                printf("done drawing?:");
            }
        }
    }//*/

    // Graphics___________________________________________________
    /*int ship_v1[] = {       // visualization:
        1,1,1,0,0,0,0,      // [][][]_ _ _ _
        0,0,0,1,0,0,0,      // _ _ _ []_ _ _
        0,1,1,0,1,1,0,      // _ [][]_ [][]_
        1,0,0,1,0,1,1,      // []_ _ []_ [][]
        0,1,1,0,1,1,0,      // _ [][]_ [][]_
        0,0,0,1,0,0,0,      // _ _ _ []_ _ _
        1,1,1,0,0,0,0       // [][][]_ _ _ _
    };*/
    //draw(ship_v1, 7, 1,1);
}//*/
