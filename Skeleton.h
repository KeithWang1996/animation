#pragma once
#include "Core.h";
#include "Joint.h";
class Skeleton {
public:
	~Skeleton();
	bool Load(const char *file);
	void Update();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	Joint *Root;
	std::vector<Joint*>joints;
	std::vector<glm::mat4x4>worlds;
	glm::mat4x4 GetWorldMatrix(int idx);
	void EditDof(int idx, int xyz, float value);
	void Translate(float x, float y, float z);
private:
	glm::mat4x4 World;
};
