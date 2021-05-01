#include <raylib.h>
#include <raymath.h>

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
DrawSpacecraft (struct Spacecraft *spacecraft)
{
    DrawTriangle(spacecraft->vertices[0], spacecraft->vertices[1], spacecraft->vertices[2], spacecraft->color);
    return 0;
}

int
TranslateSpacecraft (struct Spacecraft *spacecraft, Vector2 translation)
{
    for (int i = 0; i<3; i++)
    {
        spacecraft->vertices[i] = Vector2Add(spacecraft->vertices[i], translation);
    }
    spacecraft->center = Vector2Add(spacecraft->center, translation);
    return 0;
}

int
MovePlayer (struct Spacecraft *player)
{
    int keyStates[4] = {0};
    if (IsKeyDown(KEY_W)) keyStates[0] = 1;
    if (IsKeyDown(KEY_S)) keyStates[1] = 1;
    if (IsKeyDown(KEY_A)) keyStates[2] = 1;
    if (IsKeyDown(KEY_D)) keyStates[3] = 1;
    Vector2 translation = {keyStates[1] - keyStates[0], keyStates[3] - keyStates[2]};
    translation = Vector2Normalize(translation);
    TranslateSpacecraft(player, translation);
    return 0;
}