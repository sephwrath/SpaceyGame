// Button.h: interface for the Button class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTON_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)
#define AFX_BUTTON_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Window.h"

class Button : public Window  
{
private:
	GUISpacey *listnerClass;	//The class containing the actionListner method
	int minySize;
	int minxSize;
	int captionWidth;

	Drawable BaseNormL;
	Drawable BaseNorm;
	Drawable BaseNormR;

	Drawable BaseOverL;
	Drawable BaseOver;
	Drawable BaseOverR;

	Drawable BaseClickL;
	Drawable BaseClick;
	Drawable BaseClickR;

	Text title;
	char titleText[40];
	int textWidth;

	BUTTON_STATE btnState;

protected:
	//This function is activated whenever the button is pressed
	virtual int vr_action() {return 0;}

public:
	Button(const Frame *container, int xPos, int yPos, int width, const char CaptionText[]);
	virtual ~Button();
	
	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest,
		LPDIRECTDRAWSURFACE7 lpDDS_Source);
	void AddActionListener (const GUISpacey *listenClass) {listnerClass = (GUISpacey*)listenClass;}

//	virtual int  wm_paint(int x, int y); 
//	int wm_mousemove(int x, int y);
	int wm_gainfocus();
	int wm_losefocus();
	int wm_lbuttondown(int x, int y);
	int wm_lbuttonup(int x, int y); 
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

Button::Button(const Frame *container ,int xPos, int yPos, int width, const char CaptionText[])
	:BaseNormL (257, 60, 261, 101, true),
	 BaseNorm (261, 60, 446, 101, true),
	 BaseNormR (446, 60, 451, 101, true),

	 BaseOverL (257, 101, 261, 142, true),
	 BaseOver (261, 101, 446, 142, true),
	 BaseOverR (446, 101, 451, 142, true),

	 BaseClickL (257, 142, 261, 182, true),
	 BaseClick (261, 142, 446, 183, true),
	 BaseClickR (446, 142, 451, 182, true),
	 title (0, 0)
//-------------------------------------------------------------------
// Object constructor
// container - reffers the window that the button belongs to
// xPos, yPos - the offset from the top left of the parent window
// width - the requested width this may not be the end width if the
//		   button in too short for the caption or not greather than the minimum
// captionText - a constant string to be used as the caption
//-------------------------------------------------------------------
{
	listnerClass = 0;
	btnState = NORMAL;

	if (CaptionText != NULL)
	{
		strcpy(titleText, CaptionText);
		textWidth = title.GetWidth(titleText);
	}

	//less than 15 pixels is too small
	int tempLength = textWidth + 20;
	if (tempLength < width)
	{
		tempLength = width;
	}
	if (tempLength < 15)
	{
		tempLength = 15;
	}

	wndParent = (Window*)container;

	//set the bounding rectangle according to the width
	SetRect(&bounds, xPos, yPos, xPos + tempLength, yPos + 41);
	client = bounds;
	
	return;
}
//*******************************************************************


Button::~Button()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************


void Button::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
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

	switch (btnState)
	{
		case OVER:
			TempRect.right = gblBounds.left + 4;
			BaseOverL.SetPos(TempRect);
			BaseOverL.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right - 4;
			BaseOver.SetPos(TempRect);
			BaseOver.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right;
			BaseOverR.SetPos(TempRect);
			BaseOverR.Draw(lpDDS_Dest, lpDDS_Source, 0);
			title.ChangeType(0, 0);
			break;

		case CLICKED:
			TempRect.right = gblBounds.left + 4;
			BaseClickL.SetPos(TempRect);
			BaseClickL.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right - 4;
			BaseClick.SetPos(TempRect);
			BaseClick.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right;
			BaseClickR.SetPos(TempRect);
			BaseClickR.Draw(lpDDS_Dest, lpDDS_Source, 0);
			title.ChangeType(1, 0);
			break;
		
		case NORMAL:
			TempRect.right = gblBounds.left + 4;
			BaseNormL.SetPos(TempRect);
			BaseNormL.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right - 4;
			BaseNorm.SetPos(TempRect);
			BaseNorm.Draw(lpDDS_Dest, lpDDS_Source, 0);

			TempRect.left = TempRect.right;
			TempRect.right = gblBounds.right;
			BaseNormR.SetPos(TempRect);
			BaseNormR.Draw(lpDDS_Dest, lpDDS_Source, 0);
			title.ChangeType(0, 0);
			break;
	}
	title.DrawString(titleText,
		gblBounds.left + ((gblBounds.right - gblBounds.left - textWidth)/2),
		gblBounds.top + 14, lpDDS_Dest, lpDDS_Source);
}
//***********************************************************************


int Button::wm_lbuttondown(int x, int y)
//-------------------------------------------------------------------
// handles the left button down message for a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it. Always returns 1.
//-------------------------------------------------------------------
{
	if (btnState == OVER)
	{
		btnState = CLICKED;
		Window::drawFlag = true;	
	}
	return 1;
}
//***********************************************************************


int Button::wm_lbuttonup(int x, int y)
//-------------------------------------------------------------------
// handles the left button up message for a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it. Always returns 1.
//-------------------------------------------------------------------
{
	
	if (listnerClass != 0 && btnState == CLICKED)
	{
		PlayWAVInfo(lpwiClick, 0);
		listnerClass->ActionListner(this);
	}
	btnState = OVER;
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************


int Button::wm_gainfocus()
//-------------------------------------------------------------------
// handles the gain focus message for a button. Triggered when the
// container window is active and the mouse is over the buttons area
// Always returns 1.
//-------------------------------------------------------------------
{
	btnState = OVER;
	PlayWAVInfo(lpwiActive, 0);
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************


int Button::wm_losefocus()
//-------------------------------------------------------------------
// handles the gain focus message for a button. Triggered when a button
// that had focus loses it or when the mouse pointer leaves the client area
// Always returns 1.
//-------------------------------------------------------------------
{
	btnState = NORMAL;
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************

#endif // !defined(AFX_BUTTON_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)
