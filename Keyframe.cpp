#include "Keyframe.h"

Keyframe::Keyframe()
{
}

Keyframe::Keyframe(float time, float value, char rulein, char ruleout) {
	Time = time;
	Value = value;
	TangentIn = 0;
	TangentOut = 0;
	RuleIn = rulein;
	RuleOut = ruleout;
	Pre = NULL;
	Next = NULL;
}

void Keyframe::setPre(Keyframe* pre) {
	Pre = pre;
}

void Keyframe::setNext(Keyframe* next) {
	Next = next;
}

Keyframe* Keyframe::getPre() {
	return Pre;
}
Keyframe* Keyframe::getNext() {
	return Next;
}

void Keyframe::setTangentIn(float tanin) {
	TangentIn = tanin;
}

void Keyframe::setTangentOut(float tanout) {
	TangentOut = tanout;
}

float Keyframe::getTangentIn() {
	return TangentIn;
}

float Keyframe::getTangentOut() {
	return TangentOut;
}


float Keyframe::getValue() {
	return Value;
}

float Keyframe::getTime() {
	return Time;
}

char Keyframe::getRuleIn() {
	return RuleIn;
}

char Keyframe::getRuleOut() {
	return RuleOut;
}

void Keyframe::computeCubic() {
	//for v0
	if (RuleOut == 'f') {
		TangentOut = 0;
	}
	else if (RuleOut == 'l') {
		TangentOut = (Next->getValue() - getValue()) / (Next->getTime() - getTime());
	}
	else if (RuleOut == 's') {
		if (Pre == NULL) {
			TangentOut = (Next->getValue() - getValue()) / (Next->getTime() - getTime());
		}
		else {
			TangentOut = (Next->getValue() - Pre->getValue()) / (Next->getTime() - Pre->getTime());
		}
	}
	//for v1
	if (Next->getRuleIn() == 'f') {
		Next->setTangentIn(0);
	}
	else if (Next->getRuleIn() == 'l') {
		Next->setTangentIn((Next->getValue() - getValue()) / (Next->getTime() - getTime()));
	}
	else if (Next->getRuleIn() == 's') {
		if (Next->getNext() == NULL) {
			Next->setTangentIn((Next->getValue() - getValue()) / (Next->getTime() - getTime()));
		}
		else {
			Next->setTangentIn((Next->getNext()->getValue() - getValue()) / (Next->getNext()->getTime() - getTime()));
		}
	}
	glm::vec4 temp = glm::vec4(Value, Next->getValue(), (Next->getTime()-Time)*TangentOut, (Next->getTime() - Time)*Next->getTangentIn());
	temp = mtx * temp;
	A = temp[0];
	B = temp[1];
	C = temp[2];
	D = temp[3];
}

float Keyframe::Evaluate(float t) {
	if (t == Time || Next == NULL) {
		return Value;
	}
	float u = (t - Time) / (Next->getTime() - Time);
	return A * pow(u, 3) + B * pow(u, 2) + C * u + D;
}

Keyframe::~Keyframe()
{
}
