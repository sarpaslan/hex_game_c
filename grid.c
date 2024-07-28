#include <stdlib.h>
#include <math.h>
#include "grid.h"

HexGrid *CreateHexGrid(int cellSize, int width, int height)
{
    HexGrid *grid = malloc(sizeof(HexGrid));
    if (grid != NULL)
    {
        grid->CellSize = cellSize;
        grid->Width = width;
        grid->Height = height;
        grid->Tiles = malloc(width * height * sizeof(Tile));
        if (grid->Tiles == NULL)
        {
            free(grid);
            return NULL;
        }
    }
    return grid;
}

void SetTile(HexGrid *grid, int targetX, int targetY, Texture2D texture)
{
    int index = 0;
    for (int x = 0; x < targetX; x++)
    {
        for (int y = 0; y < targetY; y++)
        {
            index++;
            if (targetX == x && targetY == y)
            {
                grid->Tiles[index].Texture = texture;
            }
        }
    }
}

void DrawHex(Tile tile, Vector2 center, int size)
{
    float targetSizeX = sqrt(3.0) * size;
    float targetSizeY = (3.0 / 2.0) * size;

    Rectangle source = (Rectangle){0, 0, tile.Texture.width, tile.Texture.height};
    Rectangle target = (Rectangle){center.x, center.y, targetSizeX, size * 2};

    DrawTexturePro(tile.Texture, source,
                   target, (Vector2){target.width / 2, target.height / 2}, 0, (Color){255, 255, 255, 255});
}

void DrawHexGrid(HexGrid *grid)
{
    int index = 0;
    for (int x = 0; x < grid->Width; x++)
    {
        for (int y = 0; y < grid->Height; y++)
        {
            float posX = sqrt(3.0) * x * grid->CellSize;
            float posY = (3.0 / 2.0) * y * grid->CellSize;
            if (y % 2 == 0)
            {
                posX += sqrt(3) * grid->CellSize / 2;
            }
            Tile tile = grid->Tiles[index++];
            // if (posX > grid->camera->target.x + GetScreenWidth() + grid->CellSize ||
            //     posY > grid->camera->target.y + GetScreenHeight() + grid->CellSize)
            //     continue;

            // if (posX < grid->camera->target.x - grid->CellSize ||
            //     posY < grid->camera->target.y - grid->CellSize)
            //     continue;

            DrawHex(tile, (Vector2){posX, posY}, grid->CellSize);
        }
    }
}
