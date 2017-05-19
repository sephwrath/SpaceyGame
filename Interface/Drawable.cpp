#include "Drawable.h"

//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//								Member Function Inplimentations

Drawable::Drawable(int left, int top, int right, int bottom,
				   bool tileIt, int oX, int oY)
//-------------------------------------------------------------------
//Creates a graphic object. Needs the sorce coords for the graphic so
// it can be drawn onto the back buffer. Sets up offsets.
//-------------------------------------------------------------------
{
	tile = tileIt;
	offX = oX;
	offY = oY;
	src.left = left;
	src.top = top;
	src.right = right;
	src.bottom = bottom;
	initalWidth = right - left;
	initalHeight = bottom - top;
	SetVisible(true);
}
//*******************************************************************


Drawable::Drawable(int left, int top, int right, int bottom,
				   bool tileIt)
//-------------------------------------------------------------------
//Creates a graphic object. Needs the sorce coords for the graphic so
// it can be drawn onto the back buffer. No offsets.
//-------------------------------------------------------------------
{
	tile = tileIt;
	src.left = left;
	src.top = top;
	src.right = right;
	src.bottom = bottom;
	initalWidth = right - left;
	initalHeight = bottom - top;
	SetVisible(true);
}
//*******************************************************************


Drawable::~Drawable()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------

{
	return;
}
//*******************************************************************


void Drawable::SetPos(RECT screenPos)
//-------------------------------------------------------------------
// Sets the position of the graphic on the display surface
//-------------------------------------------------------------------

{
	dest.left = screenPos.left;
	dest.top = screenPos.top;
	dest.right = screenPos.right;
	dest.bottom = screenPos.bottom;
	width = dest.right - dest.left;
	height = dest.bottom - dest.top;
	return;
}
//*******************************************************************


void Drawable::SetSource(RECT offScreenPos)
//-------------------------------------------------------------------
// Sets the position of the graphic on the display surface
//-------------------------------------------------------------------

{
	src.left = offScreenPos.left;
	src.top = offScreenPos.top;
	src.right = offScreenPos.right;
	src.bottom = offScreenPos.bottom;
	initalWidth = src.right - src.left;
	initalHeight = src.bottom - src.top;
	return;
}
//*******************************************************************

void Drawable::Draw(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source,
					DWORD effects)
//-------------------------------------------------------------------
// Draws the object to the screen. Nothing is drawn if visible is not
// set to TRUE. If the the Tiled variable is set then the image is
// repeated until the destination space is filled otherwise the image
// is stretched
//-------------------------------------------------------------------

{
	if (visible == false)
	{
		return;
	}
	if (lpDDS_Source == NULL)
	{
		this->~Drawable();
		return;
	}

	DDBLTFX fx;
	INIT_DXSTRUCT(fx);
	fx.dwDDFX = effects;
	if (tile == false)
	{	
		while(1)
		{
			HRESULT ddrval = lpDDS_Dest->Blt(&dest, lpDDS_Source, &src,
				DDBLT_KEYSRC|DDBLT_DDFX, &fx);//DDBLT_WAIT|
	
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
	}
	else
	{
		RECT tSrc = src;
		RECT tDest = dest;
		tDest.top = dest.top - initalHeight; //- so that first addition starts from start
		tDest.bottom = tDest.top + initalHeight;
		while (tDest.top < dest.bottom)
		{
			tDest.top += initalHeight;
			if ((tDest.bottom += initalHeight) > dest.bottom)
			{
				tDest.bottom = dest.bottom;
				tSrc.bottom = tSrc.top + (tDest.bottom - tDest.top);
			}
			tDest.left = dest.left - initalWidth; //- so that first addition starts from start
			tDest.right = tDest.left + initalWidth;
			tSrc.right = src.right;
			while (tDest.left < dest.right)
			{
				tDest.left += initalWidth;
				if ((tDest.right += initalWidth) > dest.right)
				{
					tDest.right = dest.right;
					tSrc.right = tSrc.left + (tDest.right - tDest.left);
				}
				while (1)
				{
					HRESULT ddrval = lpDDS_Dest->Blt( &tDest, lpDDS_Source,
						&tSrc, DDBLT_KEYSRC|DDBLT_DDFX, &fx);//DDBLT_WAIT|
	
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
			}
		}
	}
	return;
}
//*******************************************************************

