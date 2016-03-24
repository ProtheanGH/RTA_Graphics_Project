#include "FBXConverter.h"
#include "Writer.h"
#include "Animation.h"

void ConvertFBXMatrix(FbxMatrix& _fbxMatrix, DirectX::XMFLOAT4X4& _matrix);

#pragma region Singleton

FBXConverter* FBXConverter::instance = nullptr;

FBXConverter::~FBXConverter(){
	if (fbxManager)
		fbxManager->Destroy();
}

FBXConverter* FBXConverter::GetInstance(){
	if (instance == nullptr)
		instance = new FBXConverter;

	return instance;
}

void FBXConverter::Terminate(){
	delete instance;
	instance = nullptr;
}

#pragma endregion


bool FBXConverter::LoadFBX(const char* _fileName, Object* _rootObject){

	if (Writer::GetInstance()->LoadObject(_fileName, *_rootObject) == true) return true;

	if (fbxManager == nullptr){
		fbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");

	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name.append(".fbx");
	bool result = fbxImporter->Initialize(file_name.c_str(), -1, fbxManager->GetIOSettings());

	if (!result) return false;

	result = fbxImporter->Import(fbxScene);

	if (!result) return false;

	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	if (rootNode){
		std::string name = std::string(_fileName);
		_rootObject->SetName(name);

		Bone* root_bone = new Bone();
		ProcessBone(rootNode, root_bone);

		ProcessBone(rootNode, root_bone);

		LoadSkeleton(rootNode, root_bone);

		std::vector<Bone*> boneList;
		Bone::CreateBoneList(root_bone, boneList);

		Animation* animation = new Animation();
		LoadAnimation(rootNode, fbxScene, *animation, root_bone);
		_rootObject->AddAnimation(animation);

		if (LoadFBX(rootNode, _rootObject) == false){
			_rootObject->Destroy();

			return false;
		}

		_rootObject->SetRootBone(root_bone);

		LoadBoneInfo(_rootObject, rootNode, root_bone);
	}

	return true;
}

bool FBXConverter::LoadFBX(FbxNode* _rootNode, Object* _rootObject){

	int child_count = _rootNode->GetChildCount();

	Transform& root_transform = _rootObject->GetTransform();
	FbxDouble3 translation = _rootNode->LclTranslation.Get();
	FbxDouble3 rotation = _rootNode->LclRotation.Get();
	FbxDouble3 scale = _rootNode->LclScaling.Get();

	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	matrix = matrix * DirectX::XMMatrixScaling((float)scale.mData[0], (float)scale.mData[0], (float)scale.mData[0]);
	matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw((float)rotation.mData[0], (float)rotation.mData[1], (float)rotation.mData[2]);
	matrix = matrix * DirectX::XMMatrixTranslation((float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]);

	DirectX::XMStoreFloat4x4(&root_transform.GetLocalMatrix(), matrix);

	for (int i = 0; i < child_count; ++i){
		FbxNode* child_node = _rootNode->GetChild(i);
		if (child_node->GetNodeAttribute() == NULL) continue;

		FbxNodeAttribute::EType attributeType = child_node->GetNodeAttribute()->GetAttributeType();
		if (attributeType != FbxNodeAttribute::eMesh) continue;

		Object* child_obj = Object::Create();
		child_obj->SetName(std::string(child_node->GetName()));
		child_obj->SetParent(_rootObject);
		_rootObject->GetChildren().push_back(child_obj);

		bool result = LoadFBX(child_node, child_obj);
		if (!result) return false;

		FbxMesh* mesh = (FbxMesh*)child_node->GetNodeAttribute();
		LoadMesh(mesh, child_obj);

		LoadTexture(child_node, *child_obj);
	}

	return true;
}

void FBXConverter::LoadMesh(FbxMesh* _mesh, Object* _object){

	FbxVector4* controlPoints = _mesh->GetControlPoints();
	unsigned int polygonCount = _mesh->GetPolygonCount();
	int vertexCounter = 0;
	Mesh* objectMesh = new Mesh();

	int controlpointcount = _mesh->GetControlPointsCount();

	for (unsigned int polygon = 0; polygon < polygonCount; ++polygon){

		unsigned int polygonVertexCount = _mesh->GetPolygonSize(polygon);
		for (unsigned int polygonVertex = 0; polygonVertex < polygonVertexCount; ++polygonVertex){

			Vertex_POSNORMUV vertex;
			int controlPointIndex = _mesh->GetPolygonVertex(polygon, polygonVertex);
			vertex.controlpointIndex = controlPointIndex;
			vertex.pos[0] = (float)controlPoints[controlPointIndex].mData[0];
			vertex.pos[1] = (float)controlPoints[controlPointIndex].mData[1];
			vertex.pos[2] = (float)controlPoints[controlPointIndex].mData[2];
			vertex.pos[3] = 1;

			DirectX::XMFLOAT2 uv;
			LoadUV(_mesh, controlPointIndex, polygon, polygonVertex, uv);
			vertex.uv[0] = uv.x;
			vertex.uv[1] = fabsf(1.0f - uv.y);

			DirectX::XMFLOAT3 normal;
			LoadNormal(_mesh, controlPointIndex, vertexCounter, normal);
			vertex.normal[0] = normal.x;
			vertex.normal[1] = normal.y;
			vertex.normal[2] = normal.z;
			vertex.normal[3] = 1.0f;

			DirectX::XMFLOAT3 biNormal;
			LoadBiNormal(_mesh, controlPointIndex, vertexCounter, biNormal);
			vertex.binormal[0] = biNormal.x;
			vertex.binormal[1] = biNormal.y;
			vertex.binormal[2] = biNormal.z;
			vertex.binormal[3] = 1.0f;


			DirectX::XMFLOAT3 tangent;
			LoadTangent(_mesh, controlPointIndex, vertexCounter, tangent);
			vertex.tangent[0] = tangent.x;
			vertex.tangent[1] = tangent.y;
			vertex.tangent[2] = tangent.z;
			vertex.tangent[3] = 1.0f;

			unsigned int index = -1;
			if (CheckDuplicates(objectMesh->GetVerts(), vertex, index) == false){
				objectMesh->GetVerts().push_back(vertex);
			}

			objectMesh->GetIndices().push_back(index);

			++vertexCounter;
		}
	}

	_object->SetMesh(objectMesh);
}

void FBXConverter::LoadNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal){

	if (_mesh->GetElementNormalCount() < 1) return;

	FbxGeometryElementNormal* vertex_normal = _mesh->GetElementNormal();

	if (vertex_normal->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(_controlPointIndex).mData[2]);
		}
		else if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_normal->GetIndexArray().GetAt(_controlPointIndex);
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
	else if (vertex_normal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(_vertexCounter).mData[2]);
		}
		else if (vertex_normal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_normal->GetIndexArray().GetAt(_vertexCounter);
			_outNormal.x = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_normal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
}

void FBXConverter::LoadBiNormal(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outNormal){

	if (_mesh->GetElementBinormalCount() < 1) return;

	FbxGeometryElementBinormal* vertex_biNormal = _mesh->GetElementBinormal();

	if (vertex_biNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_biNormal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_biNormal->GetDirectArray().GetAt(_controlPointIndex).mData[0]);
			_outNormal.y = (float)(vertex_biNormal->GetDirectArray().GetAt(_controlPointIndex).mData[1]);
			_outNormal.z = (float)(vertex_biNormal->GetDirectArray().GetAt(_controlPointIndex).mData[2]);
		}
		else if (vertex_biNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_biNormal->GetIndexArray().GetAt(_controlPointIndex);
			_outNormal.x = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
	else if (vertex_biNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_biNormal->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outNormal.x = (float)(vertex_biNormal->GetDirectArray().GetAt(_vertexCounter).mData[0]);
			_outNormal.y = (float)(vertex_biNormal->GetDirectArray().GetAt(_vertexCounter).mData[1]);
			_outNormal.z = (float)(vertex_biNormal->GetDirectArray().GetAt(_vertexCounter).mData[2]);
		}
		else if (vertex_biNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_biNormal->GetIndexArray().GetAt(_vertexCounter);
			_outNormal.x = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[0]);
			_outNormal.y = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[1]);
			_outNormal.z = (float)(vertex_biNormal->GetDirectArray().GetAt(index).mData[2]);
		}
	}
}

void FBXConverter::LoadTangent(FbxMesh* _mesh, int _controlPointIndex, int _vertexCounter, DirectX::XMFLOAT3& _outTangent){

	if (_mesh->GetElementTangentCount() < 1) return;

	FbxGeometryElementTangent* vertex_tangent = _mesh->GetElementTangent();

	if (vertex_tangent->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_tangent->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outTangent.x = (float)(vertex_tangent->GetDirectArray().GetAt(_controlPointIndex).mData[0]);
			_outTangent.y = (float)(vertex_tangent->GetDirectArray().GetAt(_controlPointIndex).mData[1]);
			_outTangent.z = (float)(vertex_tangent->GetDirectArray().GetAt(_controlPointIndex).mData[2]);
		}
		else if (vertex_tangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_tangent->GetIndexArray().GetAt(_controlPointIndex);
			_outTangent.x = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[0]);
			_outTangent.y = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[1]);
			_outTangent.z = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[2]);
		}
	}
	else if (vertex_tangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_tangent->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outTangent.x = (float)(vertex_tangent->GetDirectArray().GetAt(_vertexCounter).mData[0]);
			_outTangent.y = (float)(vertex_tangent->GetDirectArray().GetAt(_vertexCounter).mData[1]);
			_outTangent.z = (float)(vertex_tangent->GetDirectArray().GetAt(_vertexCounter).mData[2]);
		}
		else if (vertex_tangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_tangent->GetIndexArray().GetAt(_vertexCounter);
			_outTangent.x = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[0]);
			_outTangent.y = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[1]);
			_outTangent.z = (float)(vertex_tangent->GetDirectArray().GetAt(index).mData[2]);
		}
	}
}

void FBXConverter::LoadUV(FbxMesh* _mesh, int _controlPointIndex, int polygon, int polygonVertex, DirectX::XMFLOAT2& _outUV){

	if (_mesh->GetElementUVCount() < 1) return;

	FbxGeometryElementUV* vertex_uv = _mesh->GetElementUV();

	if (vertex_uv->GetMappingMode() == FbxGeometryElement::eByControlPoint){
		if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eDirect){
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(_controlPointIndex).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(_controlPointIndex).mData[1];

		}
		else if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			int index = vertex_uv->GetIndexArray().GetAt(_controlPointIndex);
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(index).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(index).mData[1];
		}
	}
	else if (vertex_uv->GetMappingMode() == FbxGeometryElement::eByPolygonVertex){
		if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect || vertex_uv->GetReferenceMode() == FbxGeometryElement::eDirect){
			int index = _mesh->GetTextureUVIndex(polygon, polygonVertex);
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(index).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(index).mData[1];
		}
	}
}

void FBXConverter::LoadTexture(FbxNode* _node, Object& _object){
	
	int materialCount = _node->GetSrcObjectCount<FbxSurfaceMaterial>();
	
	for (int i = 0; i < materialCount; ++i){

		FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)_node->GetSrcObject<FbxSurfaceMaterial>(i);

		if (material != nullptr){
			FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

			if (layeredTextureCount > 0){
				for (int j = 0; j < layeredTextureCount; ++j){
					FbxLayeredTexture* layeredTexture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
					int layeredCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();
					for (int k = 0; k < layeredCount; ++k){
						FbxFileTexture* texture = FbxCast<FbxFileTexture>(layeredTexture->GetSrcObject<FbxFileTexture>(k));

						_object.GetTextureNames().push_back(std::string(texture->GetName()));
					}
				}
			}
			else{
				int textureCount = prop.GetSrcObjectCount<FbxFileTexture>();
				for (int j = 0; j < textureCount; ++j){
					const FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxFileTexture>(j));
					_object.GetTextureNames().push_back(std::string(texture->GetFileName()));
				}
			}
		}
	}
	
}

bool FBXConverter::CheckDuplicates(std::vector<Vertex_POSNORMUV>& _vertices, Vertex_POSNORMUV& _vertex, unsigned int& _outIndex){

	unsigned int i;
	for (i = 0; i < _vertices.size(); ++i){

		if (_vertices[i].isDuplicate(_vertex)){
			_outIndex = i;
			return true;
		}
	}

	_outIndex = i;

	return false;
}

void FBXConverter::LoadSkeleton(FbxNode* _rootNode, Bone* bone){

	int child_count = _rootNode->GetChildCount();

	for (int i = 0; i < child_count; ++i){
		FbxNode* child_node = _rootNode->GetChild(i);
		if (child_node->GetNodeAttribute() == NULL) continue;

		FbxNodeAttribute::EType attributeType = child_node->GetNodeAttribute()->GetAttributeType();
		if (attributeType != FbxNodeAttribute::eSkeleton) continue;

		Bone* child_bone = new Bone();
		ProcessBone(child_node, child_bone);

		LoadSkeleton(child_node, child_bone);
		child_bone->SetParent(bone);

		bone->GetChildren().push_back(child_bone);
	}
}

void FBXConverter::LoadSkeleton(const char* _fileName, Bone* bone){

	if (fbxManager == nullptr){
		fbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");

	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name.append(".fbx");
	bool result = fbxImporter->Initialize(file_name.c_str(), -1, fbxManager->GetIOSettings());

	if (!result) return;

	result = fbxImporter->Import(fbxScene);

	if (!result) return;

	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	if (rootNode){
		LoadSkeleton(rootNode, bone);
		Animation animation;
		LoadAnimation(rootNode, fbxScene, animation, bone);
	}
}

void FBXConverter::ProcessBone(FbxNode* _node, Bone* bone){
	bone->GetName() = std::string(_node->GetName());

	FbxDouble3 translation = _node->LclTranslation.Get();
	FbxDouble3 rotation = _node->LclRotation.Get();
	FbxDouble3 scale = _node->LclScaling.Get();

	FbxMatrix globalInverseBindPose = _node->EvaluateGlobalTransform();
	globalInverseBindPose = globalInverseBindPose.Inverse();

	ConvertFBXMatrix(globalInverseBindPose, bone->GetGlobalBindPose());

	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	matrix = matrix * DirectX::XMMatrixScaling((float)scale.mData[0], (float)scale.mData[1], (float)scale.mData[2]);
	matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw((float)rotation.mData[0], (float)rotation.mData[1], (float)rotation.mData[2]);
	matrix = matrix * DirectX::XMMatrixTranslation((float)translation.mData[0], (float)translation.mData[1], (float)translation.mData[2]);

	DirectX::XMMATRIX xMat = DirectX::XMLoadFloat4x4(&bone->GetGlobalBindPose());

	matrix = matrix * xMat;

	DirectX::XMStoreFloat4x4(&bone->GetLocal(), matrix);
}

void FBXConverter::LoadBoneInfo(Object* _object, FbxNode* _node, Bone* bone){
	
	unsigned int child_count = (unsigned int)_object->GetChildren().size();
	for (unsigned int i = 0; i < child_count; ++i){

		Object* child_obj = _object->GetChildren()[i];

		if (child_obj->GetMesh() != nullptr)
			LoadJoints(_node, bone, child_obj->GetMesh());

		LoadBoneInfo(child_obj, _node, bone);
	}

	_object->SetBuffers();
}


void FBXConverter::LoadJoints(FbxNode* _rootNode, Bone* _rootBone, Mesh* _mesh){

	int child_count = _rootNode->GetChildCount();
	for (int i = 0; i < child_count; ++i){
		FbxNode* child_node = _rootNode->GetChild(i);
		if (child_node->GetNodeAttribute() == NULL) continue;

		FbxNodeAttribute::EType attributeType = child_node->GetNodeAttribute()->GetAttributeType();
		if (attributeType != FbxNodeAttribute::eMesh) continue;

		ProcessJoints(child_node, _rootBone, _mesh);

		LoadJoints(child_node, _rootBone, _mesh);
	}
}

void FBXConverter::ProcessJoints(FbxNode* _node, Bone* _rootBone, Mesh* _mesh){
	FbxMesh* currMesh = _node->GetMesh();
	unsigned int deformer_count = currMesh->GetDeformerCount();

	for (unsigned int i = 0; i < deformer_count; ++i){
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(i, FbxDeformer::eSkin));
		if (currSkin == nullptr) continue;

		unsigned int cluster_count = currSkin->GetClusterCount();
		for (unsigned int cluster_index = 0; cluster_index < cluster_count; ++cluster_index){
			FbxCluster* currCluster = currSkin->GetCluster(cluster_index);
			std::string bone_name(currCluster->GetLink()->GetName());
			Bone* curr_bone = Bone::FindBone(_rootBone, bone_name);
			if (curr_bone == nullptr) continue;
			
			unsigned int indicesCount = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < indicesCount; ++i){
				int index = currCluster->GetControlPointIndices()[i];
				float weight = (float)currCluster->GetControlPointWeights()[i];

				for (unsigned int j = 0; j < _mesh->GetVerts().size(); ++j){
					if (_mesh->GetVerts()[j].controlpointIndex == index){
						_mesh->GetVerts()[j].AddBoneWeightPair(weight, curr_bone->GetIndex());
					}
				}
			}
		}
	}
}

void FBXConverter::LoadAnimation(FbxNode* _node, FbxScene* _scene, Animation& _animation, Bone* _rootBone){

		int anim_stack_count = _scene->GetSrcObjectCount<FbxAnimStack>();

		for (int i = 0; i < anim_stack_count; ++i){
	
			FbxAnimStack* anim_stack = _scene->GetSrcObject<FbxAnimStack>(i);
	
			std::string anim_name(anim_stack->GetName());
			_animation.GetName() = anim_name;

			FbxString animStackName = anim_stack->GetName();

			FbxTakeInfo* takeInfo = _scene->GetTakeInfo(animStackName);
			
			LoadAnimation(_node, takeInfo, _animation, _rootBone);
		}
	
		_animation.CalculateDuration();
}

void FBXConverter::LoadAnimation(FbxNode* _node, FbxTakeInfo* _takeInfo, Animation& _animation, Bone* _rootBone){

	FbxTime start = _takeInfo->mLocalTimeSpan.GetStart();
	FbxTime end = _takeInfo->mLocalTimeSpan.GetStop();
	Bone* bone = Bone::FindBone(_rootBone, std::string(_node->GetName()));

	if (bone == nullptr) return;

	Animation::KeyFrame* keyFrame;
	Animation::Key* key;

	for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i < end.GetFrameCount(FbxTime::eFrames30); ++i){
		FbxTime currTime;
		currTime.SetFrame(i, FbxTime::eFrames30);
		FbxMatrix globalMatrix = _node->EvaluateGlobalTransform(currTime);
		FbxVector4 translation = _node->EvaluateLocalTranslation(currTime);
		FbxVector4 rotation = _node->EvaluateLocalRotation(currTime);
		FbxVector4 scale = _node->EvaluateLocalScaling(currTime);

		float frameTime = (float)currTime.GetSecondDouble();

		//find the keyframe
		keyFrame = _animation.FindKeyFrame(frameTime);

		//if there isn't one create a new one and add it to animation's key frames
		if (keyFrame == nullptr){
			keyFrame = new Animation::KeyFrame;
			keyFrame->time = frameTime;
			_animation.GetKeyFrames().push_back(keyFrame);

			key = new Animation::Key;
			key->name = bone->GetName();
			keyFrame->keys.push_back(key);
		}
		else{
			//find the key 
			key = keyFrame->FindKey(bone->GetName());

			//if there isn't create a new one and add it to keyframe's keys
			if (key == nullptr){
				key = new Animation::Key;
				key->name = bone->GetName();
				keyFrame->keys.push_back(key);
			}
		}

		key->translation = DirectX::XMFLOAT4(translation.mData[0], translation.mData[1], translation.mData[2], 1.0f);
		key->rotation = DirectX::XMFLOAT4(rotation.mData[0], rotation.mData[1], rotation.mData[2], 1.0f);
		key->scale = DirectX::XMFLOAT4(scale.mData[0], scale.mData[1], scale.mData[2], 1.0f);
	}

	int child_count = _node->GetChildCount();
	for (int i = 0; i < child_count; ++i){
		FbxNode* childNode = _node->GetChild(i);
		LoadAnimation(childNode, _takeInfo, _animation, _rootBone);
	}
}

void ConvertFBXMatrix(FbxMatrix& fbxMatrix, DirectX::XMFLOAT4X4& matrix){

	matrix._11 = (float)fbxMatrix.Get(0, 0);
	matrix._12 = (float)fbxMatrix.Get(0, 1);
	matrix._13 = -(float)fbxMatrix.Get(0, 2);
	matrix._14 = (float)fbxMatrix.Get(0, 3);

	matrix._21 = (float)fbxMatrix.Get(1, 0);
	matrix._22 = (float)fbxMatrix.Get(1, 1);
	matrix._23 = -(float)fbxMatrix.Get(1, 2);
	matrix._24 = (float)fbxMatrix.Get(1, 3);

	matrix._31 = -(float)fbxMatrix.Get(2, 0);
	matrix._32 = -(float)fbxMatrix.Get(2, 1);
	matrix._33 = (float)fbxMatrix.Get(2, 2);
	matrix._34 = (float)fbxMatrix.Get(2, 3);

	matrix._41 = (float)fbxMatrix.Get(3, 0);
	matrix._42 = (float)fbxMatrix.Get(3, 1);
	matrix._43 = -(float)fbxMatrix.Get(3, 2);
	matrix._44 = (float)fbxMatrix.Get(3, 3);
}



