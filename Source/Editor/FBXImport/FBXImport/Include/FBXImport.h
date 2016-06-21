/* Copyright 2015 Myles Salholm */

#if defined(FBXIMPORT_EXPORT)
#define FBXIMPORT_API __declspec(dllexport)
#else
#define FBXIMPORT_API __declspec(dllimport)
#endif

namespace FBXImport
{
	FBXIMPORT_API bool Initialize(const char* filename);
	FBXIMPORT_API void Shutdown();

	FBXIMPORT_API bool Import(const char* srcFilename);
	FBXIMPORT_API bool WriteToDisk(const char* dstFilename);
}