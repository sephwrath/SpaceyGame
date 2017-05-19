////////////////////////////////////////////////////////////////////////
//This file contains external functions for terrain manipulation within the game
//including reading the terrain .inf files for each level and also the badguy and
//object files that setup placement of different objects on the terrain
//Written by Stephen Moran
//Creation Date: 28/9/1999
////////////////////////////////////////////////////////////////////////

#include "Terrain.h"


void DrawBlock(int startIndex, int xCells, int startSlice, int yCells, int xStart, int yStart)
//-------------------------------------------------------------------
//  Draws a block of cells to the terain buffer. Starting from the
//	startSlice, startColumn global position for xCells in the x direction
//	and yCells in the y direction. The destination of the block is the
//	xStart and yStart cells. This function simplifies the blitting
//	algorythms. No parts of cells are drawn since this function is only
//	meant to draw to the terrain buffer.
//-------------------------------------------------------------------
{
	int mapIndex, mapRow, tileIndex;
	int xStartTemp;
	RECT  srcRect;

	//draws the correct cells to the screen starting with the top left hand corner
	//and moving along the row (slice) to the end before beginning the next row.
	for (mapRow = startSlice; mapRow < (startSlice + yCells); mapRow++)
	{
		xStartTemp = xStart;
		for (mapIndex = startIndex; mapIndex < (startIndex + xCells); mapIndex++)
		{
			//Handle the base tiles
			tileIndex = slicePtr[mapRow].contents[mapIndex].base;
			
			if (tileIndex > 0)
			{			
				// retrieve tile location in lpDDSOffscreen
				srcRect.left = cells[tileIndex].posx;
				srcRect.top = cells[tileIndex].posy;
				srcRect.right = srcRect.left + CELLSIZE;
				srcRect.bottom = srcRect.top + CELLSIZE;

				// blit the tile to the location
				BlitSurface( lpDDSOffscreenBase,lpDDSTerrainBuffer,xStartTemp,yStart,&srcRect, DDBLTFAST_NOCOLORKEY);	//DDBLTFAST_WAIT | 
			} //if

			//Handle the level1 tiles
			tileIndex = slicePtr[mapRow].contents[mapIndex].level1;
			
			if (tileIndex > 0)
			{			
				// retrieve tile location in lpDDSOffscreen
				srcRect.left = cells[tileIndex].posx;
				srcRect.top = cells[tileIndex].posy;
				srcRect.right = srcRect.left + CELLSIZE;
				srcRect.bottom = srcRect.top + CELLSIZE;

				// blit the tile to the location
				BlitSurface( lpDDSOffscreenBase,lpDDSTerrainBuffer,xStartTemp,yStart,&srcRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 
			} //if

			//Handle the level2 tiles
			tileIndex = slicePtr[mapRow].contents[mapIndex].level2;
			
			if (tileIndex > 0)
			{			
				// retrieve tile location in lpDDSOffscreen
				srcRect.left = cells[tileIndex].posx;
				srcRect.top = cells[tileIndex].posy;
				srcRect.right = srcRect.left + CELLSIZE;
				srcRect.bottom = srcRect.top + CELLSIZE;

				// blit the tile to the location
				BlitSurface( lpDDSOffscreenBase,lpDDSTerrainBuffer,xStartTemp,yStart,&srcRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 
			} //if

			//Handle the level3 tiles
			tileIndex = slicePtr[mapRow].contents[mapIndex].level3;
			
			if (tileIndex > 0)
			{			
				// retrieve tile location in lpDDSOffscreen
				srcRect.left = cells[tileIndex].posx;
				srcRect.top = cells[tileIndex].posy;
				srcRect.right = srcRect.left + CELLSIZE;
				srcRect.bottom = srcRect.top + CELLSIZE;

				// blit the tile to the location
				BlitSurface( lpDDSOffscreenBase,lpDDSTerrainBuffer,xStartTemp,yStart,&srcRect, DDBLTFAST_SRCCOLORKEY);	//DDBLTFAST_WAIT | 
			} //if

			//Handle the level4 tiles
			tileIndex = slicePtr[mapRow].contents[mapIndex].level4;

			if (tileIndex > 0)
			{			
				// retrieve tile location in lpDDSOffscreen
				srcRect.left = cells[tileIndex].posx;
				srcRect.top = cells[tileIndex].posy;
				srcRect.right = srcRect.left + CELLSIZE;
				srcRect.bottom = srcRect.top + CELLSIZE;

				// blit the tile to the location
				BlitSurface( lpDDSOffscreenBase,lpDDSTerrainBuffer,xStartTemp,yStart,&srcRect, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 
			} //if
			xStartTemp += CELLSIZE;
		} // for mapIndex
		yStart += CELLSIZE;
	} // for mapRow
}
//***********************************************************************


bool InitSlices( void )
//-------------------------------------------------------------------
//  Inititialize terrain array 
//  from the TERRAIN#.INF
//-------------------------------------------------------------------
{
	FILE	*stream; //std type found in <stdio.h>
	int		i,j;
    fpos_t  fileSize;


	//If the level is not being loaded for the first time then delete previously allocated mem before allocating more
	if (slicePtr != NULL)
	{
		LocalFree(slicePtr);
	}

    // Open the terrain.inf file for reading.
	if(!(stream = fopen( "TERRAIN003.INF", "r")))
	{
		MessageBox(GetActiveWindow(),"Oh no I can't read TERRAIN003.INF", "Spacey II", MB_OK);
		return false;
	}

    fseek(stream,0L,SEEK_END);
    fgetpos( stream, &fileSize );
    fseek(stream,0L,SEEK_SET);
    numSlices = (int)fileSize / 601; //481 for only four levels


    slicePtr = (SLICE *)LocalAlloc( LPTR, numSlices * sizeof(SLICE) );
	if (slicePtr == NULL)
	{
		MessageBox(GetActiveWindow(),"Memory allocation error in InitSlices", "Spacey II", MB_OK);
		return false;
	}
    // Go through the file and initialize the terrain array.
	for (i = 0; i < numSlices; i++)
	{
		for(j = 0; j < NUMCONTENTS; j++)
		{
				fscanf(stream,"%d",&slicePtr[i].contents[j].base);
				fscanf(stream,"%d",&slicePtr[i].contents[j].level1);
				fscanf(stream,"%d",&slicePtr[i].contents[j].level2);
				fscanf(stream,"%d",&slicePtr[i].contents[j].level3);
			if (j != NUMCONTENTS-1)
                fscanf(stream,"%d",&slicePtr[i].contents[j].level4);
            else
                fscanf(stream,"%d\n",&slicePtr[i].contents[j].level4);
		}
	}
	fclose(stream);
	globalPosy = (float)(numSlices * 32)-480;
	tilePosy = (float)(numSlices * 32)-480;
	bufferGlobaly = (int)tilePosy - 32;
	bufferTop = (bufferGlobaly % BUFFERSIZEY);
	int cellsDone = (BUFFERSIZEX - bufferTop)/CELLSIZE;

	//iniatlise the terrainBuffer ie draw the starting terrain
	//because superdraw only ever draws the "exta" rows or columns need some starting terrain
	DrawBlock(bufferGlobalx/CELLSIZE, BUFFERSIZE,
		bufferGlobaly/CELLSIZE, cellsDone + 1, 0, bufferTop);
	
	if (cellsDone != BUFFERSIZE)
	{
		DrawBlock(bufferGlobalx/CELLSIZE, BUFFERSIZE,
			bufferGlobaly/CELLSIZE + cellsDone, BUFFERSIZE - cellsDone, 0, 0);
	}

	return true;
}
//***********************************************************************


bool InitItems( void )
//-------------------------------------------------------------------
//  Initalise the item array 
//  from item#.inf
//-------------------------------------------------------------------
{
	FILE	*stream; //std type found in <stdio.h>
	int		i;
    fpos_t  fileSize;

	//If the items are not being loaded for the first time then delete previously allocated mem before allocating more
	if (itemPtr != NULL)
	{
		LocalFree(itemPtr);
	}

    // Open the item003.inf file for reading.
	if(!(stream = fopen( "ITEM003.INF", "r")))
	{
		MessageBox(GetActiveWindow(),"Oh no I can't read ITEM003.INF", "Spacey II", MB_OK);
		return false;
	}

    fseek(stream,0L,SEEK_END);
    fgetpos( stream, &fileSize );
    fseek(stream,0L,SEEK_SET);
    numItems = (int)fileSize / 16; //5 for each record entry 2 for return

	//Memory is freed in the main object when the program is terminatd
    itemPtr = (TERRAIN_ITEM *)LocalAlloc( LPTR, numItems * sizeof(TERRAIN_ITEM) );
	if (itemPtr == NULL)
	{
		MessageBox(GetActiveWindow(),"Memory allocation error in InitItems", "Spacey II", MB_OK);
		return false;
	}
    // Go through the file and initialize the slice array using the itemPtr.
	for (i = 0; i < numItems; i++)
	{
		fscanf(stream,"%d",&itemPtr[i].gblPosy);
		fscanf(stream,"%d",&itemPtr[i].gblPosx);
		fscanf(stream,"%d\n",&itemPtr[i].itemType);
	}

	fclose(stream);

	//Set up indexes to the array
	listLevel = 0;
	int cellsDone = (BUFFERSIZEX - bufferTop)/CELLSIZE;

	DrawItems (bufferGlobalx, BUFFERSIZEX, bufferGlobaly, (cellsDone + 1)*CELLSIZE,
			0, bufferTop);
	if (cellsDone != BUFFERSIZE)
	{
		DrawItems(bufferGlobalx, BUFFERSIZEX, bufferGlobaly + cellsDone*CELLSIZE,
			BUFFERSIZEY - cellsDone*CELLSIZE, 0, 0);
	}
	listLevel = 0;
	SuperDraw();
	return true;
}
//***********************************************************************


bool ClipObject(RECT *source, int *left, int *top, int xPos, int yPos, int srcX, int srcY, int height, int width)
//-------------------------------------------------------------------
//  Performs the operations necesary to determine which parts of an image should
//  be clipped and returns the correct co-ordinates to the calling function via
//  the addresses that were passed in.
//-------------------------------------------------------------------
{
	//Test if off the top of the screen not displayed
	if (yPos + height < globalPosy)
	{
		return false;
	}
	//If too far to the left of the view port not displayed
	if (xPos + width < globalPosx)
	{
		return false;
	}
	//If too far to the right of the view port then not displayed
	if (xPos > globalPosx + SCREENSIZEX)
	{
		return false;
	}
	//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
	//Test if clipped and set up the y parts of the rect accordingly
	//Test if 'hanging' off the top of the screen
	if(yPos < globalPosy)
	{
		height = yPos + height - (int)globalPosy;
		source->top = (int)globalPosy - yPos + srcY;
		*top = 0;
	}
	else
	{
		//Test if 'hangging' off the bottom of the screen
		if (yPos + height > globalPosy + SCREENSIZEY)
		{
			height = (int)globalPosy + SCREENSIZEY - yPos;
		}
		source->top = srcY;
		*top = yPos - (int)globalPosy;
	}
	//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	//Test if clipped and set up the x parts of the rect accordingly
	//Test if 'hanging' off the left of the screen
	if(xPos < globalPosx)
	{
		width = xPos + width - globalPosx;
		source->left = globalPosx - xPos + srcX;
		*left = 0;
	}
	else
	{
		//Test if 'hangging' off the right of the screen
		if (xPos + width > globalPosx + SCREENSIZEX)
		{
			width = globalPosx + SCREENSIZEX - xPos;
		}
		source->left = srcX;
		*left = xPos - globalPosx;
	}

	//allocate the values of the right and bottom parts of the rects.
	//Simply by adding height and width
	source->bottom = source->top + height;
	source->right = source->left + width;

	return true;
}
//***********************************************************************


bool DrawItems (int xStart, int width, int yStart, int height, int xDest, int yDest)
//-------------------------------------------------------------------
//  Draw the items on the back surface from the array
//  and update the starting position in the array.
//	the items are drawn from top to bottom and the item with the highest
//	y value is the first item drawn as well as the last item in the array
//	array pos	0											x
//				______________________________________________
//	y position	| 10 | 12 | 12 | 34 | 54 | 120 | 145 | etc
//				top of the screen		-->		bottom of the screen
//						drawn this way-->
//
//	the drawing is stopped when the bottom of the screen is reached
//	so the drawn chunk of the array will mover closer and closer to the front
//-------------------------------------------------------------------
{
	int location;
	RECT blitFrom;
	int posX, posY;
	int iWidth, iHeight;
	bool firstBlit;
	
	//start From the top of the view port
	location = listLevel;
	firstBlit = true;
	//If below the view port then not displayed either
	while ((itemPtr[location].gblPosy < yStart + height)
		&& (location < numItems)) //still in y range//
	{
		if ((itemPtr[location].gblPosy + items[itemPtr[location].itemType].height) < yStart)
		{
			location++;
			continue;
		}
		if (itemPtr[location].gblPosx + items[itemPtr[location].itemType].width < xStart)
		{
			location++;
			continue;
		}
		if (itemPtr[location].gblPosx > xStart + width)
		{
			location++;
			continue;
		}
		
		//For the first blit set the pointer to the location so that the whole list dosn't
		//have to be traversed the next time round.
		if (firstBlit == true)
		{
			//Im not entirely sure what this does anymore however if items arent being drawn properly
			// increasing the number works like a charm - not sure why but it does!
			// ok I remembered - the new blit will start at the listLevel which is the start of the array
			// initally however most of the top part of the level is obvously not on screen at first
			// so when the first drawable item is hit then the next starting level is set to the current
			// point minus a few items to allow for smaller items to be drawn and for the list to be able to move up
			// at all - it makes more sence when you consider that item has to be in the viewport to reach this point
			if (location >= 100)
				listLevel = location - 100;
			else
				listLevel = 0;

			firstBlit = false;
		}

		iWidth = items[ itemPtr[location].itemType ].width;
		iHeight = items[ itemPtr[location].itemType ].height;

		//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
		//Test if clipped and set up the y parts of the rect accordingly
		//Test if 'hanging' off the top of the area
		if(itemPtr[location].gblPosy < yStart)
		{
			blitFrom.top = yStart - itemPtr[location].gblPosy + 
				items[itemPtr[location].itemType].srcY;
			posY = yDest;//yStart - globalPosy;

			//Test if 'hangging' off the bottom of the screen
			if (itemPtr[location].gblPosy + iHeight > yStart + height)
				iHeight = height;
			else
				iHeight = itemPtr[location].gblPosy + iHeight - yStart;
		}
		else
		{
			//Test if 'hangging' off the bottom of the screen
			if (itemPtr[location].gblPosy + iHeight > yStart + height)
				iHeight = yStart + height - itemPtr[location].gblPosy;

			blitFrom.top = items[itemPtr[location].itemType].srcY;
			posY = yDest + itemPtr[location].gblPosy - yStart;
		}
		//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		//Test if clipped and set up the x parts of the rect accordingly
		//Test if 'hanging' off the left of the screen
		if(itemPtr[location].gblPosx < xStart)
		{
			blitFrom.left = xStart - itemPtr[location].gblPosx + 
				items[itemPtr[location].itemType].srcX;
				posX = xDest;//xStart - globalPosx;
				
			//Test if 'hangging' off the right of the screen
			if (itemPtr[location].gblPosx + iWidth > xStart + width)
				iWidth = width;
			else
				iWidth = itemPtr[location].gblPosx + iWidth - xStart;
		}
		else
		{
			//Test if 'hangging' off the right of the screen
			if (itemPtr[location].gblPosx + iWidth > xStart + width)
				iWidth = xStart + width - itemPtr[location].gblPosx;

			blitFrom.left = items[itemPtr[location].itemType].srcX;
			posX = xDest + itemPtr[location].gblPosx - xStart;
		}

		//allocate the values of the right and bottom parts of the rects.
		//Simply by adding height and width
		blitFrom.bottom = blitFrom.top + iHeight;
		blitFrom.right = blitFrom.left + iWidth;

		BlitSurface( lpDDSOffscreenItems,lpDDSTerrainBuffer, posX, posY,
			&blitFrom, DDBLTFAST_SRCCOLORKEY);//DDBLTFAST_WAIT | 

		
		location++;
	}
return true;
}
//***********************************************************************


bool SuperDraw(void)
//-------------------------------------------------------------------
//  Speeds up the drawing process by drawing the terrain and storring
//	it on its own ddsurface then when the terrain is updated only the
//	new rows have to be added. This saves redraving the same terrain
//	from scratch over and over again when it only changes a little.
//-------------------------------------------------------------------
{	
	//////////////////////////////////////////////////////////////////
	//LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
	//Test if the view has moved left further than the bounds and redraw accordingly
	if (globalPosx < bufferGlobalx)
	{
		bufferGlobalx -= CELLSIZE;
		bufferLeft = bufferGlobalx % BUFFERSIZEX;

		if (bufferTop == 0)
		{
			DrawBlock(bufferGlobalx/CELLSIZE,	//the index to start on
				1,								//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				BUFFERSIZE,						//the number to travel down (tiles down buffer)
				bufferLeft,						//the  x pos in the buffer
				0);								//the y pos in the buffer
			DrawItems(bufferGlobalx, CELLSIZE, bufferGlobaly, BUFFERSIZEY,
				bufferLeft, 0);
		}
		else
		{
			int cellsDone = (BUFFERSIZEY - bufferTop)/CELLSIZE;
			int pixelsDone = cellsDone * CELLSIZE;
			DrawBlock(bufferGlobalx/CELLSIZE,	//the index to start on
				1,								//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				cellsDone,						//the number to travel down (tiles down buffer)
				bufferLeft,						//the  x pos in the buffer
				bufferTop);						//the y pos in the buffer
			DrawItems(bufferGlobalx, CELLSIZE, bufferGlobaly, pixelsDone,
				bufferLeft, bufferTop);
			
			DrawBlock(bufferGlobalx/CELLSIZE,	//the index to start on
				1,								//the number to travel across
				(bufferGlobaly/CELLSIZE) + cellsDone,//the slice number to start on
				BUFFERSIZE - cellsDone,			//the number to travel down (tiles down buffer)
				bufferLeft,						//the  x pos in the buffer
				0);								//the y pos in the buffer
			DrawItems(bufferGlobalx, CELLSIZE,
				bufferGlobaly + pixelsDone,
				BUFFERSIZEY - pixelsDone, bufferLeft, 0);
		}
	}
	///////////////////////////////////////////////////////////////////////////////////
	//RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	//Else Test if the view has moved right further than the bounds and redraw accordingly
	else if ((globalPosx + SCREENSIZEX) > (bufferGlobalx + BUFFERSIZEX))
	{
		bufferGlobalx += CELLSIZE;
		bufferLeft = bufferGlobalx % BUFFERSIZEX;

		if (bufferTop == 0)
		{
			DrawBlock(bufferGlobalx/CELLSIZE + GAME_PORT_WIDTH,	//the index to start on
				1,								//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				BUFFERSIZE,						//the number to travel down (tiles down buffer)
				(bufferLeft + SCREENSIZEX)%BUFFERSIZEX,		//the  x pos in the buffer
				0);								//the y pos in the buffer
			DrawItems(bufferGlobalx + SCREENSIZEX,
				CELLSIZE,
				bufferGlobaly,
				BUFFERSIZEY, (bufferLeft + SCREENSIZEX)%BUFFERSIZEX, 0);
		}
		else
		{
			int pixelsDone = BUFFERSIZEY - bufferTop;
			int cellsDone = pixelsDone/CELLSIZE;
			int startRight = (bufferLeft + SCREENSIZEX)%BUFFERSIZEX;
			DrawBlock(bufferGlobalx/CELLSIZE + GAME_PORT_WIDTH,	//the index to start on
				1,								//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				cellsDone,						//the number to travel down (tiles down buffer)
				startRight,						//the  x pos in the buffer
				bufferTop);						//the y pos in the buffer
			DrawItems(bufferGlobalx + SCREENSIZEX,
				CELLSIZE,
				bufferGlobaly,
				pixelsDone, startRight, bufferTop);
			
			DrawBlock(bufferGlobalx/CELLSIZE + GAME_PORT_WIDTH,	//the index to start on
				1,								//the number to travel across
				(bufferGlobaly/CELLSIZE) + cellsDone,//the slice number to start on
				BUFFERSIZE - cellsDone,			//the number to travel down (tiles down buffer)
				startRight,		//the  x pos in the buffer
				0);								//the y pos in the buffer
			DrawItems(bufferGlobalx + SCREENSIZEX,
				CELLSIZE,
				bufferGlobaly + pixelsDone,
				BUFFERSIZEY - pixelsDone, startRight, 0);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
	//Test if the top buffer has been exhausted and needs to be refreshed
	if (tilePosy < bufferGlobaly)
	{
		bufferGlobaly -= CELLSIZE;
		
		if ((bufferTop -= 32) < 0)
			bufferTop = BUFFERSIZEY - 32;

		
		if (bufferLeft == 0)
		{
			DrawBlock(bufferGlobalx/CELLSIZE,	//the index to start on
				BUFFERSIZE,						//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				1,								//the number to travel down (tiles down buffer)
				0,								//the  x pos in the buffer
				bufferTop);						//the y pos in the buffer
			DrawItems(bufferGlobalx, BUFFERSIZEX, bufferGlobaly, CELLSIZE, 0, bufferTop);
		}
		else
		{
			int pixelsDone = BUFFERSIZEX - bufferLeft;
			int cellsDone = pixelsDone/CELLSIZE;
			DrawBlock(bufferGlobalx/CELLSIZE,	//the index to start on
				cellsDone ,						//the number to travel across
				bufferGlobaly/CELLSIZE,			//the slice number to start on
				1,								//the number to travel down (tiles down buffer)
				bufferLeft,						//the  x pos in the buffer
				bufferTop);						//the y pos in the buffer
			DrawItems(bufferGlobalx, pixelsDone, bufferGlobaly, CELLSIZE,
				bufferLeft, bufferTop);
			
			DrawBlock((bufferGlobalx/CELLSIZE) + cellsDone,	//the index to start on
				BUFFERSIZE - cellsDone,						//the number to travel across
				(bufferGlobaly/CELLSIZE),		//the slice number to start on
				1,								//the number to travel down (tiles down buffer)
				0,								//the  x pos in the buffer
				bufferTop);						//the y pos in the buffer
			DrawItems(bufferGlobalx + pixelsDone, bufferLeft,
				bufferGlobaly, CELLSIZE, 0, bufferTop);
			
		}
	}
	BlitTerrainBuffer();
	return true;
}
//***********************************************************************



void BlitTerrainBuffer(void)
//-------------------------------------------------------------------
// Simply blits the blocks in the terrain bufer to the back buffer in
// Their correct places
//-------------------------------------------------------------------
{
	int top;
	int bottom;
	int left;
	int right;
	RECT  srcRect;

	//Calculate the view port extremities in relation to the buffer
	top = (int)globalPosy % BUFFERSIZEY;
	bottom = (top + SCREENSIZEY)%BUFFERSIZEY;
	left = globalPosx % BUFFERSIZEX;
	right = (left + SCREENSIZEX)%BUFFERSIZEX;


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Blitting - the terrain is actually drawn to the back buffer from the terrain buffer
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//In the worst case the terrain is split into four sections which need to be blitted individually.

	//Best case - top is less than bottom 
	if (top < bottom)
	{
		//Best case left is less than right (only one blit)
		if (left < right)
		{
			srcRect.top = top;
			srcRect.bottom = bottom;
			srcRect.left = left;
			srcRect.right = right;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
		}
		//when right is less than left then need two blits
		else
		{
			srcRect.top = top;
			srcRect.bottom = bottom;
			srcRect.left = left;
			srcRect.right = BUFFERSIZEX;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
			srcRect.left = 0;
			srcRect.right = right;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, BUFFERSIZEX - left, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
		}
	}
	//when the top is greater than the bottom 
	else
	{
		//Best case left is less than right (two blits)
		if (left < right)
		{
			srcRect.left = left;
			srcRect.right = right;
			srcRect.top = top;
			srcRect.bottom = BUFFERSIZEY;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
			srcRect.top = 0;
			srcRect.bottom = bottom;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, BUFFERSIZEY - top, &srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
		}
		//when right is less than left then need four blits
		else
		{
			srcRect.left = left;
			srcRect.right = BUFFERSIZEX;
			srcRect.top = top;
			srcRect.bottom = BUFFERSIZEY;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
			srcRect.top = 0;
			srcRect.bottom = bottom;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, 0, BUFFERSIZEY - top, &srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
			srcRect.left = 0;
			srcRect.right = right;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, BUFFERSIZEX - left, BUFFERSIZEY - top, &srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
			srcRect.top = top;
			srcRect.bottom = BUFFERSIZEY;
			BlitSurface( lpDDSTerrainBuffer, lpDDSBack, BUFFERSIZEX - left, 0,&srcRect, DDBLTFAST_NOCOLORKEY);//DDBLTFAST_WAIT | 
		}
	}
	return;
}
//***********************************************************************