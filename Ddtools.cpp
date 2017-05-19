///////////////////////////////////////
// DDTOOLS DirectDraw Tools
// File: ddtools.cpp
// Author: Mike J. Norton
//		   Heavily updated and changed by Stephen Moran to include windowed capabilites
//		   as well as updating the interfaces to be compatable with directX7 (from 5 I think)
//
// Developer:  Date:     Comments 
// mjn         06/10/95  DirectDraw GDK
///////////////////////////////////////

///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include "ddtools.h"

LPDIRECTDRAW7   lpDD = NULL;
LPDIRECTDRAWSURFACE7 lpDDSPrimary;
LPDIRECTDRAWSURFACE7 lpDDSBack;
LPDIRECTDRAWSURFACE7 lpDDSOffscreen;
LPDIRECTDRAWSURFACE7 lpDDSTerrainBuffer;
LPDIRECTDRAWSURFACE7 lpDDSOffscreenBase;
LPDIRECTDRAWSURFACE7 lpDDSOffscreenItems;
LPDIRECTDRAWSURFACE7 lpDDSOffscreenStatus;
LPDIRECTDRAWSURFACE7 lpDDSSplash = 0;
LPDIRECTDRAWCLIPPER  lpDDCMain;
bool windowed = true;
#ifdef _DEBUG_
LPDIRECTDRAWCLIPPER  lpDDCWin;
#endif


///////////////////////////////////////
// Initialize a DirectDraw Object
///////////////////////////////////////
bool DDInit(HWND hwnd)
{
	HRESULT ddrval;
    ddrval = DirectDrawCreateEx( NULL, (void**)&lpDD, IID_IDirectDraw7, NULL );                           
    if( ddrval != DD_OK )                                                     
    {                                                                         
        return DDError("DirectDrawCreate Failed");                                            
    }                                                                         
                                                                              
    // Get exclusive mode  
#ifndef _DEBUG_
    ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
#else
	// using DDSCL_NORMAL means we will coexist with GDI
    ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_NORMAL );
#endif


    if( ddrval != DD_OK )                                                     
    {                                                                         
        return DDError("SetCooperativeLevel Failed");                                            
    }                                                                         
#ifndef _DEBUG_                                                                              
    // Set the video mode to 640x480x16                                        
    ddrval = lpDD->SetDisplayMode( 640, 480, 16, 0, 0);                              
    if( ddrval != DD_OK )                                                     
    {                                                                         
        return DDError("SetDisplayMode Failed, you have a crap computer");                                            
    }
#endif
                                                                              
  return true;
}

///////////////////////////////////////
// Initialize DirectDraw Surfaces
///////////////////////////////////////
bool DDSInit(HWND hwnd)
{

	DDSURFACEDESC2       ddsd;
	//DDSCAPS2             ddsCaps;
	HRESULT             ddrval;
#ifndef _DEBUG_
	/////////////////////////////////////////////////////////////////////////
	//Create the front and back buffers
	/////////////////////////////////////////////////////////////////////////
	// Create the primary surface with 1 back buffer
	memset( &ddsd, 0, sizeof(ddsd) );
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE |    // primary surface
                        DDSCAPS_COMPLEX |           // back buffer is chained
                        DDSCAPS_FLIP | DDSCAPS_VIDEOMEMORY; //DDSCAPS_SYSTEMMEMORY;              // allow page flipping
                        //DDSCAPS_VIDEOMEMORY;        // create in video memory
        
	ddsd.dwBackBufferCount = 1;
 

	ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	if( ddrval != DD_OK )
	{
		return DDError("Create lpDDSPrimary Failed");
	}

	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	ddrval = lpDDSPrimary->GetAttachedSurface(&ddsd.ddsCaps, &lpDDSBack);
	if( ddrval != DD_OK )
	{
		return DDError("Attach lpDDSBack Failed");
	}

#else
	/////////////////////////////////////////////////////////////////////////
	//Create the front and back buffers as normal surfaces and attach a clipper to the front
	/////////////////////////////////////////////////////////////////////////
	memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;
	// The primary surface is not a page flipping surface this time
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK )
    {
        DDError("Create lpDDSPrimary Failed");
		lpDD->Release();
        return(false);
    }

    // Create a clipper to ensure that our drawing stays inside our window
    ddrval = lpDD->CreateClipper( 0, &lpDDCWin, NULL );
    if( ddrval != DD_OK )
    {
        lpDDSPrimary->Release();
        lpDD->Release();
        return(false);
    }

    // setting it to our hwnd gives the clipper the coordinates from our window
    ddrval = lpDDCWin->SetHWnd( 0, hwnd );
    if( ddrval != DD_OK )
    {
        lpDDCWin-> Release();
        lpDDSPrimary->Release();
        lpDD->Release();
        return(false);
    }

    // attach the clipper to the primary surface
    ddrval = lpDDSPrimary->SetClipper( lpDDCWin );
    if( ddrval != DD_OK )
    {
        lpDDCWin-> Release();
        lpDDSPrimary->Release();
        lpDD->Release();
        return(false);
    }

	memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
    ddsd.dwWidth = 640;
    ddsd.dwHeight = 480;

    // create the backbuffer separately
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
    if( ddrval != DD_OK )
    {
		DDError("Create Separate lpDDSBack Failed");
        lpDDCWin-> Release();
        lpDDSPrimary->Release();
        lpDD->Release();
        return(false);
    }
#endif

	/////////////////////////////////////////////////////////////////////////
	//Create a clipper to be attched to the bakc buffer for sprites etc
	/////////////////////////////////////////////////////////////////////////
  	ddrval = lpDD->CreateClipper(0, &lpDDCMain, NULL);
	if( ddrval != DD_OK )
	{
		return DDError("CreateClipper Failed");
	}
	
	// first set up the pointer -- we allocate enough memory for the RGNDATAHEADER
	// along with one RECT. If we were using multiple clipping area, we would have
	// to allocate more memory.
	LPRGNDATA lpClipList = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + sizeof(RECT));
	
	// this is the RECT we want to clip to
	RECT rcClipRect = {0, 0, 480, 480};
	
	// now fill out all the structure fields
	memcpy(lpClipList->Buffer, &rcClipRect, sizeof(RECT));  // copy the actual clip region
	lpClipList->rdh.dwSize = sizeof(RGNDATAHEADER);         // size of header structure
	lpClipList->rdh.iType = RDH_RECTANGLES;                 // type of clip region
	lpClipList->rdh.nCount = 1;                             // number of clip regions
	lpClipList->rdh.nRgnSize = sizeof(RECT);                // size of lpClipList->Buffer
	lpClipList->rdh.rcBound = rcClipRect;                   // the bounding RECT

	ddrval = lpDDCMain->SetClipList(lpClipList,0);
	if( ddrval != DD_OK )
	{
		free(lpClipList);
		return DDError("SetClipList Failed");
	}
	free(lpClipList);

	return true;
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Deinitialize a DirectDraw Object
///////////////////////////////////////
void DDShutdown()
{
	if (lpDD != NULL)
	{
		if (lpDDSPrimary != NULL)
		{
			lpDDSPrimary->Release();
			lpDDSPrimary = NULL;
		}
		
		if (lpDDSBack != NULL)
		{
			lpDDSBack->Release();
			lpDDSBack = NULL;
		}
		
		if (lpDDSOffscreen != NULL)
		{
			lpDDSOffscreen->Release();
			lpDDSOffscreen = NULL;
		}
		
		if (lpDDSTerrainBuffer != NULL)
		{
			lpDDSTerrainBuffer->Release();
			lpDDSTerrainBuffer = NULL;
		}
			
		if (lpDDSOffscreenBase != NULL)
		{
			lpDDSOffscreenBase->Release();
			lpDDSOffscreenBase = NULL;
		}
			
		if (lpDDSOffscreenItems != NULL)
		{
			lpDDSOffscreenItems->Release();
			lpDDSOffscreenItems = NULL;
		}
		
		if (lpDDSOffscreenStatus != NULL)
		{
			lpDDSOffscreenStatus->Release();
			lpDDSOffscreenStatus = NULL;
		}
		
		if (lpDDSSplash != NULL)
		{
			lpDDSSplash->Release();
			lpDDSSplash = NULL;
		}

		lpDD->Release();
		lpDD = NULL;
	}
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Initialize DDSOffscreen Surface
///////////////////////////////////////
bool InitDDSOffscreen()
{
	DDSURFACEDESC2       ddsd;
	HRESULT             ddrval;

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwHeight = 480;
	ddsd.dwWidth = 640;
	ddrval = lpDD->CreateSurface(&ddsd, &lpDDSOffscreen, NULL);
	if (ddrval != DD_OK)
		return false;

	return true;
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Create a NEW unattached surface
///////////////////////////////////////
IDirectDrawSurface7*  GetNewSurface( int width, int height, bool videoMem)
{
	IDirectDrawSurface7* lpDDSurface;
	DDSURFACEDESC2       ddsd;
	HRESULT             ddrval;
	
	
	memset( &ddsd, 0, sizeof(ddsd) );
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	if (videoMem != true)
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	}
	else
	{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	}
	ddsd.dwHeight = height;
	ddsd.dwWidth = width;
	ddrval = lpDD->CreateSurface(&ddsd, &lpDDSurface, NULL);
	if (ddrval != DD_OK)
	{
	  return NULL;
	}
	
	return (IDirectDrawSurface7*)lpDDSurface; 
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Erase a DirectDraw Surface
///////////////////////////////////////
void EraseSurface(LPDIRECTDRAWSURFACE7 lpDDSurfPtr)
{
  DDBLTFX ddbltfx;
  ddbltfx.dwSize = sizeof(ddbltfx);
  ddbltfx.dwFillColor = 0;
  HRESULT ddrval;

  while (1)
  {
    ddrval = lpDDSurfPtr->Blt(NULL,NULL,NULL,DDBLT_COLORFILL,&ddbltfx);
    if (ddrval == DD_OK)
      break;
    if (ddrval ==DDERR_SURFACELOST)
    {
      ddrval = lpDD->RestoreAllSurfaces();
      if (ddrval != DD_OK)
        return;
    }

    if (ddrval != DDERR_WASSTILLDRAWING )
      return;
  }
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Erase lpDDSPrimary Surface
///////////////////////////////////////
void ErasePrimarySurface()
{
  EraseSurface(lpDDSPrimary);
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Erase lpDDSBack Surface
///////////////////////////////////////
void EraseBackSurface()
{
  EraseSurface(lpDDSBack);
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// DirectDraw Error Message Handler
///////////////////////////////////////
bool DDError( char *errMsg )
{
  char errMsgBuf[80];

  DDShutdown();  // shut down DirectDraw
  wsprintf(errMsgBuf, errMsg);
  MessageBox(GetActiveWindow(), errMsgBuf, "DirectDraw Error", MB_OK);
  return false;
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// DirectDraw Video Update Delay
///////////////////////////////////////
void DDUpdateDelay()
{
  DWORD tickCount;
  DWORD delayCount = 93 + timeGetTime();

  while(1)
  {
    tickCount = timeGetTime();
    if ( delayCount < tickCount )
      break;
  }
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// DirectDraw Write Pixel Utility
///////////////////////////////////////
void WriteSurface( UINT x, UINT y, BYTE colorValue )
{
  UINT bitsIndex;
  DDSURFACEDESC2 DDSDesc;
  LPSTR lpDstBits;
  HRESULT ddrval;

  DDSDesc.dwSize = sizeof(DDSDesc);
  
  while(1)
  {
#ifndef _DEBUG_
	ddrval = lpDDSBack->Lock(NULL, &DDSDesc, 0, NULL);
#else
	ddrval = lpDDSBack->Lock(NULL, &DDSDesc, DDLOCK_NOSYSLOCK, NULL);
#endif

	  if(ddrval == DD_OK)
		  break;
	  if(ddrval != DDERR_WASSTILLDRAWING)
		  return;

  }

  lpDstBits = (LPSTR) DDSDesc.lpSurface;
  bitsIndex = 640 * y + x;

  lpDstBits[bitsIndex] = colorValue;

  lpDDSBack->Unlock(NULL);
}
//////////////////////////////////////////////////////////////////////////////

    
///////////////////////////////////////
// DirectDraw Surface Blit Utility
///////////////////////////////////////
bool BlitSurface( LPDIRECTDRAWSURFACE7 lpDDSSrc,
				  LPDIRECTDRAWSURFACE7 lpDDSDst,
				  DWORD dwX, DWORD dwY,
				  LPRECT lpSrcRect,
				  DWORD copyMode )
{
    HRESULT ddrval;

	while(1)
	{
		ddrval = lpDDSDst->BltFast( dwX, dwY, lpDDSSrc, lpSrcRect, copyMode);
		if ( ddrval == DD_OK)
		    break;

		if (ddrval == DDERR_SURFACELOST)
			ddrval = lpDD->RestoreAllSurfaces();

		if (ddrval != DD_OK)
			return false;
		if (ddrval != DDERR_WASSTILLDRAWING)
			return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Copy a DIB object to a Surface
///////////////////////////////////////
bool CopyDibToSurface( LPDIRECTDRAWSURFACE7 lpDDS, CDib *pDibObj )
{
  HDC hdc;
  
  lpDDS->Restore();

  if (lpDDS->GetDC(&hdc) == DD_OK)
  {
	  pDibObj->DrawBits(hdc, 0,0);
      lpDDS->ReleaseDC(hdc);
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Set the DirectDraw ColorKey
///////////////////////////////////////
bool SetTransparentColor( LPDIRECTDRAWSURFACE7 lpDDS, COLORREF transColor )
{
    DDCOLORKEY ddColorKey;
    HRESULT ddrval;

    ddColorKey.dwColorSpaceLowValue = transColor;
    ddColorKey.dwColorSpaceHighValue = transColor;
    
    ddrval = lpDDS->SetColorKey(DDCKEY_SRCBLT, &ddColorKey);
    if (ddrval == DD_OK)
      return true;
    return false;
}


///////////////////////////////////////
// Set the DirectDraw ColorKey
///////////////////////////////////////
void SetTransColorKey( LPDIRECTDRAWSURFACE7 lpDDS, UINT x, UINT y )
{
	DDCOLORKEY    ddColorKey;
	COLORREF      clrRef;
	HRESULT       ddrval;
	DWORD         dwColor;
	DDSURFACEDESC2 ddsd;
	HDC           hdc, hdcMem; 
	
	// get surface DC(s)
	lpDDS->GetDC(&hdcMem);
	lpDDSPrimary->GetDC(&hdc);
	
	// get the COLORREF of current value 
	clrRef = GetPixel(hdc, 0,0); // get original color 
	// set the desired value
	SetPixel(hdc, 0, 0, GetPixel(hdcMem,x,y));
	
	// release the DC(s)
	lpDDS->ReleaseDC(hdcMem); 
	lpDDSPrimary->ReleaseDC(hdc); 
		
	// lock the Primary surface
	ddsd.dwSize = sizeof(ddsd);
#ifndef _DEBUG_
	while ((ddrval = lpDDSPrimary->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);
#else
	while ((ddrval = lpDDSPrimary->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK, NULL)) == DDERR_WASSTILLDRAWING);
#endif
	
	
	// read the value we wrote on lpDDSPrimary
	if (ddrval == DD_OK)
	{
		dwColor  = *(DWORD *)ddsd.lpSurface;                     // get DWORD
		//this causes problems in 32bit mode so I removed it
		//dwColor &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;  // mask it to bpp
	}
	
	lpDDSPrimary->Unlock(NULL);
	
	// write back the original value at that location
	lpDDSPrimary->GetDC(&hdc);
	SetPixel(hdc, 0, 0, clrRef);
	lpDDSPrimary->ReleaseDC(hdc);
	
	// set the color written on lpDDSPrimary as color key
	ddColorKey.dwColorSpaceLowValue = dwColor;
	ddColorKey.dwColorSpaceHighValue = dwColor;
	  
	lpDDS->SetColorKey(DDCKEY_SRCBLT, &ddColorKey);
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Blit lpDDSBack to lpDDSOne
///////////////////////////////////////
int DrawBackToOne(int x,int y, RECT rcRect, bool Full)
{

  HRESULT  ddrval = DD_OK;
  DWORD    dFlags;

  if(Full)
    dFlags = DDBLTFAST_NOCOLORKEY;//DDBLTFAST_WAIT | 
  else
    dFlags =  DDBLTFAST_SRCCOLORKEY; //DDBLTFAST_WAIT |
    
  return (int) BlitSurface( lpDDSBack, lpDDSOffscreen, x, y,&rcRect, dFlags);
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Blit lpDDSOne to lpDDSBack
//////////////////////////////////////////////////////////////////////////////
int DrawOneToBack(int x,int y, RECT rcRect, bool Full, LPDIRECTDRAWSURFACE7 lpDDSOffScrSurf)
{
    
  HRESULT  ddrval = DD_OK;
  DWORD    dFlags;

  if(Full)
    dFlags = DDBLTFAST_NOCOLORKEY;//DDBLTFAST_WAIT | 
  else
    dFlags = DDBLTFAST_SRCCOLORKEY; //DDBLTFAST_WAIT | 

  return (int) BlitSurface( lpDDSOffScrSurf,lpDDSBack, x, y,&rcRect, dFlags);
}
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Flip Surfaces
//////////////////////////////////////////////////////////////////////////////
void FlipSurface(HWND hwnd)
{
	HRESULT ddrval = DD_OK;
	if (windowed == false)
	{
		while( 1 )
		{
			ddrval = lpDDSPrimary->Flip(  NULL, 0 );
			if( ddrval == DD_OK )
			{
				break;
			}
			if( ddrval == DDERR_SURFACELOST )
			{
				ddrval = lpDD->RestoreAllSurfaces();
				if( ddrval != DD_OK )
				{
					break;
				}
			}
			if( ddrval != DDERR_WASSTILLDRAWING )
			{
				break;
			}
		}
	}
	else
	{
		RECT rcRectSrc;
		RECT rcRectDest;
		POINT p;

		while( 1 )
		{
			hwnd = GetForegroundWindow();
			// first we need to figure out where on the primary surface our window lives
			p.x = 0; p.y = 0;
			ClientToScreen(hwnd, &p);
			GetClientRect(hwnd, &rcRectDest);
			OffsetRect(&rcRectDest, p.x, p.y);
			SetRect(&rcRectSrc, 0, 0, 640, 480);
        
			ddrval = lpDDSPrimary->Blt( &rcRectDest, lpDDSBack, &rcRectSrc, DDBLT_WAIT, NULL);//
			if( ddrval == DD_OK )
			{
				break;
			}
			if( ddrval == DDERR_SURFACELOST )
			{
				ddrval = lpDD->RestoreAllSurfaces();
 
				if( ddrval != DD_OK )
				{
					break;
				}
			}
			if( ddrval != DDERR_WASSTILLDRAWING )
			{
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////
// Load and Display Splash Screen
///////////////////////////////////////
bool Splash( LPSTR szBMPFileName )
{
	RECT r;
	CDib* pSplashDib;
		
	// new Dib object for Splash
	pSplashDib = new CDib;

	// read in the Splash from a BMP
	pSplashDib->ReadFile( szBMPFileName );

	// bounding RECT
	r.left = 0;
	r.top = 0;
	r.right =  pSplashDib->GetCDibBiWidth();
	r.bottom = pSplashDib->GetCDibBiHeight();

	lpDDSSplash = GetNewSurface(pSplashDib->GetCDibBiWidth(), pSplashDib->GetCDibBiHeight(), false);

	CopyDibToSurface( lpDDSSplash, pSplashDib );
	CopyDibToSurface( lpDDSBack, pSplashDib );

	BlitSurface( lpDDSBack, lpDDSPrimary,0,0,&r,DDBLTFAST_NOCOLORKEY);

	HWND hwnd = GetForegroundWindow();
	FlipSurface(hwnd);

	pSplashDib->FreeCDib();

	return true;
}
//////////////////////////////////////////////////////////////////////////////