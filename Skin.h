#pragma once
#include "Core.h";
#include "Triangle.h";
#include "Tokenizer.h";
#include "Skeleton.h";
#include "Vertex.h";
#include "Model.h"
class Skin
{
public:
	Skin();
	Skin(Skeleton* skeleton);
	~Skin();
	Skeleton* skel;
	std::vector<Triangle*> triangles;
	std::vector<Vertex*> vertices;
	std::vector<glm::mat4x4> bindings;
	std::vector<glm::mat4x4> matrices;
	glm::mat4x4 WorldMtx;
	bool Load(const char *file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void SetBuffers(const std::vector<ModelVertex> &vtx, const std::vector<uint> &idx);
private:
	uint VertexBuffer;
	uint IndexBuffer;
	int Count;
};

