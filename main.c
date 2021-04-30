#include <raylib.h>
#define TRUE 1
#define FALSE 0
//#include "sprites.c"

int main(){

    const int WIDTH = 1280;
    const int HEIGHT = 640;
    InitWindow(WIDTH, HEIGHT, "Space Shooter C v1.0");

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Space Shooter", 640, 320, 40, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

