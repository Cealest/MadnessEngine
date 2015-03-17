/* Copyright 2015 Myles Salholm */
#pragma once

/*
A buffer with a limited size and whose head leads to its tail.
*/
template<typename Type>
class TRingBuffer
{
public:
	/* Constructor. */
	TRingBuffer();
	/* Constructor with buffer size specifier. */
	TRingBuffer(const unsigned int InBufferSize);
	/* Constructor with the option to enable overwriting. */
	TRingBuffer(bool InOverwrite);
	/* Constructor with buffer size specifier and the option to enable overwriting. */
	TRingBuffer(const unsigned int InBufferSize, bool InOverwrite);
	/* Destructor. */
	~TRingBuffer();
	/* Copy constructor. */
	TRingBuffer(const TRingBuffer &InBuffer);
	/* Assignment operator. */ 
	TRingBuffer& operator = (const TRingBuffer &InBuffer);

	/* Returns the number of elements in the buffer. */
	unsigned int Num() const;
	
	/* Sets the buffer's size. */
	void SetSize(const unsigned int InBufferSize);

	/* Gets the buffer's size. */
	unsigned int GetSize();
	
	/* Pushes a new element to the front of the buffer. */
	bool Push(const Type& InElement);
	
	/* Pop the last element out of the buffer. */
	bool Pop(Type& OutElement);
	
	/* Removes all data from the buffer. */
	void Empty();
	
	/* Returns true if the buffer is empty. */
	bool IsEmpty() const;
	
private:
	/* The actual array of elements. */
	Type* Elements;
	
	/* The number of elements in the buffer. */
	unsigned int BufferSize;
	/* The buffer size, by default, when not specified. */
	static const unsigned int DefaultBufferSize = 128;
	
	/* The first valid element. */
	unsigned int FrontIndex;
	
	/* The last valid element. */
	unsigned int BackIndex;

	/* Whether or not any data is valid. */
	bool bEmpty;

	/* Whether or not pushing can overwrite old data. */
	bool bOverwrite;
};

template<typename Type>
TRingBuffer<Type>::TRingBuffer()
{
	BufferSize = DefaultBufferSize;
	Elements = (Type*)malloc(sizeof(Type) * BufferSize);
	
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	
	FrontIndex = 0;
	BackIndex = 0;
	bEmpty = true;
	bOverwrite = false;
}

template<typename Type>
TRingBuffer<Type>::TRingBuffer(const unsigned int InBufferSize)
{
	BufferSize = InBufferSize;
	Elements = (Type*)malloc(sizeof(Type) * BufferSize);
	
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	
	FrontIndex = 0;
	BackIndex = 0;
	bEmpty = true;
	bOverwrite = false;
}

template<typename Type>
TRingBuffer<Type>::TRingBuffer(bool InOverwrite)
{
	BufferSize = DefaultBufferSize;
	Elements = (Type*)malloc(sizeof(Type) * BufferSize);

	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}

	FrontIndex = 0;
	BackIndex = 0;
	bEmpty = true;
	bOverwrite = InOverwrite;
}

template<typename Type>
TRingBuffer<Type>::TRingBuffer(const unsigned int InBufferSize, bool InOverwrite)
{
	BufferSize = InBufferSize;
	Elements = (Type*)malloc(sizeof(Type) * BufferSize);

	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}

	FrontIndex = 0;
	BackIndex = 0;
	bEmpty = true;
	bOverwrite = InOverwrite;
}

template<typename Type>
TRingBuffer<Type>::~TRingBuffer()
{
	if (Elements)
	{
		free(Elements);
		Elements = nullptr;
	}
}

template<typename Type>
TRingBuffer<Type>::TRingBuffer(const TRingBuffer &InBuffer)
{
	Elements = (Type*)malloc(sizeof(Type) * InBuffer.BufferSize);
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	
	memcpy(Elements, InBuffer.Elements, sizeof(Type) * InBuffer.BufferSize);
	BufferSize = InBuffer.BufferSize;
	FrontIndex = InBuffer.FrontIndex;
	BackIndex = InBuffer.BackIndex;
	bEmpty = InBuffer.bEmpty;
}

template<typename Type>
TRingBuffer<Type>& TRingBuffer<Type>::operator = (const TRingBuffer &InBuffer)
{
	// If this is the same object, just return.
	if (this == &InBuffer)
	{
		return *this;
	}
	
	if (InBuffer.IsEmpty())
	{
		Empty();
	}
	
	SetSize(InBuffer.BufferSize);
	
	memcpy(Elements, InBuffer.Elements, sizeof(Type) * InBuffer.BufferSize);
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	FrontIndex = InBuffer.FrontIndex;
	BackIndex = InBuffer.BackIndex;
	bEmpty = InBuffer.bEmpty;
	return *this;
}

template<typename Type>
unsigned int TRingBuffer<Type>::Num() const
{
	if (bEmpty)
	{
		return 0;
	}
	else if (BackIndex == FrontIndex)
	{
		return 1;
	}
	else if (BackIndex > FrontIndex)
	{
		return (BackIndex - FrontIndex) + 1;
	}
	else if (BackIndex < FrontIndex)
	{
		return ((BufferSize - FrontIndex) + BackIndex) + 1;
	}
}

template<typename Type>
void TRingBuffer<Type>::SetSize(const unsigned int InBufferSize)
{
	if (BufferSize == InBufferSize)
	{
		return;
	}

	BufferSize = InBufferSize;
	Elements = (Type*)realloc(Elements, sizeof(Type) * InBufferSize);
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	bEmpty = true;
	FrontIndex = 0;
	BackIndex = 0;
}

template<typename Type>
unsigned int GetSize()
{
	return BufferSize;
}

template<typename Type>
bool TRingBuffer<Type>::Push(const Type& InElement)
{
	if (bEmpty)
	{
		Elements[BackIndex] = InElement;
		bEmpty = false;
		return true;
	}

	if ((BackIndex + 1) > BufferSize)
	{
		if (FrontIndex == 0)
		{
			if (!bOverwrite)
			{
				return false;
			}
			FrontIndex++;
		}
		BackIndex = 0;
	}
	else if (BackIndex + 1 == FrontIndex)
	{
		if (!bOverwrite)
		{
			return false;
		}
		FrontIndex++;
	}
	else
	{
		BackIndex++;
	}

	Elements[BackIndex] = InElement;
	return true;
}

template<typename Type>
bool TRingBuffer<Type>::Pop(Type& OutElement)
{
	if (bEmpty)
	{
		return false;
	}

	OutElement = Elements[FrontIndex];

	if ((FrontIndex + 1) > BufferSize)
	{
		if (BackIndex == 0)
		{
			bEmpty = true;
			FrontIndex = 0;
			return true;
		}
		FrontIndex = 0;
	}
	else
	{
		FrontIndex++;
	}
	
	return true;
}

template<typename Type>
void TRingBuffer<Type>::Empty()
{
	Elements = (Type*)realloc(Elements, sizeof(Type) * BufferSize);
	if (!Elements)
	{
		// Failed to allocate memory.
		throw 0;
	}
	bEmpty = true;
	FrontIndex = 0;
	BackIndex = 0;
}

template<typename Type>
bool TRingBuffer<Type>::IsEmpty() const
{
	return bEmpty;
}