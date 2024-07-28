#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "grid.h"
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define GRID_COUNT 100

typedef struct
{
    Texture2D Texture;
} Card;

typedef struct
{
    Card Dragging;
} GameState;

enum TextureSprites
{
    PLAIN,
    DIRT,
    WOODLANDS,
    HIGHLANDS,
    VILLAGE,
    FRAME,
    FRAME2
} TextureSprites;

bool IsPointerOverUI = false;
GameState State;

Texture2D textures[7];
Card cards[1];
int main(void)
{
    srand(time(NULL));
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Pointy-Top Hexagon Grid Example");

    textures[0] = LoadTexture("assets/Ground/Plain.png");
    textures[1] = LoadTexture("assets/Ground/Dirt.png");
    textures[2] = LoadTexture("assets/Ground/WoodLandsBiome.png");
    textures[3] = LoadTexture("assets/Ground/HighlandsBiome.png");
    textures[4] = LoadTexture("assets/Placable/village00.png");
    textures[5] = LoadTexture("assets/UI/frame.png");
    textures[6] = LoadTexture("assets/UI/frame2.png");

    cards[0].Texture = textures[VILLAGE];

    bool drag = false;
    Camera2D camera = {0};
    camera.target = (Vector2){-200, -200};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1;

    HexGrid *grid = CreateHexGrid(50, GRID_COUNT, GRID_COUNT);
    grid->camera = &camera;
    for (int i = 0; i < GRID_COUNT * GRID_COUNT; i++)
    {
        int r = rand() % 4;
        switch (r)
        {
        case 0:
            grid->Tiles[i].Texture = textures[PLAIN];
            break;
        case 1:
            grid->Tiles[i].Texture = textures[DIRT];
            break;
        case 2:
            grid->Tiles[i].Texture = textures[WOODLANDS];
            break;
        case 3:
            grid->Tiles[i].Texture = textures[HIGHLANDS];
            break;
        default:
            printf("error");
            break;
        }
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){65, 56, 19, 255});
        BeginMode2D(camera);

        if (!IsPointerOverUI && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            drag = true;

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            drag = false;

        if (drag)
            camera.target = (Vector2){camera.target.x + GetMouseDelta().x * -1, camera.target.y + GetMouseDelta().y * -1};

        DrawHexGrid(grid);
        EndMode2D();
        DrawPlacementUI();
        DrawFPS(5, 5);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void Draw(Rectangle rectangle, enum TextureSprites sprites)
{
    Rectangle source = (Rectangle){0, 0, textures[sprites].width, textures[sprites].height};
    Rectangle target = (Rectangle){rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    DrawTexturePro(textures[sprites], source, target, (Vector2){0, 0}, 0, WHITE);
}

void DrawPlacementUI()
{
    int width = GetScreenWidth() / 4;
    int height = 200;
    int x = GetScreenWidth() / 2 - width / 2;
    int y = GetScreenHeight() - height;
    Rectangle uiRect = (Rectangle){x, y, width, height};
    Vector2 mousePosition = GetMousePosition();
    IsPointerOverUI = CheckCollisionPointRec(mousePosition, uiRect);

    Draw((Rectangle){uiRect.x, uiRect.y, uiRect.width, uiRect.height}, FRAME);
    Rectangle source = (Rectangle){0, 0, textures[VILLAGE].width, textures[VILLAGE].height};
    Rectangle target = (Rectangle){uiRect.x, uiRect.y, 90, 125};
    if (CheckCollisionPointRec(mousePosition, target))
    {
        target.width = target.width * 1.15;
        target.height = target.height * 1.15;
        if (IsMouseButtonDown(0))
        {
            target.x = mousePosition.x - target.width / 2;
            target.y = mousePosition.y - target.height / 2;
        }
    }
    DrawTexturePro(textures[VILLAGE], source, target, (Vector2){0, 0}, 0, WHITE);
}
