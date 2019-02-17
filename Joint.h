#include "Core.h"
#include "DOF.h"
#include "Tokenizer.h"
#include "Model.h"
class Joint {
public:
	Joint();
	~Joint();
	void Draw(const glm::mat4 &viewProjMtx, uint shader);
	void Update(glm::mat4x4 Mtx, std::vector<glm::mat4x4>&worlds);
	bool Load(Tokenizer &token, std::vector<Joint*> &joints);
	void AddChild(Joint *child);
	glm::mat4x4 GetWorldMatrix();
	void EditDof(int xyz, float value);
	static int counter;
private:
	glm::mat4x4 WorldMtx;
	glm::mat4x4 LocalMtx;
	glm::vec3 Offset;
	glm::vec3 Boxmax;
	glm::vec3 Boxmin;
	std::vector<Joint*>children;
	Joint *parent;
	uint VertexBuffer;
	uint IndexBuffer;
	Model model;
	DOF *x;
	DOF *y;
	DOF *z;
};