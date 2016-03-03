#pragma once

struct Vertex_POSNORMUV{
	float pos[3];
	float normal[3];
	float uv[2];

	bool isDuplicate(const Vertex_POSNORMUV& vertex){
		for (int i = 0; i < 3; ++i){
			if (pos[i] != vertex.pos[i])
				return false;

			if (normal[i] != vertex.normal[i])
				return false;

			if (i > 1) continue;

			if (uv[i] != vertex.uv[i])
				return false;
		}

		return true;
	}
};