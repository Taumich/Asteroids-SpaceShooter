#include <stdio.h> //importing standard io

void consoleRender (int x, int y, int *art, int length)
{
    int size = sizeof(art)/sizeof(int);
    printf("size = %d\n", size);
    int width = 20;
    int height = 10;
    int display[width][height];
    for (int i = 0; i < height; i++ )
    {
        if (i < length)
        {
            int compare = art[i];
            for (int j = 0; j < width; j++)
            {
                //printf(" <%d> ",compare);
                if (compare & 0x1 == 1)
                {
                    printf("X");
                }
                else
                {
                    printf(" ");
                }
                compare = compare >> 1;
            }
        }
        else
        {
            for (int j = 0; j < width; j++)
            {
            printf(" ");
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
