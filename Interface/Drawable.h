// Drawable.h: interface for the Drawable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWABLE_H__72F4DD03_0791_11D5_822B_E18021549D30__INCLUDED_)
#define AFX_DRAWABLE_H__72F4DD03_0791_11D5_822B_E18021549D30__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <ddraw.h>
#include "..\ddtools.h"

#define INIT_DXSTRUCT(dxs) { ZeroMemory(&dxs, sizeof(dxs)); dxs.dwSize = sizeof(dxs); }

class Drawable
{
	friend class Text;
	friend class DigitalCounter;

private:
	bool tile;		//Set if the image is to be tiled
	bool visible;	//Set if the image is to be displayed

	RECT dest;
	int width;
	int height;

	int offX;
	int offY;

	RECT src;
	int initalWidth;
	int initalHeight;
	//LPDIRECTDRAWSURFACE7 lpDDS_Source;

public:
	Drawable(int left, int top, int right, int bottom, bool tile);
	Drawable(int left, int top, int right, int bottom, 
		bool tileIt, int oX, int oY);
	virtual ~Drawable();

	void SetPos(RECT screenPos);
	void SetSource(RECT offScreenPos);
	RECT GetPos(){return dest;}

	int GetStartWidth(){return initalWidth;}
	int GetStartHeight(){return initalHeight;}
	int GetWidth(){return width;}
	int GetHeight(){return height;}

	void Draw(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source,
		DWORD effects);
 	void SetVisible(bool value) {visible = value;}

	//virtual void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source);
	virtual void Show() {SetVisible(true);}
	virtual void Hide() {SetVisible(false);}
	
};

#endif // !defined(AFX_DRAWABLE_H__72F4DD03_0791_11D5_822B_E18021549D30__INCLUDED_)
