#include <raylib.h>
#include "sprites.c"
#define TRUE 1

int
StartScreen (int screenWidth, int screenHeight)
{
    int status = -1;
    char title[] = "Space Shooter";
    char message[] = "Press ENTER to play";
    Font defaultFont = GetFontDefault();
    int titleFontSize = 80;
    int messageFontSize = 30;
    int titleWidth = MeasureText(title, titleFontSize);
    int messageWidth = MeasureText(message, messageFontSize);
    int titleX = (screenWidth - titleWidth)/2;
    int titleY = (screenHeight - 250)/2;
    int messageX = (screenWidth - messageWidth)/2;
    int messageY = (screenHeight - 80)/2;
    while (TRUE)
    {
        if (WindowShouldClose())
        {
            status = 0;
            break;
        }
        if (IsKeyPressed(KEY_ENTER))
        {
            status = 2;
            break;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(title, titleX, titleY, titleFontSize, WHITE);
        DrawText(message, messageX, messageY, messageFontSize, WHITE);
        EndDrawing();
    }
    return status;
}

int
GameplayScreen (int screenWidth, int screenHeight)
{
    int status = 2;
    struct Spacecraft player;
    Vector2 screenCenter = {(float)screenWidth/2.0f, (float)screenHeight/2.0f};
    player = InitializeSpacecraft(screenCenter, 5, BLUE);
    while (TRUE)
    {
        if (WindowShouldClose())
        {
            status = 0;
            break;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        MovePlayer(&player);
        RotatePlayer(&player);
        DrawSpacecraft(&player);
        EndDrawing();
    }
    return status;
}