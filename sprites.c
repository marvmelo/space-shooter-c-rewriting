#include <raylib.h>

struct Spacecraft
{
    int center[2];
    int life;
    Color color;
};

struct Spacecraft InitializeSpacecraft(int center[2], int life, Color color)
{
    struct Spacecraft spacecraft;
    spacecraft.center[0] = center[0];
    spacecraft.center[1] = center[1];
    spacecraft.life = life;
    spacecraft.color = color;
};