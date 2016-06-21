/* Copyright 2015 Myles Salholm */

// Imports a model from an external format and saves it as a runtime-ready format.
class ModelImport
{
public:
	ModelImport();
	ModelImport(const char* FileLocation);
	ModelImport(const char* FileLocation, const char* FileDestination);
	~ModelImport();
	ModelImport(const ModelImport& Other);

	bool Import(const char* FileLocation);
	bool Import(const char* FileLocation, const char* FileDestination);

private:
	const char* InFile;
	const char* OutFile;
};