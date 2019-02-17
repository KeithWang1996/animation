#include "Skeleton.h"
bool Skeleton::Load(const char *file){
	Tokenizer token;
	token.Open(file);
	token.FindToken("balljoint");
	Root = new Joint;
	joints.push_back(Root);
	Root->Load(token, joints);
	token.Close();
	World = glm::mat4x4(1.0f);
	return true;
}

Skeleton::~Skeleton() {
	delete Root;
}

void Skeleton::Update() {
	worlds.clear();
	Root->Update(World * glm::mat4(1.0f), worlds);
}

void Skeleton::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	Root->Draw(viewProjMtx, shader);
}

glm::mat4x4 Skeleton::GetWorldMatrix(int idx) {
	return worlds[idx];
}

void Skeleton::EditDof(int idx, int xyz, float value) {
	joints[idx]->EditDof(xyz, value);
	Update();
}

void Skeleton::Translate(float x, float y, float z) {
	World = glm::translate(glm::vec3(x, y, z));
}