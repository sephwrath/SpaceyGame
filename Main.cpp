///////////////////////////////////////
//
// Developer: Stephen Moran
// Date: 4-9-1999
// Comments: This is the main file for the 
//			 spacey game and it contains all the 
//			 initalisation functions and global variables used by
//			 windows. Other globals are in the Externals File
//			 the frames are stored in the frames file.
//      
///////////////////////////////////////

///////////////////////////////////////
// Include Files
///////////////////////////////////////

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

//Std windows and C libraries that containg all round usefull functions
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>

//The file that contains all the external definitions
// eg global vars and arrays
#include "Externals.h"
//The file that makes loading DIBs easy
#include "cdib.hpp"
//More DDraw helper functions
#include "ddtools.h"
//Dsound helpers
#include "sound\dstools.h"
//Interface jazz - powerbars etc
#include "Interface\GameDisplay.h"
//Drawing the background
#include "terrain.h"
#include "Enemies\SpriteManage.h"
#include "GameKeys.h"
#include "ShipDraw.h"
//Misc drawing functions
#include "Drawing.h"
#include "Interface\GUISpacey.h"
#include "Weapons\WeaponManage.h"

//////////////////////////////////////////////////////////////////
// A DEBUG TOOL USED FOR FPS COUNTER AMONG OTHER THINGS NOT INCLUDED IN RELEASE 
// AS IT SLOWS THE GAME DOWN CONSIDERABLY
#ifdef _DEBUG_
#include <stdio.h>
#include <string.h>
extern HDC hDCbb;
extern char dbBuffer[40];

void debugOut(char *name, int var, int x, int y)
{
	if (lpDDSPrimary->GetDC(&hDCbb) == DD_OK)
	{
		_itoa(var, dbBuffer, 10);
		strcat(dbBuffer, name);
		TextOut(hDCbb, x, y, dbBuffer, strlen(dbBuffer));
		lpDDSPrimary->ReleaseDC(hDCbb);
	}
}
#endif

///////////////////////////////////////
// Globals
///////////////////////////////////////
//The task state type that determines what the game will be doing - the variable that determines in which state the program is in
enum { TS_NONE, TS_SPLASH, TS_MENU, TS_CRITICAL, TS_TERMINATE, TS_SUSPEND, TS_START, TS_DEATH, TS_LEVEL_OVER, TS_EXIT }; //Task States

int currentMenuChoice;

HWND        hwnd;

int   taskState = TS_NONE;   // initialize task state to no current task
bool  bIsActive = false;     // initialize application active flag to FALSE
DWORD scrollTickCount, scrollDelay;

GUISpacey mainMenu;


char currentChar;


///////////////////////////////////////
// Function Defs
///////////////////////////////////////
//Necesary Functions
LRESULT APIENTRY WndFunc( HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam );
bool	InitGameApp( HINSTANCE hThisInstance, int nCmdShow );
void	Scheduler(void);
void	UpdateGame(void);
void	StartAnimation(void);
void	EndAnimation(void);
void	LevelOverProc(void);
void	InitaliseVariables(void);
//void	DrawSpaceShip(void);

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//	THE FUNCTIONS


int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance,
                    LPSTR lpszCmdParam, int nCmdShow)
//-------------------------------------------------------------------
//The beginning
//-------------------------------------------------------------------
{
	MSG msg;
	lpszCmdParam = lpszCmdParam;
	hPrevInstance = hPrevInstance;

	//calls the initalise game app function to set up any variables or structures
	//for use within the game
	if (!InitGameApp(hThisInstance, nCmdShow))
		return false;

	//The windows message loop that checks for any messages (keypress) and sends them to the
	//windProc function for processing
	while(1)
	{
		//See if there are any messages
		if(PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))
		{
			//if there are translate and dispatch them (windproc)
			if(!GetMessage (&msg, NULL, 0, 0))
				return msg.wParam;
			TranslateMessage (&msg);
			DispatchMessage (&msg);        
		}
		else
		{
			// do critical game stuff
			Scheduler();
		}
	} // end of message loop
	return msg.wParam;
}
//***********************************************************************



bool InitGameApp( HINSTANCE hThisInstance, int nCmdShow )
//-------------------------------------------------------------------
//Initalise different structures used in the game
//Sets up all of the offscreen surfaces and the window type
//Loads the bit maps and initalise the different structures
//Returns true if the initalisation is successful otherwise it returns false
//-------------------------------------------------------------------
{
    WNDCLASS    wndclass;
    char szAppName[] = "Spacey II";
	//set up the window class for a game app
#ifndef _DEBUG_ 
	wndclass.style         = CS_DBLCLKS;
    wndclass.lpfnWndProc   = WndFunc;		 // window function
    wndclass.cbClsExtra    = 0;			 // no extra count of bytes
    wndclass.cbWndExtra    = 0;			 // no extra count of bytes
    wndclass.hInstance     = hThisInstance; // this instance
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor (hThisInstance, "CURSOR_ARROW");//(NULL, IDC_ARROW);
    
	wndclass.hbrBackground = reinterpret_cast<HBRUSH__ *>(GetStockObject (BLACK_BRUSH));
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;

	//register the class set up above (not sure how or why but you do OK)
    RegisterClass (&wndclass);

	//set the current window to how you want it (hwnd is the window and create windowex is called
	//with the below parameters so it is usefull for a game app

    hwnd = CreateWindowEx (
					WS_EX_TOPMOST,         // window style
                    szAppName,             // window class name
                    "Spacey II",		   // window caption
                    WS_VISIBLE | WS_POPUP, // window style
                    CW_USEDEFAULT,         // initial x position
                    CW_USEDEFAULT,         // initial y position
                    640,                   // initial x size
                    480,                   // initial y size
                    NULL,                  // parent window handle
                    NULL,                  // window menu handle
                    hThisInstance,         // program instance handle
                    NULL);                 // creation parameters
#else
	wndclass.style         = CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc   = WndFunc;		 // window function
    wndclass.cbClsExtra    = 0;			 // no extra count of bytes
    wndclass.cbWndExtra    = 0;			 // no extra count of bytes
    wndclass.hInstance     = hThisInstance; // this instance
    wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
    wndclass.hCursor       = LoadCursor (hThisInstance, "CURSOR_ARROW");
    wndclass.hbrBackground = (HBRUSH__ *)(GetStockObject (BLACK_BRUSH));
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = szAppName;

	RegisterClass (&wndclass);

    hwnd = CreateWindow (
                    //WS_EX_TOPMOST,         // window style
                    szAppName,             // window class name
                    "Spacey II",		   // window caption
                    WS_OVERLAPPEDWINDOW, // window style
                    CW_USEDEFAULT,         // initial x position
                    CW_USEDEFAULT,         // initial y position
                    640,                   // initial x size
                    480,                   // initial y size
                    NULL,                  // parent window handle
                    NULL,                  // window menu handle
                    hThisInstance,         // program instance handle
                    NULL);                 // creation parameters

#endif

	//if the window wasn't created properly return an error
    if (!hwnd)
		return false;

	//show the created window, update it then set the input focus to it (key presses ->hwnd)
    ShowWindow (hwnd, nCmdShow);
    UpdateWindow (hwnd);
    SetFocus(hwnd);

    // initialize the flip surfaces (function in the ddtools file)
    if(!DDInit(hwnd))
    {
		MessageBox(hwnd,"ERROR: DDInit Failed","Spacey", MB_OK);
        return false;
    }                                                                        
                                                                                     
	if (!DDSInit(hwnd))
    {
		MessageBox(hwnd,"ERROR: DDSInit Failed","Spacey", MB_OK);
		return false;
    }

	//Create a new cdib object that will be used to store the game sprites
    pDib = new CDib;
    if (!pDib)
    {
		MessageBox(hwnd,"ERROR: CDib Failed","Spacey", MB_OK);
		return false;
    }
	
	///////////////////////////////////////////////////////////
	//Sprite frames
	//read in the file containing the ship cells
    if (!pDib->ReadFile("Full Frame.bmp"))
    {
		MessageBox(hwnd,"ERROR: FullFrame.bmp ::ReadFile Failed","Spacey", MB_OK);
		return false;
    }
	// get a new off screen surface for our sprites
	lpDDSOffscreen = GetNewSurface(pDib->GetCDibBiWidth(), pDib->GetCDibBiHeight(), true);
    if (!lpDDSOffscreen)
    {
		MessageBox(hwnd,"ERROR: GetNewSurface Failed(off screen)","Spacey", MB_OK);
		return false;
    }
	//copy pDib to the offscreen surface
    if (!CopyDibToSurface( lpDDSOffscreen, pDib ))
    {
		MessageBox(hwnd,"ERROR: CopyDibToSurface Failed","Spacey", MB_OK);
		return false;
    }

	///////////////////////////////////////////////////////////
	//Terrain tiles
	//read in the file containing the base tiles
    if (!pDib->ReadFile("Tileidea.bmp"))
    {
		MessageBox(hwnd,"ERROR: 32 by 32 TileIdea.bmp ::ReadFile Failed","Spacey", MB_OK);
		return false;
    }
	// get a new offscreen surface for our base tiles
	lpDDSOffscreenBase = GetNewSurface(pDib->GetCDibBiWidth(), pDib->GetCDibBiHeight(), false);
    if (!lpDDSOffscreenBase)
    {
		MessageBox(hwnd,"ERROR: GetNewSurface Failed(base)","Spacey", MB_OK);
		return false;
    }
	if (!CopyDibToSurface( lpDDSOffscreenBase, pDib ))
    {
		MessageBox(hwnd,"ERROR: CopyDibToSurface Failed","Spacey", MB_OK);
		return false;
    }

	///////////////////////////////////////////////////////////
	//Scenery items
	//read in the file containing the items
    if (!pDib->ReadFile("Scenery1.bmp"))
    {
		MessageBox(hwnd,"ERROR: Scenery Or Items.bmp ::ReadFile Failed","Spacey", MB_OK);
		return false;
    }
	// get a new offscreen surface for our game items
	lpDDSOffscreenItems = GetNewSurface(pDib->GetCDibBiWidth(), pDib->GetCDibBiHeight(), false);
    if (!lpDDSOffscreenItems)
    {
		MessageBox(hwnd,"ERROR: GetNewSurface Failed(items)","Spacey", MB_OK);
		return false;
    }
	if (!CopyDibToSurface( lpDDSOffscreenItems, pDib ))
    {
		MessageBox(hwnd,"ERROR: CopyDibToSurface Failed","Spacey", MB_OK);
		return false;
    }

	///////////////////////////////////////////////////////////
	//GUI graphics
	//read in the file containing the inteface graphics
    if (!pDib->ReadFile("Interface.bmp"))
    {
		MessageBox(hwnd,"ERROR: Interface.bmp ::ReadFile Failed","Spacey", MB_OK);
		return false;
    }
	// get a new off screen surface for our status graphics
	lpDDSOffscreenStatus = GetNewSurface(pDib->GetCDibBiWidth(), pDib->GetCDibBiHeight(), true);
    if (!lpDDSOffscreenStatus)
    {
		MessageBox(hwnd,"ERROR: GetNewSurface Failed(status)","Spacey", MB_OK);
		return false;
    }
	if (!CopyDibToSurface( lpDDSOffscreenStatus, pDib ))
    {
		MessageBox(hwnd,"ERROR: CopyDibToSurface Failed","Spacey", MB_OK);
		return false;
    }

	//clear the surfaces to black
	ErasePrimarySurface ();
	EraseBackSurface ();

	// get the terrain buffer suface - stores the terrain so it dosn't have to be
	// totally reconstructed each time.
	lpDDSTerrainBuffer = GetNewSurface(512, 512, false);
    if (!lpDDSTerrainBuffer)
    {
		MessageBox(hwnd,"ERROR: GetNewSurface Failed (terrain buffer)","Spacey", MB_OK);
		return false;
    }

    //Set the transpaent color to the color at (0,0) - an ugly pink
	SetTransColorKey (lpDDSOffscreen,0,0);
	//SetTransparentColor(lpDDSTerrainBuffer, RGB(255, 0, 128));
	SetTransColorKey (lpDDSOffscreenStatus, 60, 130);
	SetTransColorKey (lpDDSOffscreenBase,30,0);
	SetTransColorKey (lpDDSOffscreenItems,0 ,0);

	/////////////////////////////////////////////////////
	//Direct sound initalisation for the different sounds
	if (!DSInit(hwnd))
    {
		MessageBox(hwnd,"ERROR: Direct Sound initalisation Failed","Spacey", MB_OK);
		return false;
    }
	
	lpwiLaser = FCreateWAVInfo(lpDS, "sound/laser4.wav", 1);
	lpwiHMissile = FCreateWAVInfo(lpDS, "sound/missile.wav", 2);
	lpwiMissile = FCreateWAVInfo(lpDS, "sound/hmissile.wav", 2);
	lpwiBullet = FCreateWAVInfo(lpDS, "sound/spread1.wav", 2);
	lpwiExplosion = FCreateWAVInfo(lpDS, "sound/explosion.wav", 5);
	lpwiBeep  = FCreateWAVInfo(lpDS, "sound/weaponchange.wav", 2);
	lpwiBulletHit = FCreateWAVInfo(lpDS, "sound/hit.wav", 2);
	lpwiEnemyFire = FCreateWAVInfo(lpDS, "sound/enemyFire.wav", 4);
	lpwiActive = FCreateWAVInfo(lpDS, "sound/chirrup.wav", 3);
	lpwiClick = FCreateWAVInfo(lpDS, "sound/click.wav", 3);


	//set the inital task to show the splash screen
	taskState = TS_SPLASH;

	//Create a table of random numbers that can be looked up later on to save processing time
	CreateRndTable();

	//hide the mouse cursor
   // ShowCursor(false);

	//////////////////////////////////////
	//actual game setup
	mainMenu.Setup(&currentChar, lpDDSBack, lpDDSOffscreenStatus,
		lpDDSPrimary, lpDD, hwnd);
	eventPanel = new GameDisplay();
	eventPanel->Setup(lpDDSBack, lpDDSOffscreenStatus, lpDDSPrimary, lpDD, hwnd);

	//Sets up a pointer to the weapn object which the ships weapons are manged through
	weaponPtr = new WeaponManage();

	//Sets all of the changable game variables to their inital states.
	InitaliseClouds();

    return true;
}
//***********************************************************************


LRESULT APIENTRY WndFunc(HWND hwnd, UINT message, WPARAM wParam,LPARAM lParam)
//-------------------------------------------------------------------
//Handles all windows messages sent to this application
//-------------------------------------------------------------------
{
  HDC         hdc;
  PAINTSTRUCT ps;
	
	switch (message)
	{
		case WM_ACTIVATEAPP:
			bIsActive = (wParam == 0? false: true);
			break;


		case WM_GETMINMAXINFO:
			//Prevents the windowed version (DEBUG) from being resized
			int width, height;
			LPPOINT lppt;
			RECT rect;
      
			lppt = (LPPOINT)lParam;   // lParam points to array of POINTs

			GetWindowRect(hwnd, &rect);   // Get current window size
			width = 640;//rect.right - rect.left + 1;
			height = 480;//rect.bottom - rect.top + 1;

			lppt[3].x = width;   // Set minimum width to current width
			lppt[4].x = width;  // Set maximum width to current width
			lppt[3].y = height;   // Set minimum height to current height
			lppt[4].y = height;   // Set maximum height to current height

			return DefWindowProc(hwnd, message, wParam, lParam);

		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps);
			//if the window it updated then redraw the menuscreen.
			if (taskState == TS_MENU) mainMenu.Render(false);
				
			EndPaint (hwnd, &ps);
			return 0;

		case WM_KILLFOCUS:
			switch(taskState)
			{
			case TS_CRITICAL:
				taskState = TS_SUSPEND;
				break;
			default:
				break;
			}
			return 0;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			if (taskState == TS_SPLASH)
			{
				//call these functions so none of the keys are set on the first mouse check on the menu
				//it's easier to do this than to check the msb and lsb in the mouse class each time
				GetAsyncKeyState(VK_LBUTTON);
				GetAsyncKeyState(VK_RBUTTON);
				taskState = TS_MENU;
				//ShowCursor(true);
				ErasePrimarySurface ();
				EraseBackSurface();
				mainMenu.Render(false);
			}//taskState TS_SPLASH
			return 0;

		case WM_KEYDOWN:
			if (taskState == TS_SPLASH)
			{
				taskState = TS_MENU;
				//ShowCursor(true);
				ErasePrimarySurface ();
				EraseBackSurface();
				mainMenu.Render(false);
				return 0;
			}//taskState TS_SPLASH
			switch(wParam)
			{
				case VK_RETURN:
					return 0;
					
				case VK_ESCAPE:
					if (taskState == TS_CRITICAL)
					{
						ShowCursor(true);
						mainMenu.SetScreen(2);
						mainMenu.Render(true);
						taskState = TS_EXIT;
					}
					else
					{
						taskState = TS_TERMINATE;
					}
					return 0;

				case GAME_KEY_P:
					switch(taskState)
					{
					case TS_CRITICAL:
						taskState = TS_SUSPEND;
						break;
					case TS_SUSPEND:
						taskState = TS_CRITICAL;
						// Update the timer so everything doesnt think that it was just a really long cycle
						globalDiff = 0;
						globalTL = 0;
						globalTC = timeGetTime();
					case TS_SPLASH:
					default:
						break;
					}
					break;
			}
			return 0;
		case WM_CHAR:
			currentChar = wParam;
			break;
		case WM_DESTROY:
			delete eventPanel;
			PostQuitMessage (0);
			return 0;
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}
//***********************************************************************


void Scheduler()
//-------------------------------------------------------------------
// Scheduler - branches the game into its different states
//-------------------------------------------------------------------
{
	//The global timing variables are set up
	DWORD thisTC = timeGetTime();
	globalDiff = thisTC - globalTC;
	if (globalDiff == 0) globalDiff = 1;
//	Sleep(10);

#ifdef _DEBUG_

/*	if (FPSLoopCount-- < 0)
	{
		FPSLoopCount = 100;
		currentFPS = (int)(10000/(thisTC - FPSCount+1));
		FPSCount = thisTC;
		debugOut("   FPS      ", currentFPS, 100, 100);
	}*/

	if ((thisTC - FPSCount) > 1000)
	{
		currentFPS = FPSLoopCount;
		FPSCount = thisTC;
		FPSLoopCount = 0;
		debugOut("   FPS      ", currentFPS, 100, 100);
	}
	++FPSLoopCount;

#endif

	int result;
	switch( taskState)
	{
		case TS_NONE:
			break;
		case TS_SPLASH:
			Splash("Title Screen.bmp");
			break;
		case TS_MENU:
			result = mainMenu.StartInterface(false);
			if (result == 2)//The result starts the game
			{
				ShowCursor(false);
				
				InitaliseVariables();
				InitSlices();
				InitItems();

				//Clear any sprites before loading in new ones
				ClearSprites();
				LoadSprites();
				lpDD->FlipToGDISurface();

				//Set up the variables that allow the background to loop
				currentChar = '\0';
				globalTL = 0;
				taskState = TS_START;
				ErasePrimarySurface ();
				EraseBackSurface();

				RECT intRect = {0, 293, 131, 772};

				BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 496, 0, &intRect, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 
				BlitSurface( lpDDSOffscreenStatus, lpDDSPrimary, 496, 0, &intRect, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 
			}
			else if(result == 0)//This result ends the game
			{
				taskState = TS_TERMINATE;
			}
			break;
		case TS_CRITICAL:
			if (eventPanel->TestAlive())
			{
				UpdateGame();
				if (!CheckForSprites())
					taskState = TS_LEVEL_OVER;
			}
			else
			{
				PlayWAVInfo(lpwiExplosion, 0);
				currentChar = '\0';
				taskState = TS_DEATH;
			}
			break;
		case TS_START:
			StartAnimation();
			break;

		case TS_DEATH:
			EndAnimation();
			break;

		case TS_LEVEL_OVER:
			LevelOverProc();
			break;

		case TS_EXIT:
			result = mainMenu.StartInterface(true);
			if (result == 0)//This result ends the game
			{
				taskState = TS_TERMINATE;
			}
			else if (result == 3)//This result resumes the game
			{
				globalDiff = 0;
				globalTL = 0;
				globalTC = timeGetTime();
				ShowCursor(false);
				taskState = TS_CRITICAL;
			}
			break;

		case TS_TERMINATE:
			//Free up the memory used for the terrain
			LocalFree(itemPtr);
			LocalFree(slicePtr);
			ReleaseClouds();
	
			FreeWAVInfo(lpwiLaser);
			FreeWAVInfo(lpwiBullet);
			FreeWAVInfo(lpwiExplosion);
			FreeWAVInfo(lpwiBeep);
			FreeWAVInfo(lpwiHMissile);
			FreeWAVInfo(lpwiMissile);
			FreeWAVInfo(lpwiBulletHit);
			FreeWAVInfo(lpwiEnemyFire);
			FreeWAVInfo(lpwiActive);
			FreeWAVInfo(lpwiClick);

			if(lpDS) lpDS->Release();

			delete weaponPtr;
			DDShutdown();
			PostQuitMessage(0);
			break;
		case TS_SUSPEND:
		default:
			break;
	} // switch taskState

	globalTC = thisTC;
}
//***********************************************************************


void UpdateGame( void )
//-------------------------------------------------------------------
// UpdateGame
//-------------------------------------------------------------------
{
	//handles its timing independantly
	CheckKeyBoard();

	//Simply draws the terrain in its current state
	SuperDraw();

	//The clipper is set for this section because all of the sprites need to be clipped.
	lpDDSBack->SetClipper(lpDDCMain);
		UpdateSprites(lpDDSBack, lpDDSOffscreen);

		UpdateCloud(spaceShip.flameIndex, spaceShip.dest.left + 9, spaceShip.dest.bottom);
		DrawClouds();
	lpDDSBack->SetClipper(NULL);

	//Probably should be called before sprite routines however it seems to be fine here.
	DrawSpaceShip();

	// The event panel update object redraws the event pannel according to any changes and returns a value for special events
	// like (life lost = 0, ok = 1)
	eventPanel->Update();
	
	//Performs the actual front to back flip of the buffers
	FlipSurface(hwnd);

	//DWORD thisTickCount;
	//DWORD tickDiff;

	//thisTickCount = timeGetTime();
	//tickDiff = thisTickCount - scrollTickCount;
	//if (tickDiff > 0)//scrollDelay)
	//{
		//Update the position in relation to the buffer
		if ((globalPosy -= (float)globalDiff/40) < 0)
		{
			globalPosy += BUFFERSIZEY;
		}
		//Update the position in relation to the terrainArray
		if ((tilePosy -= (float)globalDiff/40) < 0)
		{
			tilePosy += 608;
			bufferGlobaly = (int)tilePosy + 1;
		}
	//	scrollTickCount = thisTickCount;
	//}
	//static float fGlobalY;
	
	//Update the position in relation to the buffer
	/*	if ((globalPosy -= globalDiff/20) < 0)
		{
			globalPosy += BUFFERSIZEY;
		}
		//Update the position in relation to the terrainArray
		if ((tilePosy -= globalDiff/20) < 0)
		{
			tilePosy += 608;
			bufferGlobaly = tilePosy + 1;
		}
		//scrollTickCount = thisTickCount;*/
	
}
//***********************************************************************


void StartAnimation( void )
//-------------------------------------------------------------------
// Handles the starting animation at the start of any level
//-------------------------------------------------------------------
{
	if (currentChar == '\0' || globalTL < 14)
	{
		if(globalPosy < (numSlices * 32)-512)
		{
			globalPosy = (float)(numSlices * 32)-480;
			tilePosy = (float)(numSlices * 32)-480;
		}

		SuperDraw();
		DrawSpaceShip();

		//The clipper is set for this section because all of the sprites need to be clipped.
		lpDDSBack->SetClipper(lpDDCMain);
			UpdateCloud(spaceShip.flameIndex, spaceShip.dest.left + 9, spaceShip.dest.top + 64);
			DrawClouds();

			//If the level has just loaded display the ready message.

		lpDDSBack->SetClipper(NULL);

		RECT intRect;
		SetRect(&intRect, 366 , 184 + (int)globalTL, 456, 211 - (int)globalTL);
		BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 195, 226 + (int)globalTL, &intRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 
		if (currentChar != '\0' && globalDiff < 200) 
			globalTL += (float)(globalDiff)/40;

		FlipSurface(hwnd);

		DWORD thisTickCount;
		DWORD tickDiff;

		thisTickCount = timeGetTime();
		tickDiff = thisTickCount - scrollTickCount;
		if (tickDiff > scrollDelay)
		{
			//Update the position in relation to the buffer
			if ((globalPosy -= 1) < 0)
			{
				globalPosy += BUFFERSIZEY;
			}
			//Update the position in relation to the terrainArray
			if ((tilePosy -= 1) < 0)
			{
				tilePosy += 608;
				bufferGlobaly = (int)tilePosy + 1;
			}
			scrollTickCount = thisTickCount;
		}
	}
	else
	{
		globalPosy = (float)((numSlices) * 32)-480;
		tilePosy = (float)((numSlices) * 32)-480;
		taskState = TS_CRITICAL;
	}
}
//***********************************************************************


void EndAnimation( void )
//-------------------------------------------------------------------
// Handles the starting animation at the start of any level
//-------------------------------------------------------------------
{
	SuperDraw();

	//The clipper is set for this section because all of the sprites need to be clipped.
	lpDDSBack->SetClipper(lpDDCMain);
		UpdateSprites(lpDDSBack, lpDDSOffscreen);
		//UpdateCloud(spaceShip.flameIndex, spaceShip.dest.left + 9, spaceShip.dest.top + 64);
		DrawClouds();
	lpDDSBack->SetClipper(NULL);
			
	if ((globalTL -= (float)(globalDiff)/160) > 7)
	{
		explList.Add(globalPosx + spaceShip.dest.left + rand()%18, spaceShip.dest.top + rand()%32, 0, 3, false);
		StopCloud(spaceShip.flameIndex);
		weaponPtr->NoFire();
		//so that the ship is not hit by any bullets
		currentChar = '\0';
	}
	else if ((globalTL -= (float)(globalDiff)/160) > 0)
	{
		RECT intRect;
		SetRect(&intRect, 366 , 213 + (int)globalTL, 508, 240 - (int)globalTL);
		BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 170, 226 + (int)globalTL, &intRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 

		spaceShip.dest.top = -1000;
		spaceShip.dest.bottom = -1000;
		currentChar = '\0';
	}
	else
	{
		RECT intRect;
		SetRect(&intRect, 366 , 213, 508, 240);
		BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 170, 226, &intRect, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 
	}
	
	if (currentChar != '\0')
	{
		mainMenu.TestScoreUpdate(eventPanel->GetScore());
		taskState = TS_MENU;
		ShowCursor(true);
		ErasePrimarySurface ();
		EraseBackSurface();
		mainMenu.Render(false);
		return;
		//taskState = TS_TERMINATE;
	}

	FlipSurface(hwnd);

	DWORD thisTickCount;
	DWORD tickDiff;

	thisTickCount = timeGetTime();
	tickDiff = thisTickCount - scrollTickCount;
	if (tickDiff > scrollDelay)
	{
		//Update the position in relation to the buffer
		if ((globalPosy -= 1) < 0)
		{
			globalPosy += BUFFERSIZEY;
		}
		//Update the position in relation to the terrainArray
		if ((tilePosy -= 1) < 0)
		{
			tilePosy += 608;
			bufferGlobaly = (int)tilePosy + 1;
		}
		scrollTickCount = thisTickCount;
	}
}
//***********************************************************************


void LevelOverProc()
//-------------------------------------------------------------------
// handles the end of a level ie reload a new level calculate text etc
//-------------------------------------------------------------------
{
	SuperDraw();
	DrawSpaceShip();

		//The clipper is set for this section because all of the sprites need to be clipped.
	lpDDSBack->SetClipper(lpDDCMain);
		UpdateSprites(lpDDSBack, lpDDSOffscreen); //this needs to be here to draw any weapon fire that may still be on the screen
		DrawClouds(); //Still need to draw the flames etc
	lpDDSBack->SetClipper(NULL);
			
	if ((globalTL -= (float)(globalDiff)/80) > 0)
	{
		RECT intRect;
		SetRect(&intRect, 302 , 241 + (int)globalTL, 510, 268 - (int)globalTL);
		BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 138, 226 + (int)globalTL, &intRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 
		weaponPtr->NoFire();
		currentChar = '\0';
		spaceShip.motion = NO_MOVE;			// no inital movement
		spaceShip.currentCell =0;			// show in an upright position
	}
	else
	{
		RECT intRect;
		SetRect(&intRect, 302 , 241, 510, 268);
		BlitSurface( lpDDSOffscreenStatus, lpDDSBack, 138, 226, &intRect, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 
		if (eventPanel->TestAlive())
		{
			eventPanel->SetUpdate(0, -0.05f);
			eventPanel->SetUpdate(3, 1.0f);
			eventPanel->Update();
		}
	}
	
	if (currentChar != '\0')
	{
		mainMenu.TestScoreUpdate(eventPanel->GetScore());
		taskState = TS_MENU;
		ShowCursor(true);
		ErasePrimarySurface ();
		EraseBackSurface();
		mainMenu.Render(false);
		return;
	}

	FlipSurface(hwnd);

	DWORD thisTickCount;
	DWORD tickDiff;

	thisTickCount = timeGetTime();
	tickDiff = thisTickCount - scrollTickCount;
	if (tickDiff > scrollDelay)
	{
		if (eventPanel->TestAlive())
			PlayWAVInfo(lpwiActive, 0);
		//Update the position in relation to the buffer
		if ((globalPosy -= 1) < 0)
		{
			globalPosy += BUFFERSIZEY;
		}
		//Update the position in relation to the terrainArray
		if ((tilePosy -= 1) < 0)
		{
			tilePosy += 608;
			bufferGlobaly = (int)tilePosy + 1;
		}
		scrollTickCount = thisTickCount;
	}
}
//***********************************************************************


void InitaliseVariables(void)
//-------------------------------------------------------------------
// Resets the game variables to therir inital state used at the end of
// a level to reset the score/position in the level etc
//-------------------------------------------------------------------
{
	globalPosx = 0;
	bufferLeft = 0;
	bufferGlobalx = 0;
	bufferTop = 0;

	// Sets all the clouds to useable
	StopAllClouds();

	//initalise the space ship structure
	spaceShip.motion = NO_MOVE;			// no inital movement
	spaceShip.dest.left =224;			// inital xposition
	spaceShip.dest.top =298;			// inital yposition
	spaceShip.currentCell =0;			// show in an upright position
	spaceShip.delay = 65;				// delay between cell updates
	spaceShip.moveDelay = 10;			// delay between movement updates
	// variable for calculating when the next update should occour
	spaceShip.lastTickCount = timeGetTime();
	spaceShip.moveTickCount = spaceShip.lastTickCount;
	
	scrollDelay = 20;
	scrollTickCount = spaceShip.lastTickCount;

	// set up a flame for the main spaceship.
	spaceShip.flameIndex = SetCloud(1, spaceShip.dest.left + 9, spaceShip.dest.top + 64, 0, 0);

	eventPanel->ResetDisplay();
	weaponPtr->ResetState();
}
//***********************************************************************