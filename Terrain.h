#ifndef TERRAIN_H_
#define TERRAIN_H_
///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <stdio.h>

#include "cdib.hpp"
#include "ddtools.h"
#include "externals.h"

extern void debugOut(char *name, int var, int x, int y);
extern void DrawTerrain();
extern bool InitSlices();
extern bool InitItems();
extern bool ClipObject(RECT *source,
					   int *left,
					   int *top,
					   int xPos,
					   int yPos,
					   int srcX,
					   int srcY,
					   int height,
					   int width);
extern bool DrawItems (int xStart, int width, int yStart, int height, int xDest, int yDest);
extern bool SuperDraw ();
extern void BlitTerrainBuffer(void);
extern void DrawBlock(int startIndex, int xCells, int startSlice, int yCells, int xStart, int yStart);

#endif