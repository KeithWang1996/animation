#include "Pose.h"

Pose::Pose()
{
}

vector<float>* Pose::getValues() {
	return &values;
}

void Pose::pushValue(float value) {
	values.push_back(value);
}

void Pose::insertValue(float value, int index) {
	values[index] = value;
}

int Pose::getSize() {
	return values.size();
}

Pose::~Pose()
{
}
