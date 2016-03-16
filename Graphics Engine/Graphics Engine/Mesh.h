#pragma once
#include <vector>
#include "VertexTypes.h"
#include <DirectXMath.h>


class Mesh{

private:
	std::vector<Vertex_POSNORMUV> vertices;
	std::vector<unsigned int> indices;

public:
	Mesh() = default;
	~Mesh() = default;

	inline std::vector<Vertex_POSNORMUV>& GetVerts() { return vertices; }
	inline std::vector<unsigned int>& GetIndices() { return indices; }

	void copy(Mesh* _rhs){
		vertices.clear();
		unsigned int vertCount = (unsigned int)_rhs->GetVerts().size();
		vertices.resize(vertCount);
		for (unsigned int i = 0; i < vertCount; ++i){
			vertices[i] = _rhs->GetVerts()[i];
		}

		indices.clear();
		unsigned int indicesCount = (unsigned int)_rhs->GetIndices().size();
		indices.resize(vertCount);
		for (unsigned int i = 0; i < indicesCount; ++i){
			indices[i] = _rhs->GetIndices()[i];
		}
	}
};