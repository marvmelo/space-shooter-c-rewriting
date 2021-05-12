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
    InitAudioDevice();
    SetTargetFPS(30);
    Music music = LoadMusicStream("Envision.mp3");
    music.looping = TRUE;
    PlayMusicStream(music);
    float pitch = 1.0f;
    SetMusicPitch(music, pitch);
    struct GameplayScreenReturnValues returnValues;
    while (status)
    {
        switch (status)
        {
        case (1):
            status = StartScreen(WIDTH, HEIGHT, music);
            break;
        case (2):
            returnValues = GameplayScreen(WIDTH, HEIGHT, music);
            status = returnValues.status;
            break;
        case (3):
            status = EndScreen(WIDTH, HEIGHT, returnValues.score);
            break;
        }
    }
    CloseWindow();
    return 0;
}

