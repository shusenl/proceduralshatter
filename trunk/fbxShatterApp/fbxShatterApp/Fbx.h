#ifndef FBX_H
#define FBX_H

#include "stdafx.h"

/*
Naming Convention: 
Class and Class method name:
	begin with upper case letter
	The second word will also begin with upper case letter
Protected member:
	start with _  word 
	separated by _
	all lower case

*/

class Fbx
{
public:

	Fbx();
	~Fbx();

	//about mesh polygon
	//polygon => mesh => geometry object

	bool addPolygonObjectToScene(std::vector<KFbxVector4> &vertex_list);

	//static method to be used for loading and save without creating the Fbx class
	static bool SaveScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia);
	static bool LoadScene(KFbxSdkManager* pSdkManager, KFbxDocument* pScene, const char* pFilename);

	//member object method
	bool SaveScene(char* filename);
	bool LoadScene(char* filename);

protected:

	KFbxSdkManager * _sdk_manager;
	KFbxScene *_input_scene, *_output_scene;


	//Geometry data storage
	std::vector<KFbxMesh*> _mesh_list;



	
};

#endif