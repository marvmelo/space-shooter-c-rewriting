#include <raylib.h>

struct Spacecraft
{
    Vector2 vertices[3];
    Vector2 center;
    Color color;
    int life;
};

struct Spacecraft 
InitializeSpacecraft (Vector2 center, int life, Color color)
{
    Vector2 vertex1 = {center.x, center.y - 15};
    Vector2 vertex2 = {center.x - 15, center.y + 15};
    Vector2 vertex3 = {center.x + 15, center.y + 15};
    struct Spacecraft spacecraft;
    spacecraft.center = center;
    spacecraft.vertices[0] = vertex1;
    spacecraft.vertices[1] = vertex2;
    spacecraft.vertices[1] = vertex3;
    spacecraft.life = life;
    spacecraft.color = color;
    return spacecraft;
};

int 
DrawSpacecraft (struct Spacecraft *spac)
{

}