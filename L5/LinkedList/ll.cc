#include "ll.h" 
#include <cstdlib>

//void LinkedList::Link(unsigned uiData, Link *pNext) : m_uiData(uiData), m_pNext(pNext) { }

LinkedList::LinkedList() : m_pHead(nullptr) { }
    //this->m_pHead = nullptr;			// This linked list is empty.

bool LinkedList::insert(unsigned uiData) {
    Link* new_link = new Link();			// Get a new node.
    m_uiData = uiData;
    //new_link->initialize(uiData, this->m_pHead); // Fill it with data.
    m_pHead = new_link;			// Put it at the head.

    return true;				// Indicate success.
}

bool LinkedList::remove(unsigned *pData) {
    if (!this->m_pHead)				// Empty list?
	return false;				// Indicate failure.

    Link *temp = this->m_pHead;			// Point to the first node.
    this->m_pHead = this->m_pHead->m_pNext;	// Remove the first node.
    *pData = temp->m_uiData;			// Obtain first node’s data.
    delete(temp);

    return true;				// Indicate success.
}
