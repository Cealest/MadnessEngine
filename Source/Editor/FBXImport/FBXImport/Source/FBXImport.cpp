/* Copyright 2015 Myles Salholm */
// FBXImport.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FBXImport.h"
#include <fbxsdk.h>

namespace FBXImport
{
	static bool bInitialized = false;
	static FbxManager* fbxManager = nullptr;
	static FbxIOSettings* fbxIOSettings = nullptr;
	static FbxImporter* fbxImporter = nullptr;
	static FbxScene* fbxScene = nullptr;

	bool Initialize(const char* filename)
	{
		// Initialize the sdk manager.  This object handles memory management.
		fbxManager = FbxManager::Create();

		// Create the input output settings object.
		fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(fbxIOSettings);

		// Create an importer using the sdk manager.
		fbxImporter = FbxImporter::Create(fbxManager, "FBXImport");

		// Import the file.
		if (!fbxImporter->Initialize(filename, -1, fbxIOSettings))
		{
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", fbxImporter->GetStatus().GetErrorString());
			return bInitialized;
		}

		// Create a scene for the imported file to populate.
		fbxScene = FbxScene::Create(fbxManager, "ImportScene");

		// Import the contents of the file to the scene.
		fbxImporter->Import(fbxScene);

		// Finished with the importer, release it.
		fbxImporter->Destroy();
		fbxImporter = nullptr;

		bInitialized = true;
		return bInitialized;
	}
	void Shutdown()
	{
		if (fbxScene)
		{
			fbxScene->Destroy();
			fbxScene = nullptr;
		}
		if (fbxImporter)
		{
			fbxImporter->Destroy();
			fbxImporter = nullptr;
		}
		if (fbxIOSettings)
		{
			fbxIOSettings->Destroy();
			fbxIOSettings = nullptr;
		}
		if (fbxManager)
		{
			fbxManager->Destroy();
			fbxManager = nullptr;
		}

		bInitialized = false;
	}

	bool Import(const char* srcFilename)
	{
		bool bSuccess = true;

		if (!Initialize(srcFilename))
			bSuccess = false;
		if (bSuccess && !WriteToDisk(srcFilename))
			bSuccess = false;

		Shutdown();
		return bSuccess;
	}

	bool WriteToDisk(const char* dstFilename)
	{

		return true;
	}
}