/* Copyright 2015 Myles Salholm */
#pragma once

/*
A generic dynamic array type for most data.
*/
template<typename Type>
class TArray
{
public:
	/* Constructor. */
	TArray() : Elements(nullptr)
	{ }
	/* Destructor. */
	~TArray()
	{
		Empty();
		Elements = nullptr;
	}

	/* Adds a new element to the array. */
	void Add(Type* InItem);

	/* Creates and adds a new element to the array. */
	void CreateAndAdd();

	/* Returns an element, if any, in the array for the given index. */
	Type& Get(int Index);

	/* Finds and removes the element from the array if it exists. */
	void Remove(Type InItem);

	/* Removes all elements of the array. */
	void Empty();

private:
	Type* Elements;
};