#include <stdio.h> //importing standard io

void consoleRender (int x, int y)
{
    int width = 128;
    int height = 32;
    int display[width][height];
    for (int i = 0; i < height; i++ )
    {
        for (int j = 0; j < width; j++ )
        {
            if(y == i && x == j)
            {
                printf("X");
            }
            else
            {
                printf("-");
            }
        }
        printf("\n");
    }
}


/*void draw (int art[][])
{
    printf("sizeof(art) = %d \n", sizeof(art));
    //for (int i = 0; i < sizeof(art))
*/


int main (void)
{
    printf("managed to launch game:\n");

    /*int ret[][] =
    {
        {0,0,0,1,0,0,0},
        {0,0,1,1,1,0,0},
        {0,0,1,0,1,0,0},
        {0,1,0,1,0,1,0},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,0,1,0,0,1}
    };*/

    consoleRender(0,1);

    //drawing:
    /*
    ---x---
    --xxx--
    --x-x--
    -x-x-x-
    x-x-x-x
    x-x-x-x
    x--x--x
*/

    //image file:
}
