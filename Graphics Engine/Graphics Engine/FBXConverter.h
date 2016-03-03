#pragma once

#include <fbxsdk.h>
#include "Object.h"

class FBXConverter{

private:
	FbxManager* fbxManager = nullptr;

	void LoadFBX(FbxNode* _rootNode, Object* rootObject);

	//Load the fbx data into the objects mesh
	void LoadMesh(FbxMesh* _mesh, Object* object);

	void LoadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal);
	void LoadUV(FbxMesh* _mesh, int _controlPointIndex, int _textureUVIndex, DirectX::XMFLOAT2& _outUV);

	void FBXConverter::SaveObject(std::fstream* file, Object& _object);
	void LoadObject(std::fstream* _file, Object& _object);

	void Destroy();

public:

	FBXConverter() = default;
	~FBXConverter();
	
	void LoadFBX(const char* fileName, Object* object);

	//Saves the object as a binary file 
	void SaveObject(const char* fileName, Object& object);

	void LoadObject(const char* fileName, Object& object);

	static FBXConverter* Create();
};