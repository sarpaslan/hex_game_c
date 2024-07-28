#include "TileBase.h"

void LoadTile(TileBase *tile, const char *filePath)
{
    tile->texture = LoadTexture(filePath);
}
