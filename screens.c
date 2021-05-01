#include <raylib.h>
#define TRUE 1

int StartScreen(int screenWidth, int screenHeight)
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
    while(TRUE)
    {
        if(WindowShouldClose()){
            status = 0;
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
