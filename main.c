#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "grid.h"
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define GRID_COUNT 10
#define CARD_COUNT 2

typedef struct
{
    Texture2D Texture;
    char *Name;
} Card;

typedef struct
{
    bool IsDraggingCard;
    Card TargetDragCard;
    char *InfoText;
} GameState;

enum TextureSprites
{
    PLAIN,
    DIRT,
    WOODLANDS,
    HIGHLANDS,
    VILLAGE,
    LOGGING_CAMP,
    UIFRAME,
    UIFRAME2
} TextureSprites;

bool isPointerOverUI = false;
GameState State;

Texture2D textures[7];
Card cards[CARD_COUNT];

bool isCameraMoving = false;

void LoadTextures()
{
    textures[PLAIN] = LoadTexture("assets/Ground/Plain.png");
    textures[DIRT] = LoadTexture("assets/Ground/Dirt.png");
    textures[WOODLANDS] = LoadTexture("assets/Ground/WoodLandsBiome.png");
    textures[HIGHLANDS] = LoadTexture("assets/Ground/HighlandsBiome.png");
    textures[VILLAGE] = LoadTexture("assets/Placable/VillagePeasant.png");
    textures[LOGGING_CAMP] = LoadTexture("assets/Placable/LoggingCamp.png");
    textures[UIFRAME] = LoadTexture("assets/UI/frame.png");
    textures[UIFRAME2] = LoadTexture("assets/UI/frame2.png");
}

HexGrid *LoadGrid()
{
    HexGrid *grid = CreateHexGrid(50, GRID_COUNT, GRID_COUNT);
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
    return grid;
}

void LoadCards()
{
    //
    cards[0].Name = "Village";
    cards[0].Texture = textures[VILLAGE];

    //
    cards[1].Name = "Logging Camp";
    cards[1].Texture = textures[LOGGING_CAMP];
}

int main(void)
{
    srand(time(NULL));
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Hexatown");
    LoadTextures();
    LoadGrid();
    LoadCards();
    State.InfoText = "Drag a card to start";

    Camera2D camera = {0};
    camera.target = (Vector2){-200, -200};
    camera.offset = (Vector2){0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1;
    HexGrid *grid = LoadGrid();
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground((Color){65, 56, 19, 255});
        BeginMode2D(camera);

        if (!isPointerOverUI && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            isCameraMoving = true;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            isCameraMoving = false;

        if (isCameraMoving)
            camera.target = (Vector2){camera.target.x + GetMouseDelta().x * -1, camera.target.y + GetMouseDelta().y * -1};

        DrawHexGrid(grid);
        EndMode2D();
        DrawCardControllers();
        if (State.IsDraggingCard)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                State.IsDraggingCard = false;
            }
            DrawDrag(State.TargetDragCard);
        }
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

void DrawCardControllers()
{
    if (isCameraMoving)
        return;
    int width = GetScreenWidth() / 4;
    int height = 200;
    int x = GetScreenWidth() / 2 - width / 2;
    int y = GetScreenHeight() - height;
    Rectangle uiRect = (Rectangle){x, y, width, height};
    Vector2 mousePosition = GetMousePosition();
    isPointerOverUI = CheckCollisionPointRec(mousePosition, uiRect);
    Draw((Rectangle){uiRect.x, uiRect.y, uiRect.width, uiRect.height}, UIFRAME);

    int padding = 10;
    int spacing = 50;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        Card card = cards[i];
        Texture2D texture = card.Texture;
        Rectangle source = (Rectangle){0, 0, texture.width, texture.height};
        Rectangle target = (Rectangle){uiRect.x + (90 * i) + (i * spacing) + padding, uiRect.y + padding, 90, 125};
        if (CheckCollisionPointRec(mousePosition, target))
        {
            State.InfoText = card.Name;
            if (IsMouseButtonDown(0))
            {
                State.TargetDragCard = card;
                State.IsDraggingCard = true;
            }
        }
        DrawTexturePro(texture, source, target, (Vector2){0, 0}, 0, WHITE);
    }
    DrawText(State.InfoText, uiRect.x, uiRect.y - 25, 25, WHITE);
}

void DrawDrag(Card card)
{
    Vector2 mousePos = GetMousePosition();
    Texture2D texture = card.Texture;
    Rectangle source = (Rectangle){0, 0, texture.width, texture.height};
    int width = 90;
    int height = 125;
    Rectangle target = (Rectangle){mousePos.x - width / 2, mousePos.y - height / 2, width, height};
    DrawTexturePro(texture, source, target, (Vector2){0.5, 0.5}, 0, WHITE);
}
