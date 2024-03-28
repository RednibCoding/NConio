#define NCONIO_IMPL
#include "../nconio.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Simple rogue like game example using nconio.h
 */

int main(void)
{
    nconioinit(); // Initialize the custom console I/O library
    hidecursor(); // Hide the console cursor for cleaner movement
    clrscr();     // Clear the screen initially

    int input = 0;                          // User input placeholder
    int px = 11, py = 11;                   // Player initial coordinates
    int prevx = px, prevy = py;             // Track previous coordinates
    int rows = getconh(), cols = getconw(); // Console dimensions

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            putchat('#', x, y);
        }
    }

    char prevch = getchat(px, py);
    char nextch = prevch;
    putchat('@', px, py); // Place player at initial position

    while ((input = getchr()) != 27) // ESC to exit
    {
        if (consizechanged())
        {
            clrscr();
            rows = getconh();
            cols = getconw();
            for (int y = 0; y < rows; y++)
            {
                for (int x = 0; x < cols; x++)
                {
                    putchat('#', x, y);
                }
            }

            char prevch = getchat(px, py);
            char nextch = prevch;
            putchat('@', px, py); // Place player at initial position
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
