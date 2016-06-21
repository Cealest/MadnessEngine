/* Copyright 2015 Myles Salholm */
#pragma once
#include <cstdlib>

/*
A generic linked list type for pointer lists.
*/
template<typename Type>
class TLinkedList
{
	/* Constructor. */
	TLinkedList();
	/* Destructor. */
	~TLinkedList();
	/* Copy constructor. */
	TLinkedList(const TLinkedList &List);
	/* Assignment operator. */
	TLinkedList& operator = (const TLinkedList &List);

	//@TODO
	// Next
	// Add
	// Remove
	// Empty
	// IsEmpty
	// Contains

private:
	/* First element in the list. */
	Type* Head;
	/* Last element in the list. */
	Type* Tail;
};