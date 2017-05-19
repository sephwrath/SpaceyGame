////////////////////////////////////////////////////////////////////////
//This file contains external functions for drawing the ship in the correct 
//location within the game
//Written by Stephen Moran
//Date: 18/12/1999
////////////////////////////////////////////////////////////////////////

#include "ShipDraw.h"



void DrawSpaceShip()
//-------------------------------------------------------------------
//Draw the ship animation
//-------------------------------------------------------------------
{
	DWORD thisTickCount;
	DWORD tickDiff, moveDiff;

	thisTickCount = timeGetTime();
	tickDiff = thisTickCount - spaceShip.lastTickCount;
	moveDiff = thisTickCount - spaceShip.moveTickCount;
	
	//////
	//LEFT
	//////

	switch(spaceShip.motion)
	{
		//The first cases are for the full rotations of the ship that cant be interrupted by the kyboard
		case ROTATE_RIGHT:
			//move the ship only if it is in the bounds still and if the movement delay has passed.
			if(moveDiff > spaceShip.moveDelay)
			{
				if ((globalPosx < MAXGLOBALPOSX) && (spaceShip.dest.left > 224))
				{
					if ((globalPosx += 8)>= MAXGLOBALPOSX)
						globalPosx = MAXGLOBALPOSX;
				}
				else
				{
					if ((spaceShip.dest.left +=8)> (RIGHT_BOUNDS))
						spaceShip.dest.left = RIGHT_BOUNDS;
				}
				spaceShip.moveTickCount = thisTickCount;
			}

			//calculate the next cell to be shown
			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell++ > MAX_ROTATE_CELLS-1)
				{
					spaceShip.motion = NO_MOVE;
					spaceShip.currentCell = 0;
					spaceShip.delay = 55;
				}	
				spaceShip.lastTickCount = timeGetTime();
			}
			spaceShip.src = rotatingRightRect[spaceShip.currentCell];
			DrawOneToBack(spaceShip.dest.left, spaceShip.dest.top, spaceShip.src, false,lpDDSOffscreen);
			
			return;

		case ROTATE_LEFT:
			if(moveDiff > spaceShip.moveDelay)
			{
				if ((globalPosx > 0) && (spaceShip.dest.left < 224))
				{
					if ((globalPosx -= 8)<= 0)
						globalPosx = 0;
				}
				else
				{
					if ((spaceShip.dest.left -=8) < (LEFT_BOUNDS))
						spaceShip.dest.left = LEFT_BOUNDS;
				}
				spaceShip.moveTickCount = thisTickCount;
			}

			//Increment to the next cell
			if (tickDiff > spaceShip.delay)
			{
				if (spaceShip.currentCell++ > MAX_ROTATE_CELLS-1)
				{
					spaceShip.motion = NO_MOVE;
					spaceShip.currentCell = 0;
					spaceShip.delay = 65;
				}
				
				spaceShip.lastTickCount = timeGetTime();
			}
			spaceShip.src = rotatingLeftRect[spaceShip.currentCell];
			DrawOneToBack(spaceShip.dest.left, spaceShip.dest.top, spaceShip.src, false,lpDDSOffscreen);
			return;

		case LEFT:
			spaceShip.src = movingLeftRect[spaceShip.currentCell];
			DrawOneToBack(spaceShip.dest.left, spaceShip.dest.top, spaceShip.src, false,lpDDSOffscreen);
			return;

		case RIGHT:
			spaceShip.src = movingRightRect[spaceShip.currentCell];
			DrawOneToBack(spaceShip.dest.left, spaceShip.dest.top, spaceShip.src, false,lpDDSOffscreen);
			return;

		case NO_MOVE:
			spaceShip.src = noLRMovementRect[spaceShip.currentCell];
			DrawOneToBack(spaceShip.dest.left, spaceShip.dest.top, spaceShip.src, false,lpDDSOffscreen);
			return;
	}
}
//***********************************************************************