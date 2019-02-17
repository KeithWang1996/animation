#pragma once
#include "Core.h"
class Keyframe
{
public:
	Keyframe();
	Keyframe(float time, float value, char rulein, char ruleout);
	void computeCubic();
	void setPre(Keyframe* pre);
	void setNext(Keyframe* next);
	Keyframe* getPre();
	Keyframe* getNext();
	void setTangentIn(float tanin);
	void setTangentOut(float tanout);
	float getTangentIn();
	float getTangentOut();
	float getValue();
	float getTime();
	char getRuleIn();
	char getRuleOut();
	float Evaluate(float t);
	~Keyframe();
private:
	float Time;
	float Value;
	float TangentIn, TangentOut;
	char RuleIn;//f, l, s, x
	char RuleOut;
	float A, B, C, D;
	glm::mat4x4 mtx = glm::mat4x4(2, -3, 0, 1, -2, 3, 0, 0, 1, -2, 1, 0, 1, -1, 0 ,0);
	Keyframe* Pre;
	Keyframe* Next;
};

