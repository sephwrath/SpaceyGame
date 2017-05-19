// CheckBox.h: interface for the checkbox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKBOX_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)
#define AFX_CHECKBOX_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Window.h"


class CheckBox : public Window
{
private:
	int minySize;
	int minxSize;
	int captionWidth;

	//Frame *parent;
	//int xLoc, yLoc;
	Drawable BaseNorm;
	Drawable BaseOver;
	Drawable BaseClick;
	Drawable CheckMark;
	Text title;
	char titleText[40];
	int textWidth;
	
	bool checked;
	BUTTON_STATE checkState;
public:
	CheckBox(const Frame *container, int xPos, int yPos, char CaptionText[],
		bool check);
	virtual ~CheckBox();
	
	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest,
		LPDIRECTDRAWSURFACE7 lpDDS_Source);

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

CheckBox::CheckBox(const Frame *container ,int xPos, int yPos, char CaptionText[],
			   bool check)
	:BaseNorm (450, 60, 490, 100, false),
	 BaseOver (450, 101, 490, 141, false),
	 BaseClick (450, 142, 490, 182, false),
	 CheckMark (429, 0, 444, 15, false),
	 title (0, -4)
//-------------------------------------------------------------------
// Object constructor
//-------------------------------------------------------------------
{
	checkState = NORMAL;
	checked = check;

	if (CaptionText != NULL)
	{
		strcpy(titleText, CaptionText);
		textWidth = title.GetWidth(titleText);
	}

	wndParent = (Window*)container;
	
	SetRect(&bounds, xPos, yPos, xPos + textWidth + 25, yPos + 20);
	SetRect(&client, bounds.left, bounds.top, bounds.left + 20, bounds.bottom);
	
	return;
}
//*******************************************************************


CheckBox::~CheckBox()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************


void CheckBox::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
// Redraws the button by calling the draw routine of the appropriate
// member.
//-------------------------------------------------------------------
{
	RECT pArea = wndParent->GetClient();
	RECT gblBounds, gblClient;

	//Set up the current golbal boundaries according to the parent rect
	SetRect(&gblBounds, bounds.left + pArea.left, bounds.top + pArea.top,
					   bounds.right + pArea.left, bounds.bottom + pArea.top);
	SetRect(&gblClient, client.left + pArea.left, client.top + pArea.top,
					   client.right + pArea.left, client.bottom + pArea.top);

	switch (checkState)
	{
		case OVER:
			BaseOver.SetPos(gblClient);
			BaseOver.Draw(lpDDS_Dest, lpDDS_Source, 0);
			break;

		case CLICKED:
			BaseClick.SetPos(gblClient);
			BaseClick.Draw(lpDDS_Dest, lpDDS_Source, 0);
			break;
		
		case NORMAL:
			BaseNorm.SetPos(gblClient);
			BaseNorm.Draw(lpDDS_Dest, lpDDS_Source, 0);
			break;
	}
	
	if (checked == true)
	{
		RECT temp;
		SetRect(&temp, gblClient.left + 4, gblClient.top + 4,
			gblClient.right - 4, gblClient.bottom - 4);
		CheckMark.SetPos(temp);
		CheckMark.Draw(lpDDS_Dest, lpDDS_Source, 0);
	}

	title.DrawString(titleText, gblBounds.left + 25,
		gblBounds.top + 5, lpDDS_Dest, lpDDS_Source);
	
}


int CheckBox::wm_lbuttondown(int x, int y)
//-------------------------------------------------------------------
// handles the left button down messagefor a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it.
//-------------------------------------------------------------------
{
	if (checkState != CLICKED)
	{
		checkState = CLICKED;
		Window::drawFlag = true;	
	}
	return 1;
}
//***********************************************************************


int CheckBox::wm_lbuttonup(int x, int y)
//-------------------------------------------------------------------
// handles the left button up message for a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it.
//-------------------------------------------------------------------
{
	if (checkState == CLICKED)
	{
		PlayWAVInfo(lpwiClick, 0);
		if (checked == true)
		{
			checked = false;
		}
		else
		{
			checked = true;
		}
	}

	checkState = OVER;
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************


int CheckBox::wm_gainfocus()
//-------------------------------------------------------------------
// handles the left button down messagefor a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it.
//-------------------------------------------------------------------
{
	checkState = OVER;
	PlayWAVInfo(lpwiActive, 0);
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************


int CheckBox::wm_losefocus()
//-------------------------------------------------------------------
// handles the left button down messagefor a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it.
//-------------------------------------------------------------------
{
	checkState = NORMAL;
	Window::drawFlag = true;	
	return 1;
}
//***********************************************************************

#endif // !defined(AFX_CHECKBOX_H__F205F8C4_07CF_11D5_822B_BCBBEB810C37__INCLUDED_)
