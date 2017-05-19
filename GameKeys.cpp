////////////////////////////////////////////////////////////////////////
//This file contains external functions for acting on user input for all 
//of the main in game functions
//Written by Stephen Moran
//Date: 18/12/1999
////////////////////////////////////////////////////////////////////////

#include "GameKeys.h"


///////////////////////////////////////
// The function for detecting and acting on key input
///////////////////////////////////////

void CheckKeyBoard()
//-------------------------------------------------------------------
// Processes the main key input for the game
//-------------------------------------------------------------------
{
	//only check the keyboard when the ship isn't rotating so that 
	//the animation must finish before controll is regained
	if ((spaceShip.motion != ROTATE_RIGHT)&&(spaceShip.motion != ROTATE_LEFT))
	{
		DWORD thisTickCount;
		DWORD tickDiff, moveDiff;

		thisTickCount = timeGetTime();
		tickDiff = thisTickCount - spaceShip.lastTickCount;
		moveDiff = thisTickCount - spaceShip.moveTickCount;
	
		//////
		//LEFT
		//////
		if (moveDiff < spaceShip.moveDelay) return;
		else spaceShip.moveTickCount = thisTickCount;

		if (GetAsyncKeyState(VK_NUMPAD4))//move the ship to the left (-x)
		{
			spaceShip.motion = LEFT;
		
			if ((globalPosx > 0) && (spaceShip.dest.left < 224))
			{
				if ((globalPosx -= 3)<= 0)
				{
					//spaceShip.posx += globalPosx - MAXGLOBALPOSX;
					globalPosx = 0;
				}
			}
			else
			{
				if ((spaceShip.dest.left -= 3)<= (LEFT_BOUNDS))
					spaceShip.dest.left =LEFT_BOUNDS;
			}

			if (wrongWayFlag ==  WAS_RIGHT)
			{
				spaceShip.currentCell =0;
				wrongWayFlag = WAS_LEFT;
			}
			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell++ > MAX_MOVE_CELLS-1)
				{
					spaceShip.currentCell = 5;
				}
				spaceShip.lastTickCount = timeGetTime();
			}
		}
		else if ((wrongWayFlag == WAS_LEFT)&&(spaceShip.currentCell>0))
		{

			if ((globalPosx > 0) && (spaceShip.dest.left < 224))
			{
				if ((globalPosx -= 2)<= 0)
				{
					//spaceShip.posx += globalPosx - MAXGLOBALPOSX;
					globalPosx = 0;
				}
			}
			else
			{
				if ((spaceShip.dest.left -=2) <= (LEFT_BOUNDS))
					spaceShip.dest.left = LEFT_BOUNDS;
			}
			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell-- < 0)
				{
					spaceShip.currentCell = 0;
				}
				spaceShip.lastTickCount = timeGetTime();
			}
		}

		///////
		//RIGHT
		///////
		if (GetAsyncKeyState(VK_NUMPAD6))//move the ship to the right (+x)
		{
			spaceShip.motion = RIGHT;
			if ((globalPosx < MAXGLOBALPOSX) && (spaceShip.dest.left > 224))
			{

				if ((globalPosx +=3)>= MAXGLOBALPOSX)
				{
					//spaceShip.posx += globalPosx - MAXGLOBALPOSX;
					globalPosx = MAXGLOBALPOSX;
				}
			}
			else
			{
				if ((spaceShip.dest.left += 3)>= (RIGHT_BOUNDS))
				spaceShip.dest.left = RIGHT_BOUNDS;

			}

			if (wrongWayFlag ==  WAS_LEFT)
			{
				spaceShip.currentCell =0;
				wrongWayFlag = WAS_RIGHT;
			}
			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell++ > MAX_MOVE_CELLS-1)
				{
					spaceShip.currentCell = 5;
				}
				spaceShip.lastTickCount = timeGetTime();
			}
		}
		else if ((wrongWayFlag == WAS_RIGHT)&&(spaceShip.currentCell>0))
		{

			if ((globalPosx < MAXGLOBALPOSX) && (spaceShip.dest.left > 224))
			{

				if ((globalPosx += 2)>= MAXGLOBALPOSX)
				{
					//spaceShip.posx += globalPosx - MAXGLOBALPOSX;
					globalPosx = MAXGLOBALPOSX;
				}
			}
			else
			{
				if ((spaceShip.dest.left +=2)>= (RIGHT_BOUNDS))
					spaceShip.dest.left=RIGHT_BOUNDS;
			}

			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell-- < 0)
				{
					spaceShip.currentCell = 0;
				}
				spaceShip.lastTickCount = timeGetTime();
			}
		}

		//////
		//UP
		//////
		if (GetAsyncKeyState(VK_NUMPAD8))//move the ship forward (-y)
		{
			if ((spaceShip.dest.top-=3) < (UPPER_BOUNDS))
			{
				spaceShip.dest.top = UPPER_BOUNDS;
			}
		}
	
		///////
		//DOWN
		///////
		if (GetAsyncKeyState(VK_NUMPAD5))//move the ship back (+y)
		{
			if ((spaceShip.dest.top+= 2)> (LOWER_BOUNDS))
			{
				spaceShip.dest.top = LOWER_BOUNDS;
			}
		}
	
		///////
		//L ROLL
		///////
		if (GetAsyncKeyState(VK_NUMPAD1))//roll the ship to the left (-x)
		{

			if (spaceShip.dest.left < (LEFT_BOUNDS))
			{
				//spaceShip.posx = LEFT_BOUNDS;
				spaceShip.currentCell =0;
				spaceShip.motion = NO_MOVE;
			}
			else
			{
				spaceShip.delay = 30;
				spaceShip.motion = ROTATE_LEFT;
			}
		}

		////////
		//R ROLL
		////////
		if (GetAsyncKeyState(VK_NUMPAD3))//roll the ship to the right (+x)
		{
	
			if (spaceShip.dest.left < (LEFT_BOUNDS))
			{
				//spaceShip.posx = LEFT_BOUNDS;
				spaceShip.currentCell =0;
				spaceShip.motion = NO_MOVE;
			}
			else
			{
				spaceShip.delay = 30;
				spaceShip.motion = ROTATE_RIGHT;
			}
		}
	}
	////////
	//SHOOT
	////////
	if (GetAsyncKeyState(VK_SPACE))//shoot laser
	{
		weaponPtr->Fire();
	}
	else
	{
		weaponPtr->NoFire();
	}

	////////
	//FLIP SLOT 1
	////////
	//for some reason VK_C isn't working so use 67 instead (same thing)
	SHORT keyVal = GetAsyncKeyState(67);
	if (keyVal)
	{
		if (togleKeyState[0] == false)
		{
			togleKeyState[0] = true;
		}
		
	}
	else if (togleKeyState[0] == true)
	{
		togleKeyState[0] = false;
		if (weaponPtr->GetSelected(0))
			weaponPtr->SwitchOff(0, true);
		else
			weaponPtr->SwitchOn(0, true);
	}
	
	////////
	//FLIP SLOT 2
	////////
	keyVal = GetAsyncKeyState(86);
	if (keyVal)
	{
		if (togleKeyState[1] == false)
		{
			togleKeyState[1] = true;
		}
		
	}
	else if (togleKeyState[1] == true)
	{
		togleKeyState[1] = false;
		if (weaponPtr->GetSelected(1))
			weaponPtr->SwitchOff(1, true);
		else
			weaponPtr->SwitchOn(1, true);
	}

	////////
	//FLIP SLOT 3
	////////
	keyVal = GetAsyncKeyState(66);
	if (keyVal)
	{
		if (togleKeyState[2] == false)
		{
			togleKeyState[2] = true;
		}
		
	}
	else if (togleKeyState[2] == true)
	{
		togleKeyState[2] = false;
		if (weaponPtr->GetSelected(2))
			weaponPtr->SwitchOff(2, true);
		else
			weaponPtr->SwitchOn(2, true);
	}

	////////
	//FLIP SLOT 4
	////////
	keyVal = GetAsyncKeyState(78);
	if (keyVal)
	{
		if (togleKeyState[3] == false)
		{
			togleKeyState[3] = true;
		}
		
	}
	else if (togleKeyState[3] == true)
	{
		togleKeyState[3] = false;
		if (weaponPtr->GetSelected(3))
			weaponPtr->SwitchOff(3, true);
		else
			weaponPtr->SwitchOn(3, true);
	}

	////////
	//FLIP SLOT 5
	////////
	keyVal = GetAsyncKeyState(77);
	if (keyVal)
	{
		if (togleKeyState[4] == false)
		{
			togleKeyState[4] = true;
		}
		
	}
	else if (togleKeyState[4] == true)
	{
		togleKeyState[4] = false;
		if (weaponPtr->GetSelected(4))
			weaponPtr->SwitchOff(4, true);
		else
			weaponPtr->SwitchOn(4, true);
	}
}