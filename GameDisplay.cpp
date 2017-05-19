#include "Interface\GameDisplay.h"

GameDisplay *eventPanel;

RECT weaponIcon[] = {
	{146,	380,	196,	423}, //laser
	{146,	423,	196,	466}, //spread
	{146,	466,	196,	509}, //missile
	{146,	509,	196,	552}, //homing missile
	{146,	552,	196,	595}  //wave
};

RECT weaponStateIcon[] = {
	{61,	456,	111,	499}, //off
	{146,	337,	196,	380}, //air
	{146,	293,	196,	336}  //ground
};

RECT weaponBoxPos[] = {
	{557,	163,	607,	206}, //slot 0
	{557,	225,	607,	268},
	{557,	287,	607,	330},
	{557,	349,	607,	392},
	{557,	411,	607,	454}  //slot 4
};


GameDisplay::GameDisplay()
//-------------------------------------------------------------------
//Constructor
//-------------------------------------------------------------------
{
	// Set up all the starting vales for the bars etc
	RECT TempBound, TempBack, TempBar;
	SetRect(&TempBound, 502, 160, 507, 452);
	SetRect(&TempBack, 6, 453, 11, 745);
	SetRect(&TempBar, 131, 293, 136, 585);
	shipHealth = new ProgressBar(TempBound, TempBack, TempBar, 100, 0, 100);
	TempBack.left  = 21;
	TempBack.right = 26;
	TempBound.left = 517;
	TempBound.right = 522;
	TempBar.left = 136;
	TempBar.right = 141;
	shipPower = new ProgressBar(TempBound, TempBack, TempBar, 1000, 0, 1000);
	scoreDisplay = new DigitalCounter(509, 8, 11, 0);

	weaponState = new Drawable(61, 456, 111, 499, false);
	weaponType = new Drawable(146, 380, 196, 423, false);

	//A hack for the purposes of testing
	WeaponSelectUpdate(0, W_EMPTY);
	WeaponSelectUpdate(1, W_EMPTY);
	WeaponSelectUpdate(2, W_EMPTY);
	WeaponSelectUpdate(3, W_EMPTY);
	WeaponSelectUpdate(4, W_EMPTY);

	WeaponStateUpdate(0, W_OFF);
	WeaponStateUpdate(1, W_OFF);
	WeaponStateUpdate(2, W_OFF);
	WeaponStateUpdate(3, W_OFF);
	WeaponStateUpdate(4, W_OFF);

	return;
}
/**********************************************************************/


GameDisplay::~GameDisplay()
//-------------------------------------------------------------------
//	Destructor
//-------------------------------------------------------------------
{
	delete shipHealth;
	delete shipPower;
	delete scoreDisplay;
	delete weaponState;
	delete weaponType;
}
/**********************************************************************/


void GameDisplay::Update(void)
//-------------------------------------------------------------------
//	updates the in game display if one of the update flags is set
//	otherwise nothing has changed so ther is no need for an update
//-------------------------------------------------------------------
{
	//only update the components if something has changed.
	if (backUpdate == true || frontUpdate == true)
	{
		//check if there have been any changes and update the display
		shipHealth->Update(lpDDSBack, lpDDSOffscreen);
		shipPower->Update(lpDDSBack, lpDDSOffscreen);
		scoreDisplay->Update(lpDDSBack, lpDDSOffscreen);

		// for each weapon box draw the boxes weapon icon and its hilight state
		for (int i =0; i < 5; ++i)
		{
			if (iconDraw[i] != W_EMPTY)
			{
				weaponType->SetPos(weaponBoxPos[i]);
				weaponState->SetPos(weaponBoxPos[i]);

				weaponType->SetSource(weaponIcon[iconDraw[i]]);
				weaponState->SetSource(weaponStateIcon[hilightDraw[i]]);
			
				// if W_OFF then draw the hilight first because otherwise it covers the icon.
				if (hilightDraw[i] == W_OFF)
				{
					weaponState->Draw(lpDDSBack, lpDDSOffscreen, 0);
					weaponType->Draw(lpDDSBack, lpDDSOffscreen, 0);
				}
				else
				{
					weaponType->Draw(lpDDSBack, lpDDSOffscreen, 0);
					weaponState->Draw(lpDDSBack, lpDDSOffscreen, 0);
				}
			}
		}

		//reset both variables once both buffers have been updated.
		if (frontUpdate == true)
		{
			frontUpdate = false;
		}
		else if (backUpdate == true)
		{
			backUpdate = false;
		}
	}
}
/**********************************************************************/


void GameDisplay::WeaponStateUpdate(int box, WEAPON_STATE state)
//-------------------------------------------------------------------
//  Sets the border that is displayed around the weapon depening on the state
//-------------------------------------------------------------------
{
	hilightDraw[box] = state;

	// because this happens in the middle of the action the display needs to be updated.
	backUpdate = true;
	frontUpdate = true;
	return;
}
/**********************************************************************/


int GameDisplay::WeaponSelectUpdate(int box, WEAPON_ID type)
//-------------------------------------------------------------------
//  Sets the icon that is displayed in the box according to the weapon type
//	returns 0 if the weapon is already in another box and 1 if sucessfull
//-------------------------------------------------------------------
{
	backUpdate = true;
	frontUpdate = true;

	// so far this function is only partially implimented so 1 is always returned
	iconDraw[box] = type;
	return 1;
}
/**********************************************************************/


int GameDisplay::SetUpdate(int bar, float adjustment)
//-------------------------------------------------------------------
// change the different controlls by adjustment ammount; bar reffers to the
// controll in question
//-------------------------------------------------------------------
{
	backUpdate = true;
	frontUpdate = true;
	switch(bar)
	{
	case 0:
		return shipHealth->ChangeLevel(adjustment);
	case 1:
		return shipPower->ChangeLevel(adjustment);
	case 2:
		return 0;
	case 3:
		scoreDisplay->ChangeNumber((int)adjustment);
		return 1;
	case 4:
		return 0;
	default:
		break;
	}
	return 0;
}
/**********************************************************************/


void GameDisplay::Setup(LPDIRECTDRAWSURFACE7 lpDDS_Back,
		LPDIRECTDRAWSURFACE7 lpDDS_Offscreen,
		LPDIRECTDRAWSURFACE7 lpDDS_Primary,
		LPDIRECTDRAW7 lpDDraw,
		HWND hwindow)
//-------------------------------------------------------------------
//Get the surface pointers so that they can be accessed later.
//-------------------------------------------------------------------
{
	lpDD = lpDDraw;
	lpDDSBack = lpDDS_Back;
	lpDDSOffscreen = lpDDS_Offscreen;
	lpDDSPrimary = lpDDS_Primary;
	hwnd = hwindow;
}
/**********************************************************************/


void GameDisplay::ResetDisplay()
//-------------------------------------------------------------------
//  Resets the in game display so that all bars are full and the score is 0
//-------------------------------------------------------------------
{
	backUpdate = true;
	frontUpdate = true;

	//Reset the powerbars to full
	shipHealth->ResetFull();
	shipPower->ResetFull();
	scoreDisplay->SetSize(0);
}
/**********************************************************************/
