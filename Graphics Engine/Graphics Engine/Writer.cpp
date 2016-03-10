#include "Writer.h"
#include <fstream>

Writer* Writer::instance = nullptr;

Writer* Writer::GetInstance(){
	if (instance == nullptr)
		instance = new Writer;

	return instance;
}

void Writer::Terminate(){
	delete instance;
}

void Writer::SaveObject(const char* _fileName, Object& _object){
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

bool Writer::LoadObject(const char* _fileName, Object& _object){
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

void Writer::SaveObject(std::fstream* file, Object& _object){
	_object.GetName() += '\0';

	//write the size of the objects name
	unsigned int nameSize = (unsigned int)_object.GetName().size();
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

		unsigned int vertCount = (unsigned int)_object.GetMesh()->GetVerts().size();
		//write the number of vertices in the mesh
		file->write((char*)&vertCount, sizeof(vertCount));

		//write out all of the vertices
		file->write((char*)_object.GetMesh()->GetVerts().data(), sizeof(Vertex_POSNORMUV)* vertCount);

		unsigned int indicesCount = (unsigned int)_object.GetMesh()->GetIndices().size();
		//write the number of indices
		file->write((char*)&indicesCount, sizeof(indicesCount));

		//write out all of the indices
		file->write((char*)_object.GetMesh()->GetIndices().data(), sizeof(unsigned int)* indicesCount);
	}

	unsigned int child_count = (unsigned int)_object.GetChildren().size();
	file->write((char*)&child_count, sizeof(unsigned int));

	for (unsigned int i = 0; i < child_count; ++i){
		SaveObject(file, *_object.GetChildren()[i]);
	}
}

bool Writer::LoadObject(std::fstream* file, Object& _object){
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

		_object.SetMesh(object_mesh);
	}

	//read in the amount of children this object has
	unsigned int child_count = 0;
	file->read((char*)&child_count, sizeof(child_count));

	for (unsigned int i = 0; i < child_count; ++i){
		Object* child_object = Object::Create();
		child_object->SetParent(&_object);
		_object.GetChildren().push_back(child_object);

		LoadObject(file, *child_object);
	}

	return true;
}

