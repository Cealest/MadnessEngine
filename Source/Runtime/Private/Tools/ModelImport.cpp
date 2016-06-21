/* Copyright 2015 Myles Salholm */

#include "Runtime/Public/Tools/ModelImport.h"
#include "FBXImport.h"

ModelImport::ModelImport()
{

}

ModelImport::~ModelImport()
{

}

ModelImport::ModelImport(const ModelImport& Other)
{
	
}

bool ModelImport::Import(const char* FileLocation)
{
	return FBXImport::Import(FileLocation);
}

bool ModelImport::Import(const char* FileLocation, const char* FileDestination)
{
	return Import(FileLocation);
}