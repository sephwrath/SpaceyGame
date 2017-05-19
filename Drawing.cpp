////////////////////////////////////////////////////////////////////////
//This file contains external functions for drawing and collision used 
//throughout the game. Mainly focusing on the particle effects.
//
//This (Particularly the partical functions) should probably be redone in an object oriented fashion for clarity
//however it exists in the procedural format because I thought I may be able to get
//it to run faster this way.
//
//Written by Stephen Moran
//Date: 18/12/1999
////////////////////////////////////////////////////////////////////////

#include "Drawing.h"

// Macros used for combining the different color segments on different displays when wrighting individual pixels
#define RGB_16BIT565(r, g, b)  ((r << 11) | (g << 5) | (b))	
#define RGB_16BIT555(r, g, b)  ((r << 10) | (g << 5) | (b))

// Clears the memory of a direct x structure
#define INIT_DXSTRUCT(dxs) { ZeroMemory(&dxs, sizeof(dxs)); dxs.dwSize = sizeof(dxs); }

#include <stdio.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////
// only used occasionally for debugging purposes otherwise should be commented out.
/////////////////////////////////////////////////////////////////////////////////////
//extern HDC hDCbb;
//extern char dbBuffer[40];

//////////////////////////////////////////////////////////////////////////////////////////////
//							Frames for the different particle animations
//////////////////////////////////////////////////////////////////////////////////////////////
	RECT flameFrms[] = {
	//Spaceship jetFlame type frame definitions.
	//	left	top		right	bottom
		{420,	64,		435,	80},
		{436,	64,		451,	80}
	};

	RECT fireFrms[] = {
	//Fire to smoke frame definitions.
	//	left	top		right	bottom
		{228,	175,	246,	193},
		{246,	175,	263,	192},
		{263,	175,	279,	191},
		{279,	175,	294,	190},
		{294,	175,	308,	189},
		{308,	175,	321,	188},
		{321,	175,	333,	187},
		{333,	175,	344,	186},
		{344,	175,	354,	185},// fire 1
		{354,	175,	363,	184},
		{363,	175,	371,	183},
		{371,	175,	378,	182},
		{378,	175,	384,	181},
		{384,	175,	389,	180},
		{389,	175,	393,	179},
		{393,	175,	396,	178},
		{396,	175,	398,	177},

		{380,	183,	398,	201},
		{363,	184,	380,	201},
		{347,	185,	363,	201},
		{332,	186,	347,	201},
		{318,	187,	332,	201},
		{305,	188,	318,	201},
		{293,	189,	305,	201},
		{282,	190,	293,	201},
		{272,	191,	282,	201},
		{263,	192,	272,	201},// fire 2
		{255,	193,	263,	201},
		{248,	194,	255,	201},
		{242,	195,	248,	201},
		{237,	196,	242,	201},
		{233,	197,	237,	201},
		{230,	198,	233,	201},
		{228,	199,	230,	201}
	};

	RECT smokeFrms[] = {
	//Asteroid Alien type frame definitions
	// in each case largest to smallest lightest to darkest
	// to get the correct frame darkness*17 + frame
	//	left	top		right	bottom
		{58,	149,	76,		167},//0
		{76,	149,	93,		166},//1
		{93,	149,	109,	165},//2
		{109,	149,	124,	164},//3
		{124,	149,	138,	163},//4
		{138,	149,	151,	162},//5
		{151,	149,	163,	161},//6
		{163,	149,	174,	160},//7
		{174,	149,	184,	159},//8 light grey top row
		{184,	149,	193,	158},//9
		{193,	149,	201,	157},//10
		{201,	149,	208,	156},//11
		{208,	149,	214,	155},//12
		{214,	149,	219,	154},//13
		{219,	149,	223,	153},//14
		{223,	149,	226,	152},//15
		{226,	149,	228,	151},//16

		{210,	157,	228,	175},
		{193,	158,	210,	175},
		{177,	159,	193,	175},
		{162,	160,	177,	175},
		{148,	161,	162,	175},
		{135,	162,	148,	175},
		{123,	163,	135,	175},
		{112,	164,	123,	175},
		{102,	165,	112,	175},
		{93,	166,	102,	175},
		{85,	167,	93,		175},
		{78,	168,	85,		175},
		{72,	169,	78,		175},
		{67,	170,	72,		175},
		{63,	171,	67,		175},
		{60,	172,	63,		175},
		{58,	173,	60,		175},

		{58,	175,	76,		193},
		{76,	175,	93,		192},
		{93,	175,	109,	191},
		{109,	175,	124,	190},
		{124,	175,	138,	189},
		{138,	175,	151,	188},
		{151,	175,	163,	187},
		{163,	175,	174,	186},
		{174,	175,	184,	185},// bottom row
		{184,	175,	193,	184},
		{193,	175,	201,	183},
		{201,	175,	208,	182},
		{208,	175,	214,	181},
		{214,	175,	219,	180},
		{219,	175,	223,	179},
		{223,	175,	226,	178},
		{226,	175,	228,	177},

		{210,	183,	228,	201},
		{193,	184,	210,	201},
		{177,	185,	193,	201},
		{162,	186,	177,	201},
		{148,	187,	162,	201},
		{135,	188,	148,	201},
		{123,	189,	135,	201},
		{112,	190,	123,	201},
		{102,	191,	112,	201},
		{93,	192,	102,	201},
		{85,	193,	93,		201},
		{78,	194,	85,		201},
		{72,	195,	78,		201},
		{67,	196,	72,		201},
		{63,	197,	67,		201},
		{60,	198,	63,		201},
		{58,	199,	60,		201},

		{228,	149,	246,	167},
		{246,	149,	263,	166},
		{263,	149,	279,	165},
		{279,	149,	294,	164},
		{294,	149,	308,	163},
		{308,	149,	321,	162},
		{321,	149,	333,	161},
		{333,	149,	344,	160},
		{344,	149,	354,	159},// right top row
		{354,	149,	363,	158},
		{363,	149,	371,	157},
		{371,	149,	378,	156},
		{378,	149,	384,	155},
		{384,	149,	389,	154},
		{389,	149,	393,	153},
		{393,	149,	396,	152},
		{396,	149,	398,	151},

		{380,	157,	398,	175},
		{363,	158,	380,	175},
		{347,	159,	363,	175},
		{332,	160,	347,	175},
		{318,	161,	332,	175},
		{305,	162,	318,	175},
		{293,	163,	305,	175},
		{282,	164,	293,	175},
		{272,	165,	282,	175},
		{263,	166,	272,	175},// black
		{255,	167,	263,	175},
		{248,	168,	255,	175},
		{242,	169,	248,	175},
		{237,	170,	242,	175},
		{233,	171,	237,	175},
		{230,	172,	233,	175},
		{228,	173,	230,	175}
	};

	RECT collisionFrms[] = {
	//collision spark frame definitions.
	//	left	top		right	bottom
		{300,	64,		315,	79},
		{315,	64,		330,	79},
		{330,	64,		345,	79},
		{345,	64,		360,	79},
		{360,	64,		375,	79},
		{375,	64,		390,	79},
		{390,	64,		405,	79},
		{405,	64,		420,	79}
	};

	RECT hitFrms[] = {
	//bulltet hit animation in groups of three.
	//	left	top		right	bottom
		{515,	64,		538,	82},// largest
		{538,	64,		561,	82},
		{561,	64,		584,	82},
		{364,	80,		387,	92},// medium
		{387,	80,		410,	92},
		{410,	80,		433,	92},
		{299,	79,		315,	93},// smallest
		{318,	78,		334,	92},
		{337,	78,		353,	92}
	};

	RECT shrapnel1Frms[] = {
	//shrapnel type one frames. 0 - 35
	//	left	top		right	bottom
		{115,	460,	125,	472},
		{125,	460,	135,	472},
		{135,	460,	145,	472},
		{145,	460,	155,	472},
		{155,	460,	165,	472},
		{165,	460,	175,	472},
		{175,	460,	185,	472},
		{185,	460,	195,	472},
		{195,	460,	205,	472},
		{205,	460,	215,	472},
		{215,	460,	225,	472},
		{225,	460,	235,	472},
		{235,	460,	245,	472},
		{245,	460,	255,	472},
		{255,	460,	265,	472},
		{265,	460,	275,	472},
		{275,	460,	285,	472},
		{285,	460,	295,	472},
		{295,	460,	305,	472},
		{305,	460,	315,	472},
		{315,	460,	325,	472},
		{325,	460,	335,	472},
		{335,	460,	345,	472},
		{345,	460,	355,	472},
		{355,	460,	365,	472},
		{365,	460,	375,	472},
		{375,	460,	385,	472},
		{385,	460,	395,	472},
		{395,	460,	405,	472},
		{405,	460,	415,	472},
		{415,	460,	425,	472},
		{425,	460,	435,	472},
		{435,	460,	445,	472},
		{445,	460,	455,	472},
		{455,	460,	465,	472},
		{465,	460,	475,	472}
	};

	RECT shrapnel2Frms[] = {
	//shrapnel type two frames. 0 - 35
	//	left	top		right	bottom
		{115,	472,	125,	482},
		{125,	472,	135,	482},
		{135,	472,	145,	482},
		{145,	472,	155,	482},
		{155,	472,	165,	482},
		{165,	472,	175,	482},
		{175,	472,	185,	482},
		{185,	472,	195,	482},
		{195,	472,	205,	482},
		{205,	472,	215,	482},
		{215,	472,	225,	482},
		{225,	472,	235,	482},
		{235,	472,	245,	482},
		{245,	472,	255,	482},
		{255,	472,	265,	482},
		{265,	472,	275,	482},
		{275,	472,	285,	482},
		{285,	472,	295,	482},
		{295,	472,	305,	482},
		{305,	472,	315,	482},
		{315,	472,	325,	482},
		{325,	472,	335,	482},
		{335,	472,	345,	482},
		{345,	472,	355,	482},
		{355,	472,	365,	482},
		{365,	472,	375,	482},
		{375,	472,	385,	482},
		{385,	472,	395,	482},
		{395,	472,	405,	482},
		{405,	472,	415,	482},
		{415,	472,	425,	482},
		{425,	472,	435,	482},
		{435,	472,	445,	482},
		{445,	472,	455,	482},
		{455,	472,	465,	482},
		{465,	472,	475,	482}
	};

	RECT shrapnel3Frms[] = {
	//shrapnel type three frames. 0 - 25
	//	left	top		right	bottom
		{115,	483,	125,	493},
		{125,	483,	135,	493},
		{135,	483,	145,	493},
		{145,	483,	155,	493},
		{155,	483,	165,	493},
		{165,	483,	175,	493},
		{175,	483,	185,	493},
		{185,	483,	195,	493},
		{195,	483,	205,	493},
		{205,	483,	215,	493},
		{215,	483,	225,	493},
		{225,	483,	235,	493},
		{235,	483,	245,	493},
		{245,	483,	255,	493},
		{255,	483,	265,	493},
		{265,	483,	275,	493},
		{275,	483,	285,	493},
		{285,	483,	295,	493},
		{295,	483,	305,	493},
		{305,	483,	315,	493},
		{315,	483,	325,	493},
		{325,	483,	335,	493},
		{335,	483,	345,	493},
		{345,	483,	355,	493},
		{355,	483,	365,	493},
		{365,	483,	375,	493}
	};
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||



void DrawClouds()
//-------------------------------------------------------------------
//	Draws any visible particle clouds.
//-------------------------------------------------------------------
{
	int i;
/*	DDSURFACEDESC2 ddsd;
	INIT_DXSTRUCT(ddsd);

	buffer = (USHORT*)ddsd.lpSurface;
	nPitch = (USHORT)ddsd.lPitch >> 1;
// All of the code to draw individual pixels goes between the lock and unlock function calls
// even though there is none here yet this is where it will go
// Ps if these calls are removed the game slows down - go figure. 
// (According to all logic removing the calls should speed the game up however it doesnt - perhaps a bug?)
// Ok this has been fixed - turns out it was to do with where the surfaces were being loaded Im still not sure
// that there is any actual logic behind it but it's now fixed.
#ifdef _DEBUG_
	lpDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR | DDLOCK_NOSYSLOCK , NULL);
#else
	lpDDSBack->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR , NULL);
#endif

	lpDDSBack->Unlock(NULL);*/

	for (i = 0; i < 55; i++)//CLOUD_NO
	{
		if (clouds[i].activated == true)
		{
			switch (clouds[i].type)
			{
			case 1://6
				DrawFlame(i);
				break;
			case 2://2
				DrawCollision(i, globalDiff);
				break;
			case 3://2
				DrawSmoke(i, globalDiff);
				break;
			case 4://2
				DrawFire(i, globalDiff);
				break;
			case 5://3
				DrawShrapnel (i, globalDiff);
				break;
			case 6://2
				DrawHit(i, globalDiff);
				break;
			case 7://8
				DrawSmokeTrail(i, globalDiff);
				break;
			default:
				break;
			}
		}
	}
}
//***********************************************************************


bool InitaliseClouds()
//-------------------------------------------------------------------
//	Must be called first and is used to set up variables used by the other functions
//	specifically its used to set all of the elements in the clouds array to their
//	initial values
//
//	Return Value - returns false if any of the particle memory allocations
//				   fail otherwise returns true
//-------------------------------------------------------------------
{
	//////////////////////////////////////////////
	//JET FLAMES 0 - 5
	for (int i = 0; i < 5; ++i)
	{
		clouds[i].particles = NULL;

		clouds[i].type = 1;
		clouds[i].activated = false;
		clouds[i].noPart = 1;
		clouds[i].width = 15;
		clouds[i].height = 16;
	}

	//////////////////////////////////////////////
	//COLLISION 6 - 7
	for (i = 5; i < 9; ++i)
	{
		clouds[i].particles = NULL;

		clouds[i].type = 2;
		clouds[i].activated = false;
		clouds[i].noPart = 1;
		clouds[i].timeLeft = 0;
		clouds[i].width = 15;
		clouds[i].height = 15;
	}

	//////////////////////////////////////////////
	//SMOKE
	for (i = 9; i < 14; ++i)
	{
		clouds[i].particles = (PARTICLE *)calloc( 40, sizeof(PARTICLE) );
		if (clouds[i].particles == NULL)
		{
			MessageBox(GetActiveWindow(),"Memory allocation error in initaliseFlames", "Spacey II", MB_OK);
			return false;
		}

		clouds[i].type = 3;
		clouds[i].activated = false;
		clouds[i].noPart = 40;
		clouds[i].height = 100;

		for (int number = 0; number < clouds[i].noPart; number++)
		{
			clouds[i].particles[number].xPos = 0.0f;
			clouds[i].particles[number].yPos = 0.0f;
			clouds[i].particles[number].starty = (rand()%2) + 2;
			clouds[i].particles[number].startx = (rand()%3) - 1;

			clouds[i].particles[number].animationSet = (rand()%6)*17;
			clouds[i].particles[number].cell = 16.999f;
			clouds[i].particles[number].cellChange = (float)(clouds[i].height/abs(clouds[i].particles[number].starty))/17;
			clouds[i].particles[number].delay = clouds[i].particles[number - 1].delayLeft + (float)(rand()%2);
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}
	}

	//////////////////////////////////////////////
	//FIRE
	for (i = 14; i < 17; i++)
	{
		clouds[i].particles = (PARTICLE *)calloc( 80, sizeof(PARTICLE) );
		if (clouds[i].particles == NULL)
		{
			MessageBox(GetActiveWindow(),"Memory allocation error in initaliseFlames", "Spacey II", MB_OK);
			return false;
		}

		clouds[i].type = 4;
		clouds[i].activated = false;
		clouds[i].noPart = 80;
		clouds[i].height = 100;

		for (int number = 0; number < clouds[i].noPart; number++)
		{
			clouds[i].particles[number].xPos = 0.0f;
			clouds[i].particles[number].yPos = 0.0f;
			clouds[i].particles[number].starty = (rand()%2) + 2;
			clouds[i].particles[number].startx = (rand()%3) - 1;

			clouds[i].particles[number].animationSet = ((rand()%2))*17;
			clouds[i].particles[number].cell = 16.999f;
			clouds[i].particles[number].cellChange = (float)(clouds[i].height/abs(clouds[i].particles[number].starty))/17;
			clouds[i].particles[number].delay = clouds[i].particles[number - 1].delayLeft + (float)(rand()%2);
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}
	}

	//////////////////////////////////////////////
	//SHRAPNEL
	for (i = 17; i < 30; i++)
	{
		clouds[i].particles = (PARTICLE *)calloc( 17, sizeof(PARTICLE) );
		if (clouds[i].particles == NULL)
		{
			MessageBox(GetActiveWindow(),"Memory allocation error in initaliseFlames", "Spacey II", MB_OK);
			return false;
		}

		clouds[i].type = 5;
		clouds[i].activated = false;
		clouds[i].noPart = 17;
		clouds[i].height = 100;

		for (int number = 0; number < clouds[i].noPart; number++)
		{
			clouds[i].particles[number].xPos = 0.0f;
			clouds[i].particles[number].yPos = 0.0f;
			clouds[i].particles[number].starty = (rand()%7) - 3;
			clouds[i].particles[number].startx = (rand()%7) - 3;

			clouds[i].particles[number].cell = 0;
			clouds[i].particles[number].cellChange = 4;

			if (number != 0)
			{
				clouds[i].particles[number].delay = clouds[i].particles[number - 1].delayLeft + 1;
				clouds[i].particles[number].animationSet = 5*17;
			}
			else
			{
				clouds[i].particles[number].delay = 0;
				clouds[i].particles[number].animationSet = (rand()%3) + 1;
			}

			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}
	}

	//////////////////////////////////////////////
	//WEAPON HIT 30 - 35
	for (i = 30; i < 35; i++)
	{
		clouds[i].particles = NULL;

		clouds[i].type = 6;
		clouds[i].activated = false;
		clouds[i].noPart = 1;
		clouds[i].timeLeft = 0;
	}

	//////////////////////////////////////////////
	//SMOKE TRAIL
	for (i = 35; i < 55; i++)
	{
		clouds[i].particles = (PARTICLE *)calloc( 20, sizeof(PARTICLE) );
		if (clouds[i].particles == NULL)
		{
			MessageBox(GetActiveWindow(),"Memory allocation error in initaliseFlames", "Spacey II", MB_OK);
			return false;
		}

		clouds[i].type = 7;
		clouds[i].activated = false;
		clouds[i].noPart = 17;
		clouds[i].height = 100;

		for (int number = 0; number < clouds[i].noPart; number++)
		{
			clouds[i].particles[number].xPos = 0.0f;
			clouds[i].particles[number].yPos = 0.0f;
			clouds[i].particles[number].starty = (rand()%7) - 3;
			clouds[i].particles[number].startx = (rand()%7) - 3;

			clouds[i].particles[number].cell = 0;
			clouds[i].particles[number].cellChange = 4;


			clouds[i].particles[number].delay = (float)number*1;
			clouds[i].particles[number].animationSet = 3*17; //grey


			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}
	}
	return true;

}
//******************************************************************************



int SetCloud(int type, int xPos, int yPos, float xVel, float yVel)
//-------------------------------------------------------------------
//Sets the clouds inital position, height, width and starts the animation
//
//Return Value - an index used to update the cloud. If no more clouds
//				 of this type could be allocated then -1 is returned.
//
//Comment - This function is probably the strongest argument for this
//			not being procedural ie some of the variable names are used
//			for different things - this needs to be changed!!!
//-------------------------------------------------------------------
{
	int i, number;

	switch (type)
	{
	case 1: //flame - 
		for (i = 0; i < 5; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 4)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos;
		clouds[i].yPos = yPos;
		clouds[i].speedY = 0; //used to indicate the current frame for exhaust flames

		if (xVel > 0)
			clouds[i].spXRand = 1; //used to indicate wheter the image is fliped on the horizontal axis or not
		else
			clouds[i].spXRand = 0;

		break;

	case 2: //collision
		for (i = 5; i < 9; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 8)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos - 7; //put the x y point in the middle 
		clouds[i].yPos = yPos - 7;
		clouds[i].timeLeft  = 0; //used to indicate the current frame for exhaust flames

		break;

	case 3: //smoke
		for (i = 9; i < 14; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 13)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos; 
		clouds[i].yPos = yPos;
		
		if (xVel == 0) xVel = 1;
		if (yVel == 0) yVel = 1;
		for (number = 0; number < clouds[i].noPart; number++)
		{
			// Sets the speed and direction of the particles
			clouds[i].particles[number].speedY = (float)(clouds[i].particles[number].starty/yVel);
			// Sets the spread of the particles
			clouds[i].particles[number].speedX = (float)(clouds[i].particles[number].startx/xVel);
			//restarts the delay
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}

		break;

	case 4: //fire
		for (i = 14; i < 17; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 16)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos; 
		clouds[i].yPos = yPos;
		
		if (xVel == 0) xVel = 1;
		if (yVel == 0) yVel = 1;
		for (number = 0; number < clouds[i].noPart; number++)
		{
			// Sets the speed and direction of the particles
			clouds[i].particles[number].speedY = (float)(clouds[i].particles[number].starty/yVel);
			// Sets the spread of the particles
			clouds[i].particles[number].speedX = (float)(clouds[i].particles[number].startx/xVel);
			//restarts the delay
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}

		break;

	case 5: //Shrapnel
		for (i = 17; i < 30; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 29)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos; 
		clouds[i].yPos = yPos;
		
		clouds[i].particles[0].speedY = yVel;
		clouds[i].particles[0].speedX = xVel;
		clouds[i].particles[0].xPos = 0;
		clouds[i].particles[0].yPos = 0;
		for (number = 1; number < clouds[i].noPart; number++)
		{
			// Sets the speed and direction of the particles
			clouds[i].particles[number].speedY = yVel/3;//1.5f;
			// Sets the spread of the particles
			clouds[i].particles[number].speedX = xVel/3;//1.5f;
			clouds[i].particles[number].cell = 0;
			// Restarts the delay
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}

		break;

	case 6: //weapon hit
		for (i = 30; i < 35; i++)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 34)
				return -1; // return -1 if a cloud of this type could not be added
		}
		
		clouds[i].xPos = xPos - 7; //put the x y point in the middle 
		clouds[i].yPos = yPos - 7;
		if (i > 30 && xPos == clouds[i - 1].xPos && yPos == clouds[i - 1].yPos) return -1;


		clouds[i].activated = true;
		if (xVel == 0)
		{
			clouds[i].height =  14;
			clouds[i].width = 16;
			clouds[i].timeLeft  = 0; //used to indicate the current frame for exhaust flames
			clouds[i].delayLeft = 2; //used to indicate the current animation set.
		}
		else if (xVel == 1)
		{
			clouds[i].height = 12;
			clouds[i].width = 23;
			clouds[i].timeLeft  = 0; //used to indicate the current frame for exhaust flames
			clouds[i].delayLeft = 1; //used to indicate the current animation set.
		}
		else 
		{
			clouds[i].height = 18;
			clouds[i].width = 23;
			clouds[i].timeLeft  = 0; //used to indicate the current frame for exhaust flames
			clouds[i].delayLeft = 0; //used to indicate the current animation set.
		}

		break;

	case 7: //Smoke Trail
		for (i = 35; i < 55; ++i)
		{
			if (clouds[i].activated == false)
				break;
			else if (i == 54)
				return -1; // return -1 if a cloud of this type could not be added
		}

		clouds[i].activated = true;
		clouds[i].xPos = xPos;
		clouds[i].yPos = yPos;
		
		
		for (number = 0; number < clouds[i].noPart; ++number)
		{
			// Sets the speed and direction of the particles
			clouds[i].particles[number].speedY = yVel;//1.5f;
			// Sets the spread of the particles
			clouds[i].particles[number].speedX = xVel;//1.5f;
			clouds[i].particles[number].cell = 0;
			//clouds[i].particles[number].xPos
			// Restarts the delay
			clouds[i].particles[number].delayLeft = clouds[i].particles[number].delay;
		}

		break;
	default:
		break;
	}
	return i;
	
}
//***********************************************************************



void UpdateCloud(int index, int x, int y)

//-------------------------------------------------------------------
//	updates the position of the cloud denoted by the index, the
//  coordinates given are screen coordinates
//-------------------------------------------------------------------
{
	if (index >= 0 && index < 55)
	{
		clouds[index].xPos = x;
		clouds[index].yPos = y;
	}
}
//***********************************************************************



void StopCloud(int index)
//-------------------------------------------------------------------
//	Called to flag a cloud that is ready for reuse
//-------------------------------------------------------------------
{
	if (index >= 0 && index < 55)
		clouds[index].activated = false;
}
//***********************************************************************



void StopAllClouds()
//-------------------------------------------------------------------
//	Called to reset all positions of clouds called at the start of each level to clear
//		prevoius positions etc
//-------------------------------------------------------------------
{
	for (int i = 0; i < 55; i++)
	{
		clouds[i].activated = false;
	}
}
//***********************************************************************



void ReleaseClouds()
//-------------------------------------------------------------------
//	Releases the memory allocated to the clouds array when the program ends
//-------------------------------------------------------------------
{
	for (int i = 9; i < 55; i++)
	{
		free(clouds[i].particles);
	}
}
//***********************************************************************



void CreateRndTable ()
//-------------------------------------------------------------------
//	Initalise an array of random values
//-------------------------------------------------------------------
{
	srand(time( NULL ) );
	for (int i = 0; i < 2000; i++)
		randTable[i] = (int)rand();
}
//***********************************************************************



inline void PlotPixel16(int x, int y, USHORT color, USHORT* buffer, int nPitch)
//-------------------------------------------------------------------
//	This function is used to plot pixels
//-------------------------------------------------------------------
{
  buffer[y*nPitch + x] = color;
}
//***********************************************************************



inline USHORT GetPixel16(int x, int y, USHORT* buffer, int nPitch)
//-------------------------------------------------------------------
//							This function is used to plot pixels
//-------------------------------------------------------------------
{
  return buffer[y*nPitch + x];
}
//***********************************************************************



//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// The following functions are the different drawing routines for the different clouds

//-------------------------------------------------------------------
//								SHIP FLAME
//-------------------------------------------------------------------
void DrawFlame (int index)
{
	RECT dest;
	DDBLTFX fx;
	INIT_DXSTRUCT(fx);
	
	SetRect(&dest, clouds[index].xPos, clouds[index].yPos,
		clouds[index].xPos + clouds[index].width, clouds[index].yPos + clouds[index].height); 
	
    if (clouds[index].speedY == 0)
		clouds[index].speedY = 1;
	else
		clouds[index].speedY = 0;

	if (clouds[index].spXRand == 1)
		fx.dwDDFX = DDBLTFX_MIRRORUPDOWN;
	else
		fx.dwDDFX = NULL;

	while(1)
	{
		HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen, &flameFrms[clouds[index].speedY],
			DDBLT_KEYSRC|DDBLT_DDFX, &fx);//DDBLT_WAIT|

		if ( ddrval == DD_OK)
			break;

		if (ddrval == DDERR_SURFACELOST)
		{
			ddrval = lpDD->RestoreAllSurfaces();
		}

		if (ddrval != DD_OK)
			break;
		if (ddrval != DDERR_WASSTILLDRAWING)
			break;
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								COLLISION
//-------------------------------------------------------------------
void DrawCollision (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	RECT dest;	

	SetRect(&dest, clouds[index].xPos, clouds[index].yPos,
		clouds[index].xPos + clouds[index].width, clouds[index].yPos + clouds[index].height); 
	
    if ((clouds[index].timeLeft += tickTemp) >= 8)
	{
		clouds[index].timeLeft = 0;
		clouds[index].activated = false;
		return;
	}

	while(1)
	{
		HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen, &collisionFrms[(int)(clouds[index].timeLeft)],
			DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

		if ( ddrval == DD_OK)
			break;

		if (ddrval == DDERR_SURFACELOST)
		{
			ddrval = lpDD->RestoreAllSurfaces();
		}

		if (ddrval != DD_OK)
			break;
		if (ddrval != DDERR_WASSTILLDRAWING)
			break;
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								DRAW WEAPON HIT
//-------------------------------------------------------------------
void DrawHit (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	RECT dest;	

	SetRect(&dest, clouds[index].xPos, clouds[index].yPos,
		clouds[index].xPos + clouds[index].width, clouds[index].yPos + clouds[index].height); 
	
    if ((clouds[index].timeLeft += tickTemp) >= 3)
	{
		clouds[index].timeLeft = 0;
		clouds[index].activated = false;
		return;
	}

	while(1)
	{
		HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
			&hitFrms[(int)(clouds[index].timeLeft + clouds[index].delayLeft*3)],
			DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

		if ( ddrval == DD_OK)
			break;

		if (ddrval == DDERR_SURFACELOST)
		{
			ddrval = lpDD->RestoreAllSurfaces();
		}

		if (ddrval != DD_OK)
			break;
		if (ddrval != DDERR_WASSTILLDRAWING)
			break;
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								SMOKE
//-------------------------------------------------------------------
void DrawSmoke (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	RECT dest;
	PARTICLE *partPtr = clouds[index].particles;

	for (int i = 0; i < clouds[index].noPart; i++)
	{
		if (clouds[index].particles[i].delayLeft > 0)
		{
			// all particles after should have a greater delay so just exit now
			clouds[index].particles[i].delayLeft -= tickTemp;
			break;
		}
		else
		{
			// increment at a fraction of the sppen of the acceleration
			partPtr[i].cell -= (tickTemp/partPtr[i].cellChange);
			if (partPtr[i].cell <= 0)
			{
				partPtr[i].cell = 16.999f;
				partPtr[i].xPos = 0;
				partPtr[i].yPos = 0;
			}

			SetRect(&dest, (int)partPtr[i].xPos + clouds[index].xPos, (int)partPtr[i].yPos + clouds[index].yPos,
				((int)partPtr[i].xPos + clouds[index].xPos + 18 - (int)partPtr[i].cell),
				((int)partPtr[i].yPos + clouds[index].yPos + 18 - (int)partPtr[i].cell));

			partPtr[i].xPos += (partPtr[i].speedX*tickTemp);
			partPtr[i].yPos += (partPtr[i].speedY*tickTemp);

			while(1)
			{
				HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
					&smokeFrms[(int)(partPtr[i].cell) + partPtr[i].animationSet],
					DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

				if ( ddrval == DD_OK)
					break;

				if (ddrval == DDERR_SURFACELOST)
				{
					ddrval = lpDD->RestoreAllSurfaces();
				}

				if (ddrval != DD_OK)
					break;
				if (ddrval != DDERR_WASSTILLDRAWING)
					break;
			}
		}
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								FIRE
//-------------------------------------------------------------------
void DrawFire (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	RECT dest;
	PARTICLE *partPtr = clouds[index].particles;

	for (int i = 0; i < clouds[index].noPart; i++)
	{
		if (clouds[index].particles[i].delayLeft > 0)
		{
			// all particles after should have a greater delay so just exit now
			clouds[index].particles[i].delayLeft -= tickTemp;
			break;
		}
		else
		{
			// increment at a fraction of the sppen of the acceleration
			partPtr[i].cell -= (tickTemp/partPtr[i].cellChange);
			if (partPtr[i].cell <= 0)
			{
				partPtr[i].cell = 16.999f;
				partPtr[i].xPos = 0;
				partPtr[i].yPos = 0;
			}

			SetRect(&dest, (int)partPtr[i].xPos + clouds[index].xPos, (int)partPtr[i].yPos + clouds[index].yPos,
				((int)partPtr[i].xPos + clouds[index].xPos + 18 - (int)partPtr[i].cell),
				((int)partPtr[i].yPos + clouds[index].yPos + 18 - (int)partPtr[i].cell));

			partPtr[i].xPos += (partPtr[i].speedX*tickTemp);
			partPtr[i].yPos += (partPtr[i].speedY*tickTemp);

			while(1)
			{
				HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
					&fireFrms[(int)(partPtr[i].cell) + partPtr[i].animationSet],
					DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

				if ( ddrval == DD_OK)
					break;

				if (ddrval == DDERR_SURFACELOST)
				{
					ddrval = lpDD->RestoreAllSurfaces();
				}

				if (ddrval != DD_OK)
					break;
				if (ddrval != DDERR_WASSTILLDRAWING)
					break;
			}
		}
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								SHRAPNEL
//-------------------------------------------------------------------
void DrawShrapnel (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	if (tickTemp > 10)
	{
		return;
	}

	RECT dest, tempSrc;
	PARTICLE *partPtr = clouds[index].particles;
	int cellNo, yWidth;
	
	// selects the different properties for different bit of metal
	switch(partPtr[0].animationSet)
	{
	case 1:
		cellNo = 36;
		yWidth = 12;
		tempSrc = shrapnel1Frms[(int)(partPtr[0].cell)];
		break;
	case 2:
		cellNo = 36;
		yWidth = 10;
		tempSrc = shrapnel2Frms[(int)(partPtr[0].cell)];
		break;
	case 3:
		cellNo = 26;
		yWidth = 10;
		tempSrc = shrapnel3Frms[(int)(partPtr[0].cell)];
		break;
	}

	//Deal with the actual piece of shrapnel first
	partPtr[0].cell += (tickTemp*2);
	if (partPtr[0].cell >= cellNo)
	{
		partPtr[0].cell = 0;
	}
	
	//update the shrapnel position on the screen
	partPtr[0].xPos += (partPtr[0].speedX*tickTemp);
	partPtr[0].yPos += (partPtr[0].speedY*tickTemp);

	SetRect(&dest, (int)partPtr[0].xPos + clouds[index].xPos - globalPosx, (int)partPtr[0].yPos + clouds[index].yPos,
				(int)partPtr[0].xPos + clouds[index].xPos - globalPosx + 10, (int)partPtr[0].yPos + clouds[index].yPos + yWidth);
	while(1)
	{
		HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
			&tempSrc, DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

		if ( ddrval == DD_OK)
			break;

		if (ddrval == DDERR_SURFACELOST)
		{
			ddrval = lpDD->RestoreAllSurfaces();
		}

		if (ddrval != DD_OK)
			break;
		if (ddrval != DDERR_WASSTILLDRAWING)
			break;
	}

	//Update the trail of smoke
	for (int i = 1; i < clouds[index].noPart; i++)
	{
		if (clouds[index].particles[i].delayLeft > 0)
		{
			// all particles after should have a greater delay so just exit now
			clouds[index].particles[i].delayLeft -= tickTemp;
			break;
		}
		else
		{
			// increment at a fraction of the speed of the acceleration
			partPtr[i].cell -= (tickTemp/partPtr[i].cellChange);
			if (partPtr[i].cell <= 0)
			{
				partPtr[i].cell = 16.999f;
				partPtr[i].xPos = partPtr[0].xPos;
				partPtr[i].yPos = partPtr[0].yPos;
				cellNo = i + 1;
			}

			// Set the destination on the display
			SetRect(&dest, (int)partPtr[i].xPos + clouds[index].xPos - globalPosx, (int)partPtr[i].yPos + clouds[index].yPos,
				((int)partPtr[i].xPos + clouds[index].xPos + 18 - (int)partPtr[i].cell - globalPosx),
				((int)partPtr[i].yPos + clouds[index].yPos + 18 - (int)partPtr[i].cell));

			//Test if the smoke is off the screen.
			if (i  == cellNo)
			{
				if (dest.left > 768 || dest.right < 0 || dest.top > 480 || dest.bottom < 0)
				{
					StopCloud(index);
					return;
				}
			}

			// Update the display for next time
			partPtr[i].xPos += (partPtr[i].speedX*tickTemp);
			partPtr[i].yPos += (partPtr[i].speedY*tickTemp);

			while(1)
			{
				HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
					&smokeFrms[(int)(partPtr[i].cell) + partPtr[i].animationSet],
					DDBLT_WAIT|DDBLT_KEYSRC, NULL);

				if ( ddrval == DD_OK)
					break;

				if (ddrval == DDERR_SURFACELOST)
				{
					ddrval = lpDD->RestoreAllSurfaces();
				}

				if (ddrval != DD_OK)
					break;
				if (ddrval != DDERR_WASSTILLDRAWING)
					break;
			}
		}
	}
}
//***********************************************************************


//-------------------------------------------------------------------
//								SMOKE TRAIL
//-------------------------------------------------------------------
void DrawSmokeTrail (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/5;
	if (tickTemp > 10)
	{
		return;
	}

	RECT dest;
	int cellNo;
	PARTICLE *partPtr = clouds[index].particles;

	//Update the trail of smoke
	for (int i = 0; i < clouds[index].noPart; i++)
	{
		if (clouds[index].particles[i].delayLeft > 0)
		{
			// all particles after should have a greater delay so just exit now
			clouds[index].particles[i].delayLeft -= tickTemp;
			break;
		}
		else
		{
			// increment at a fraction of the speed of the acceleration
			partPtr[i].cell -= (tickTemp/partPtr[i].cellChange);
			//keep the particles centered.
			partPtr[i].xPos -= (tickTemp/(partPtr[i].cellChange*2));
			if (partPtr[i].cell <= 0)
			{
				partPtr[i].cell = 16.999f;
				partPtr[i].xPos = (float)clouds[index].xPos;
				partPtr[i].yPos = (float)clouds[index].yPos;
				//the index of the last particle cloud
				cellNo = i + 1;
			}

			// Set the destination on the display
			SetRect(&dest, (int)partPtr[i].xPos - globalPosx, (int)partPtr[i].yPos,
				((int)partPtr[i].xPos + 18 - (int)partPtr[i].cell - globalPosx),
				((int)partPtr[i].yPos + 18 - (int)partPtr[i].cell));

			//Test if the smoke is off the screen.
			if (i  == cellNo)
			{
				if (dest.left > 768 || dest.right < 0 || dest.top > 480 || dest.bottom < 0)
				{
					StopCloud(index);
					return;
				}
			}

			// Update the display for next time
			partPtr[i].yPos += (partPtr[i].speedY*tickTemp);

			while(1)
			{
				HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
					&smokeFrms[(int)(partPtr[i].cell) + partPtr[i].animationSet],
					DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

				if ( ddrval == DD_OK)
					break;

				if (ddrval == DDERR_SURFACELOST)
				{
					ddrval = lpDD->RestoreAllSurfaces();
				}

				if (ddrval != DD_OK)
					break;
				if (ddrval != DDERR_WASSTILLDRAWING)
					break;
			}
		}
	}
}
//***********************************************************************



bool PixelPerfectCollision(int x1, int y1, int x2, int y2, int width, int height)
//-------------------------------------------------------------------
//	Refined collision detection
//	Need to send two top left co-ordiates for the source of the boxes being
//	being tested as well as the width and height (which must be the same)
//
//	Return Values - Returns true for a collision and false for no collision
//-------------------------------------------------------------------
{
	int i, j;
	USHORT box1, box2;
	DDSURFACEDESC2 ddsd;
	INIT_DXSTRUCT(ddsd);

// All of the code to draw individual pixels goes between the lock and unlock function calls
// Ps if these calls are removed the game slows down - go figure.
#ifdef _DEBUG_
	lpDDSOffscreen->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR | DDLOCK_NOSYSLOCK , NULL);
#else
	lpDDSOffscreen->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR , NULL);
#endif
	buffer = (USHORT*)ddsd.lpSurface;
	nPitch = (USHORT)ddsd.lPitch >> 1;
	
	USHORT transParent = GetPixel16(60, 98, buffer, nPitch);
	//loop through coresponding every second pixel in every second row (reduce time by 0.5) and
	// return a collision if the both corresponding pixels are not transparent.
	for (i = 0; i < height; i+=2)
	{
		for(j = 0; j < width; j+=2)
		{
			box1 = GetPixel16(x1 + j, y1 + i, buffer, nPitch);
			box2 = GetPixel16(x2 + j, y2 + i, buffer, nPitch);
			if (box1 != transParent && box2 != transParent)
			{
				lpDDSOffscreen->Unlock(NULL);			
				return true;
			}
		}
	}
	lpDDSOffscreen->Unlock(NULL);
	return false;
}
//***********************************************************************


/*void drawFire (int index, DWORD tickDiff)
{
	float tickTemp = (float)tickDiff/30;
	RECT dest;
	PARTICLE *partPtr = clouds[index].particles;

	for (int i = 0; i < clouds[index].noPart; i++)
	{
		if (clouds[index].particles[i].delayLeft > 0)
		{
			// all particles after should have a greater delay so just exit now
			clouds[index].particles[i].delayLeft -= tickTemp;
			break;
		}
		else
		{
			// increment at a fraction of the sppen of the acceleration
			partPtr[i].cell -= (tickTemp/partPtr[i].cellChange);
			if (partPtr[i].cell <= 0)
			{
				partPtr[i].cell = 16.999;
				partPtr[i].xPos = 0;
				partPtr[i].yPos = 0;
			}
			else if (partPtr[i].cell <= 6)
			{
				partPtr[i].yPos = partPtr[i].yPos;
			}

			SetRect(&dest, (int)partPtr[i].xPos + clouds[index].xPos, (int)partPtr[i].yPos + clouds[index].yPos,
				((int)partPtr[i].xPos + clouds[index].xPos + 18.0f - (int)partPtr[i].cell),
				((int)partPtr[i].yPos + clouds[index].yPos + 18.0f - (int)partPtr[i].cell));

			partPtr[i].xPos += (partPtr[i].speedX*tickTemp);
			partPtr[i].yPos += (partPtr[i].speedY*tickTemp);

			while(1)
			{
				HRESULT ddrval = lpDDSBack->Blt(&dest, lpDDSOffscreen,
					&smokeFrms[(int)(partPtr[i].cell) + partPtr[i].animationSet],
					DDBLT_WAIT|DDBLT_KEYSRC, NULL);

				if ( ddrval == DD_OK)
					break;

				if (ddrval == DDERR_SURFACELOST)
				{
					ddrval = lpDD->RestoreAllSurfaces();
				}

				if (ddrval != DD_OK)
					break;
				if (ddrval != DDERR_WASSTILLDRAWING)
					break;
			}
		}
	}
}
/*

void drawCollision (PARTICLE_CLOUD *ptrCloud)
{

	int screenY, screenX;

	for (int i = 1; i < ptrCloud->noPart; i++)
	{
		screenY = ptrCloud->yPos + ptrCloud->particles[i].yPos;//
		screenX = ptrCloud->xPos + ptrCloud->particles[i].xPos;//
		//debugOut(" scY", screenY, 480, 460);
		//debugOut(" scX", screenX, 480, 460);
		if ( screenY < SCREENSIZEY &&
			screenY > 0 &&
			screenX < SCREENSIZEX &&
			screenX > 0)
		{
			plotPixel16(screenX, screenY,
			RGB_16BIT565(ptrCloud->particles[i].red,
				ptrCloud->particles[i].green,
				ptrCloud->particles[i].blue),
			buffer, nPitch);

		}

		if ((ptrCloud->particles[i].height -= 10) <= 0)
		{
			ptrCloud->particles[i].yPos = ptrCloud->particles[i].starty;
			ptrCloud->particles[i].xPos = ptrCloud->particles[i].startx;
			ptrCloud->particles[i].blue = 0;
			ptrCloud->particles[i].height = ptrCloud->particles[i].width;
		}
		else
		{
			ptrCloud->particles[i].yPos += ptrCloud->particles[i].speedY;
			ptrCloud->particles[i].xPos += ptrCloud->particles[i].speedX;
			ptrCloud->particles[i].blue -= 5;
		}
	}
	if (0 == (ptrCloud->frmsLeft--))
	{
		ptrCloud->activated = false;
	}
}*/
