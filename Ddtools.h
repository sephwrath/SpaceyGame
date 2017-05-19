///////////////////////////////////////
// Spells of Fury
// DDTOOLS DirectDraw Tools
// File: ddtools.h
// Author: Mike J. Norton
//
// Developer:  Date:     Comments 
// mjn         06/10/95  DirectDraw (GDK)    
///////////////////////////////////////
#define _DEBUG_
#ifndef DDTOOLS_H_
#define DDTOOLS_H_

///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <ddraw.h>
#include "cdib.hpp"

///////////////////////////////////////
// global declarations
///////////////////////////////////////
extern LPDIRECTDRAW7   lpDD;
extern LPDIRECTDRAWSURFACE7 lpDDSPrimary;
extern LPDIRECTDRAWSURFACE7 lpDDSBack;
extern LPDIRECTDRAWSURFACE7 lpDDSTerrainBuffer;
extern LPDIRECTDRAWSURFACE7 lpDDSOffscreen;
extern LPDIRECTDRAWSURFACE7 lpDDSOffscreenBase;
extern LPDIRECTDRAWSURFACE7 lpDDSOffscreenItems;
extern LPDIRECTDRAWSURFACE7 lpDDSOffscreenStatus;
extern LPDIRECTDRAWSURFACE7 lpDDSSplash;
extern LPDIRECTDRAWCLIPPER  lpDDCMain;
//extern LPDIRECTDRAWPALETTE lpDDPal, lpDDPalBase, lpDDPalSplash;



///////////////////////////////////////
// global prototypes
///////////////////////////////////////
extern bool DDInit(HWND hwnd);  // initialize a DirectDraw object
extern void DDShutdown(); // deinitialize DirectDraw object
extern bool DDSInit(HWND hwnd); // initialize DirectDraw Surfaces
extern bool InitDDSOffscreen(); // initialize lpDDSOffscreen
extern void ErasePrimarySurface();
extern void EraseBackSurface();
extern void EraseSurface(LPDIRECTDRAWSURFACE7 lpDDSurfPtr);

extern void FlipSurface(HWND hwnd); // flip the surfaces

extern bool DDError( char *errMsg ); // error message and shutdown DD
extern void DDUpdateDelay();    // video update delay
extern void WriteSurface( UINT x, UINT y, BYTE colorValue );
extern IDirectDrawSurface7*  GetNewSurface( int width, int height, bool videoMem);
extern bool BlitSurface( LPDIRECTDRAWSURFACE7 lpDDSSrc,
				  LPDIRECTDRAWSURFACE7 lpDDSDst,
				  DWORD dwX, DWORD dwY,
				  LPRECT lpSrcRect,
				  DWORD copyMode );
extern int DrawBackToOne(int x,int y, RECT rcRect, bool Full);
extern int DrawOneToBack(int x,int y, RECT rcRect, bool Full, LPDIRECTDRAWSURFACE7 lpDDSOffScrSurf);
extern bool CopyDibToSurface( LPDIRECTDRAWSURFACE7 lpDDS, CDib *pDibObj );
extern void SetRGBQuadPalette(LPDIRECTDRAWPALETTE *lpPal, RGBQUAD *lpRGBQuad );
extern bool SetTransparentColor( LPDIRECTDRAWSURFACE7 lpDDS, DWORD transColor );
extern void SetTransColorKey( LPDIRECTDRAWSURFACE7 lpDDS, UINT x, UINT y );
extern bool Splash( LPSTR szBMPFileName );


#endif
