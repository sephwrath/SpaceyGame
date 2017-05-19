#ifndef DRAWING_H_
#define DRAWING_H_
///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <stdio.h>

#include "cdib.hpp"
#include "ddtools.h"
#include "externals.h"

#include <math.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

extern inline void PlotPixel16(int x, int y, USHORT color, USHORT* buffer, int nPitch);
extern inline USHORT GetPixel16(int x, int y, USHORT* buffer, int nPitch);

extern bool PixelPerfectCollision(int x1, int y1, int x2, int y2, int width, int height);
//Draws any visible particle clouds.
extern void DrawClouds();
extern void CreateRndTable ();

//Must be called first and is used to set up variables used by the other functions
extern bool InitaliseClouds();
//sets the clouds inital position, height, width and starts the animation returns a index used to update the cloud
extern int SetCloud(int type, int xPos, int yPos, float xVel, float yVel);
extern void UpdateCloud(int index, int x, int y);
extern void StopCloud(int index);
extern void StopAllClouds();
extern void ReleaseClouds();

//These funtions are called oly from within DrawClouds()
void DrawFlame (int index);
void DrawCollision (int index, DWORD tickDiff);
void DrawSmoke (int index, DWORD tickDiff);
void DrawFire (int index, DWORD tickDiff);
void DrawShrapnel (int index, DWORD tickDiff);
void DrawHit (int index, DWORD tickDiff);
void DrawSmokeTrail (int index, DWORD tickDiff);

#endif