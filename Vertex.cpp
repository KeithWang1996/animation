#include "Vertex.h";
#include <iostream>;
Vertex::Vertex() {
	position = glm::vec4(1.0f);
	normal = glm::vec4(0.0f);
}

Vertex::Vertex(glm::vec4 pos)
{
	position = pos;
}

void Vertex::AddNormal(glm::vec4 nor)
{
	normal = nor;
}

void Vertex::AddWeight(float wgt)
{
	weights.push_back(wgt);
}

void Vertex::AddIndex(int idx)
{
	indices.push_back(idx);
}


glm::vec4 Vertex::Update(std::vector<glm::mat4x4> matrices, bool isNormal) {
	glm::mat4x4 temp_mtx;
	glm::vec4 toreturn = glm::vec4(0.0f);
	if (isNormal) {
		for (int i = 0; i < indices.size(); i++) {
			temp_mtx = matrices.at(indices[i]);
			toreturn += weights[i] * (temp_mtx * normal);
			toreturn = glm::normalize(toreturn);
		}
	}
	else {
		for (int i = 0; i < indices.size(); i++) {
			temp_mtx = matrices.at(indices[i]);
			toreturn += weights[i] * (temp_mtx * position);
		}
	}
	return toreturn;
}

Vertex::~Vertex()
{

}
