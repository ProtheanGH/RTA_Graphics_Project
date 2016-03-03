#pragma once
#include <vector>
#include "VertexTypes.h"

class Mesh{

private:
	std::vector<Vertex_POSNORMUV> vertices;
	std::vector<unsigned int> indices;

public:
	Mesh() = default;
	~Mesh() = default;

	inline std::vector<Vertex_POSNORMUV>& GetVerts() { return vertices; }
	inline std::vector<unsigned int>& GetIndices() { return indices; }
};