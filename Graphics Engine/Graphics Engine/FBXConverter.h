#pragma once
#include <fbxsdk.h>
#include "Object.h"
#include "Animation.h"


class FBXConverter{

private:
	static FBXConverter* instance;

	FbxManager* fbxManager = nullptr;

	bool LoadFBX(FbxNode* _rootNode, Object* rootObject);
	void LoadMesh(FbxMesh* _mesh, Object* object);
	void LoadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal);
	void LoadUV(FbxMesh* _mesh, int _controlPointIndex, int _textureUVIndex, DirectX::XMFLOAT2& _outUV);
	void LoadSkeleton(FbxNode* _rootNode, Bone* bone);
	void LoadJoints(FbxNode* _rootNode, Bone* bone);
	void LoadAnimation(FbxNode* _node, FbxScene* _scene, Animation& _animation, Bone* _rootBone);
	void LoadAnimation(FbxNode* _node, FbxAnimLayer* _animLayer, FbxScene* _scene, Animation& _animation, Bone* _rootBone);

	void ProcessBone(FbxNode* _node, Bone* bone);
	void ProcessJoints(FbxNode* _node, Bone* _rootBone);

	FBXConverter() = default;
	~FBXConverter();

public:
	
	bool LoadFBX(const char* fileName, Object* object);

	static FBXConverter* GetInstance();
	static void Terminate();
};