#include "digitalcounter.h"

	RECT numberPic[] = {
	//	  t		  l		  b		  r
		{116,	223,	125,	239},
		{106,	223,	115,	239},
		{16,	223,	25,		239},
		{26,	223,	35,		239},
		{36,	223,	45,		239},
		{46,	223,	55,		239},
		{56,	223,	65,		239},
		{66,	223,	75,		239},
		{76,	223,	85,		239},
		{86,	223,	95,		239},
		{96,	223,	105,	239}
	};

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//								Member Function Inplimentations

DigitalCounter::DigitalCounter(int xPos, int yPos, int digits, int startSize)
	:digit(0, 0, 0, 0, false)
//-------------------------------------------------------------------
// Object constructor
//-------------------------------------------------------------------
{
	bounds.left = xPos;
	bounds.right = bounds.left + 10*digits;
	bounds.top = yPos;
	bounds.bottom = yPos + 16;
	maxDigits = digits;
	score = startSize;

	return;
}
//*******************************************************************


DigitalCounter::~DigitalCounter()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************

int DigitalCounter::ConvertToIndex(char chr)
//-------------------------------------------------------------------
// Converts the character to the index in the array where the 
// character is stored.
//-------------------------------------------------------------------
{	
	if (chr >= '0' && chr <= '9')
	{
		return (int)chr - 47;
	}
	return 0;
}
//***********************************************************************


void DigitalCounter::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
// Redraws the button by calling the draw routine of the appropriate
// member.
//-------------------------------------------------------------------
{
	
	RECT tempRect;
	int startOffset;
	int i, j;
	
	//converts the number to a string
	j = sprintf( textScore, "%d", score );
	startOffset = (maxDigits - j)*10;
	// The width and height of the numbers doesnt change
	digit.initalWidth = 9;
	digit.initalHeight = 16;

	for (i = 0; textScore[i] != '\0'; ++i)
	{
		tempRect = numberPic[ConvertToIndex(textScore[i])];

		//Set the members of cursor that need setting
		digit.src = tempRect;
			
		SetRect(&tempRect, bounds.left + startOffset + (i * 10), bounds.top,
			bounds.left + 9 + startOffset + (i * 10), bounds.bottom);
		digit.SetPos(tempRect); 
		digit.Draw(lpDDS_Dest, lpDDS_Source, 0);
	}
}
//***********************************************************************

