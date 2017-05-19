///////////////////////////////////////
// Contains the arrays of animation information
// used in the game and defined external in 
// the Externals file
///////////////////////////////////////

///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <windows.h>
#include "cdib.hpp"


RECT rotatingRightRect[] = {
	{0,		0,		31,		63},
	{64,	0,		95,		63},
	{128,	0,		159,	63},
	{160,	0,		191,	63},
	{192,	0,		223,	63},
	{224,	0,		255,	63},
	{256,	0,		287,	63},
	{288,	0,		319,	63},
	{320,	0,		351,	63},
	{352,	0,		383,	63},
	{384,	0,		415,	63},
	{416,	0,		447,	63},
	{448,	0,		479,	63},
	{480,	0,		511,	63},
	{512,	0,		543,	63},
	{576,	0,		607,	63}
};

RECT rotatingLeftRect[] = {
	{0,		0,		31,		63},
	{576,	0,		607,	63},
	{512,	0,		543,	63},
	{480,	0,		511,	63},
	{448,	0,		479,	63},
	{416,	0,		447,	63},
	{384,	0,		415,	63},
	{352,	0,		383,	63},
	{320,	0,		351,	63},
	{288,	0,		319,	63},
	{256,	0,		287,	63},
	{224,	0,		255,	63},
	{192,	0,		223,	63},
	{160,	0,		191,	63},
	{128,	0,		159,	63},
	{64,	0,		95,		63}
};

RECT movingRightRect[] = {
	{0,		0,		31,		63},
	{32,	0,		63,		63},
	{64,	0,		95,		63},
	{96,	0,		127,	63},
	{128,	0,		159,	63},
	{160,	0,		191,	63}
};

RECT movingLeftRect[] = {
	{0,		0,		31,		63},
	{608,	0,		639,	63},
	{576,	0,		607,	63},
	{544,	0,		574,	63},
	{512,	0,		543,	63},
	{480,	0,		511,	63}
};

RECT noLRMovementRect [] ={
	{0,		0,		31,		63}
};
