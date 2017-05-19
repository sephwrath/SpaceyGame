// Frame.h: interface for the Frame class.
//	
//	Old initalisers for the drawable objects
//		Base (0, 156, 174, 331, true),
//		Horizontal (0, 332, 391, 340, true),
//		Vertical (442, 0, 450, 346, true),
//		Corner (252, 84, 260, 92, true),
//		CaptionLeft (268, 30, 276, 50, true),
//		CaptionMiddle (276, 30, 420, 50, true),
//		CaptionRight (420, 30, 424, 50, true)
//	
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_)
#define AFX_FRAME_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_



#include "Text.h"
#include "window.h"

	const int BWIDTH =	8;	//width of the borders
	const int CWIDTH =	30;	//width of the caption frame
	const int ENDWIDTH = 10; //width of the caption terminator

	class Frame : public Window
{
private:
	int minySize;
	int minxSize;
	int captionWidth;

	int saveX, saveY;

	Drawable Base;
	Drawable Horizontal;
	Drawable VerticalL;
	Drawable VerticalR;
	Drawable Corner;
	Drawable CaptionLeft;
	Drawable CaptionSpace;
	Drawable CaptionMiddle;
	Drawable CaptionRight;
	Text title;
	char titleText[40];

	Window *currentFocus;
	Window *currentInputFocus;

	bool moveable;

	//Private Functions
	void UpdateClient();

public:
	Frame(RECT bound, const char *CaptionText, bool move);
	virtual ~Frame();

	void SetPos(RECT bounds);
	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source);
	void SendFocusToFront();
	//void LostFocus();
	//void SendToFront();

	void SetMoveable(bool move) {moveable = move;}
	bool GetMoveable() {return moveable;}
	void SetInputFocus(const Window* newFocus);

//	virtual int  wm_paint(int x, int y); 
	int  wm_mousemove(int x, int y);
	int  wm_lbuttondown(int x, int y);
	int  wm_lbuttonup(int x, int y);
	int  wm_cursortimer();
	int  wm_keychar(const char* str);
	int  wm_losefocus();
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

	Frame::Frame(RECT bound, const char CaptionText[], bool move)
	:Base (16, 0, 79, 63, false),
	 Horizontal (0, 285, 573, 292, false),
	 VerticalL (0, 0, 7, 284, false),
	 VerticalR (8, 0, 15, 284, false),
	 Corner (430, 20, 439, 29, true),
	 CaptionLeft (80, 0, 90, 29, true),
	 CaptionSpace (91, 0, 339, 29, false),
	 CaptionMiddle (80, 30, 464, 59, false),
	 CaptionRight (465, 30, 475, 59, true),
	 title (2, 0)
//-------------------------------------------------------------------
// Initalises the object
//-------------------------------------------------------------------
{
	currentFocus = 0;
	currentInputFocus = 0;
	bounds = bound;
	wndParent = 0;

	//set the window so it can be seen and perhaps moved.
	SetVisible(true);
	moveable = move;

	saveX = -1;
	saveY = -1;

	if (CaptionText != NULL)
	{
		strcpy(titleText, CaptionText);
		captionWidth = title.GetWidth(titleText) + 30;
	}
	this->UpdateClient();

	return;
}
//*******************************************************************


Frame::~Frame()
//-------------------------------------------------------------------
// Destroys the object the subwindows are automatically destroyed when
// the the baseclass destructor is called.
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************


void Frame::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
// Redraws the window by calling each members draw routine
// the window is drawn --->--->
//                             |
//                     <---<---
//					   |
//					   ---->--->
//-------------------------------------------------------------------
{
	RECT TempRect;

	if (captionWidth >= 0)
	{
		//Draw top left corner.
		SetRect(&TempRect, bounds.left, bounds.top,
			client.left , client.top);
		CaptionLeft.SetPos(TempRect);
		CaptionLeft.Draw(lpDDS_Dest, lpDDS_Source, 0);

		//Draw caption frame
		TempRect.left = client.left;
		TempRect.right = client.left + captionWidth;
		CaptionSpace.SetPos(TempRect);
		CaptionSpace.Draw(lpDDS_Dest, lpDDS_Source, 0);

		//Draw caption end
		//TempRect.left = TempRect.right;
		//TempRect.right += ENDWIDTH;
		//CaptionRight.SetPos(TempRect);
		//CaptionRight.Draw(lpDDS_Dest, lpDDS_Source, 0);

		//Draw caption left filler
		TempRect.left = TempRect.right;
		TempRect.right = client.right;
		//TempRect.top = client.top - BWIDTH;
		CaptionMiddle.SetPos(TempRect);
		CaptionMiddle.Draw(lpDDS_Dest, lpDDS_Source, 0);
		
		//Draw top right corner
		TempRect.left = TempRect.right;
		TempRect.right = bounds.right;
		CaptionRight.SetPos(TempRect);
		CaptionRight.Draw(lpDDS_Dest, lpDDS_Source, 0);

		//Print the frames caption
		title.DrawString(titleText, bounds.left + BWIDTH + 10,
		bounds.top + 10, lpDDS_Dest, lpDDS_Source);
	}
	else
	{
		//Draw top left corner.
		SetRect(&TempRect, bounds.left, bounds.top,
			client.left, client.top);
		Corner.SetPos(TempRect);
		Corner.Draw(lpDDS_Dest, lpDDS_Source, DDBLTFX_MIRRORUPDOWN);
		
		//Draw top horizontal bar
		TempRect.left = TempRect.right;
		TempRect.right = client.right;
		Horizontal.SetPos(TempRect);
		Horizontal.Draw(lpDDS_Dest, lpDDS_Source, 0);
	
		//Draw top right corner
		TempRect.left = TempRect.right;
		TempRect.right = bounds.right;
		Corner.SetPos(TempRect);
		Corner.Draw(lpDDS_Dest, lpDDS_Source, DDBLTFX_MIRRORLEFTRIGHT|DDBLTFX_MIRRORUPDOWN);

	}
	
	//Draw the right vertical bar
	TempRect.top = client.top;
	TempRect.bottom =client.bottom;
	VerticalR.SetPos(TempRect);
	VerticalR.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the background base
	TempRect.right = client.right;
	TempRect.left = client.left;
	Base.SetPos(TempRect);
	Base.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the left vertical bar
	TempRect.right = client.left;
	TempRect.left = bounds.left;
	VerticalL.SetPos(TempRect);
	VerticalL.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the bottom left corner
	TempRect.top = client.bottom;
	TempRect.bottom = bounds.bottom;
	Corner.SetPos(TempRect);
	Corner.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the bottom horizontal bar
	TempRect.left = TempRect.right;
	TempRect.right = client.right;
	Horizontal.SetPos(TempRect);
	Horizontal.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the bottom right corner
	TempRect.left = TempRect.right;
	TempRect.right = bounds.right;
	Corner.SetPos(TempRect);
	Corner.Draw(lpDDS_Dest, lpDDS_Source, DDBLTFX_MIRRORLEFTRIGHT);


	//Redraw all of the sub windows as well
	Window::Update(lpDDS_Dest, lpDDS_Source);

}
//*******************************************************************


void Frame::UpdateClient()
//-------------------------------------------------------------------
// Updates the client rect according to the position of the bounding
// rect - an Private function used only by this class
//-------------------------------------------------------------------
{
	if (captionWidth >= 0)
	{
		client.top = bounds.top + CWIDTH;
	}
	else
	{
		client.top = bounds.top + BWIDTH;
	}
	client.left = bounds.left + BWIDTH;
	client.right = bounds.right - BWIDTH;
	client.bottom = bounds.bottom - BWIDTH-1;
	
}
//*******************************************************************


void Frame::SetPos(RECT bound)
//-------------------------------------------------------------------
// Set the position of the window, the box that contains the entire
// window. Sets the positions of the window components so they are all
// placed correctly within the bounding box.
//-------------------------------------------------------------------
{
	bounds = bound;
	this->UpdateClient();
}
//*******************************************************************


void Frame::SetInputFocus(const Window* newFocus)
//-------------------------------------------------------------------
// Set the input focus to the window pointed to by newFocus
//-------------------------------------------------------------------
{
	currentInputFocus = (Window*)newFocus;
	currentInputFocus->wm_gaininputfocus();
}
//*******************************************************************


int Frame::wm_lbuttondown(int x, int y)
//-------------------------------------------------------------------
// handles the left button down messagefor a button. Changes the button
// bitmap either when the button is clicked or when the mouse is over
// it.
//-------------------------------------------------------------------
{
	if (this->InBounds(x, y) && (!this->InClient(x, y)) && moveable)
	{
		saveX = x;
		saveY = y;
		Window::flgStaticFocus = true;
	}
	else
	{
		//test to see if there are any objects in the list before trying to delete them
		if (!subWindows.SetPosHead())
			return 1;
		windowPtr = reinterpret_cast<Window*>(subWindows.GetCurrentItem());
		do
		{
			//see if the click was in the bounds
			if (windowPtr->InBounds(x, y))
			{
					//then change image sprite.
					windowPtr->wm_lbuttondown(x, y);
					return 0;

			}
		}while (windowPtr = reinterpret_cast<Window*>(subWindows.GetNext()));
	}
	return 0;
}
//***********************************************************************


int Frame::wm_lbuttonup(int x, int y)
//-------------------------------------------------------------------
// handles the left button up message for a button.
//-------------------------------------------------------------------
{
	// test to see if the button is clicked int he frames border and if it is meant to be moved
	// before allowing it to be moved
	if (this->InBounds(x, y) && (!this->InClient(x, y)) && moveable)
	{
		saveY = saveX = -1;
		Window::flgStaticFocus = false;
	}
	else
	{
		//test to see if there are any objects in the list before trying to delete them
		if (!subWindows.SetPosHead())
			return 1;

		//if the mouse is not clicked on input focus object then it loses input focus
		if ((currentInputFocus != 0) && (!currentInputFocus->InBounds(x, y)))
		{
			currentInputFocus->wm_loseinputfocus();
			currentInputFocus = 0;
		}

		windowPtr = reinterpret_cast<Window*>(subWindows.GetCurrentItem());
		do
		{
			//see if the click was in the bounds
			if (windowPtr->InBounds(x, y))
			{
				//then change image sprite.
				windowPtr->wm_lbuttonup(x, y);

				//if the object does not currently have input focus then focus it.
				if(currentInputFocus != windowPtr)
				{
					windowPtr->wm_gaininputfocus();
					currentInputFocus = windowPtr;
				}
				return 0;
			}
		}while (windowPtr = reinterpret_cast<Window*>(subWindows.GetNext()));
	}

	return 0;
}
//***********************************************************************


int Frame::wm_mousemove(int x, int y)
//-------------------------------------------------------------------
// handles the mouse movement tracking message
//-------------------------------------------------------------------
{
	//Updates the position of the frame and sets the drawflag so it is redrawn.
	if (saveX != -1)
	{
		saveX -= x;
		saveY -= y;
		bounds.bottom -= saveY;
		bounds.top -= saveY;
		bounds.left -= saveX;
		bounds.right -= saveX;
		client.bottom -= saveY;
		client.top -= saveY;
		client.left -= saveX;
		client.right -= saveX;
		Window::drawFlag = true;
	}
	else
	{
		//test to see if there are any objects in the list before trying to delete them
		if (!subWindows.SetPosHead())
			return 1;

		//if the mouse is not over the focused object then it loses focus
		if ((currentFocus != 0) && (!currentFocus->InBounds(x, y)))
		{
			currentFocus->wm_losefocus();
			currentFocus = 0;
		}

		windowPtr = reinterpret_cast<Window*>(subWindows.GetCurrentItem());
		do
		{
			//see if the mouse is in the bounds
			if (windowPtr->InBounds(x, y))
			{
				//call the appropriate message handler.
			
				//if the object is not currently focused then focus it.
				if(currentFocus != windowPtr)
				{
					windowPtr->wm_gainfocus();
					currentFocus = windowPtr;
				}

				windowPtr->wm_mousemove(x,y);
				return 0;
			}
		}while (windowPtr = reinterpret_cast<Window*>(subWindows.GetNext()));
	}

	return 0;
}
//***********************************************************************


int Frame::wm_keychar(const char* str)
//-------------------------------------------------------------------
// handles the coursor timer messages
//-------------------------------------------------------------------
{
	if (currentInputFocus != 0)
	{
		currentInputFocus->wm_keychar(str);
	}
	return 0;
}
//***********************************************************************


int Frame::wm_cursortimer()
//-------------------------------------------------------------------
// handles the coursor timer messages
//-------------------------------------------------------------------
{
	if (currentInputFocus != 0)
	{
		currentInputFocus->wm_cursortimer();
	}
	return 0;
}
//***********************************************************************


int Frame::wm_losefocus()
//-------------------------------------------------------------------
// Gets rid of any focus items if any are set
//-------------------------------------------------------------------
{
	if (currentFocus != 0)
	{
		currentFocus->wm_losefocus();
		currentFocus = 0;
	}
	return 0;
}
//***********************************************************************


void Frame::SendFocusToFront()
//-------------------------------------------------------------------
// Sends the currently focused object to the front of the list enabling
// it to receive the first messages. Used by an object which may temporarily
// overlapp other items in the frame.
//-------------------------------------------------------------------
{
	subWindows.MoveToFront();
	return;
}
//***********************************************************************

#endif // !defined(AFX_FRAME_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_)
