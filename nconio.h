// nconio.h

#ifndef NCONIO_H
#define NCONIO_H

#ifdef __cplusplus
extern "C"
{
#endif

// Color definitions to match conio.h style
#define NCONIO_BLACK 0
#define NCONIO_BLUE 1
#define NCONIO_GREEN 2
#define NCONIO_CYAN 3
#define NCONIO_RED 4
#define NCONIO_MAGENTA 5
#define NCONIO_BROWN 6
#define NCONIO_LIGHTGRAY 7
#define NCONIO_DARKGRAY 8
#define NCONIO_LIGHTBLUE 9
#define NCONIO_LIGHTGREEN 10
#define NCONIO_LIGHTCYAN 11
#define NCONIO_LIGHTRED 12
#define NCONIO_LIGHTMAGENTA 13
#define NCONIO_YELLOW 14
#define NCONIO_WHITE 15

    // Initialize nconio
    // This must be called before anything else
    void nconioinit(void);

    // Frees and cleans up memory
    void nconiocleanup(void);

    // Check if a key has been pressed
    // Returns the virtual key code of the pressed key or 0
    int kbhit(void);

    // Print character to console
    int putch(int ch);

    // Set cursor position
    void gotoxy(int x, int y);

    // Clear the screen
    void clrscr(void);

    // Set text color (see NCONIO_ colors)
    void textcolor(int color);

    // Reset text color to default
    void textcolorreset(void);

    // Set background color (see NCONIO_ colors)
    void textbackground(int color);

    // Reset background color to default
    void textbackgroundreset();

    // Get current cursor X position
    int wherex(void);

    // Get current cursor Y position
    int wherey(void);

    // Get the width in characters of the console
    int getconw(void);

    // Get the height in characters of the console
    int getconh(void);

    // Returns whether the size of the console window has changed since
    // the last call to consizechanged
    int consizechanged(void);

#ifdef __cplusplus
}
#endif

#ifdef NCONIO_IMPL

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64)
    // ##################################################################
    //    Windows
    // ##################################################################

#include <windows.h>
    // Global variable to hold the current text and background color attributes
    static WORD __nconio_currentAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    void nconioinit(void)
    {
        __nconio_currentAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    void nconiocleanup(void)
    {
        // Do nothing
    }

    // Custom implementation of kbhit for Windows
    int kbhit(void)
    {
        HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE);
        DWORD numberOfEvents = 0;

        GetNumberOfConsoleInputEvents(stdinHandle, &numberOfEvents);

        if (numberOfEvents > 0)
        {
            INPUT_RECORD *eventBuffer = (INPUT_RECORD *)malloc(sizeof(INPUT_RECORD) * numberOfEvents);
            DWORD numEventsRead = 0;

            PeekConsoleInput(stdinHandle, eventBuffer, numberOfEvents, &numEventsRead);

            for (DWORD i = 0; i < numEventsRead; ++i)
            {
                if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown)
                {
                    int scan_code = eventBuffer[i].Event.KeyEvent.wVirtualKeyCode;
                    free(eventBuffer);
                    return scan_code;
                }
            }
            free(eventBuffer);
        }
        return 0;
    }

    // Custom implementation of getch for Windows
    int getch(void)
    {
        DWORD mode, cc;
        HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
        if (h == NULL)
        {
            return 0; // console not found
        }
        GetConsoleMode(h, &mode);
        SetConsoleMode(h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
        TCHAR c = 0;
        ReadConsole(h, &c, 1, &cc, NULL);
        SetConsoleMode(h, mode);
        return c;
    }

    // Custom implementation of clrscr for Windows
    void clrscr(void)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD coordScreen = {0, 0}; // Home for the cursor
        DWORD cCharsWritten;
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        DWORD dwConSize;

        // Get the number of character cells in the current buffer
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            return;
        }

        dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

        // Fill the entire screen with blanks
        if (!FillConsoleOutputCharacter(hConsole,        // Handle to console screen buffer
                                        (TCHAR)' ',      // Character to write to the buffer
                                        dwConSize,       // Number of cells to write
                                        coordScreen,     // Coordinates of first cell
                                        &cCharsWritten)) // Receive number of characters written
        {
            return;
        }

        // Get the current text attribute
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            return;
        }

        // Set the buffer's attributes accordingly
        if (!FillConsoleOutputAttribute(hConsole,         // Handle to console screen buffer
                                        csbi.wAttributes, // Character attributes to use
                                        dwConSize,        // Number of cells to set attribute
                                        coordScreen,      // Coordinates of first cell
                                        &cCharsWritten))  // Receive number of characters written
        {
            return;
        }

        // Put the cursor at its home coordinates
        SetConsoleCursorPosition(hConsole, coordScreen);
    }

    void gotoxy(int x, int y)
    {
        // Create a COORD structure with the desired cursor position
        COORD coord = {(SHORT)x, (SHORT)y};

        // Get the console output handle
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Set the cursor position
        SetConsoleCursorPosition(hConsole, coord);
    }

    int putch(int ch)
    {
        // Get the standard output handle
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD written;

        // Write the character to the console
        if (WriteConsole(hConsole, &ch, 1, &written, NULL))
        {
            return ch; // Return the character written if successful
        }

        return 0; // Return 0 if there was an error
    }

    int wherex(void)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            // Subtracting the starting X coordinate to get the relative position
            return csbi.dwCursorPosition.X; // Return the X coordinate
        }
        return -1; // Return -1 on error
    }

    int wherey(void)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            // Subtracting the starting Y coordinate to get the relative position
            return csbi.dwCursorPosition.Y; // Return the Y coordinate
        }
        return -1; // Return -1 on error
    }

    void textcolor(int color)
    {
        // Map conio.h colors to Windows console foreground colors
        WORD winColor = 0;
        switch (color)
        {
        case NCONIO_BLACK:
            winColor = 0;
            break;
        case NCONIO_BLUE:
            winColor = FOREGROUND_BLUE;
            break;
        case NCONIO_GREEN:
            winColor = FOREGROUND_GREEN;
            break;
        case NCONIO_CYAN:
            winColor = FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case NCONIO_RED:
            winColor = FOREGROUND_RED;
            break;
        case NCONIO_MAGENTA:
            winColor = FOREGROUND_RED | FOREGROUND_BLUE;
            break;
        case NCONIO_BROWN:
            winColor = FOREGROUND_RED | FOREGROUND_GREEN;
            break;
        case NCONIO_LIGHTGRAY:
            winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        case NCONIO_DARKGRAY:
            winColor = FOREGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTBLUE:
            winColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTGREEN:
            winColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTCYAN:
            winColor = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTRED:
            winColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTMAGENTA:
            winColor = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case NCONIO_YELLOW:
            winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case NCONIO_WHITE:
            winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        // Default to light gray if an unknown color is specified
        default:
            winColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }

        // Apply only the foreground bits, preserving the background
        __nconio_currentAttributes = (__nconio_currentAttributes & 0xFFF0) | winColor;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, __nconio_currentAttributes);
    }

    void textbackground(int color)
    {
        // Map conio.h colors to Windows console background colors
        WORD winColor = 0;
        switch (color)
        {
        case NCONIO_BLACK:
            winColor = 0;
            break;
        case NCONIO_BLUE:
            winColor = BACKGROUND_BLUE;
            break;
        case NCONIO_GREEN:
            winColor = BACKGROUND_GREEN;
            break;
        case NCONIO_CYAN:
            winColor = BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        case NCONIO_RED:
            winColor = BACKGROUND_RED;
            break;
        case NCONIO_MAGENTA:
            winColor = BACKGROUND_RED | BACKGROUND_BLUE;
            break;
        case NCONIO_BROWN: // There's no direct brown color in background attributes; using yellow instead
            winColor = BACKGROUND_RED | BACKGROUND_GREEN;
            break;
        case NCONIO_LIGHTGRAY:
            winColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
            break;
        case NCONIO_DARKGRAY:
            winColor = BACKGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTBLUE:
            winColor = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTGREEN:
            winColor = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTCYAN:
            winColor = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTRED:
            winColor = BACKGROUND_RED | BACKGROUND_INTENSITY;
            break;
        case NCONIO_LIGHTMAGENTA:
            winColor = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        case NCONIO_YELLOW:
            winColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
            break;
        case NCONIO_WHITE:
            winColor = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
            break;
        // Default to no background color (black) if an unknown color is specified
        default:
            winColor = 0;
            break;
        }

        // Apply only the background bits, preserving the foreground
        __nconio_currentAttributes = (__nconio_currentAttributes & 0xFF0F) | winColor;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, __nconio_currentAttributes);
    }

    void textcolorreset()
    {
        textcolor(-1);
    }

    void textbackgroundreset()
    {
        textbackground(-1);
    }

    int getconw(void)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int width;

        // Get the handle to the standard output device (the console)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

        // Get the console screen buffer info
        if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
        {
            // Return a default value if the function call fails
            width = 80; // Default width
        }
        else
        {
            // Calculate the width
            width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        }

        return width;
    }

    int getconh(void)
    {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int height;

        // Get the handle to the standard output device (the console)
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

        // Get the console screen buffer info
        if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
        {
            // Return a default value if the function call fails
            height = 25; // Default height
        }
        else
        {
            // Calculate the height
            height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }

        return height;
    }

    int consizechanged(void)
    {
        static int prev_height = 0, prev_width = 0; // Previous dimensions

        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_SCREEN_BUFFER_INFO csbi;

        if (GetConsoleScreenBufferInfo(hStdout, &csbi))
        {
            int currentWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            int currentHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

            if (currentWidth != prev_width || currentHeight != prev_height)
            {
                // Update last known dimensions
                prev_width = currentWidth;
                prev_height = currentHeight;
                return 1; // True: Size has changed
            }
        }
        return 0; // False: Size has not changed
    }

#elif defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
// ##################################################################
//    Linux
// ##################################################################
#include <ncurses.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

// Global variables to store current foreground and background colors
static int __nconio_current_fg = NCONIO_WHITE; // Default to white on black
static int __nconio_current_bg = NCONIO_BLACK;

void nconioinit()
{
    initscr();             // Start curses mode
    cbreak();              // Line buffering disabled
    noecho();              // Don't echo while we do getch
    nodelay(stdscr, TRUE); // Make getch non-blocking
    keypad(stdscr, TRUE);  // Enable function and arrow keys

    // init colors
    if (has_colors() == FALSE)
    {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    start_color();
    // Initialize color pairs
    for (int fg = 0; fg < 8; fg++)
    {
        for (int bg = 0; bg < 8; bg++)
        {
            // Calculate a unique pair number based on fg and bg
            // Assumes COLOR_BLACK to COLOR_WHITE are 0 to 7
            int pair_number = fg * 8 + bg + 1;
            init_pair(pair_number, fg, bg);
        }
    }
}

void nconiocleanup(void)
{
    endwin(); // Clean up ncurses environment before exiting
}

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR)
    {
        // For ASCII characters, ch contains the keycode
        return ch; // Return the ASCII keycode
    }
    else
    {
        return 0; // No key pressed
    }
}

int getch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void putch(int ch)
{
    addch(ch);
    refresh(); // Refresh the screen to show the output
}

void gotoxy(int x, int y)
{
    move(y, x); // Note: ncurses uses y, x instead of x, y
    refresh();  // Refresh the screen to apply the cursor move
}

void clrscr(void)
{
    clear();   // Clears the screen in ncurses
    refresh(); // Refreshes the screen to apply changes
}

void textcolor(int color)
{
    __nconio_current_fg = color; // Update the global foreground color
    // Calculate the pair number based on current foreground and background
    int pair_number = __nconio_current_fg * 8 + __nconio_current_bg + 1;
    attron(COLOR_PAIR(pair_number));
}

void textcolorreset()
{
    textcolor(NCONIO_WHITE);
}

void textbackground(int color)
{
    __nconio_current_bg = color;
    // Calculate the pair number based on current foreground and background
    int pair_number = __nconio_current_fg * 8 + __nconio_current_bg + 1;
    attron(COLOR_PAIR(pair_number));
}

void textbackgroundreset()
{
    textbackground(NCONIO_BLACK);
}

// Get current cursor X position
int wherex(void)
{
    int x, y;
    // getyx is a macro, so we don't use & for variables
    getyx(stdscr, y, x);
    return x;
}

// Get current cursor Y position
int wherey(void)
{
    int x, y;
    // getyx is a macro, so we don't use & for variables
    getyx(stdscr, y, x);
    return y;
}

// Get the width in characters of the console
int getconw(void)
{
    int height, width;
    getmaxyx(stdscr, height, width); // Use stdscr for the standard screen
    return width;
}

// Get the height in characters of the console
int getconh(void)
{
    int height, width;
    getmaxyx(stdscr, height, width);
    return height;
}

// Returns whether the size of the console window has changed since
// the last call to consizechanged
int consizechanged(void)
{
    static int prev_height = 0, prev_width = 0; // Previous dimensions
    int height, width;

    // Get current dimensions
    getmaxyx(stdscr, height, width);

    // Check if the dimensions have changed
    if (width != prev_width || height != prev_height)
    {
        // Update stored dimensions to current for the next call
        prev_width = width;
        prev_height = height;
        return 1; // True, size has changed
    }

    // No change in size
    return 0;
}

#elif defined(__APPLE__) && defined(__MACH__)
// ##################################################################
//    MAC
// ##################################################################
// We dont need mac specific stuff yet

#endif // _WIN32 / __linux__ / __APPLE__ / __MACH__

#ifdef __cplusplus
}
#endif

#endif // NCONIO_IMPL

#endif // NCONIO_H
