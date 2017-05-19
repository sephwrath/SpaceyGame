// DigitalCounter.h: interface for the DigitalCounter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIGITALCOUNTER_H__DA333350_3B17_4CF5_BE21_7C36E5835146__INCLUDED_)
#define AFX_DIGITALCOUNTER_H__DA333350_3B17_4CF5_BE21_7C36E5835146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include "Drawable.h"
	
class DigitalCounter 
{
private:
	int score;
	char textScore[15];
	int current;
	RECT bounds;
	int maxDigits;

	Drawable digit;

public:
	DigitalCounter(int xPos, int yPos, int digits, int startSize);
	virtual ~DigitalCounter();
	
	void Update(LPDIRECTDRAWSURFACE7 lpDDS_Dest,
		LPDIRECTDRAWSURFACE7 lpDDS_Source);
	void SetSize(int size) {score = size;}
	void ChangeNumber(int adjustment) {score += adjustment;}
	int GetSize() {return score;}
	int ConvertToIndex(char chr);


};

#endif // !defined(AFX_DIGITALCOUNTER_H__DA333350_3B17_4CF5_BE21_7C36E5835146__INCLUDED_)
