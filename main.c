#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#define LEVEL_MAX 8

typedef enum
{
    ERROR,
    NORMAL,
    EXPLOSIVE,
    VIRUS,
    ZOMBIE,
    WISE
}
bit_type;

int main(void)
{
    Image level = LoadImage("level.png");

    // The plan is each colour maps to a type of bit
    // Red    = explosive       Dark Red    = dead e
    // Green  = normal          Dark Green  = dead n
    // Purple = virus           Dark Violet = dead v
    // Yellow = zombie          Lime        = dead z
    // White  = wise            Black       = dead w

    if (level.width == 0 || level.height == 0 || level.width > LEVEL_MAX || level.height > LEVEL_MAX)
    {
        printf("Level/Width no good.\n");
        UnloadImage(level);
        return -1;
    }

    char* data = malloc(level.height * 8 + 7);

    for (int y = 0; y < level.height; ++y)
        for (int x = 0; x < level.width; ++x)
        {
            Color color = GetImageColor(level, x, y);
            bit_type bit = ERROR;
            char is_alive = 1;

            if (color.r == 255 && !color.g && !color.b)
                bit = EXPLOSIVE;

            else if (!color.r && color.g == 255 && !color.b)
                bit = NORMAL;

            else if (color.r == 255 && !color.g && color.b == 255)
                bit = VIRUS;

            else if (color.r == 255 && color.g == 255 && !color.b)
                bit = ZOMBIE;

            else if (color.r == 255 && color.g == 255 && color.b == 255)
                bit = WISE;

            else if (color.r == 128 && !color.g && !color.b)
            {
                bit = EXPLOSIVE;
                is_alive = 0;
            }

            else if (color.r == 128 && !color.g && color.b == 128)
            {
                bit = VIRUS;
                is_alive = 0;
            }

            else if (color.r == 128 && color.g == 128 && !color.b)
            {
                bit = ZOMBIE;
                is_alive = 0;
            }

            else if (!color.r && !color.g && !color.b)
            {
                bit = WISE;
                is_alive = 0;
            }

            else if (!color.r && color.g == 128 && !color.b)
            {
                bit = NORMAL;
                is_alive = 0;
            }

            if (bit == ERROR)
            {
                printf("color no good at %d %d %d\n", color.r, color.g, color.b);

                free(data);
                UnloadImage(level);
                return -2;
            }

            printf("%02x\n", (char)(bit) << 1);
            // combining both types because together they don't go over a quad
            data[7 + (y * level.width + x)] = ((char)(bit) << 1) | is_alive;
        }

    data[0] = (char)level.height;
    data[1] = 92; // 0b01011100
    data[2] = 3;
    data[3] = 2;
    data[4] = 0;
    data[5] = 0;
    data[6] = 0;

    SaveFileData("Level.dat", data, level.height * 8 + 7);
    UnloadImage(level);
    free(data);
    return 0;
}
