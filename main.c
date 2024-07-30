#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "grid.h"
#include <time.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define DEBUG 1

#define GRID_COUNT 10
#define CARD_COUNT 2

typedef enum TextureSprites
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

typedef struct
{
    enum TextureSprites SpriteID;
    char Name[50];
} Card;

typedef struct
{
    bool IsDragging;
    Card TargetDragCard;
    char *InfoText;
} GameState;

bool isPointerOverUI = false;
GameState State;

Texture2D textures[7];
Card cards[CARD_COUNT];

bool isCameraMoving = false;

char *CardToString_Malloc(Card card);

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
    printf("%s", "\n-----------------");
    //
    strncpy(cards[0].Name, "Village", sizeof(cards[0].Name));
    cards[0].SpriteID = VILLAGE;

    strncpy(cards[1].Name, "Logging Camp", sizeof(cards[1].Name));
    cards[1].SpriteID = LOGGING_CAMP;

    for (int i = 0; i < CARD_COUNT; i++)
        PrintCard(&cards[i]);

    printf("%s", "\n------------------\n");
}

void PrintCard(Card card)
{
    printf("\n\t{Card Name: %s SpriteID:%d}\n", card.Name, card.SpriteID);
}

bool CheckIsMouseOverHex(HexGrid *grid)
{
    Tile *tiles = grid->Tiles;
    for (int i = 0; i < GRID_COUNT * GRID_COUNT; i++)
    {
        Tile *tile = &tiles[i];
        tile->Color = WHITE;
        // TODO: Check here
    }
    return true;
}

int main(void)
{
    srand(time(NULL));
    SetTargetFPS(120);
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Hexatown");
    LoadGrid();
    LoadCards();
    State.InfoText = "Drag a card to start";
    State.IsDragging = false;

    LoadTextures();
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

        if (CheckIsMouseOverHex(grid))
        {
            // do something when is hovering
        }
        DrawHexGrid(grid);
        EndMode2D();
        DrawCardControllers();
        HandleDrag();
        DrawFPS(5, 5);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void DrawSprite(Rectangle rectangle, enum TextureSprites sprite)
{
    Rectangle source = (Rectangle){0, 0, textures[sprite].width, textures[sprite].height};
    Rectangle target = (Rectangle){rectangle.x, rectangle.y, rectangle.width, rectangle.height};
    DrawTexturePro(textures[sprite], source, target, (Vector2){0, 0}, 0, WHITE);
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
    DrawSprite((Rectangle){uiRect.x, uiRect.y, uiRect.width, uiRect.height}, UIFRAME);

    int padding = 10;
    int spacing = 50;
    for (int i = 0; i < CARD_COUNT; i++)
    {
        Card card = cards[i];
        Texture2D texture = textures[card.SpriteID];
        Rectangle source = (Rectangle){0, 0, texture.width, texture.height};
        Rectangle target = (Rectangle){uiRect.x + (90 * i) + (i * spacing) + padding, uiRect.y + padding, 90, 125};

        // DragBegin
        if (!State.IsDragging)
        {
            if (CheckCollisionPointRec(mousePosition, target))
            {
                State.InfoText = card.Name;
                if (IsMouseButtonDown(0))
                {
                    State.IsDragging = true;
                    State.TargetDragCard = card;
                }
            }
        }
        DrawSprite(target, card.SpriteID);
#if DEBUG
        DrawRectangleLines(target.x, target.y, target.width, target.height, WHITE);
#endif
    }
    DrawText(State.InfoText, uiRect.x, uiRect.y - 25, 25, WHITE);
}

void HandleDrag()
{
    if (!State.IsDragging)
    {
        return;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        State.IsDragging = false;
        return;
    }
    Card card = State.TargetDragCard;
    PrintCard(card);

    // TODO Optimize this.
    Texture2D texture = textures[card.SpriteID];
    Vector2 mousePos = GetMousePosition();
    Rectangle source = (Rectangle){0, 0, texture.width, texture.height};
    int width = 90;
    int height = 125;
    Rectangle target = (Rectangle){mousePos.x - width / 2, mousePos.y - height / 2, width, height};
    DrawSprite(target, card.SpriteID);
}

// TODO this is not a good solution because you have to free the
// memory after each usage.
char *CardToString_Malloc(Card card)
{
    static char buffer[100];
    snprintf(buffer, sizeof(buffer), "Name: %s Sprite: %d", card.Name, card.SpriteID);
    return buffer;
}