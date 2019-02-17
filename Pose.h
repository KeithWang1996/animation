#pragma once
#include <vector>
using namespace std;
class Pose
{
public:
	Pose();
	vector<float>* getValues();
	void insertValue(float value, int index);
	void pushValue(float value);
	int getSize();
	~Pose();
private:
	vector<float> values;
};

