///////////////////////////////////////
// File: clist.cpp
// Author: Stephen M Moran
//
// Date:			Comments 
// 28/6/2001		N/A
///////////////////////////////////////

///////////////////////////////////////
// include files
///////////////////////////////////////
#include <stdio.h>
#include <assert.h>

#include "clist.hpp"


cList::cList(void)
//-------------------------------------------------------------------
// cList constructor
//-------------------------------------------------------------------
{
	listHead = 0;  // new list is empty
	listTail = 0;
	listPos = 0;
	
	numNodes = 0;     // no nodes in list
}
//***********************************************************************


void cList::InsertListFront(void* eleType)
//-------------------------------------------------------------------
// insert an element into the front of the list
// this operation has no effect on the current position in the list
//-------------------------------------------------------------------
{
	if (listHead == 0)
	{
		listHead = new cListNode(0, 0, eleType);
		listTail = listHead;
		assert(listHead != 0);
	}
	else	// a node has been added to list
	{
		listHead->SetPrevPtr(new cListNode(listHead, 0, eleType));
		listHead = listHead->GetPrevPtr();
		assert(listHead != 0);
	}
	numNodes++;		
}
//***********************************************************************


void cList::InsertListBack(void* eleType)
//-------------------------------------------------------------------
// insert an element into the back of the list
// this operation has no effect on the current position in the list
//-------------------------------------------------------------------
{
	if (numNodes == 0)  // if no nodes just insert as at front.
	{
		this->InsertListFront(eleType);
	}
	else
	{
		listTail->SetNextPtr(new cListNode(0, listTail, eleType));
		listTail = listTail->GetNextPtr();
		assert(listTail != 0);
		numNodes++;
	}
}
//***********************************************************************


void cList::InsertList(void* eleType)
//-------------------------------------------------------------------
// insert an element into the back of the list
//-------------------------------------------------------------------
{
	if (numNodes == 0 || listPos == 0 || listPos == listHead)  // if no nodes just insert as at front, or if listPos is not defined
	{
		this->InsertListFront(eleType);
	}
	else
	{
		listTemp = listPos->GetPrevPtr();
		listPos->SetPrevPtr(new cListNode(listPos, listPos->GetPrevPtr(), eleType));
		listTemp->SetNextPtr(listPos->GetPrevPtr());
		assert(listPos != 0);
		numNodes++;
	}
}
//***********************************************************************


void cList::MoveToFront()
//-------------------------------------------------------------------
// move a list element to the front of the list
// the element that is moved is the one that is currently being pointed to
// when the function returns the pointer is pointing to the next element in the list
//-------------------------------------------------------------------
{
	if (listPos == listHead)
	{
		listPos = listPos->GetNextPtr();
	}
	else
	{
		if (listPos == 0)
			return;

		cListNode* ptrTemp = 0;
		if (listPos == listTail)
		{
			listTail = listPos->GetPrevPtr();
			listTail->SetNextPtr(0);
		}
		else
		{
			ptrTemp = listPos->GetPrevPtr();
			ptrTemp->SetNextPtr(listPos->GetNextPtr());
			ptrTemp = listPos->GetNextPtr();
			ptrTemp->SetPrevPtr(listPos->GetPrevPtr());
		}
			
		listPos->SetNextPtr(listHead);
		listPos->SetPrevPtr(0);
		listHead->SetPrevPtr(listPos);
		listHead = listPos;
		listPos = ptrTemp;
	}
}
//***********************************************************************


void cList::MoveToBack()
//-------------------------------------------------------------------
// move a list element to the back of the list
// the element that is moved is the one that is currently being pointed to
// when the function returns the pointer is pointing to the next element in the list
//-------------------------------------------------------------------
{
	if (listPos == listTail)
	{
		listPos = 0;
	}
	else
	{
		if (listPos == 0)
			return;

		cListNode* ptrTemp = 0;
		if (listPos == listHead)
		{
			listHead = listPos->GetNextPtr();
			listHead->SetPrevPtr(0);
		}
		else
		{
			ptrTemp = listPos->GetPrevPtr();
			ptrTemp->SetNextPtr(listPos->GetNextPtr());
			ptrTemp = listPos->GetNextPtr();
			ptrTemp->SetPrevPtr(listPos->GetPrevPtr());
		}
			
		listPos->SetPrevPtr(listTail);
		listPos->SetNextPtr(0);
		listTail->SetNextPtr(listPos);
		listTail = listPos;
		listPos = ptrTemp;
	}
}
//***********************************************************************


void* cList::GetNext()
//-------------------------------------------------------------------
//	advance the position in the list; for iterating through the list
//	Returns a pointer to the next element unless the end of the list
//	has been previously reached in which case null is returned (0)
//-------------------------------------------------------------------
{
	if ((listPos == listTail) || (listPos == 0))
		return 0;
	else
	{
		listPos = listPos->GetNextPtr();
		return listPos->GetElement();
	}
}
//***********************************************************************


void* cList::GetPrev()
//-------------------------------------------------------------------
//	advance the position in the list; for iterating through the list
//	Returns a pointer to the previous element unless the front of the list
//	has been prevously reached in which case null is returned (0)
//-------------------------------------------------------------------
{
	if ((listPos == listHead) || (listPos == 0))
		return 0;
	else
	{
		listPos = listPos->GetPrevPtr();
		return listPos->GetElement();
	}
}
//***********************************************************************


void* cList::GetCurrentItem()
//-------------------------------------------------------------------
//	returns the item at the current position in the list
//	or spcifically the item that listPos is pointing however doesnt
//	gaurantee that it is pointing to an element
//-------------------------------------------------------------------
{
	return listPos->GetElement();
}
//***********************************************************************


bool cList::SetPosHead()
//-------------------------------------------------------------------
// set the position in the list to the head of the list.
// Returns false if there are no elements in the list otherwise true
//-------------------------------------------------------------------
{
	listPos = listHead;
	if (listPos == 0)
		return false;
	else
		return true;
}
//***********************************************************************


bool cList::SetPosTail()
//-------------------------------------------------------------------
// set the position in the list to the tail of the list.
// Returns false if there are no elements in the list otherwise true
//-------------------------------------------------------------------
{
	listPos = listTail;
	if (listPos == 0)
		return false;
	else
		return true;
}
//***********************************************************************


void* cList::RemoveListInc()
//-------------------------------------------------------------------
// remove an element from the list, the current list element
// it also sets the pointer to the next element in the list and returns the element
// if there is no next element the function returns 0
//-------------------------------------------------------------------
{
	cListNode* ptrTemp = 0;

	if (listPos == 0)
		return 0;
	if (listPos == listHead)
	{
		listHead = listPos->GetNextPtr();
		ptrTemp = listHead;

		// if the head next pointer is NULL then the list is empty so set the tail to NULL as well.
		if (listHead == 0)
		{
			listTail = 0;
		}
		else
		{
			listHead->SetPrevPtr(0);
		}
	}
    else if (listPos == listTail)
	{
		listTail = listPos->GetPrevPtr();
		ptrTemp = 0;
	}
	else
	{
		ptrTemp = listPos->GetPrevPtr();
		ptrTemp->SetNextPtr(listPos->GetNextPtr());
		ptrTemp = listPos->GetNextPtr();
		ptrTemp->SetPrevPtr(listPos->GetPrevPtr());
	}
    delete listPos; // remove node
	listPos = 0;
    --numNodes;	   // decrement list node count
	listPos = ptrTemp;
	if (listPos != 0)
		return listPos->GetElement();
	else 
		return 0;
}
//***********************************************************************


void* cList::RemoveListDec()
//-------------------------------------------------------------------
// remove an element from the list, the current list element
// it also sets the pointer to the previous element in the list and returns the element
// if there is no previous element the function returns 0
//-------------------------------------------------------------------
{
	cListNode* ptrTemp = 0;

	if (listPos == 0)
		return 0;
	
	if (listPos == listTail)
	{
		listTail = listPos->GetPrevPtr();
		ptrTemp = listTail;
		
		// if the tail prev pointer is NULL then the list is empty so set the head to NULL as well.
		if (listTail == 0)
		{
			listHead = 0;
		}
		else
		{
			listTail->SetNextPtr(0);
		}
	}
    else if (listPos == listHead)
	{
		listHead = listPos->GetNextPtr();
		listHead->SetPrevPtr(0);
		ptrTemp = 0;
	} 
	else
	{
		ptrTemp = listPos->GetNextPtr();
		ptrTemp->SetPrevPtr(listPos->GetPrevPtr());
		ptrTemp = listPos->GetPrevPtr();
		ptrTemp->SetNextPtr(listPos->GetNextPtr());
	}

    delete listPos; // remove node
	listPos = 0;
    --numNodes;	   // decrement list node count
	listPos = ptrTemp;
    if (listPos != 0)
		return listPos->GetElement();
	else 
		return 0;
}
//***********************************************************************


void cList::RemoveList( void* eleType)
//-------------------------------------------------------------------
// remove an element from the list the element removed is that pointed
// by the eleType pointer. This form of removal doesnt update the list
// position in any way so after using it the listposition will need to be set
// before it is used
//-------------------------------------------------------------------
{
	cListNode* currLink; 
	cListNode* last;
	last = 0;

	for (currLink = listHead; currLink != 0; currLink = currLink->GetNextPtr())
	{
		if (currLink->GetElement() == eleType)
		{
			// case: first item in list
			if (last == 0)
				listHead = currLink->GetNextPtr();
			else if (currLink == listTail)
				listTail = currLink->GetPrevPtr();
			else
			{
				last->SetNextPtr(currLink->GetNextPtr());
				currLink = last->GetNextPtr();
				currLink->SetPrevPtr(last);
			}
			delete currLink; // remove node
			currLink = 0;
			--numNodes;	   // decrement list node count
			return;
		}
		else
		{
			last = currLink;
		}
  } // for currLink
}
//***********************************************************************


void cList::DisplayList(void)
//-------------------------------------------------------------------
// write list contents to DOS console - only really used for debugging purposes
//-------------------------------------------------------------------
{
	cListNode* currLink = listHead; // init currLink to list head
	
	// iterate the the list
	while(currLink)
	{
		printf("%d\n",currLink->GetElement());
		currLink=currLink->GetNextPtr();
	}
}
//***********************************************************************


void cList::DeleteList(void)
//-------------------------------------------------------------------
// delete any objects that are contained in the list
//-------------------------------------------------------------------
{
	if (!this->SetPosTail())
		return;
	do{}while (this->RemoveListDec());
	return;
}
//***********************************************************************