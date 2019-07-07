#ifndef LINKED_LIST_H
#define LINKED_LIST_H

class LinkedList {
  public:
    LinkedList();
    bool insert(unsigned uiData);
    bool remove(unsigned *pData);

  private:
    struct Link {
	unsigned m_uiData;
	Link *m_pNext;
    } *m_pHead; //Try accessing  this
};

#endif /* LINKED_LIST_H */ 
