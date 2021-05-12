#include <raylib.h>
#include <stdio.h>
#include "sprites.h"
#define TRUE 1

struct GameplayScreenReturnValues
{
    int status;
    int score;
};

int
StartScreen (int screenWidth, int screenHeight, Music music)
{
    int status = 1;
    char title[] = "Space Shooter";
    char message[] = "Press ENTER to play";
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
        UpdateMusicStream(music);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(title, titleX, titleY, titleFontSize, WHITE);
        DrawText(message, messageX, messageY, messageFontSize, WHITE);
        EndDrawing();
    }
    return status;
}

struct GameplayScreenReturnValues
GameplayScreen (int screenWidth, int screenHeight, Music music)
{
    struct GameplayScreenReturnValues returnValues;
    returnValues.status = 2;
    struct Spacecraft player;
    Vector2 screenCenter = {(float)screenWidth/2.0f, (float)screenHeight/2.0f};
    InitializeSpacecraft(&player, screenCenter, 5, LIGHTGRAY);
    struct BulletRegistryPlayer bulletRegistryPlayer;
    InitializeBulletRegistryPlayer(&bulletRegistryPlayer);
    struct EnemyRegistry enemyRegistry;
    InitializeEnemyRegistry(&enemyRegistry);
    struct BulletRegistryEnemy bulletRegistryEnemy;
    InitializeBulletRegistryEnemy(&bulletRegistryEnemy);
    struct PowerUpRegistry powerUpRegistry;
    InitializePowerUpRegisty(&powerUpRegistry);
    while (TRUE)
    {
        if (WindowShouldClose())
        {
            returnValues.status = 0;
            break;
        }
        if (player.life<0)
        {
            returnValues.status = 3;
            break;
        }
        UpdateMusicStream(music);
        UpdatePlayer(&player, &bulletRegistryPlayer);
        player.score += UpdateBulletPlayer(&bulletRegistryPlayer, &enemyRegistry);
        UpdateEnemy(&enemyRegistry, &bulletRegistryEnemy, &player);
        UpdateBulletEnemy(&bulletRegistryEnemy, &player);
        UpdatePowerUp(&powerUpRegistry, &player);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawSpacecraft(&player);
        DrawBulletPlayer(&bulletRegistryPlayer);
        DrawEnemy(&enemyRegistry);
        DrawBulletEnemy(&bulletRegistryEnemy);
        DrawPowerUp(&powerUpRegistry);
        ShowLife(&player);
        ShowScore(&player);
        EndDrawing();
    }
    returnValues.score = player.score;
    return returnValues;
}

int
EndScreen (int screenWidth, int screenHeight, int score)
{
    int status = 3;
    char gameOver[] = "GAME OVER";
    int gameOverSize = 80;
    char scoreStr[13];
    sprintf(scoreStr, "SCORE: %i", score);
    int scoreStrSize = 30;
    int gameOverWidth = MeasureText(gameOver, gameOverSize);
    int scoreStrWidth = MeasureText(scoreStr, scoreStrSize);
    int gameOverX = (screenWidth - gameOverWidth)/2;
    int gameOverY = (screenHeight - 250)/2;
    int scoreStrX = (screenWidth - scoreStrWidth)/2;
    int scoreStrY = (screenHeight - 80)/2;
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
        DrawText(gameOver, gameOverX, gameOverY, gameOverSize, WHITE);
        DrawText(scoreStr, scoreStrX, scoreStrY, scoreStrSize, WHITE);
        EndDrawing();
    }
    return status;
}