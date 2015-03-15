/* Copyright 2015 Myles Salholm */
#include "Runtime/Public/Input/InputHandle.h"

FInputHandle::FInputHandle()
{
	
}

FInputHandle::~FInputHandle()
{

}

void FInputHandle::Init()
{
	for (int i = 0; i < KEYS; ++i)
	{
		KeyStates[i] = false;
	}
}

void FInputHandle::KeyDown(unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key] = true;
	}
}

void FInputHandle::KeyUp(unsigned int Key)
{
	if (Key <= KEYS)
	{
		KeyStates[Key] = false;
	}
}

bool FInputHandle::IsKeyDown(unsigned int Key)
{
	if (Key <= KEYS)
	{
		return KeyStates[Key];
	}

	// That key doesn't exist.
	return false;
}