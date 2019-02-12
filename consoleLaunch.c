#include <stdio.h> //importing standard io

void consoleRender (int x, int y, int *art, int length)
{
    int size = sizeof(art)/sizeof(int);
    printf("\n");
    int width = 20;
    int height = 10;
    int display[width][height];
    for (int i = 0; i < height; i++ )
    {
        for (int j = 0; j < width; j++)
        {
            if (j < length)
            {
                int compare = art[j];
                //printf(" <%d> ",compare);
                compare = compare >> i;
                if (i < length && compare & 0x1 == 1)
                {
                    printf("X");
                }
                else
                {
                    printf(" ");
                }
            }
            else
            {
                for (int j = 0; j < width; j++)
                {
                    //printf(" ");
                }
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

    int ret[] = {112,8,54,75,54,8,112};

    consoleRender(0,1,ret,sizeof(ret)/sizeof(int));

    //image file:
}
