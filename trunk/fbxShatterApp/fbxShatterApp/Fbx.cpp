#include "stdafx.h"



Fbx::Fbx()
{
	_sdk_manager = KFbxSdkManager::Create();

	_input_scene = KFbxScene::Create(_sdk_manager, "");
	_output_scene = KFbxScene::Create(_sdk_manager, "");
}

Fbx::~Fbx()
{

}

bool Fbx::addPolygonObjectToScene( std::vector<KFbxVector4> &vertex_list )
{
	assert(vertex_list.size()>=3);
	if(!_output_scene)
	{
		//std::cout << " Not scene has been created!! " << std::endl;
		return false;
	}

	KFbxNode *root = _output_scene->GetRootNode();

	if(!root)
	{
		//std::cout<<"Don't have root!"<< std::endl;
		return false;
	}

	KFbxMesh *mesh = KFbxMesh::Create(_sdk_manager, "");
	if(!mesh)
		return false;


	mesh->BeginPolygon();
	std::vector<KFbxVector4>::iterator it;
	int index = 0;
	for (it = vertex_list.begin(); it != vertex_list.end(); it++, index++)
	{
		mesh->SetControlPointAt((*it), index);
		mesh->AddPolygon(index);
	}
	mesh->EndPolygon();
	mesh->BuildMeshEdgeArray();

	_mesh_list.push_back(mesh);

	KFbxNode *polygon_node = KFbxNode::Create(_sdk_manager, "");
	assert(polygon_node->AddNodeAttribute(mesh));

	assert(root->AddChild(polygon_node));


	return true;
}

 bool Fbx::LoadScene( KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename )
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;
	//int lFileFormat = -1;
	int i, lTakeCount;
	KString lCurrentTakeName;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	KFbxSdkManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	KFbxImporter* lImporter = KFbxImporter::Create(pSdkManager,"");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename);
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if( !lImportStatus )
	{
		printf("Call to KFbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());

		if (lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_YET ||
			lImporter->GetLastErrorID() == KFbxIO::eFILE_VERSION_NOT_SUPPORTED_ANYMORE)
		{
			printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	printf("FBX version number for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		printf("FBX version number for file %s is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access take information without
		// the expense of loading the entire file.

		printf("Take Information\n");

		lTakeCount = lImporter->GetTakeCount();

		printf("    Number of takes: %d\n", lTakeCount);
		printf("    Current take: \"%s\"\n", lImporter->GetCurrentTakeName());
		printf("\n");

		for(i = 0; i < lTakeCount; i++)
		{
			KFbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			printf("    Take %d\n", i);
			printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the take should be imported 
			// under a different name.
			printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the take should be not
			// be imported. 
			printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOSREF.SetBoolProp(IMP_FBX_MATERIAL,        true);
		IOSREF.SetBoolProp(IMP_FBX_TEXTURE,         true);
		IOSREF.SetBoolProp(IMP_FBX_LINK,            true);
		IOSREF.SetBoolProp(IMP_FBX_SHAPE,           true);
		IOSREF.SetBoolProp(IMP_FBX_GOBO,            true);
		IOSREF.SetBoolProp(IMP_FBX_ANIMATION,       true);
		IOSREF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);

	if(lStatus == false && lImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
	{
		printf("Please enter password: ");

		lPassword[0] = '\0';

		scanf("%s", lPassword);
		KString lString(lPassword);

		IOSREF.SetStringProp(IMP_FBX_PASSWORD,      lString);
		IOSREF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if(lStatus == false && lImporter->GetLastErrorID() == KFbxIO::ePASSWORD_ERROR)
		{
			printf("\nPassword is wrong, import aborted.\n");
		}
	}

	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}

bool Fbx::LoadScene( char* filename )
{
	return this->LoadScene(_sdk_manager, _input_scene, filename);

}

bool Fbx::SaveScene( KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat = -1, bool pEmbedMedia = false )
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	KFbxExporter* lExporter = KFbxExporter::Create(pSdkManager, "");

	if( pFileFormat < 0 || pFileFormat >= pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount() )
	{
		// Write in fall back format if pEmbedMedia is true
		pFileFormat = pSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

		if (!pEmbedMedia)
		{
			//Try to export in ASCII if possible
			int lFormatIndex, lFormatCount = pSdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

			for (lFormatIndex=0; lFormatIndex<lFormatCount; lFormatIndex++)
			{
				if (pSdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
				{
					KString lDesc =pSdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
					char *lASCII = "ascii";
					if (lDesc.Find(lASCII)>=0)
					{
						pFileFormat = lFormatIndex;
						break;
					}
				}
			}
		}
	}

	// Set the export states. By default, the export states are always set to 
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
	// shows how to change these states.

	IOSREF.SetBoolProp(EXP_FBX_MATERIAL,        true);
	IOSREF.SetBoolProp(EXP_FBX_TEXTURE,         true);
	IOSREF.SetBoolProp(EXP_FBX_EMBEDDED,        pEmbedMedia);
	IOSREF.SetBoolProp(EXP_FBX_SHAPE,           true);
	IOSREF.SetBoolProp(EXP_FBX_GOBO,            true);
	IOSREF.SetBoolProp(EXP_FBX_ANIMATION,       true);
	IOSREF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Initialize the exporter by providing a filename.
	if(lExporter->Initialize(pFilename, pFileFormat) == false)
	{
		printf("Call to KFbxExporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lExporter->GetLastErrorString());
		return false;
	}

	KFbxSdkManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// Export the scene.
	lStatus = lExporter->Export(pScene); 

	// Destroy the exporter.
	lExporter->Destroy();
	return lStatus;
}

bool Fbx::SaveScene( char* filename )
{
	return this->SaveScene(_sdk_manager, _output_scene, filename);
}
