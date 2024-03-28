#define NCONIO_IMPL
#include "../nconio.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Naive rogue like game example using nconio.h
 */

int main(void)
{
    nconioinit();
    hidecursor();

    int x, y;               // map coords
    int px = 5, py = 5;     // player coords
    int tx = 11, ty = 7;    // goblin coords
    char map[21][11] = {0}; // map data
    int gold_total = 0;
    int gold_drop = 2;
    char action; // player action
    char wallch = '#';
    char actionmsg[100];

    do
    {
        // ESC
        if (action == 0x1B)
            break;

        if (action == 0x25 || action == 0x41) // arrow left || a
        {
            if (map[px - 1][py] != wallch)
                px--;
        }
        else if (action == 0x27 || action == 0x44) // arrow right || d
        {
            if (map[px + 1][py] != wallch)
                px++;
        }
        else if (action == 0x26 || action == 0x57) // arrow up || w
        {
            if (map[px][py - 1] != wallch)
                py--;
        }
        else if (action == 0x28 || action == 0x53) // arrow down || s
        {
            if (map[px][py + 1] != wallch)
                py++;
        }

        clrscr();

        // Goblin fight
        if (px == tx && py == ty)
        {
            tx = rand() % 19 + 1;
            ty = rand() % 9 + 1;
            gold_drop = rand() % 10;
            gold_total += gold_drop;
            sprintf(actionmsg, "You've defeated a goblin and got %d gold", gold_drop);
        }
        else
        {
            sprintf(actionmsg, "");
        }

        // Draw the map
        for (y = 0; y <= 10; y++)
        {
            for (x = 0; x <= 20; x++)
            {
                if (x == px && y == py)
                {
                    textcolor(NCONIO_BROWN);
                    printf("@");
                    textcolorreset();
                }
                else if (x == tx && y == ty)
                {
                    textcolor(NCONIO_GREEN);
                    printf("t");
                    textcolorreset();
                }
                else if (y == 0 || y == 10)
                {
                    textcolor(NCONIO_DARKGRAY);
                    printf("%c", wallch);
                    map[x][y] = wallch;
                    if (x == 20)
                    {
                        printf("\n");
                    }
                }
                else if (x == 0)
                {
                    textcolor(NCONIO_DARKGRAY);
                    printf("%c", wallch);
                    textcolorreset();
                    map[x][y] = wallch;
                }
                else if (x == 20)
                {
                    textcolor(NCONIO_DARKGRAY);
                    printf("%c\n", wallch);
                    textcolorreset();
                    map[x][y] = wallch;
                }
                else
                {
                    textcolor(NCONIO_DARKGRAY);
                    printf(".");
                    textcolorreset();
                    map[x][y] = '.';
                }
            }
        }
        textcolor(NCONIO_LIGHTGRAY);
        printf("%s\n\n", actionmsg);
        textcolor(NCONIO_YELLOW);
        printf("Gold: %d\n", gold_total);
        textcolorreset();
        printf("\n");
    } while ((action = getchr()));

    clrscr();
    textcolorreset();
    textbackgroundreset();
    nconiocleanup();
    return 0;
}