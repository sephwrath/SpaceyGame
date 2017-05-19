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

#if !defined(AFX_CONTAINER_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_)
#define AFX_CONTAINER_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_


#include "Window.h"


class Container : public Window
{
private:
	int minySize;
	int minxSize;

	Drawable Horizontal;
	Drawable Vertical;

	Window *currentFocus;
	Window *currentInputFocus;

	//Private Functions

public:
	Container(const Frame *container, RECT bound);
	virtual ~Container();

	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source);
	void SendFocusToFront();
	void SendToFront();

//	virtual int  wm_paint(int x, int y); 
	int  wm_mousemove(int x, int y);
	int  wm_lbuttondown(int x, int y);
	int  wm_lbuttonup(int x, int y);
	int  wm_cursortimer();
	int  wm_keychar(char* str);
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

Container::Container(const Frame *container, RECT bound)
	:Horizontal (89, 142, 114, 143, true),
	 Vertical (89, 142, 90, 166, true)
//-------------------------------------------------------------------
// Initalises the object
//-------------------------------------------------------------------
{
	currentFocus = 0;
	currentInputFocus = 0;
	bounds = bound;
	wndParent = (Window*)container;

	//set the window so it can be seen.
	SetVisible(true);


	client = bounds;

	return;
}
//*******************************************************************


Container::~Container()
//-------------------------------------------------------------------
// Destroys the object
//-------------------------------------------------------------------
{
	return;
}
//*******************************************************************


void Container::Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest, LPDIRECTDRAWSURFACE7 lpDDS_Source)
//-------------------------------------------------------------------
// Redraws the window by calling each members draw routine
// the window is drawn --->--->
//                             |
//                     <---<---
//					   |
//					   ---->--->
//-------------------------------------------------------------------
{
	RECT pArea = wndParent->GetClient();
	RECT gblBounds, TempRect;

	// Set up the current golbal boundaries according to the parent rect this enables the
	// graphics to be stored relative to the parent window and drawn using global coordinates
	// like those accepted by the Draw function of the Drawale object.
	SetRect(&gblBounds, bounds.left + pArea.left, bounds.top + pArea.top,
					   bounds.right + pArea.left, bounds.bottom + pArea.top);
		
	//Draw top horizontal bar
	SetRect(&TempRect, gblBounds.left, gblBounds.top, gblBounds.right, gblBounds.top + 1);
	Horizontal.SetPos(TempRect);
	Horizontal.Draw(lpDDS_Dest, lpDDS_Source, 0);
	
	//Draw the right vertical bar
	SetRect(&TempRect, gblBounds.right - 1, gblBounds.top, gblBounds.right, gblBounds.bottom);
	Vertical.SetPos(TempRect);
	Vertical.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the left vertical bar
	SetRect(&TempRect, gblBounds.left, gblBounds.top, gblBounds.left + 1, gblBounds.bottom);
	Vertical.SetPos(TempRect);
	Vertical.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Draw the bottom horizontal bar
	SetRect(&TempRect, gblBounds.left, gblBounds.bottom - 1, gblBounds.right, gblBounds.bottom);
	Horizontal.SetPos(TempRect);
	Horizontal.Draw(lpDDS_Dest, lpDDS_Source, 0);

	//Redraw all of the sub windows as well
	Window::Update(lpDDS_Dest, lpDDS_Source);

}
//*******************************************************************


int Container::wm_lbuttondown(int x, int y)
//-------------------------------------------------------------------
// handles the left button down messagefor a frame
//-------------------------------------------------------------------
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

	return 0;
}
//***********************************************************************


int Container::wm_lbuttonup(int x, int y)
//-------------------------------------------------------------------
// handles the left button up message for the frame.
//-------------------------------------------------------------------
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

	return 0;
}
//***********************************************************************


int Container::wm_mousemove(int x, int y)
//-------------------------------------------------------------------
// handles the mouse movement tracking message
//-------------------------------------------------------------------
{
	//test to see if there are any objects in the list before trying to manipulate them
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

	return 0;
}
//***********************************************************************


int Container::wm_keychar(char* str)
//-------------------------------------------------------------------
// handles the key messages
//-------------------------------------------------------------------
{
	if (currentInputFocus != 0)
	{
		currentInputFocus->wm_keychar(str);
	}
	return 0;
}
//***********************************************************************


int Container::wm_cursortimer()
//-------------------------------------------------------------------
// handles the curser timer messages
//-------------------------------------------------------------------
{
	if (currentInputFocus != 0)
	{
		currentInputFocus->wm_cursortimer();
	}
	return 0;
}
//***********************************************************************


void Container::SendFocusToFront()
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


void Container::SendToFront()
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

#endif // !defined(AFX_CONTAINER_H__99158B21_092A_11D5_822B_F43D624BDD37__INCLUDED_)
