#define NCONIO_IMPL
#include "../nconio.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Simple rogue like game example using nconio.h
 */

//
// MAP
//

char **map_new(int numrows, int numcols)
{
    char **map = malloc(numrows * sizeof(char *));

    for (int i = 0; i < numrows; i++)
    {
        map[i] = malloc(numcols * sizeof(char));
        if (map[i] == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                free(map[j]);
            }
            free(map);
            return NULL;
        }
    }

    for (int y = 0; y < numrows; y++)
    {
        for (int x = 0; x < numcols; x++)
        {
            map[y][x] = '#';
        }
    }

    for (int y = 11; y < numrows / 2; y++)
    {
        for (int x = 11; x < numcols / 2; x++)
        {
            map[y][x] = ' ';
        }
    }

    return map;
}

void map_free(char **map, int numrows)
{
    for (int i = 0; i < numrows; i++)
    {
        free(map[i]);
    }
    free(map);
}

void map_print(char **map, int numrows, int numcols)
{
    for (int y = 0; y < numrows; y++)
    {
        for (int x = 0; x < numcols; x++)
        {
            putchat(map[y][x], x, y);
        }
    }
}

//
// Player
//

typedef struct
{
    int x;
    int y;
    int prevx;
    int prevy;
    char ch;
    char prevch;
    char nextch;
} player_t;

player_t *player_new(int x, int y)
{
    player_t *player = malloc(sizeof(player_t));

    player->x = x;
    player->y = y;
    player->prevx = player->x;
    player->prevy = player->y;
    player->ch = '@';

    player->prevch = getchat(player->x, player->y);
    player->nextch = player->prevch;

    return player;
}

void player_move(player_t *player, char **map, int input, int numrows, int numcols)
{
    player->prevx = player->x;
    player->prevy = player->y;

    // Handle input and update player position, ensuring it stays within bounds
    if ((input == 0x26 || input == 0x57) && player->y > 0) // arrow up || w
    {
        if (map[player->y - 1][player->x] == ' ')
        {
            player->y--;
            player->nextch = getchat(player->x, player->y);
        }
    }
    else if ((input == 0x28 || input == 0x53) && player->y < numrows - 1) // arrow down || s
    {
        if (map[player->y + 1][player->x] == ' ')
        {
            player->y++;
            player->nextch = getchat(player->x, player->y);
        }
    }
    else if ((input == 0x25 || input == 0x41) && player->x > 0) // arrow left || a
    {
        if (map[player->y][player->x - 1] == ' ')
        {
            player->x--;
            player->nextch = getchat(player->x, player->y);
        }
    }
    else if ((input == 0x27 || input == 0x44) && player->x < numcols - 1) // arrow right || d
    {
        if (map[player->y][player->x + 1] == ' ')
        {
            player->x++;
            player->nextch = getchat(player->x, player->y);
        }
    }
}

int main(void)
{
    nconioinit();
    hidecursor();
    clrscr();

    player_t *player = player_new(11, 11);

    int input = 0; // User input
    int rows = conh();
    int cols = conw();

    char **map = map_new(rows, cols);

    map_print(map, rows, cols);
    putchat(player->ch, player->x, player->y);

    while ((input = getchr()) != 27) // ESC to exit
    {
        if (consizechanged())
        {
            clrscr();

            player->prevch = getchat(player->x, player->y);
            player->nextch = player->prevch;
            map_print(map, rows, cols);
            putchat(player->ch, player->x, player->y);
        }

        player_move(player, map, input, rows, cols);

        // If position changed, update screen
        if (player->prevx != player->x || player->prevy != player->y)
        {
            putchat(player->prevch, player->prevx, player->prevy);
            player->prevch = player->nextch;
            putchat(player->ch, player->x, player->y);
        }
    }

    free(player);
    map_free(map, rows);
    clrscr();        // Clear screen on exit
    nconiocleanup(); // Cleanup
    return 0;
}
