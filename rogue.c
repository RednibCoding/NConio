#define NCONIO_IMPL
#include "nconio.h"

#include "stdio.h"

/**
 * Simple rogue like game using nconio.h
 */

int main(void)
{
    nconioinit();

    int x, y;               // map coords
    int px = 5, py = 5;     // player coords
    char map[21][11] = {0}; // map data
    char action;            // player action

    do
    {
        if (action == 0x1B)
        {
            break;
        }
        clrscr();
        printf("%x\n", action);
        for (y = 0; y <= 10; y++)
        {
            for (x = 0; x <= 20; x++)
            {
                if (x == px && y == py)
                {
                    printf("@");
                }
                else if (y == 0 || y == 10)
                {
                    printf("#");
                    if (x == 20)
                    {
                        printf("\n");
                    }
                }
                else if (x == 0)
                {
                    printf("#");
                }
                else if (x == 20)
                {
                    printf("#\n");
                }
                else
                {
                    printf(" ");
                }
            }
        }
    } while (action = getch());

    nconiocleanup();
    return 0;
}