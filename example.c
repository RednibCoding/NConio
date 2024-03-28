#define NCONIO_IMPL
#include "nconio.h"

#include "stdio.h"

void example_clrscr_and_textcolor()
{
    nconioinit();          // Always initialize first
    clrscr();              // Clear the screen
    textcolor(NCONIO_RED); // Set text color to red
    printf("This is red text on the screen.\n");
    getch();
    textcolorreset();
}

void example_textbackground()
{
    nconioinit();
    clrscr();
    textbackground(NCONIO_YELLOW); // Set background color to yellow
    printf("This text has a yellow background.\n");
    getch();
    textbackgroundreset();
}

void example_gotoxy_and_putch()
{
    nconioinit();
    clrscr();
    gotoxy(10, 5); // Move cursor to (10, 5)
    putch('X');    // Print an 'X' at the current cursor position
    getch();
}

void example_wherex_wherey()
{
    nconioinit();
    clrscr();
    printf("The cursor was here");
    int x = wherex();
    int y = wherey();
    printf("\n");
    printf("                   ^ ");
    printf("X: %d, Y: %d\n", x, y);
    getch();
}

void example_getconw_getconh()
{
    nconioinit();
    clrscr();
    printf("Console dimensions - Width: %d, Height: %d\n", getconw(), getconh());
    getch();
}

void example_consizechanged()
{
    nconioinit();
    clrscr();
    printf("Resize the console window and then press any key.\n");
    getch();
    if (consizechanged())
    {
        printf("Console size has changed.\n");
    }
    else
    {
        printf("Console size has not changed.\n");
    }
    getch();
}

int main()
{
    // Call example functions here
    // Comment out the examples you don't want to run

    // Init nconio
    nconioinit();

    // example_clrscr_and_textcolor();
    // example_gotoxy_and_putch();
    // example_textbackground();
    // example_wherex_wherey();
    // example_getconw_getconh();
    // example_consizechanged();

    nconiocleanup(); // Clean up nconio
    return 0;
}
