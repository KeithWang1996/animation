////////////////////////////////////////
// Tester.h
////////////////////////////////////////

#pragma once

#include "Shader.h"
#include "Camera.h"
#include "SpinningCube.h"
#include "Tokenizer.h"
#include "Skeleton.h"
#include "AnimationClip.h"
#include "Skin.h";
#include "Pose.h";
#include <string>
#include <time.h>
////////////////////////////////////////////////////////////////////////////////

// The 'Tester' is a simple top level application class. It creates and manages a
// window with the GLUT extension to OpenGL and it maintains a simple 3D scene
// including a camera and some other components.

class Tester {
public:
	Tester(const char *windowTitle,int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;
	int DOF_idx;
	// Components
	ShaderProgram *Program;
	SpinningCube *Cube;
	Skeleton *skel;
	Skin* skin;
	AnimationClip* clip;
	Camera *Cam;
	clock_t start;
	clock_t end;
	vector<float>* pose_values;
};

////////////////////////////////////////////////////////////////////////////////
