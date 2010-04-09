// fbxShatterApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia);
extern "C" bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);



int _tmain(int argc, _TCHAR* argv[])
{
    KFbxSdkManager* SdkManager = NULL;
    KFbxScene* Scene = NULL;
	KFbxNode* Node = NULL;
	KFbxImporter* IImporter = NULL;
	KFbxExporter* IExporter = NULL;
    bool Result;

    // Prepare the FBX SDK.
	SdkManager = KFbxSdkManager::Create();

	Scene = KFbxScene::Create(SdkManager, "");
	Node = KFbxNode::Create(SdkManager, "");

	//IImporter = KFbxImporter::Create(lSdkManager, "");
	//IExporter = KFbxExporter::Create(ISdkManager, "");


    // Load the scene.
	Result = LoadScene(SdkManager,Scene, "toBeShatter.fbx");
	std::cout<<"loading "<< (Result ? "Successful!" : "Failed!") <<std::endl;
	

	// Get the geometry from the scene
	std::cout<<"\n Geometry count: " << Scene->GetGeometryCount() << std::endl;
	KFbxGeometry *plane;// = KFbxGeometry::Create(SdkManager, "plane");
	plane = Scene->GetGeometry(0);

	std::cout<<"\n Control point count: "<<plane->GetControlPointsCount() << std::endl;

	
	
	//DisplayHierarchy(Scene);



	return 0;
}

