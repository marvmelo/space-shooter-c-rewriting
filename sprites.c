#include <raylib.h>
#include <raymath.h>
#include <math.h>

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
    Vector2 vertex1 = {center.x, center.y - 15.0f};
    Vector2 vertex2 = {center.x - 15.0f, center.y + 15.0f};
    Vector2 vertex3 = {center.x + 15.0f, center.y + 15.0f};
    struct Spacecraft spacecraft;
    spacecraft.center = center;
    spacecraft.vertices[0] = vertex1;
    spacecraft.vertices[1] = vertex2;
    spacecraft.vertices[2] = vertex3;
    spacecraft.life = life;
    spacecraft.color = color;
    return spacecraft;
};

int 
DrawSpacecraft (struct Spacecraft *spacecraft)
{
    DrawTriangle(spacecraft->vertices[0],
                 spacecraft->vertices[1],
                 spacecraft->vertices[2],
                 spacecraft->color);
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
    float keyStates[4] = {0, 0 , 0, 0};
    if (IsKeyDown(KEY_W)) keyStates[0] = 1;
    if (IsKeyDown(KEY_S)) keyStates[1] = 1;
    if (IsKeyDown(KEY_A)) keyStates[2] = 1;
    if (IsKeyDown(KEY_D)) keyStates[3] = 1;
    Vector2 translation = {keyStates[3] - keyStates[2], keyStates[1] - keyStates[0]};
    if (Vector2Length(translation)!=0) translation = Vector2Normalize(translation);
    TranslateSpacecraft(player, translation);
    return 0;
}

int
PointSpacecraftAt (struct Spacecraft *spacecraft, Vector2 target)
{
    Vector2 spacecraftDirection = Vector2Subtract(spacecraft->vertices[0], spacecraft->center);
    Vector2 targetDirection = Vector2Subtract(target, spacecraft->center);
    float angle = atan2(targetDirection.y, targetDirection.x) - atan2(spacecraftDirection.y, spacecraftDirection.x);
    float angleCos, angleSin;
    angleCos = cos(angle);
    angleSin = sin(angle);
    for (int i = 0; i<3; i++)
    {
        float oldX, oldY;
        oldX = spacecraft->vertices[i].x;
        oldY = spacecraft->vertices[i].y;
        spacecraft->vertices[i].x = angleCos*oldX - angleSin*oldY;
        spacecraft->vertices[i].y = angleSin*oldX + angleCos*oldY;
    }
    return 0;
}

int
RotatePlayer (struct Spacecraft *player)
{
    Vector2 target = GetMousePosition();
    PointSpacecraftAt(player, target);
}

