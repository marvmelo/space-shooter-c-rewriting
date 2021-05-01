#include <raylib.h>
#define TRUE 1
#define FALSE 0
#include "screens.c"

int main(){

    const int WIDTH = 1280;
    const int HEIGHT = 640;
    int status = 1;
    InitWindow(WIDTH, HEIGHT, "Space Shooter C v1.0");
    while (status)
    {
        switch (status)
        {
        case (1):
            status = StartScreen(WIDTH, HEIGHT);
            break;
        }
    }
    CloseWindow();
    return 0;
}

