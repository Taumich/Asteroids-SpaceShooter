/*#include <stdio.h> //importing standard io

// rendering-function for game testing in command line interface or terminal
void consoleRender (int x, int y, int *art, int length)
{

    int size = sizeof(art)/sizeof(int);
    printf("\n");
    int width = 20;
    int height = 10;
    for (int i = 0; i < height; i++ )
    {
        for (int j = 0; j < width; j++)
        {
            if (x <= j && j < length + x)
            {
                int compare = art[j-x];
                compare = compare >> i-y;
                (y <= i && i < length +y && compare & 0x1 == 1)? printf("[]") : printf("  ");
                //(i < length && compare & 0x1 == 1)? printf("%d%d",i,j) : printf("  ");
            } else if (x > j)
                printf("  ");
        }
        printf("\n");
    }
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
    consoleRender(x,y,sprite,length);
}


int main (void)
{
    printf("managed to launch game:\n");

    int ship_up[] = {112,8,54,75,54,8,112};
    int ship_right[] = {7,8,54,105,54,8,7};

    consoleRender(0,1,ship_right,sizeof(ship_right)/sizeof(int));

    // Graphics___________________________________________________
    int ship_v1[] = {       // visualization:
        1,1,1,0,0,0,0,      // [][][]_ _ _ _
        0,0,0,1,0,0,0,      // _ _ _ []_ _ _
        0,1,1,0,1,1,0,      // _ [][]_ [][]_
        1,0,0,1,0,1,1,      // []_ _ []_ [][]
        0,1,1,0,1,1,0,      // _ [][]_ [][]_
        0,0,0,1,0,0,0,      // _ _ _ []_ _ _
        1,1,1,0,0,0,0       // [][][]_ _ _ _
    };

    // flattened these into less lines to reduce space
    int ship_v2[] = {
        1,1,1,1,1,0,0, 0,1,1,0,0,0,0, 1,1,0,1,1,1,0, 0,1,1,0,0,1,1,
        1,1,0,1,1,1,0, 0,1,1,0,0,0,0, 1,1,1,1,1,0,0 };

    int ship_v3[] = {
        0,1,1,1,1,0,0, 1,0,0,1,1,1,0, 1,0,1,0,0,1,1, 0,1,0,1,0,0,0,
        1,0,1,0,0,1,1, 1,0,0,1,1,1,0, 0,1,1,1,1,0,0 };

    // Use this draw-function to pick which ship to convert from
    // binary array into 16-byte segment array
    draw(ship_v1, 7, 1,1);
    draw(ship_v2, 7, 2,2);
    draw(ship_v3, 7, 3,3);
    draw(ship_v3, 7, 4,4);
}
*/
