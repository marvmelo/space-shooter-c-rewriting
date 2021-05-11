#include <raylib.h>
#include <raymath.h>
#include <math.h>
#define WIDTH 1280
#define HEIGHT 640
#define MAXPLAYERBULLET 10
#define MAXENEMYBULLET 50
#define POWERUPAMOUNT 2


/* Struct definitions */

struct Spacecraft
{
    Vector2 vertices[3];
    Vector2 center;
    Color color;
    int life;
    int bulletLock;

    int hasShield;
    int score;

    int maxDistanceToPlayer;
    int direction;
};

struct Bullet
{
    Vector2 center;
    Vector2 direction;
    Color color;
    int radius;
};

struct BulletRegistryPlayer
{
    struct Bullet bulletArray[MAXPLAYERBULLET];
    int bulletAllocation[MAXPLAYERBULLET];
};

struct EnemyRegistry
{
    struct Spacecraft enemyArray[5];
    int enemyAllocation[5];
};

struct BulletRegistryEnemy
{
    struct Bullet bulletArray[MAXENEMYBULLET];
    int bulletAllocation[MAXENEMYBULLET];
};

struct PowerUp
{
    Vector2 center;
    int type;
    Color color;
};

struct PowerUpRegistry
{
    int powerUpAllocation[POWERUPAMOUNT];
    struct PowerUp powerUpArray[POWERUPAMOUNT];
};


/* Collisions Functions */

int
CheckCollisionBulletSpacecraft (struct Bullet bullet, struct Spacecraft spacecraft)
{
    return CheckCollisionCircles(bullet.center, 4, spacecraft.center, 21);
}

int
CheckCollisionPoweUpSpacecraft (struct PowerUp powerUp, struct Spacecraft spacecraft)
{
    return CheckCollisionCircles(powerUp.center, 10, spacecraft.center, 21);
}


/* Initialization functions */

int 
InitializeSpacecraft (struct Spacecraft *spacecraft, Vector2 center, int life, Color color)
{
    Vector2 vertex1 = {center.x, center.y - 15.0f};
    Vector2 vertex2 = {center.x - 15.0f, center.y + 15.0f};
    Vector2 vertex3 = {center.x + 15.0f, center.y + 15.0f};
    spacecraft->center = center;
    spacecraft->vertices[0] = vertex1;
    spacecraft->vertices[1] = vertex2;
    spacecraft->vertices[2] = vertex3;
    spacecraft->life = life;
    spacecraft->color = color;
    spacecraft->bulletLock = 0;
    spacecraft->hasShield = 0;
    spacecraft->score = 0;
    return 0;
};

int
InitializeBulletRegistryPlayer (struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    for (int i = 0; i < MAXPLAYERBULLET; i++)
    {
        bulletRegistryPlayer->bulletAllocation[i] = 0;
    }
    return 0;
}

int
InitializeEnemyRegistry(struct EnemyRegistry *enemyRegistry)
{
    for (int i = 0; i < 5; i++)
    {
        enemyRegistry->enemyAllocation[i] = 0;
    }
    return 0;
}

int
InitializeBulletRegistryEnemy (struct BulletRegistryEnemy *bulletRegistryEnemy)
{
    for (int i = 0; i < MAXENEMYBULLET; i++)
    {
        bulletRegistryEnemy->bulletAllocation[i] = 0;
    }
    return 0;
}

int
InitializePowerUpRegisty(struct PowerUpRegistry *powerUpRegistry)
{
    for (int i = 0; i < POWERUPAMOUNT; i++)
    {
        powerUpRegistry->powerUpAllocation[i] = 0;
    }
    
}


/* General functions for Spacecrafts */

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
    for (int i = 0; i < 3; i++)
    {
        if ((spacecraft->vertices[i].x<15 && translation.x<0) ||
            (spacecraft->vertices[i].x>(WIDTH-15) && translation.x>0))
        {
            translation.x = 0;
        }
        if ((spacecraft->vertices[i].y<15 && translation.y<0) ||
            (spacecraft->vertices[i].y>(HEIGHT-15) && translation.y>0))
        {
            translation.y = 0;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        spacecraft->vertices[i] = Vector2Add(spacecraft->vertices[i], translation);
    }
    spacecraft->center = Vector2Add(spacecraft->center, translation);
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
    for (int i = 0; i < 3; i++)
    {
        float relativeX, relativeY;
        relativeX = spacecraft->vertices[i].x - spacecraft->center.x;
        relativeY = spacecraft->vertices[i].y - spacecraft->center.y;
        spacecraft->vertices[i].x = angleCos*relativeX - angleSin*relativeY + spacecraft->center.x;
        spacecraft->vertices[i].y = angleSin*relativeX + angleCos*relativeY + spacecraft->center.y;
    }
    return 0;
}

struct Bullet
MakeSpacecraftShoot (struct Spacecraft *spacecraft)
{
    struct Bullet bullet;
    bullet.center = spacecraft->vertices[0];
    Vector2 direction = Vector2Subtract(spacecraft->vertices[0], spacecraft->center);
    bullet.direction = Vector2Normalize(direction);
    bullet.radius = 4;
    return bullet;
}


/* Player functions */

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
    translation = Vector2Multiply(translation, (Vector2){6, 6});
    TranslateSpacecraft(player, translation);
    return 0;
}

int
RotatePlayer (struct Spacecraft *player)
{
    Vector2 target = GetMousePosition();
    PointSpacecraftAt(player, target);
    return 0;
}

int
DrawBulletPlayer (struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    for (int i = 0; i < MAXPLAYERBULLET; i++)
    {
        if (bulletRegistryPlayer->bulletAllocation[i])
        {
            Vector2 center = bulletRegistryPlayer->bulletArray[i].center;
            float radius = bulletRegistryPlayer->bulletArray[i].radius;
            Color color = bulletRegistryPlayer->bulletArray[i].color;
            DrawCircleV(center, radius, color);
        }
    }
    return 0;
}

int
MakePlayerShoot (struct Spacecraft *player, struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    struct Bullet bullet = MakeSpacecraftShoot(player);
    bullet.color = ORANGE;
    for (int i = 0; i < MAXPLAYERBULLET; i++)
    {
        if (!bulletRegistryPlayer->bulletAllocation[i])
        {
            bulletRegistryPlayer->bulletArray[i] = bullet;
            bulletRegistryPlayer->bulletAllocation[i] = 1;
            break;
        }
    }
    return 0;
}

int
UpdateBulletPlayer (struct BulletRegistryPlayer *bulletRegistryPlayer, struct EnemyRegistry *enemyRegistry)
{
    int score = 0;
    for (int i = 0; i < MAXPLAYERBULLET; i++)
    {
        if (bulletRegistryPlayer->bulletAllocation[i])
        {
            Vector2 translation = Vector2Multiply(bulletRegistryPlayer->bulletArray[i].direction, (Vector2){10, 10});
            bulletRegistryPlayer->bulletArray[i].center = Vector2Add(bulletRegistryPlayer->bulletArray[i].center, translation);
            if (bulletRegistryPlayer->bulletArray[i].center.x<0 ||
                bulletRegistryPlayer->bulletArray[i].center.x>WIDTH ||
                bulletRegistryPlayer->bulletArray[i].center.y<0 ||
                bulletRegistryPlayer->bulletArray[i].center.y>HEIGHT)
            {
                bulletRegistryPlayer->bulletAllocation[i] = 0;
            }
            for (int j = 0; j < 5; j++)
            {
                int collision = CheckCollisionBulletSpacecraft(bulletRegistryPlayer->bulletArray[i], enemyRegistry->enemyArray[j]);
                if (collision && enemyRegistry->enemyAllocation[j])
                {
                    enemyRegistry->enemyAllocation[j] = 0;
                    bulletRegistryPlayer->bulletAllocation[i] = 0;
                    score += 50;
                }
            }
        }
    }
    return score;
}

int
UpdatePlayer (struct Spacecraft *player, struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    MovePlayer(player);
    RotatePlayer(player);
    if (player->hasShield)
    {
        player->color = DARKBLUE;
    }
    else
    {
        player->color = LIGHTGRAY;
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !player->bulletLock) 
    {
        MakePlayerShoot(player, bulletRegistryPlayer);
        player->bulletLock++;
    }
    else if (player->bulletLock<10 && player->bulletLock>0)
    {
        player->bulletLock++;
    }
    else if (player->bulletLock==10)
    {
        player->bulletLock = 0;
    }
}


/* Enemy Functions */

int
CreateEnemyInRegistry (struct EnemyRegistry *enemyRegistry)
{
    for (int i = 0; i < 5; i++)
    {
        if (!enemyRegistry->enemyAllocation[i])
        {
            float enemyX, enemyY;
            enemyX = (float)GetRandomValue(15, WIDTH-15);
            enemyY = (float)GetRandomValue(15, HEIGHT-15);
            Vector2 enemyCenter = {enemyX, enemyY};
            struct Spacecraft enemySpacecraft;
            InitializeSpacecraft(&enemySpacecraft, enemyCenter, 1, MAROON);
            enemyRegistry->enemyArray[i] = enemySpacecraft;
            enemyRegistry->enemyAllocation[i] = 1;
            enemyRegistry->enemyArray[i].maxDistanceToPlayer = GetRandomValue(150, 400);
            enemyRegistry->enemyArray[i].direction = GetRandomValue(0, 1);
            if (!enemyRegistry->enemyArray[i].direction) enemyRegistry->enemyArray[i].direction = -1;
            break;
        }
    }
    return 0;
}

int
MoveEnemy (struct Spacecraft *enemy, struct Spacecraft *player)
{
    Vector2 relativePosition = Vector2Subtract(player->center, enemy->center);
    Vector2 translation = {0.0f, 0.0f};
    if (Vector2Length(relativePosition)>enemy->maxDistanceToPlayer+50)
    {
        translation = Vector2Add(translation, relativePosition);
    }
    else if (Vector2Length(relativePosition)<enemy->maxDistanceToPlayer)
    {
        Vector2 inversePosition = Vector2Multiply(relativePosition, (Vector2){-1.0f, 1.0f});
        translation = Vector2Add(translation, inversePosition);
    }
    Vector2 rotation = {enemy->direction*relativePosition.y, -enemy->direction*relativePosition.x};
    translation = Vector2Add(translation, rotation);
    translation = Vector2Normalize(translation);
    translation = Vector2Multiply(translation, (Vector2){5.0f, 5.0f});
    TranslateSpacecraft(enemy, translation);
}

int
RotateEnemy (struct Spacecraft *enemy, struct Spacecraft *player)
{
    PointSpacecraftAt(enemy, player->center);
    return 0;
}

int
MakeEnemyShoot (struct Spacecraft *enemy, struct BulletRegistryEnemy *bulletRegistryEnemy)
{
    struct Bullet enemyBullet = MakeSpacecraftShoot(enemy);
    enemyBullet.color = DARKPURPLE;
    for (int i = 0; i < MAXENEMYBULLET; i++)
    {
        if (!bulletRegistryEnemy->bulletAllocation[i])
        {
            bulletRegistryEnemy->bulletArray[i] = enemyBullet;
            bulletRegistryEnemy->bulletAllocation[i] = 1;
            break;
        }
    }
    return 0;
}

int
UpdateEnemy (struct EnemyRegistry *enemyRegistry, 
             struct BulletRegistryEnemy *bulletRegistryEnemy,
             struct Spacecraft *player)
{
    int randValue = GetRandomValue(0, 89);
    if (!randValue)
    {
        CreateEnemyInRegistry(enemyRegistry);
    }
    for (int i = 0; i < 5; i++)
    {
        if (enemyRegistry->enemyAllocation[i])
        {
            MoveEnemy(&enemyRegistry->enemyArray[i], player);
            RotateEnemy(&enemyRegistry->enemyArray[i], player);
            if (!enemyRegistry->enemyArray[i].bulletLock)
            {
                MakeEnemyShoot(&enemyRegistry->enemyArray[i], bulletRegistryEnemy);
                enemyRegistry->enemyArray[i].bulletLock++;
            }
            else if (enemyRegistry->enemyArray[i].bulletLock<20)
            {
                enemyRegistry->enemyArray[i].bulletLock++;
            }
            else
            {
                enemyRegistry->enemyArray[i].bulletLock = 0;
            }
        }
    }
    return 0;
}

int
UpdateBulletEnemy (struct BulletRegistryEnemy *bulletRegistryEnemy, struct Spacecraft *player)
{
    for (int i = 0; i < MAXENEMYBULLET; i++)
    {
        if (bulletRegistryEnemy->bulletAllocation[i])
        {
            Vector2 translation = Vector2Multiply(bulletRegistryEnemy->bulletArray[i].direction, (Vector2){10, 10});
            bulletRegistryEnemy->bulletArray[i].center = Vector2Add(bulletRegistryEnemy->bulletArray[i].center, translation);
            if (bulletRegistryEnemy->bulletArray[i].center.x<0 ||
                bulletRegistryEnemy->bulletArray[i].center.x>WIDTH ||
                bulletRegistryEnemy->bulletArray[i].center.y<0 ||
                bulletRegistryEnemy->bulletArray[i].center.y>HEIGHT)
            {
                bulletRegistryEnemy->bulletAllocation[i] = 0;
            }
            if (CheckCollisionBulletSpacecraft(bulletRegistryEnemy->bulletArray[i], *player))
            {
                bulletRegistryEnemy->bulletAllocation[i] = 0;
                if (player->hasShield)
                {
                    player->hasShield = 0;
                }
                else
                {
                    player->life--;
                }
            }
        }
    }
    return 0;
}

int
DrawEnemy(struct EnemyRegistry *enemyRegistry)
{
    for (int i = 0; i < 5; i++)
    {
        if (enemyRegistry->enemyAllocation[i])
        {
            DrawSpacecraft(&enemyRegistry->enemyArray[i]);
        }
    }
    return 0;
}

int
DrawBulletEnemy (struct BulletRegistryEnemy *bulletRegistryEnemy)
{
    for (int i = 0; i < MAXENEMYBULLET; i++)
    {
        if (bulletRegistryEnemy->bulletAllocation[i])
        {
            Vector2 center = bulletRegistryEnemy->bulletArray[i].center;
            float radius = bulletRegistryEnemy->bulletArray[i].radius;
            Color color = bulletRegistryEnemy->bulletArray[i].color;
            DrawCircleV(center, radius, color);
        }
    }
    return 0;
}


/* Powerup functions */

int
CreatePowerUpInRegistry (struct PowerUpRegistry *powerUpRegistry)
{
    for (int i = 0; i < POWERUPAMOUNT; i++)
    {
        if (!powerUpRegistry->powerUpAllocation[i])
        {   
            float powerUpX, powerUpY;
            powerUpX = (float)GetRandomValue(15, WIDTH-15);
            powerUpY = (float)GetRandomValue(15, HEIGHT-15);
            powerUpRegistry->powerUpArray[i].center = (Vector2){powerUpX, powerUpY};
            powerUpRegistry->powerUpArray[i].type = GetRandomValue(0, 1);
            if (powerUpRegistry->powerUpArray[i].type)
            {
                powerUpRegistry->powerUpArray[i].color = RED;
            }
            else
            {
                powerUpRegistry->powerUpArray[i].color = DARKBLUE;
            }
            powerUpRegistry->powerUpAllocation[i] = 1;
            break;
        }
    }
    return 0;
}

int
DrawPowerUp (struct PowerUpRegistry *powerUpRegistry)
{
    for (int i = 0; i < POWERUPAMOUNT; i++)
    {
        if (powerUpRegistry->powerUpAllocation[i])
        {
            DrawCircleV(powerUpRegistry->powerUpArray[i].center,
                        10,
                        powerUpRegistry->powerUpArray[i].color);
        }
    }
    return 0;
}

int
UpdatePowerUp (struct PowerUpRegistry *powerUpRegistry, struct Spacecraft *player)
{
    int randValue = GetRandomValue(0, 299);
    if (!randValue)
    {
        CreatePowerUpInRegistry(powerUpRegistry);
        for (int i = 0; i < POWERUPAMOUNT; i++);
    }
    for (int i = 0; i < POWERUPAMOUNT; i++)
    {
        if(powerUpRegistry->powerUpAllocation[i])
        {
            if (CheckCollisionPoweUpSpacecraft(powerUpRegistry->powerUpArray[i], *player))
            {
                powerUpRegistry->powerUpAllocation[i] = 0;
                if (powerUpRegistry->powerUpArray[i].type && player->life<5)
                {
                    player->life++;
                }
                else if (!powerUpRegistry->powerUpArray[i].type)
                {
                    player->hasShield =1;
                }
            }
        }
    }
    return 0;
}


/* Info functions */

int
ShowLife (struct Spacecraft *player)
{
    char text[] = "LIFE:";
    int textSize = 20;
    int textX = 15;
    int textY = 15;
    int textWidth = MeasureText(text, textSize);
    DrawText(text, textX, textY, textSize, WHITE);
    int life = player->life;
    int lifeX = textX + MeasureText(text, textSize);
    for (int i = 0; i < life; i++)
    {
        lifeX += 20;
        DrawRectangle(lifeX, textY, 15, 15, RED);
    }
    return 0;
}

int
ShowScore (struct Spacecraft *player)
{
    char text[] = "SCORE:";
    int textSize = 20;
    int textX = 15;
    int textY = 35;
    int textWidth = MeasureText(text, textSize);
    int scoreX = textX + textWidth + 5;
    int score = player->score;
    DrawText(text, textX, textY, textSize, WHITE);
    DrawText(TextFormat("%i", score), scoreX, textY, textSize, WHITE);
    return 0;
}