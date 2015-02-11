/* Copyright 2015 Myles Salholm */
#pragma once
#include <cstdlib>

/*
A generic dynamic array type for most data.
*/
template<typename Type>
class TArray
{
public:
	/* Constructor. */
	TArray();
	/* Destructor. */
	~TArray();
	
	/* Returns the number of elements in the array. */
	unsigned int Num() const;

	/* Adds a new element to the array. */
	void Add(Type& InItem);

	/* Returns an element, if any, in the array for the given index. */
	Type* Get(unsigned int Index) const;

	/* Returns true if this array is holding the given element. */
	bool Contains(Type& InItem) const;

	/* Finds and removes the element from the array if it exists. */
	void RemoveAt(unsigned int Index);

	/* Finds and removes the element from the array if it exists. */
	void Remove(Type& InItem);

	/* Removes all elements of the array. */
	void Empty();

private:
	/* The elements filled in this array. */
	Type* Elements;

	/* The number of elements used in this array. */
	unsigned int ArraySize;

	/* The actual byte count used in this array. */
	unsigned int DataSize;

	/*
	How much buffer to give when expanding the array elements.
	Also the default buffer size.
	*/
	static const int ArrayBufferStep = 128;

	/* When adding memory buffer, how much extra slack to give it. */
	static const int ArrayBufferMultiplier = 2;
};

/* Template definitions. */
template<typename Type>
TArray<Type>::TArray()
{
	DataSize = ArrayBufferStep;
	ArraySize = 0;

	Elements = (Type*)malloc(DataSize * sizeof(Type));

	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
}

template<typename Type>
TArray<Type>::~TArray()
{
	if (Elements)
	{
		// Free our memory.
		free(Elements);
		Elements = nullptr;
	}
}

template<typename Type>
unsigned int TArray<Type>::Num() const
{
	return ArraySize;
}

template<typename Type>
void TArray<Type>::Add(Type& InItem)
{
	ArraySize++;

	if (ArraySize > DataSize)
	{
		DataSize = DataSize * ArrayBufferMultiplier;

		Elements = (Type*)realloc(Elements, sizeof(Type) * DataSize);
		
		if (!Elements)
		{
			// Failed to allocate memory.
			throw 0;
		}
	}

	Elements[ArraySize - 1] = InItem;
}

template<typename Type>
Type* TArray<Type>::Get(unsigned int Index) const
{
	if (ArraySize < Index)
	{
		// Bad index
		return nullptr;
	}

	return &Elements[Index];
}

template<typename Type>
bool TArray<Type>::Contains(Type& InItem) const
{
	for (unsigned int i = 0; i < ArraySize; ++i)
	{
		if (&Elements[i] == &InItem)
		{
			return true;
		}
	}
	return false;
}

template<typename Type>
void TArray<Type>::RemoveAt(unsigned int Index)
{
	// First check if we're empty.
	if (ArraySize == 0)
	{
		return;
	}

	if (ArraySize == 1)
	{
		Empty();
	}
	else
	{
		// Pop the particular element out and move the others down.
		for (unsigned int i = Index; i < ArraySize; ++i;)
		{
			Elements[i] = Elements[i + 1];
		}

		ArraySize--;
	}
}

template<typename Type>
void TArray<Type>::Remove(Type& InItem)
{
	for (unsigned int i = 0; i < ArraySize; ++i)
	{
		if (Elements[i] == InItem)
		{
			RemoveAt(i);
			return;
		}
	}
}

template<typename Type>
void TArray<Type>::Empty()
{
	// Reallocate to the the original array size with no elements.
	ArraySize = 0;

	Elements = (Type*)realloc(Elements, sizeof(Type) * ArrayBufferStep);
	DataSize = ArrayBufferStep;
}