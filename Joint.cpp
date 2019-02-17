#include "Joint.h";
#include <iostream>;

int Joint::counter;

Joint::Joint() {
	Offset = glm::vec3(0, 0, 0);
	Boxmin = glm::vec3(-0.1f, -0.1f, -0.1f);
	Boxmax = glm::vec3(0.1f, 0.1f, 0.1f);
	x = new DOF;
	y = new DOF;
	z = new DOF;
	WorldMtx = glm::mat4(1);
	LocalMtx = glm::mat4(1);
}

Joint::~Joint() {
	delete x;
	delete y;
	delete z;
	if (!children.empty()) {
		for (int i = 0; i < children.size(); ++i) {
			delete children[i];
		}
	}
}

bool Joint::Load(Tokenizer &token, std::vector<Joint*>& joints) {
	token.FindToken("{");
	while (1) {
		char temp[256];
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) {
			Offset.x = token.GetFloat();
			Offset.y = token.GetFloat();
			Offset.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0) {
			Boxmin.x = token.GetFloat();
			Boxmin.y = token.GetFloat();
			Boxmin.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) {
			Boxmax.x = token.GetFloat();
			Boxmax.y = token.GetFloat();
			Boxmax.z = token.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0) {
			x->SetMinMax(token.GetFloat(), token.GetFloat());
		}
		else if (strcmp(temp, "rotylimit") == 0) {
			y->SetMinMax(token.GetFloat(), token.GetFloat());
		}
		else if (strcmp(temp, "rotzlimit") == 0) {
			z->SetMinMax(token.GetFloat(), token.GetFloat());
		}
		else if (strcmp(temp, "balljoint") == 0) {
			Joint *jnt = new Joint;
			joints.push_back(jnt);
			jnt->Load(token, joints);
			AddChild(jnt);
		}
		else if (strcmp(temp, "pose") == 0) {
			x->SetValue(token.GetFloat());
			y->SetValue(token.GetFloat());
			z->SetValue(token.GetFloat());
		}
		else if (strcmp(temp, "}") == 0) return true;
		else token.SkipLine();
	}
}

void Joint::AddChild(Joint *child) {
	children.push_back(child);
}

void Joint::Update(glm::mat4 Mtx, std::vector<glm::mat4x4>&worlds) {
	glm::mat4x4 mtx = LocalMtx;
	mtx *= glm::translate(glm::mat4(1.0f), Offset);
	mtx *= glm::rotate(z->GetValue(), glm::vec3(0, 0, 1));
	mtx *= glm::rotate(y->GetValue(), glm::vec3(0, 1, 0));
	mtx *= glm::rotate(x->GetValue(), glm::vec3(1, 0, 0));
	WorldMtx = Mtx * mtx;
	worlds.push_back(WorldMtx);
	if (children.size() > 0) {
		for (unsigned int i = 0; i < children.size(); ++i) {
			children[i]->Update(WorldMtx, worlds);
		}
	}
}

void Joint::Draw(const glm::mat4 &viewProjMtx, uint shader) {
	model.MakeBox(Boxmin, Boxmax);
	model.Draw(WorldMtx, viewProjMtx, shader);
	if (children.size() > 0) {
		for (unsigned int i = 0; i < children.size(); ++i) {
			children[i]->Draw(viewProjMtx, shader);
		}
	}
}

void Joint::EditDof(int xyz, float value) {
	if (xyz == 0) {
		x->SetValue(value);
	}
	else if (xyz == 1) {
		y->SetValue(value);
	}
	else {
		z->SetValue(value);
	}

}