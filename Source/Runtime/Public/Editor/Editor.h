/* Copyright 2015 Myles Salholm */
#pragma once

#include "Runtime/Public/Definitions.h"

#if WINDOWS

#endif

#if WITH_EDITOR

class FEditor
{
public:
	FEditor();
	FEditor(const FEditor& Other);
	~FEditor();
};

#endif //WITH_EDITOR