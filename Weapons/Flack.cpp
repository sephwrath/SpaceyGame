// Spread.cpp: implementation of the Spread class.
//
//////////////////////////////////////////////////////////////////////

#include "Flack.h"


//Spread* weaponSpreadPtr;

	RECT flackFrms[] = {
	//Spread bullet type frame definitions.
	//	left	top		right	bottom	width	height	xoff	yoff
		{440,	85,		449,	90},//Left angled bullets
		{433,	80,		442,	85},
		{442,	80,		449,	85},
		{449,	80,		454,	88},
		{455,	80,		459,	89},
		
		{459,	80,		462,	89},//The middle bullet
		
		{466,	80,		470,	89},
		{471,	80,		476,	88},
		{476,	80,		483,	85},
		{483,	80,		492,	85},
		{476,	85,		485,	90}//Right angled bullets
	};

	

Flack::Flack()
//-------------------------------------------------------------------
// Construction
//-------------------------------------------------------------------
{
	strength = 0;
	state = 0;
	activated = false;
}
//*******************************************************************


Flack::~Flack()
//-------------------------------------------------------------------
//Destruction
//-------------------------------------------------------------------
{

}
//*******************************************************************


void Flack::ActivateItem()
//-------------------------------------------------------------------
//Effectively turns the weapon on sets the hitbox and activated to true
//-------------------------------------------------------------------
{
	// if there is not enough power to fire the weapon then return
	if (!eventPanel->TestPower())
	{
		this->DeactivateItem();
		return;
	}
	if(activated != true)
	{
		activated = true;
		state = 0;
		strength = 7;
		shootDelay = 0;
	}
	if (shootDelay < 2)
	{
		shootDelay += ((float)(globalDiff)/50);
	}
	else
	{
		shootDelay = 0;
		PlayWAVInfoEx(lpwiBullet, 0, -700, 0L, 22050);
		switch ((int)strength)
		{
		case 0:
			bulletPtr = new SP_BULLET;//(0, -5, , , 3, 9);
			bulletPtr->velocityX = 0;
			bulletPtr->velocityY = -2;
			bulletPtr->fX = (float)spaceShip.dest.left + 15 + globalPosx;
			bulletPtr->fY = (float)spaceShip.dest.top + 5;
			bulletPtr->xWidth = 3;
			bulletPtr->yWidth = 9;
			bulletPtr->src = flackFrms[5];

			bulletList.InsertListBack(bulletPtr);
			eventPanel->SetUpdate(1, -0.5f);
			break;

		default:
			break;
		}
	}
}
//*******************************************************************


void Flack::DeactivateItem()
//-------------------------------------------------------------------
//Effectively turns the weapon on sets the hitbox and activated to true
//-------------------------------------------------------------------
{
	strength = 0;
	activated = false;
}
//*******************************************************************


float Flack::TestCollision(RECT &test)
//-------------------------------------------------------------------
//Tests for a collision between the hitbox and the test rect, if there is a
//collision the top of the hitbox is shortened to the base of the collision
//-------------------------------------------------------------------
{
	if (!bulletList.SetPosTail())
		return 0;
	bulletPtr = (SP_BULLET*)bulletList.GetCurrentItem();

	do
	{
		///////////////Dest hits the left of Item1
		//*-->|---
		//|   | Item1
		//+---*
		//|
		if (bulletPtr->dest.right > test.left && bulletPtr->dest.right < test.right)
		{
			///////////Weapon hits the bottom of Item1
			//*---+--
			//|  Item1
			//V   | 
			//----*
			//|
			if (bulletPtr->dest.top < test.bottom && bulletPtr->dest.top > test.top)
			{
				if (bulletList.RemoveListDec() != 0)
				{
					bulletPtr = 0;
					return 0.7f;
				}
				else
				{
					bulletPtr = 0;
					return 0.7f;
				}
			}
		}
		///////////////Item2 hits the right of Item1
		//     -|<--*
		//Item1 |   |
		//      *---+-
		//          |
		else if (bulletPtr->dest.left < test.right && bulletPtr->dest.right > test.right)
		{
			///////////Weapon hits the bottom of Item1
			//*---+--
			//|  Item1
			//V   | 
			//----*
			//|
			if (bulletPtr->dest.top < test.bottom && bulletPtr->dest.top > test.top)
			{
				if (bulletList.RemoveListDec() != 0)
				{
					bulletPtr = 0;
					return 0.7f;
				}
				else
				{
					bulletPtr = 0;
					return 0.7f;
				}
			}
		}
	}while (bulletPtr = (SP_BULLET*)bulletList.GetPrev());
	return 0;
}
//*******************************************************************


void Flack::Draw(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
//Loops through drawing and updating the bullets
//-------------------------------------------------------------------
{
	float tickTemp = (float)globalDiff/5;

	HRESULT ddrval;

	if (!bulletList.SetPosTail())
		return;
	bulletPtr = (SP_BULLET*)bulletList.GetCurrentItem();

	do
	{	
		bulletPtr->fX += bulletPtr->velocityX*tickTemp;
		bulletPtr->fY += bulletPtr->velocityY*tickTemp;

		if (bulletPtr->fY < -9 || bulletPtr->fX > 768 || bulletPtr->fX < -9)
		{
			if (bulletList.RemoveListDec() != 0)
			{
				bulletPtr = 0;
				continue;
			}
			else
			{
				bulletPtr = 0;
				break;
			}
		}

		SetRect(&bulletPtr->dest, (int)bulletPtr->fX - globalPosx, (int)bulletPtr->fY,
			(int)bulletPtr->fX + bulletPtr->xWidth - globalPosx, (int)bulletPtr->fY + bulletPtr->yWidth);

		while(1)
		{
			ddrval = lpDDS_Dest->Blt(&bulletPtr->dest, lpDDS_Source, &bulletPtr->src, DDBLT_KEYSRC, NULL);//DDBLT_WAIT|

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
	}while (bulletPtr = (SP_BULLET*)bulletList.GetPrev());
}
//*******************************************************************