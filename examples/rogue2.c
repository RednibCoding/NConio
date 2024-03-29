#define NCONIO_IMPL
#include "../nconio.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Simple rogue like game example using nconio.h
 */
#define map_rows 120
#define map_cols 30
char map[map_rows][map_cols];

int map_init()
{
    for (int y = 0; y < map_rows; y++)
    {
        for (int x = 0; x < map_cols; x++)
        {
            map[x][y] = '#';
        }
    }

    for (int y = 11; y < map_rows / 2; y++)
    {
        for (int x = 11; x < map_cols / 2; x++)
        {
            map[x][y] = ' ';
        }
    }
}

int map_print()
{
    int rows = min(map_rows, conw());
    int cols = min(map_cols, conh());
    for (int y = 0; y < map_rows; y++)
    {
        for (int x = 0; x < map_cols; x++)
        {
            putchr(map[x][y], x, y);
        }
    }
}

int main(void)
{
    nconioinit(); // Initialize the custom console I/O library
    hidecursor(); // Hide the console cursor for cleaner movement
    clrscr();     // Clear the screen initially

    int input = 0;                    // User input placeholder
    int px = 11, py = 11;             // Player initial coordinates
    int prevx = px, prevy = py;       // Track previous coordinates
    int rows = conh(), cols = conw(); // Console dimensions

    char prevch = getchat(px, py);
    char nextch = prevch;

    makeDungeon(rows, cols);
    putchat('@', px, py);

    while ((input = getchr()) != 27) // ESC to exit
    {
        if (consizechanged())
        {
            clrscr();
            rows = conh();
            cols = conw();

            char prevch = getchat(px, py);
            char nextch = prevch;
            makeDungeon(rows, cols);
            putchat('@', px, py);
        }

        prevx = px;
        prevy = py;

        // Handle input and update player position, ensuring it stays within bounds
        if ((input == 0x26 || input == 0x57) && py > 0) // arrow up || w
        {
            py--;
            nextch = getchat(px, py);
        }
        else if ((input == 0x28 || input == 0x53) && py < rows - 1) // arrow down || s
        {
            py++;
            nextch = getchat(px, py);
        }
        else if ((input == 0x25 || input == 0x41) && px > 0) // arrow left || a
        {
            px--;
            nextch = getchat(px, py);
        }
        else if ((input == 0x27 || input == 0x44) && px < cols - 1) // arrow right || d
        {
            px++;
            nextch = getchat(px, py);
        }

        // If position changed, update screen
        if (prevx != px || prevy != py)
        {
            putchat(prevch, prevx, prevy);
            prevch = nextch;
            putchat('@', px, py);
        }
    }

    clrscr();              // Clear screen on exit
    showcursor();          // Show cursor on exit
    textcolorreset();      // Reset the text color
    textbackgroundreset(); // reset the background color
    nconiocleanup();       // Cleanup resources
    return 0;
}
