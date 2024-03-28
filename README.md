# NConio: Cross-Platform Console I/O Library

NConio is a single-header, cross-platform library designed to facilitate console
I/O operations similar to the classic `conio.h` in DOS. It aims to provide an
easy and intuitive API for tasks such as keyboard input handling, screen
coloring, and cursor positioning across Windows, macOS, and Linux.

## Features

- Color manipulation (foreground and background)
- Keyboard input without blocking (similar to `kbhit` in DOS)
- Cursor positioning and screen clearing
- Detecting console size changes
- Cross-platform support: Windows, macOS, Linux

## Getting Started

To use NConio in your project, simply include `nconio.h` in your source file. If
you define `NCONIO_IMPL` before including `nconio.h`, the implementation will be
included as well.

```c
#define NCONIO_IMPL
#include "nconio.h"
```

## Installation

No installation is required for Windows. For Linux and macOS, make sure
`ncurses` is installed and linked against your project.

### Linux

Install `ncurses` using your distribution's package manager. For example, on
Debian-based systems:

```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

When compiling your project, link against `ncurses`:

```bash
gcc -o myproject myproject.c -lncurses
```

### macOS

macOS should have `ncurses` preinstalled. If you do encounter issues, you might
need to install it via Homebrew or ensure your environment is set up correctly
to find the ncurses library. Link against `ncurses` as you would on Linux.

## API Overview

- `void nconioinit(void)`: Initialize NConio (must be called first).
- `void nconiocleanup(void)`: Clean up resources used by NConio.
- `int kbhit(void)`: Check if a key has been pressed. Returns the key code or 0.
- `int putchr(int ch)`: Print a character to the console.
- `void putchat(char ch, int x, int y)`: Print character to console at position
  x, y
- `int getchr(void)`: Returns the character that was input.
- `char getchat(int x, int y);`: Get the character at position x, y
- `void gotoxy(int x, int y)`: Set the cursor position.
- `void clrscr(void)`: Clear the screen.
- `void textcolor(int color)`: Set the text color.
- `void textcolorreset(void)`: Reset text color to default.
- `void textbackground(int color)`: Set the background color.
- `void textbackgroundreset()`: Reset background color to default.
- `int wherex(void)`: Get current cursor X position.
- `int wherey(void)`: Get current cursor Y position.
- `int getconw(void)`: Get the console width in characters.
- `int getconh(void)`: Get the console height in characters.
- `int consizechanged(void)`: Check if console size has changed since the last.
- `void hidecursor(void)`: Make the cursor invisible.
- `void showcursor(void)`: Make the cursor visible. call.

## Color Definitions

NConio provides a set of predefined color constants from `NCONIO_BLACK` to
`NCONIO_WHITE` to match the classic `conio.h` style.

```c
NCONIO_BLACK 0
NCONIO_BLUE 1
NCONIO_GREEN 2
NCONIO_CYAN 3
NCONIO_RED 4
NCONIO_MAGENTA 5
NCONIO_BROWN 6
NCONIO_LIGHTGRAY 7
NCONIO_DARKGRAY 8
NCONIO_LIGHTBLUE 9
NCONIO_LIGHTGREEN 10
NCONIO_LIGHTCYAN 11
NCONIO_LIGHTRED 12
NCONIO_LIGHTMAGENTA 13
NCONIO_YELLOW 14
NCONIO_WHITE 15
```

## License

NConio is open-source software distributed under the MIT license.
