#pragma once

struct Vertex_POSNORMUV{
	float pos[4];
	float normal[4];
	float uv[4];
	float tangent[4];
	float binormal[4];
	float weights[4];
	int boneIndices[4];
	int controlpointIndex;
	int padding[3];

	Vertex_POSNORMUV(){
		for (int i = 0; i < 4; ++i){
			weights[i] = 0;
			boneIndices[i] = 0;
		}
	}

	void AddBoneWeightPair(float weight, int boneIndex){
		for (int i = 0; i < 4; ++i){
			if (boneIndices[i] == boneIndex)
				break;

			if (boneIndices[i] != 0) continue;

			boneIndices[i] = boneIndex;
			weights[i] = weight;
			break;
		}
	}

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

	bool ComparePositions(const Vertex_POSNORMUV& vertex){
		for (unsigned int i = 0; i < 4; ++i){
			float diff = pos[i] - vertex.pos[i];
			diff = fabsf(diff);
			if (diff > 0.01) return false;
		}

		return true;
	}

	void copyweights(const Vertex_POSNORMUV& vertex){
		for (unsigned int i = 0; i < 4; ++i){
			weights[i] = vertex.weights[i];
			boneIndices[i] = vertex.boneIndices[i];
		}
	}
};