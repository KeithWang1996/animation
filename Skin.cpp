#include "Skin.h"
#include <iostream>;
Skin::Skin(Skeleton* skeleton)
{
	skel = skeleton;
	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &IndexBuffer);
	Count = 0;
}

bool Skin::Load(const char *file) {
	Tokenizer token;
	Triangle* temp_tri;
	Vertex* temp_vtx;
	glm::mat4x4 temp_mtx;
	int size, num, joint, a, b, c;
	float x, y, z, weight;
	token.Open(file);
	token.FindToken("positions");
	size = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < size; i++) {
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		temp_vtx = new Vertex(glm::vec4(x, y, z, 1));
		vertices.push_back(temp_vtx);
	}
	token.FindToken("normals");
	token.FindToken("{");
	for (int i = 0; i < size; i++) {
		x = token.GetFloat();
		y = token.GetFloat();
		z = token.GetFloat();
		vertices.at(i)->AddNormal((glm::vec4(x, y, z, 0)));
	}
	token.FindToken("skinweights");
	token.FindToken("{");
	for (int i = 0; i < size; i++) {
		num = token.GetInt();
		for (int j = 0; j < num; j++) {
			joint = token.GetInt();
			weight = token.GetFloat();
			vertices.at(i)->AddIndex(joint);
			vertices.at(i)->AddWeight(weight);
		}
	}
	token.FindToken("triangles");
	size = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < size; i++) {
		a = token.GetInt();
		b = token.GetInt();
		c = token.GetInt();
		temp_tri = new Triangle(a, b, c);
		triangles.push_back(temp_tri);
	}
	token.FindToken("bindings");
	size = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < size; i++) {
		token.FindToken("matrix {");
		temp_mtx = glm::mat4x4(1.0f);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 3; k++) {
				temp_mtx[j][k] = token.GetFloat();
			}
		}
		temp_mtx = glm::inverse(temp_mtx);
		bindings.push_back(temp_mtx);
	}
	token.Close();
	return true;
}

void Skin::Update() {
	matrices.clear();
	glm::mat4x4 temp;
	Triangle* temp_tri;
	std::vector<ModelVertex> vtx;
	std::vector<uint> idx;
	for (int i = 0; i < bindings.size(); i++) {
		temp = (skel->GetWorldMatrix(i)) * bindings[i];
		matrices.push_back(temp);
	}
	for (int i = 0; i < vertices.size(); i++) {
		glm::vec3 position = glm::vec3(vertices[i]->Update(matrices, false));
		glm::vec3 normal = glm::vec3(vertices[i]->Update(matrices, true));
		vtx.push_back({ position, normal });
	}
	for (int i = 0; i < triangles.size(); i++) {
		temp_tri = triangles[i];
		idx.push_back(temp_tri->GetA());
		idx.push_back(temp_tri->GetB());
		idx.push_back(temp_tri->GetC());
	}
	SetBuffers(vtx, idx);
}

void Skin::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	// Set up shader
	WorldMtx = glm::mat4x4(1);
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, false, (float*)&WorldMtx);

	glm::mat4 mvpMtx = viewProjMtx * WorldMtx;
	glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjMtx"), 1, false, (float*)&mvpMtx);

	// Set up state
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

	uint posLoc = 0;
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);

	uint normLoc = 1;
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)12);

	// Draw geometry
	glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, 0);

	// Clean up state
	glDisableVertexAttribArray(normLoc);
	glDisableVertexAttribArray(posLoc);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

void Skin::SetBuffers(const std::vector<ModelVertex> &vtx, const std::vector<uint> &idx) {
	Count = (int)idx.size();
	// Store vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vtx.size() * sizeof(ModelVertex), &vtx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Store index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx.size() * sizeof(uint), &idx[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Skin::~Skin()
{
	glDeleteBuffers(1, &IndexBuffer);
	glDeleteBuffers(1, &VertexBuffer);
}
