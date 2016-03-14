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
	void LoadJoints(FbxNode* _rootNode, Bone* bone);
	void LoadAnimation(FbxNode* _node, FbxScene* _scene, Animation& _animation, Bone* _rootBone);
	void LoadAnimation(FbxNode* _node, FbxAnimLayer* _animLayer, FbxScene* _scene, Animation& _animation, Bone* _rootBone);

	void ProcessBone(FbxNode* _node, Bone* bone);
	void ProcessJoints(FbxNode* _node, Bone* _rootBone);

	bool CheckDuplicates(std::vector<Vertex_POSNORMUV>& _vertices, Vertex_POSNORMUV& _vertex, unsigned int& _outIndex);

	FBXConverter() = default;
	~FBXConverter();

public:
	
	bool LoadFBX(const char* fileName, Object* object);

	static FBXConverter* GetInstance();
	static void Terminate();
};