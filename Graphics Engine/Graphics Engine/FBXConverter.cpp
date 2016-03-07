#include "FBXConverter.h"
#include <fstream>
#include "MeshManager.h"

FBXConverter* FBXConverter::instance = nullptr;

void RemoveExtension(std::string& name);

FBXConverter::~FBXConverter(){
	fbxManager->Destroy();
}

FBXConverter* FBXConverter::GetInstance(){
	if (instance == nullptr)
		instance = new FBXConverter;

	return instance;
}

void FBXConverter::DeleteInstance(){
	delete instance;
	instance = nullptr;
}

void FBXConverter::LoadFBX(const char* _fileName, Object* _rootObject){

	if (LoadObject(_fileName, *_rootObject) == true) return;

	if (fbxManager == nullptr){
		fbxManager = FbxManager::Create();
		FbxIOSettings* ioSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
		fbxManager->SetIOSettings(ioSettings);
	}

	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "");

	std::string file_name("Assets/");
	file_name.append(_fileName);
	bool result = fbxImporter->Initialize(file_name.c_str(), -1, fbxManager->GetIOSettings());

	if (!result) return;

	result = fbxImporter->Import(fbxScene);

	if (!result) return;

	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();

	if (rootNode){
		std::string name = std::string(_fileName);
		RemoveExtension(name);
		_rootObject->SetName(name);
		LoadFBX(rootNode, _rootObject);
	}

}

void FBXConverter::LoadFBX(FbxNode* _rootNode, Object* _rootObject){

	int child_count = _rootNode->GetChildCount();

	Transform& root_transform = _rootObject->GetTransform();
	FbxDouble3 translation = _rootNode->LclTranslation.Get();
	FbxDouble3 rotation = _rootNode->LclRotation.Get();
	FbxDouble3 scale = _rootNode->LclScaling.Get();

	DirectX::XMMATRIX matrix = DirectX::XMMatrixIdentity();
	matrix = matrix * DirectX::XMMatrixScaling(scale.mData[0], scale.mData[1], scale.mData[2]);
	matrix = matrix * DirectX::XMMatrixRotationRollPitchYaw(rotation.mData[0], rotation.mData[1], rotation.mData[2]);
	matrix = matrix * DirectX::XMMatrixTranslation(translation.mData[0], translation.mData[1], translation.mData[2]);

	DirectX::XMStoreFloat4x4(&root_transform.GetLocalMatrix(), matrix);

	for (int i = 0; i < child_count; ++i){
		FbxNode* child_node = _rootNode->GetChild(i);

		if (child_node->GetNodeAttribute() == NULL) continue;

		FbxNodeAttribute::EType attributeType = child_node->GetNodeAttribute()->GetAttributeType();
		if (attributeType != FbxNodeAttribute::eMesh) continue;

		Object* child_obj = Object::Create();
		child_obj->SetName(std::string(child_node->GetName()));
		LoadFBX(child_node, child_obj);

		FbxMesh* mesh = (FbxMesh*)child_node->GetNodeAttribute();
		LoadMesh(mesh, child_obj);

		child_obj->SetParent(_rootObject);
		_rootObject->GetChildren().push_back(child_obj);
	}
}

void FBXConverter::LoadMesh(FbxMesh* _mesh, Object* _object){

	FbxVector4* controlPoints = _mesh->GetControlPoints();
	unsigned int controlPointCount = _mesh->GetControlPointsCount();
	unsigned int vertexCount = 0;
	Mesh* object_mesh = new Mesh();

	for (unsigned int i = 0; i < controlPointCount; ++i){
		Vertex_POSNORMUV vertex;
		vertex.pos[0] = (float)controlPoints[i].mData[0];
		vertex.pos[1] = (float)controlPoints[i].mData[1];
		vertex.pos[2] = (float)controlPoints[i].mData[2];

		DirectX::XMFLOAT3 normal;
		LoadNormal(_mesh, i, vertexCount, normal);
		vertex.normal[0] = normal.x;
		vertex.normal[1] = normal.y;
		vertex.normal[2] = normal.z;

		DirectX::XMFLOAT2 uv;
		LoadUV(_mesh, i, vertexCount, uv);

		object_mesh->GetVerts().push_back(vertex);

		++vertexCount;
	}

	//Polygons are triangles
	/*
	for (int i = 0; i < _mesh->GetPolygonCount(); ++i){
	int numVertices = _mesh->GetPolygonSize(i);
	if (numVertices < 3) continue;

	for (int j = 0; j < 3; ++j){
	int controlPointIndex = _mesh->GetPolygonVertex(i, j);

	Vertex_POSNORMUV vertex;
	vertex.pos[0] = (float)controlPoints[controlPointIndex].mData[0];
	vertex.pos[1] = (float)controlPoints[controlPointIndex].mData[1];
	vertex.pos[2] = (float)controlPoints[controlPointIndex].mData[2];

	DirectX::XMFLOAT3 normal;
	LoadNormal(_mesh, controlPointIndex, vertexCount, normal);
	vertex.normal[0] = normal.x;
	vertex.normal[1] = normal.y;
	vertex.normal[2] = normal.z;

	DirectX::XMFLOAT2 uv;
	LoadUV(_mesh, controlPointIndex, _mesh->GetTextureUVIndex(i, j), uv);
	vertex.uv[0] = uv.x;
	vertex.uv[1] = uv.y;

	object_mesh->GetVerts().push_back(vertex);

	++vertexCount;
	}
	}*/

	//Vertices are indices
	int* indices = _mesh->GetPolygonVertices();
	for (int i = 0; i < _mesh->GetPolygonVertexCount(); ++i){
		int index = indices[i];
		object_mesh->GetIndices().push_back(index);
	}

	_object->SetMesh(object_mesh);

	SaveMesh(_object->GetName().c_str(), *object_mesh);
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

void FBXConverter::LoadUV(FbxMesh* _mesh, int _controlPointIndex, int _textureUVIndex, DirectX::XMFLOAT2& _outUV){

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
		if (vertex_uv->GetReferenceMode() == FbxGeometryElement::eIndexToDirect){
			_outUV.x = (float)vertex_uv->GetDirectArray().GetAt(_textureUVIndex).mData[0];
			_outUV.y = (float)vertex_uv->GetDirectArray().GetAt(_textureUVIndex).mData[1];
		}
	}
}


void FBXConverter::SaveObject(const char* _fileName, Object& _object){
	std::fstream file;
	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name.append(".object");

	file.open(file_name.c_str(), std::ios_base::binary | std::ios_base::trunc | std::ios_base::out);

	if (!file.is_open()) return;

	file.close();

	file.open(file_name.c_str(), std::ios_base::binary | std::ios_base::app | std::ios_base::out);

	if (!file.is_open()) return;

	SaveObject(&file, _object);

	file.close();
}

void FBXConverter::SaveObject(std::fstream* file, Object& _object){
	_object.GetName() += '\0';

	//write the size of the objects name
	unsigned int nameSize = _object.GetName().size();
	file->write((char*)&nameSize, sizeof(nameSize));

	//write the objects name
	const char* object_name = _object.GetName().c_str();
	file->write(object_name, nameSize);

	//write the objects transform
	file->write((char*)&_object.GetTransform(), sizeof(Transform));

	bool hasMesh = (_object.GetMesh() != nullptr);

	//write if object has a mesh or not
	file->write((char*)&hasMesh, sizeof(hasMesh));

	if (hasMesh){

		unsigned int vertCount = _object.GetMesh()->GetVerts().size();
		//write the number of vertices in the mesh
		file->write((char*)&vertCount, sizeof(vertCount));

		//write out all of the vertices
		file->write((char*)_object.GetMesh()->GetVerts().data(), sizeof(Vertex_POSNORMUV)* vertCount);

		unsigned int indicesCount = _object.GetMesh()->GetIndices().size();
		//write the number of indices
		file->write((char*)&indicesCount, sizeof(indicesCount));

		//write out all of the indices
		file->write((char*)_object.GetMesh()->GetIndices().data(), sizeof(unsigned int)* indicesCount);
	}

	unsigned int child_count = _object.GetChildren().size();
	file->write((char*)&child_count, sizeof(unsigned int));

	for (unsigned int i = 0; i < child_count; ++i){
		SaveObject(file, *_object.GetChildren()[i]);
	}
}

bool FBXConverter::LoadObject(const char* _fileName, Object& _object){
	std::fstream file;
	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name.append(".object");
	file.open(file_name.c_str(), std::ios_base::binary | std::ios_base::in);

	if (!file.is_open()) return false;

	LoadObject(&file, _object);

	file.close();

	return true;
}

bool FBXConverter::LoadObject(std::fstream* file, Object& _object){

	unsigned int nameSize = 0;
	//read the size of the objects name
	file->read((char*)&nameSize, sizeof(nameSize));

	//read the objects name
	char* object_name = new char[nameSize];
	file->read(object_name, nameSize);
	_object.SetName(std::string(object_name));
	delete[] object_name;

	//read the objects transform
	file->read((char*)&_object.GetTransform(), sizeof(Transform));

	bool hasMesh = false;

	//read if object has a mesh or not
	file->read((char*)&hasMesh, sizeof(hasMesh));

	if (hasMesh){

		Mesh* object_mesh = new Mesh();
		_object.SetMesh(object_mesh);

		unsigned int vertCount = 0;
		//read the number of vertices in the mesh
		file->read((char*)&vertCount, sizeof(vertCount));

		//read in the vertices
		for (unsigned int i = 0; i < vertCount; ++i){
			Vertex_POSNORMUV vertex;
			file->read((char*)&vertex, sizeof(vertex));
			object_mesh->GetVerts().push_back(vertex);
		}

		unsigned int indicesCount = 0;
		//read the number of indices
		file->read((char*)&indicesCount, sizeof(indicesCount));

		//read in the indices
		for (unsigned int i = 0; i < indicesCount; ++i){
			unsigned int index = 0;
			file->read((char*)&index, sizeof(index));
			object_mesh->GetIndices().push_back(index);
		}
	}

	//read in the amount of children this object has
	unsigned int child_count = 0;
	file->read((char*)&child_count, sizeof(child_count));

	for (unsigned int i = 0; i < child_count; ++i){
		Object* child_object = Object::Create();
		_object.GetChildren().push_back(child_object);

		LoadObject(file, *child_object);
	}

	return true;
}

void FBXConverter::SaveMesh(const char* _fileName, Mesh& _mesh){
	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name += ".mesh";

	std::fstream file;
	file.open(file_name.c_str(), std::ios_base::out | std::ios_base::binary);

	if (file.is_open() == false) return;

	//write out the amount of vertices
	unsigned int vertCount = _mesh.GetVerts().size();
	file.write((char*)&vertCount, sizeof(vertCount));

	//write out the amount of vertices
	unsigned int indexCount = _mesh.GetVerts().size();
	file.write((char*)&indexCount, sizeof(indexCount));

	//write out all of the vertices
	file.write((char*)_mesh.GetVerts().data(), sizeof(Vertex_POSNORMUV)* vertCount);

	//write out all of the indices
	file.write((char*)_mesh.GetIndices().data(), sizeof(unsigned int)* indexCount);

	file.close();
}

void FBXConverter::LoadMesh(const char* _fileName, Mesh& _mesh){
	std::string file_name("Assets/");
	file_name.append(_fileName);
	file_name += ".mesh";

	std::fstream file;
	file.open(file_name.c_str(), std::ios_base::in | std::ios_base::binary);

	if (file.is_open() == false) return;

	//read in the amount of verts
	unsigned int vertCount = 0;
	file.read((char*)&vertCount, sizeof(vertCount));

	//read in the amount of indices
	unsigned int indexCount = 0;
	file.read((char*)&indexCount, sizeof(indexCount));

	//read in all of the vertices
	Vertex_POSNORMUV vertex;
	for (unsigned int i = 0; i < vertCount; ++i){
		file.read((char*)&vertex, sizeof(vertex));
		_mesh.GetVerts().push_back(vertex);
	}

	//read in all of the indices
	unsigned int index = 0;
	for (unsigned int i = 0; i < indexCount; ++i){
		file.read((char*)&index, sizeof(index));
		_mesh.GetIndices().push_back(index);
	}

	file.close();
}


void RemoveExtension(std::string& name){

	unsigned int pos = name.find_last_of('.');
	name.erase(pos);
}


