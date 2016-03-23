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
	void LoadBiNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal);
	void LoadTangent(FbxMesh* _mesh, int _controlPointIndex, int, DirectX::XMFLOAT3&);
	void LoadUV(FbxMesh* _mesh, int _controlPointIndex, int polygon, int polygonVertex, DirectX::XMFLOAT2& _outUV);
	void LoadTexture(FbxNode* _node, Object& _object);
	void LoadSkeleton(FbxNode* _rootNode, Bone* bone);
	void LoadJoints(FbxNode* _rootNode, Bone* bone, Mesh* _mesh);
	void LoadAnimation(FbxNode* _node, FbxScene* _scene, Animation& _animation, Bone* _rootBone);
	void LoadAnimation(FbxNode* _node, FbxTakeInfo* _takeInfo, Animation& _animation, Bone* _rootBone);

	void ProcessBone(FbxNode* _node, Bone* bone);
	void ProcessJoints(FbxNode* _node, Bone* _rootBone, Mesh* _mesh);

	void LoadBoneInfo(Object* _object, FbxNode* _node, Bone* bone);

	bool CheckDuplicates(std::vector<Vertex_POSNORMUV>& _vertices, Vertex_POSNORMUV& _vertex, unsigned int& _outIndex);

	FBXConverter() = default;
	~FBXConverter();

public:
	
	bool LoadFBX(const char* fileName, Object* object);
	void LoadSkeleton(const char* fileName, Bone* rootBone);

	static FBXConverter* GetInstance();
	static void Terminate();
};