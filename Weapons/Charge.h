// Spread.h: interface for the Spread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARGE_H__18930B8C_E888_4737_820A_BA1BA2125B88__INCLUDED_)
#define AFX_CHARGE_H__18930B8C_E888_4737_820A_BA1BA2125B88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//Includes and all that jazz
#include "Weapon.h"

#ifndef SP_BULLET_STRUCT
#define SP_BULLET_STRUCT
typedef struct {
		float velocityX;
		float velocityY;
		float fX;
		float fY;
		int xWidth;
		int yWidth;

		RECT src;
		RECT dest;
	} SP_BULLET;
#endif

class Charge : public Weapon
{
private:

	float shootDelay;

	cList bulletList;
	SP_BULLET *bulletPtr;

public:
	Charge();
	virtual ~Charge();
	//Effectively turns the weapon on sets the hitbox and activated to true
	void ActivateItem();
	void DeactivateItem();

	//Inspector functions
	RECT &GetHitBox(){return hitBox;}
	int GetLeft(){return hitBox.left;}
	int GetTop(){return hitBox.top;}
	bool IsAcive() {return activated;}
	
	//Tests for a collision between the hitbox and the test rect, if there is a
	//collision the top of the hitbox is shortened to the base of the collision
	float TestCollision(RECT &test);

	//Uses tiling for the laser to fill in the hit box.
	void Draw(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source);

};

//Declare a variable that can be used in the game
//extern Spread* weaponSpreadPtr;

#endif // !defined(AFX_CHARGE_H__18930B8C_E888_4737_820A_BA1BA2125B88__INCLUDED_)
