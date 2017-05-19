///////////////////////////////////////
// Contains the arrays of animation information
// used in the game and defined external in 
// the externals file file
///////////////////////////////////////

///////////////////////////////////////
// Include Files
///////////////////////////////////////
#include <windows.h>
#include "cdib.hpp"
#include "terrain.h"


//#ifdef _DEBUG_
HDC hDCbb;
char dbBuffer[40];
//#endif

//variables used to plot pixels for the particle explosions.
USHORT* buffer;
USHORT nPitch;

//DIB pointers.
CDib* pDib;
CDib* pDibBase;
CDib* pDibItems;
CDib* pDibStatus;

//Pointers to the tile map
SLICE *slicePtr;
TERRAIN_ITEM *itemPtr;

int numSlices;
int numItems;
int listLevel; //records the place to start in the item array
int globalPosx;
float globalPosy;

int bufferLeft;
int bufferGlobalx;
int bufferTop;
int bufferGlobaly;

float tilePosy;

SHIPTASK spaceShip;

bool togleKeyState[] = {false, false, false, false, false};
int wrongWayFlag;
int randTable[2000];
//int rndPos = 0; - not used?

DWORD prevParticleTC;

//Variables used in animation timing
DWORD globalTC, globalDiff;
float globalTL;

#ifdef _DEBUG_
int currentFPS = 0;
int FPSLoopCount = 0;		//Keeps a record of the loop cycles so that every 10 a new frme rate can be calculated.
DWORD FPSCount = 0;
#endif

PARTICLE_CLOUD clouds[55];


ITEM items[] = {
	//x,	y,		wid,	hei,	exp,	str	
	{0,		0,		22,		20,		0,		10},	//tree1 (0)
	{22,	0,		28,		28,		0,		10},	//tree2 (1)
	{50,	0,		26,		24,		0,		10},	//tree3 (2)
	{76,	0,		31,		26,		0,		10},	//tree4 (3)
	{107,	0,		28,		24,		0,		10},	//tree5 (4)
	{135,	0,		27,		21,		0,		10},	//tree6 (5)
	{162,	0,		17,		26,		0,		10},	//tree7 (6)
	{179,	0,		13,		15,		0,		10},	//tree8 (7)
	{193,	0,		17,		24,		0,		10},	//tree9 (8)
	{210,	0,		27,		17,		0,		10},	//tree10 (9)
	{237,	0,		19,		20,		0,		10},	//tree11 (10)
	{256,	0,		28,		23,		0,		10},	//tree12 (11)
	{284,	0,		26,		19,		0,		10},	//tree13 (12)
	{309,	0,		30,		32,		0,		5},		//dead tree14 (13)
	{339,	0,		19,		11,		0,		10},	//car1 (14)
	{339,	11,		17,		15,		0,		10},	//car2 (15)
	{358,	0,		9,		16,		0,		10},	//car3 (16)
	{358,	16,		9,		19,		0,		10},	//car4 (17)
	{367,	0,		10,		16,		0,		10},	//car5 (18)
	{367,	16,		17,		9,		0,		10},	//car6 (19)
	{367,	25,		18,		8,		0,		10},	//car7 (20)
	{377,	0,		16,		10,		0,		10},	//car8 (21)
	{384,	10,		24,		11,		0,		10},	//car9 (22)
	{385,	21,		19,		10,		0,		10},	//car10 (23)
	{408,	0,		18,		17,		0,		10},	//car11 (24)
	{408,	17,		10,		17,		0,		10},	//car12 (25)
	{426,	0,		18,		9,		0,		10},	//car13 (26)
	{426,	9,		16,		15,		0,		10},	//car14 (27)
	{418,	24,		8,		16,		0,		10},	//car15 (28)
	{426,	24,		10,		22,		0,		10},	//car16 (29)
	{444,	0,		9,		16,		0,		10},	//car17 (30)////
	{441,	16,		21,		9,		0,		10},	//car18 (31)
	{436,	25,		16,		14,		0,		10},	//car19 (32)
	{452,	25,		9,		15,		0,		10},	//car20 (33)
	{463,	0,		21,		20,		0,		10},	//car21 (34)
	{463,	20,		11,		15,		0,		10},	//Boat1 (35)
	{484,	0,		74,		22,		0,		10},	//Boat2 (36)
	{474,	22,		41,		36,		0,		10},	//Plane (37)
	{558,	0,		81,		23,		0,		10},	//hz Wooden jetty (38)
	{621,	23,		19,		78,		0,		10},	//vt Wooden jetty (39)
	{515,	23,		85,		10,		0,		10},	//hz fence 1 (40)
	{618,	23,		2,		82,		0,		10},	//vt fence 1 (41)
	{515,	33,		94,		10,		0,		10},	//hz fence 2 (42)
	{615,	23,		3,		93,		0,		10},	//vt fence 2 (43)
	{0,		25,		26,		25,		0,		10},	//fountain (44)
	{26,	28,		18,		24,		0,		10},	//sat dish (45)
	{44,	29,		15,		13,		0,		10},	//crate (46)
	{59,	26,		41,		25,		0,		10},	//pool (47)
	{100,	24,		38,		40,		0,		10},	//wind mill (48)
	{138,	21,		18,		22,		0,		10},	//water tank (49)
	{138,	43,		29,		23,		0,		10},	//helipad (50)
	{167,	26,		25,		38,		0,		10},	//tennis caught (51)
	{192,	24,		30,		30,		0,		10},	//gas cylinder (52)
	{222,	23,		85,		83,		0,		10},	//house1 (53)
	{307,	35,		90,		86,		0,		10},	//house2 (54)
	{397,	46,		77,		62,		0,		10},	//house3 (55)
	{474,	58,		54,		85,		0,		10},	//house4 (56)
	{528,	43,		77,		57,		0,		10},	//house5 (57)
	{528,	100,	65,		49,		0,		10},	//house6 (58)
	{0,		52,		80,		80,		0,		10},	//house7 (59)
	{80,	64,		42,		62,		0,		10},	//house8 (60)
	{122,	66,		24,		76,		0,		10},	//house9 (61)
	{146,	66,		52,		35,		0,		10},	//house10 (62)
	{146,	101,	56,		59,		0,		10},	//house11 (63)
	{202,	106,	32,		58,		0,		10},	//house12 (64)
	{234,	106,	44,		44,		0,		10},	//house13 (65)
	{278,	121,	43,		46,		0,		10},	//house14 (66)
	{321,	121,	57,		57,		0,		10},	//house15 (67)
	{0,		132,	111,	108,	0,		10},	//house16 (68)
	{111,	168,	66,		49,		0,		10},	//house17 (69)
	{378,	121,	78,		61,		0,		10},	//house18 (70)
	{456,	143,	52,		35,		0,		10},	//house19 (71)
	{508,	149,	31,		57,		0,		10},	//house20 (72)
	{539,	149,	86,		83,		0,		10},	//house21 (73)
	{177,	164,	52,		53,		0,		10},	//house22 (74)
	{229,	167,	74,		49,		0,		10},	//house23 (75)
	{303,	178,	51,		70,		0,		10},	//house24 (76)
	{354,	182,	51,		70,		0,		10},	//house25 (77)
	{405,	182,	78,		40,		0,		10},	//house26 (78)
	{405,	222,	78,		43,		0,		10},	//house27 (79)
	{483,	232,	84,		77,		0,		10},	//house28 (80)
	{0,		240,	81,		77,		0,		10},	//house29 (81)
	{81,	240,	69,		67,		0,		10},	//house30 (82)
	{150,	217,	70,		76,		0,		10},	//house31 (83)
	{220,	216,	69,		74,		0,		10},	//house32 (84)
	{289,	248,	72,		82,		0,		10},	//house33 (85)
	{361,	265,	78,		120,	0,		10},	//tower1 (86)
	{439,	309,	73,		148,	0,		10},	//tower2 (87)
	{512,	309,	68,		138,	0,		10},	//tower3 (88)
	{580,	232,	21,		128,	0,		10},	//vt bridge (89)
	{0,		317,	141,	27,		0,		10},	//hz bridge (90)
	{141,	307,	136,	83,		0,		10},	//building1 (91)
	{277,	330,	81,		133,	0,		10},	//building2 (92)	
	{141,	390,	135,	83,		0,		10},	//building3 (93)
	{358,	385,	37,		54,		0,		10},	//boat3 (94)
	{395,	384,	22,		32,		0,		10},	//vt roadpdl (95)
	{395,	416,	33,		24,		0,		10},	//hz roadpdl (96)
	{609,	116,	16,		32,		0,		10},	//up roadbkn (97)
	{593,	116,	16,		32,		0,		10},	//down roadbkn (98)
	{450,	277,	33,		16,		0,		10},	//rignt roadbkn (99)
	{450,	293,	33,		16,		0,		10},	//left roadbkn (100)
	{615,	363,	20,		57,		0,		10},	//puddles (101)
	{615,	336,	21,		26,		0,		10},	//small rocks (102)
	{580,	360,	18,		60,		0,		10},	//up drttrack (103)
	{598,	360,	17,		60,		0,		10},	//down drttrack (104)
	{580,	420,	60,		18,		0,		10},	//left drttrack (105)
	{580,	438,	60,		18,		0,		10},	//right drttrack (106)
	{0,		344,	47,		77,		0,		10},	//field1 (107)
	{47,	344,	47,		77,		0,		10}		//field2 (108)
};

CELL cells[] = {
	 //x,   y  upper left corner of the tile
     {0,	0},//tile type 0
	 {0,	0},//tile type 1
	 {32,	0},//tile type 2
	 {64,	0},//tile type 3
	 {96,	0},//tile type 4
	 {128,	0},//tile type 5
	 {160,	0},//tile type 6
	 {192,	0},//tile type 7
	 {224,	0},//tile type 8
	 {256,	0},//tile type 9
	 {288,	0},//tile type 10
	 {320,	0},//tile type 11
	 {352,	0},//tile type 12
	 {384,	0},//tile type 13
	 {416,	0},//tile type 14
	 {448,	0},//tile type 15
	 {480,	0},//tile type 16
	 {512,	0},//tile type 17
	 {544,	0},//tile type 18
	 {576,	0},//tile type 19
	 {608,	0},//tile type 20
	 {0,	32},//tile type 21
	 {32,	32},//tile type 22
	 {64,	32},//tile type 23
	 {96,	32},//tile type 24
	 {128,	32},//tile type 25
	 {160,	32},//tile type 26
	 {192,	32},//tile type 27
	 {224,	32},//tile type 28
	 {256,	32},//tile type 29
	 {288,	32},//tile type 30
	 {320,	32},//tile type 31
	 {352,	32},//tile type 32
	 {384,	32},//tile type 33
	 {416,	32},//tile type 34
	 {448,	32},//tile type 35
	 {480,	32},//tile type 36
	 {512,	32},//tile type 37
	 {544,	32},//tile type 38
	 {576,	32},//tile type 39
	 {608,	32},//tile type 40
	 {0,	64},//tile type 41
	 {32,	64},//tile type 42
	 {64,	64},//tile type 43
	 {96,	64},//tile type 44
	 {128,	64},//tile type 45
	 {160,	64},//tile type 46
	 {192,	64},//tile type 47
	 {224,	64},//tile type 48
	 {256,	64},//tile type 49
	 {288,	64},//tile type 50
	 {320,	64},//tile type 51
	 {352,	64},//tile type 52
	 {384,	64},//tile type 53
	 {416,	64},//tile type 54
	 {448,	64},//tile type 55
	 {480,	64},//tile type 56
	 {512,	64},//tile type 57
	 {544,	64},//tile type 58
	 {576,	64},//tile type 59
	 {608,	64},//tile type 60
	 {0,	96},//tile type 61
	 {32,	96},//tile type 62
	 {64,	96},//tile type 63
	 {96,	96},//tile type 64
	 {128,	96},//tile type 65
	 {160,	96},//tile type 66
	 {192,	96},//tile type 67
	 {224,	96},//tile type 68
	 {256,	96},//tile type 69
	 {288,	96},//tile type 70
	 {320,	96},//tile type 71
	 {352,	96},//tile type 72
	 {384,	96},//tile type 73
	 {416,	96},//tile type 74
	 {448,	96},//tile type 75
	 {480,	96},//tile type 76
	 {512,	96},//tile type 77
	 {544,	96},//tile type 78
	 {576,	96},//tile type 79
	 {608,	96},//tile type 80
	 {0,	128},//tile type 81
	 {32,	128},//tile type 82
	 {64,	128},//tile type 83
	 {96,	128},//tile type 84
	 {128,	128},//tile type 85
	 {160,	128},//tile type 86
	 {192,	128},//tile type 87
	 {224,	128},//tile type 88
	 {256,	128},//tile type 89
	 {288,	128},//tile type 90
	 {320,	128},//tile type 91
	 {352,	128},//tile type 92
	 {384,	128},//tile type 93
	 {416,	128},//tile type 94
	 {448,	128},//tile type 95
	 {480,	128},//tile type 96
	 {512,	128},//tile type 97
	 {544,	128},//tile type 98
	 {576,	128},//tile type 99
	 {608,	128},//tile type 100
	 {0,	160},//tile type 101
	 {32,	160},//tile type 102
	 {64,	160},//tile type 103
	 {96,	160},//tile type 104
	 {128,	160},//tile type 105
	 {160,	160},//tile type 106
	 {192,	160},//tile type 107
	 {224,	160},//tile type 108
	 {256,	160},//tile type 109
	 {288,	160},//tile type 110
	 {320,	160},//tile type 111
	 {352,	160},//tile type 112
	 {384,	160},//tile type 113
	 {416,	160},//tile type 114
	 {448,	160},//tile type 115
	 {480,	160},//tile type 116
	 {512,	160},//tile type 117
	 {544,	160},//tile type 118
	 {576,	160},//tile type 119
	 {608,	160},//tile type 120
	 {0,	192},//tile type 121
	 {32,	192},//tile type 122
	 {64,	192},//tile type 123
	 {96,	192},//tile type 124
	 {128,	192},//tile type 125
	 {160,	192},//tile type 126
	 {192,	192},//tile type 127
	 {224,	192},//tile type 128
	 {256,	192},//tile type 129
	 {288,	192},//tile type 130
	 {320,	192},//tile type 131
	 {352,	192},//tile type 132
	 {384,	192},//tile type 133
	 {416,	192},//tile type 134
	 {448,	192},//tile type 135
	 {480,	192},//tile type 136
	 {512,	192},//tile type 137
	 {544,	192},//tile type 138
	 {576,	192},//tile type 139
	 {608,	192},//tile type 140
	 {0,	224},//tile type 141
	 {32,	224},//tile type 142
	 {64,	224},//tile type 143
	 {96,	224},//tile type 144
	 {128,	224},//tile type 145
	 {160,	224},//tile type 146
	 {192,	224},//tile type 147
	 {224,	224},//tile type 148
	 {256,	224},//tile type 149
	 {288,	224},//tile type 150
	 {320,	224},//tile type 151
	 {352,	224},//tile type 152
	 {384,	224},//tile type 153
	 {416,	224},//tile type 154
	 {448,	224},//tile type 155
	 {480,	224},//tile type 156
	 {512,	224},//tile type 157
	 {544,	224},//tile type 158
	 {576,	224},//tile type 159
	 {608,	224},//tile type 160
	 {0,	256},//tile type 161
	 {32,	256},//tile type 162
	 {64,	256},//tile type 163
	 {96,	256},//tile type 164
	 {128,	256},//tile type 165
	 {160,	256},//tile type 166
	 {192,	256},//tile type 167
	 {224,	256},//tile type 168
	 {256,	256},//tile type 169
	 {288,	256},//tile type 170
	 {320,	256},//tile type 171
	 {352,	256},//tile type 172
	 {384,	256},//tile type 173
	 {416,	256},//tile type 174
	 {448,	256},//tile type 175
	 {480,	256},//tile type 176
	 {512,	256},//tile type 177
	 {544,	256},//tile type 178
	 {576,	256},//tile type 179
	 {608,	256},//tile type 180
	 {0,	288},//tile type 181
	 {32,	288},//tile type 182
	 {64,	288},//tile type 183
	 {96,	288},//tile type 184
	 {128,	288},//tile type 185
	 {160,	288},//tile type 186
	 {192,	288},//tile type 187
	 {224,	288},//tile type 188
	 {256,	288},//tile type 189
	 {288,	288},//tile type 190
	 {320,	288},//tile type 191
	 {352,	288},//tile type 192
	 {384,	288},//tile type 193
	 {416,	288},//tile type 194
	 {448,	288},//tile type 195
	 {480,	288},//tile type 196
	 {512,	288},//tile type 197
	 {544,	288},//tile type 198
	 {576,	288},//tile type 199
	 {608,	288},//tile type 200
	 {0,	320},//tile type 201
	 {32,	320},//tile type 202
	 {64,	320},//tile type 203
	 {96,	320},//tile type 204
	 {128,	320},//tile type 205
	 {160,	320},//tile type 206
	 {192,	320},//tile type 207
	 {224,	320},//tile type 208
	 {256,	320},//tile type 209
	 {288,	320},//tile type 210
	 {320,	320},//tile type 211
	 {352,	320},//tile type 212
	 {384,	320},//tile type 213
	 {416,	320},//tile type 214
	 {448,	320},//tile type 215
	 {480,	320},//tile type 216
	 {512,	320},//tile type 217
	 {544,	320},//tile type 218
	 {576,	320},//tile type 219
	 {608,	320},//tile type 220
	 {0,	352},//tile type 221
	 {32,	352},//tile type 222
	 {64,	352},//tile type 223
	 {96,	352},//tile type 224
	 {128,	352},//tile type 225
	 {160,	352},//tile type 226
	 {192,	352},//tile type 227
	 {224,	352},//tile type 228
	 {256,	352},//tile type 229
	 {288,	352},//tile type 230
	 {320,	352},//tile type 231
	 {352,	352},//tile type 232
	 {384,	352},//tile type 233
	 {416,	352},//tile type 234
	 {448,	352},//tile type 235
	 {480,	352},//tile type 236
	 {512,	352},//tile type 237
	 {544,	352},//tile type 238
	 {576,	352},//tile type 239
	 {608,	352} //tile type 240
 };