#include <raylib.h>
#include <raymath.h>
#include <math.h>
#define WIDTH 1280
#define HEIGHT 640

struct Spacecraft
{
    Vector2 vertices[3];
    Vector2 center;
    Color color;
    int life;
    int bulletLock;
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
    spacecraft.bulletLock = 0;
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

int
RotatePlayer (struct Spacecraft *player)
{
    Vector2 target = GetMousePosition();
    PointSpacecraftAt(player, target);
    return 0;
}

struct Bullet
{
    Vector2 center;
    Vector2 direction;
    Color color;
    int radius;
};

struct BulletRegistryPlayer
{
    struct Bullet bulletArray[20];
    int bulletAllocation[20];
};

struct BulletRegistryPlayer
InitializeBulletRegistryPlayer ()
{
    struct BulletRegistryPlayer bulletRegistryPlayer;
    for (int i = 0; i < 20; i++)
    {
        bulletRegistryPlayer.bulletAllocation[i] = 0;
    }
    return bulletRegistryPlayer;
}

int
DrawBulletPlayer (struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    for (int i = 0; i < 20; i++)
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

int
MakePlayerShoot (struct Spacecraft *player, struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    struct Bullet bullet = MakeSpacecraftShoot(player);
    bullet.color = ORANGE;
    for (int i = 0; i < 20; i++)
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
UpdateBulletPlayer (struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    for (int i = 0; i < 20; i++)
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
        }
    }
    return 0;
}

int
UpdatePlayer (struct Spacecraft *player, struct BulletRegistryPlayer *bulletRegistryPlayer)
{
    MovePlayer(player);
    RotatePlayer(player);
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

struct EnemyRegistry
{
    struct Spacecraft enemyArray[5];
    int enemyAllocation[5];
};

struct EnemyRegistry
InitializeEnemyRegistry()
{
    struct EnemyRegistry enemyRegistry;
    for (int i = 0; i < 5; i++)
    {
        enemyRegistry.enemyAllocation[i] = 0;
    }
    return enemyRegistry;
}

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
            enemySpacecraft = InitializeSpacecraft(enemyCenter, 1, MAROON);
            enemyRegistry->enemyArray[i] = enemySpacecraft;
            break;
        }
    }
    return 0;
}

struct BulletRegistryEnemy
{
    struct Bullet bulletArray[100];
    int bulletAllocation[100];
};

struct BulletRegistryEnemy
InitializeBulletRegistryEnemy ()
{
    struct BulletRegistryEnemy bulletRegistryEnemy;
    for (int i = 0; i < 100; i++)
    {
        bulletRegistryEnemy.bulletAllocation[i] = 0;
    }
}

int
MoveEnemy (struct Spacecraft *enemy, struct Spacecraft *player)
{
    Vector2 translation = Vector2Subtract(player->center, enemy->center);
    translation = Vector2Normalize(translation);
    translation = Vector2Multiply(translation, (Vector2){6.0f, 6.0f});
    TranslateSpacecraft(enemy, translation);
    return 0;
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
    for (int i = 0; i < 100; i++)
    {
        if (!bulletRegistryEnemy->bulletAllocation[i])
        {
            bulletRegistryEnemy->bulletArray[i] = enemyBullet;
            break;
        }
    }
    return 0;
}

int
UpdateEnemy (struct EnemyRegistry *enemyRegistry, 
             struct EnemyRegistryEnemy *enemyRegistryEnemy,
             struct BulletRegistryEnemy *bulletRegistryEnemy,
             struct Spacecraft *player)
{
    int randValue = GetRandomValue(0, 89);
    if (randValue)
    {
        CreateEnemyInRegistry(enemyRegistry);
    }
    for (int i = 0; i < 5; i++)
    {
        if (enemyRegistry->enemyAllocation[i])
        {
            MoveEnemy(&enemyRegistry->enemyArray[i], player);
            RotateEnemy(&enemyRegistry->enemyArray[i], player);
        }
    }
}

