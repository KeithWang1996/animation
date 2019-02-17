#pragma once
#include "Core.h";
class Vertex
{
public:
	glm::vec4 position;
	glm::vec4 normal;
	std::vector<float> weights;
	std::vector<int> indices;
	Vertex();
	Vertex(glm::vec4 pos);
	void AddNormal(glm::vec4 nor);
	void AddWeight(float wgt);
	void AddIndex(int idx);
	glm::vec4 Update(std::vector<glm::mat4x4> matrices, bool isNormal);
	~Vertex();
};

