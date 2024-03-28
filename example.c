#define NCONIO_IMPL
#include "nconio.h"

#include "stdio.h"

void example_clrscr_and_textcolor()
{
    clrscr();              // Clear the screen
    textcolor(NCONIO_RED); // Set text color to red
    printf("This is red text on the screen.\n");
    getchr();         // Wait for keypress
    textcolorreset(); // Reset the text color (white text)
}

void example_textbackground()
{
    clrscr();                      // Clear the screen
    textbackground(NCONIO_YELLOW); // Set background color to yellow
    printf("This text has a yellow background.\n");
    getchr();              // Wait for keypress
    textbackgroundreset(); // Reset the background color (black background)
}

void example_gotoxy_and_putchr()
{
    clrscr();      // Clear the screen
    gotoxy(10, 5); // Move cursor to (10, 5)
    putchr('X');   // Print an 'X' at the current cursor position
    getchr();      // Wait for keypress
}

void example_wherex_wherey()
{
    clrscr(); // Clear the screen
    printf("The cursor was here");
    int x = wherex(); // Get x position of the cursor
    int y = wherey(); // Get y position of the cursor
    printf("\n");
    printf("                   ^ ");
    printf("X: %d, Y: %d\n", x, y);
    getchr(); // Wait for keypress
}

void example_getconw_getconh()
{
    clrscr(); // Clear the screen
    printf("Console dimensions - Width: %d, Height: %d\n", getconw(), getconh());
    getchr(); // Wait for keypress
}

void example_consizechanged()
{
    clrscr(); // Clear the screen
    printf("Resize the console window and then press any key.\n");
    getchr(); // Wait for keypress
    if (consizechanged())
    {
        printf("Console size has changed.\n");
    }
    else
    {
        printf("Console size has not changed.\n");
    }
    getchr(); // Wait for keypress
}

int main()
{

    nconioinit(); // Always initialize first

    // Call example functions here
    // Comment out the examples you don't want to run

    // example_clrscr_and_textcolor();
    example_gotoxy_and_putchr();
    // example_textbackground();
    // example_wherex_wherey();
    // example_getconw_getconh();
    // example_consizechanged();

    nconiocleanup(); // Always clean up nconio
    return 0;
}
