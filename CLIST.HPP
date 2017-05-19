///////////////////////////////////////
// File: clist.cpp
// Author: Stephen M Moran
//
// Date:			Comments 
// 28/6/2001		N/A
///////////////////////////////////////
#ifndef CLIST_HPP_
#define CLIST_HPP_

///////////////////////////////////////
// Class cListNode { cList link }
///////////////////////////////////////
class cListNode {
//friend class cList;	  // allow class cList protected access
  cListNode* next;        // pointer to next node {link}
  cListNode* prev;
  void* element;            // node data element
public:
  cListNode(cListNode *n=0, cListNode *p=0, void* eleType = 0) // constructor
    {next = n; prev = p; element = eleType;}
  ~cListNode() {delete element;}
  cListNode* GetNextPtr() // get link to next node
    { return next; }
  cListNode* GetPrevPtr() // get link to next node
    { return prev; }
  void* GetElement()		  // retrieve the data element from node
    { return element; }
  void SetNextPtr( cListNode* aLink ) // assign the next link pointer
    { next = aLink; }
  void SetPrevPtr( cListNode* aLink ) // assign the next link pointer
    { prev = aLink; }
  void SetElement( void* eleType )      // assign node a data element value
    { element = eleType; }
};

///////////////////////////////////////
// Class cList
///////////////////////////////////////
class cList {
protected:
  cListNode* listHead;  // first link in the list
  cListNode* listTail;
  cListNode* listPos;
  cListNode* listTemp;

  //int listPosNum;
  int numNodes;         // number of elements in the list
  
public:
  cList(void);          // constructor
  ~cList(void) { DeleteList(); }
  virtual void InsertListFront(void* eleType ); // insert a data element into list
  virtual void InsertListBack(void* eleType );
  // inserts before the item the currentPointer is currently pointing to
  virtual void InsertList(void* eleType );

  virtual void MoveToFront();					// moves the item currently bing pointed to to the front
  virtual void MoveToBack();					// opposite to move to back.
  virtual void* GetNext();
  virtual void* GetPrev();
  virtual void* GetHead() {return listHead->GetElement();}
  virtual void* GetTail() {return listTail->GetElement();}
  virtual void* GetCurrentItem();
  virtual bool SetPosHead();
  virtual bool SetPosTail();
  virtual void* RemoveListInc();
  virtual void* RemoveListDec();
  virtual void RemoveList(void* eleType ); // remove a data element from list
  virtual void DisplayList(void);        // display list data elements used for debugging
  int NumNodes() { return numNodes; }    // retrieve count of nodes in list

  virtual void DeleteList(void);
  //virtual int GetListItem(int position, int eleType);
};

#endif

