// Button.h: interface for the Button class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPTION_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)
#define AFX_CAPTION_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Window.h"

class Caption : public Window  
{
private:
	int captionWidth;

	Text captString;
	char captStringText[40];
	int textWidth;

protected:
	//This function is activated whenever the button is pressed
	virtual int vr_action() {return 0;}

public:
	Caption(const Frame *container, int xPos, int yPos, int width, int format, int size,  const char CaptionText[]);
	virtual ~Caption();
	
	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest,
		LPDIRECTDRAWSURFACE7 lpDDS_Source);

	void ChangeText(const char CaptionText[]);
	const char* Caption::GetText();
	void SetPos(int xPos, int yPos) {SetRect(&bounds, xPos, yPos, xPos + bounds.right - bounds.left, yPos + 32 + captString.GetSize());  client = bounds;}
	void GetPos(int *xPos, int *yPos) {*xPos = bounds.left;  *yPos = bounds.top;}

//	virtual int  wm_paint(int x, int y); 
//	int wm_mousemove(int x, int y);
//	int wm_gainfocus();
//	int wm_losefocus();
//	int wm_lbuttondown(int x, int y);
//	int wm_lbuttonup(int x, int y); 
//	virtual int  wm_ldrag(coord x, coord y); 
//	virtual int  wm_lclick(coord x, coord y); 
//	virtual int  wm_keydown(int key); 
//	virtual int  wm_command(gui_window *win, int cmd, int param) { return(0); }; 
//	virtual int  wm_cansize(coord x, coord y); 
//	virtual int  wm_size(coord x, coord y, int cansize); 
//	virtual int  wm_sizechanged(void) { return(0); } 
//	virtual int  wm_update(int msdelta) { return(0); }

};
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//								Member Function Inplimentations

Caption::Caption(const Frame *container ,int xPos, int yPos, int width, int format, int size, const char CaptionText[])
	:captString (0, 0)
//-------------------------------------------------------------------
// Object constructor
//-------------------------------------------------------------------
{

	captString.ChangeType(format, size);
	if (CaptionText != NULL)
	{
		strcpy(captStringText, CaptionText);
		textWidth = captString.GetWidth(captStringText);
	}

	//adjust the width is it is less than the text width
	if (textWidth > width)
	{
		width = textWidth;
	}

	wndParent = (Window*)container;

	//set the bounding rectangle according to the width
	SetRect(&bounds, xPos, yPos, xPos + width, yPos + 32 + size);
	client = bounds;
	
	return;
}
//*******************************************************************


Caption::~Caption()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************


void Caption::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
// Redraws the button by calling the draw routine of the appropriate
// member.
//-------------------------------------------------------------------
{
	RECT pArea = wndParent->GetClient();
	RECT gblBounds, TempRect;

	//Set up the current golbal boundaries according to the parent rect
	SetRect(&gblBounds, bounds.left + pArea.left, bounds.top + pArea.top,
					   bounds.right + pArea.left, bounds.bottom + pArea.top);
	TempRect = gblBounds;

	captString.DrawString(captStringText,
		gblBounds.left + ((bounds.right - bounds.left - textWidth)/2),
		gblBounds.top, lpDDS_Dest, lpDDS_Source);
}
//***********************************************************************


void Caption::ChangeText(const char CaptionText[])
//-------------------------------------------------------------------
// Changes caption text
//-------------------------------------------------------------------
{
	if (CaptionText != NULL)
	{
		strcpy(captStringText, CaptionText);
		textWidth = captString.GetWidth(captStringText);
	}

	//set the bounding rectangle according to the width
	bounds.right = bounds.left + textWidth;
	client = bounds;
}
//***********************************************************************


const char* Caption::GetText()
//-------------------------------------------------------------------
// Returns a reference to the text
//-------------------------------------------------------------------
{
	return captStringText;
}
//***********************************************************************

#endif // !defined(AFX_CAPTION_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)