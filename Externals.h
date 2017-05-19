///////////////////////////////////////
// Global and external constants used by the game
#define _DEBUG_
#ifndef EXTERNALS_H_
#define EXTERNALS_H_


///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <windows.h>
#include <ddraw.h>
#include "cdib.hpp"

#include "key.h"

//#ifdef _DEBUG_
#include <stdio.h>
#include <string.h>
extern HDC hDCbb;
extern char dbBuffer[40];


//#endif

///////////////////////////////////////
// Defintions
///////////////////////////////////////

//deffinitions for the in game screen bounds
#define LEFT_BOUNDS 1		//As far a the ship can go on the view port
#define RIGHT_BOUNDS 448
#define UPPER_BOUNDS 1
#define LOWER_BOUNDS 415
#define GAME_PORT_HEIGHT 15	//The max cells in the relevent direction
#define GAME_PORT_WIDTH 15
#define SCREENSIZEX 480		//The size in pixels of the main view port
#define SCREENSIZEY 480
#define MAXGLOBALPOSX 288	//The width of the level 288
//#define SPRITE_DELAY 60

//deffinitions for the terrainbuffer surface
#define BUFFERSIZEX 512
#define BUFFERSIZEY 512
#define BUFFERSIZE 16
//The offsets for the jets of flame behind the ship
#define HORIZONTAL_FLAME_OFFSET 15
#define VERTICLE_FLAME_OFFSET 77

//Constants for the different types of ship animations
#define MAX_STOP_CELLS 0	// 1 cells of stop animation
#define MAX_ROTATE_CELLS 15 // 16 cells of walking animation
#define MAX_MOVE_CELLS 5	// 6 cells of normal movement animation


//Base Cell definitions
#define INFRONT     2       // This cell should overlay the character.
#define INBACK      1       // The character should overlay this cell.

#define BCELLSIZE   32      // Size of the cells plus black border.
#define CELLSIZE    32      // Size of the cells.
#define NOLEVEL     255     // Cell has no level information. (merely for show)
#define ITEMSIZE    32      // Size of items
#define BITEMSIZE   32      // Size of the items plus black border.

//#define NUMSLICES   39      // Map is divided into this # of slices
#define MAXCONTENTS 24		// Maximum possible # of cells in one slice.
#define NUMCONTENTS 24		// Max number of cells used in a slice.
#define NUMCELLS    240     // Current # of defined cells

#define MAXENEMIES 1
#define CLOUD_NO 20
///////////////////////////////////////
// extern global variables
///////////////////////////////////////


//the animations of the main ship
extern RECT rotatingRightRect[];
extern RECT rotatingLeftRect[];
extern RECT noLRMovementRect[];
extern RECT movingRightRect[];
extern RECT unmovingRightRect[];
extern RECT movingLeftRect[];
extern RECT unmovingLeftRect[];

typedef struct {
	 int posx;
	 int posy;
} CELL;

typedef struct {
	int srcX;
	int srcY;
	int width;
	int height;
	int expRef; //the tile referance for the exploded version of the item
	int strength;  //the items starting strength
} ITEM;

///////////////////////////////////////
// Terrain Map Slices
///////////////////////////////////////
typedef struct {
    int     base;
	int		level1;
	int		level2;
	int		level3;
	int		level4;
	//int		air1;
	//int		air2;
	//int		airSpeedX;
	//int		airSpeedY;
} CONTENT;

typedef struct {
    CONTENT contents[NUMCONTENTS];  // Cell types in this slice of nirvana.
} SLICE;

typedef struct {
	int gblPosy;
	int gblPosx;
	int itemType;
	int moveX;		//Used if the item moves
	int moveY;		//Used if the item moves in the Y direction.
	float damage;	//Every item has a damage level if it is greater than 0
					//it is still drawn and tested for collision however if
					//it is -1f then the item is indestructable.
} TERRAIN_ITEM;


extern CELL cells[];

extern ITEM items[];

///////////////////////////////////////
// Terrain Map Slices
///////////////////////////////////////

//Used to help load and handle all the bitmaps
extern CDib* pDib;

extern SLICE   *slicePtr;
extern TERRAIN_ITEM *itemPtr;
extern int numSlices;
extern int numItems;
extern int listLevel;

//Define the global position in the level ie not in relation to the screen
extern int globalPosx;
extern float globalPosy;

//Define globals for keeping track of the terrain buffer.
extern int bufferLeft;
extern int bufferGlobalx; //The lowest global x value stored including the buffer
extern int bufferTop;
extern int bufferGlobaly;

extern float tilePosy;

//Create an enumeration variable to help identif the different annimation sequences
enum {ROTATE_RIGHT, ROTATE_LEFT, LEFT, RIGHT, NO_MOVE, ROTATE};
enum {WAS_RIGHT, WAS_LEFT};
extern int wrongWayFlag;

//Define a ship structure that enables the different ship properties to be kept track of
typedef struct	{		
	//int posx;			//the x offset to the left of the sprite on the screen 
	//int posy;			//the y offset as above
	//int width;			//the width of the sprite
	//int height;			//the height of the sprite
	RECT dest;
	RECT src;			//updated when the ship is drawan and used for pixel collision
	int health;			//the strength of the sprite
	int healthMax;		//the maximum health
	int currentCell;	//the current cell in the current animation sequence
	int motion;			//an enum type (above) that id's the different animation sequences
	DWORD delay;		//the current delay (milliseconds) for the current animation sequence
	DWORD moveDelay;	//the delay for the spaceships movement not frames.
	DWORD moveTickCount;
	DWORD lastTickCount;//a variable used to determine wether the delay has passed so the next animation cell can be drawn
	int flameIndex;
}SHIPTASK;


//Declare a variable of type SHIPTASK for the main space ship
extern SHIPTASK spaceShip;

extern bool togleKeyState[5];
extern int randTable[2000];
//extern int rndPos; - not used? But intended to e used as an index into the Random table

extern USHORT* buffer;
extern USHORT nPitch;


typedef struct {
	float xPos;
	float yPos;
	float speedX;	//The distance traveled in each incrimentation
	float speedY;
	int height;	//How far in y dir it can travel
	int width;	//How far in x dir it can travel
	float cell;
	float cellChange;
	float delay;
	float delayLeft;
	int animationSet;
	int startx;
	int starty;
}PARTICLE;

typedef struct {
	bool activated;	//whether the cloud is seen
	int type;		//type of cloud rendered
	float timeLeft;
	float delayLeft;
	int xPos;
	int width;
	int yPos;
	int height;
	int speedY;		//The speed in the y dir + = towards bottom - = towards top
	int spConv;		//The speed in the x dir + means converge while -1 is divergance
	int spXRand;	//Speed variation
	int spYRand;
	int noPart;
	bool xGlobal;	//If true then use global chords to set the position
	bool yGlobal;	//If true then use global chords to set the position
	PARTICLE *particles;
} PARTICLE_CLOUD;

extern DWORD prevParticleTC;	//used for the timing of particle updates
extern DWORD globalTC, globalDiff;  //used for all other animation timing
extern float globalTL;			//The time left in the current animation/ or time passed	

#ifdef _DEBUG_
extern int currentFPS;
extern int FPSLoopCount;		//Keeps a record of the loop cycles so that every 10 a new frme rate can be calculated.
extern DWORD FPSCount;
#endif

extern PARTICLE_CLOUD clouds[55];


#endif