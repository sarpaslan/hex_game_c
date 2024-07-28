#ifndef GRID_H
#define GRID_H

#include <raylib.h>
typedef struct Tile
{
    Texture2D Texture;
    Color Color;
} Tile;

typedef struct HexGrid
{
    int CellSize;
    int Width;
    int Height;
    Tile *Tiles;

} HexGrid;
void DrawHexGrid(HexGrid *grid);
HexGrid *CreateHexGrid(int cellSize, int width, int height);

void SetTile(HexGrid *grid, int x, int y, Texture2D texture);

#endif // GRID_H