#include <raylib.h>
#include "screens.h"
#define TRUE 1
#define FALSE 0
#define WIDTH 1280
#define HEIGHT 640

int
main () 
{
    int status = 1;
    InitWindow(WIDTH, HEIGHT, "Space Shooter C v1.0");
    SetTargetFPS(30);
    while (status)
    {
        struct GameplayScreenReturnValues returnValues;
        switch (status)
        {
        case (1):
            status = StartScreen(WIDTH, HEIGHT);
            break;
        case (2):
            returnValues = GameplayScreen(WIDTH, HEIGHT);
            status = returnValues.status;
            break;
        }
    }
    CloseWindow();
    return 0;
}

