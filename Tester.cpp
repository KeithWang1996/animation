////////////////////////////////////////
// Tester.cpp
////////////////////////////////////////

#include <iostream>
#include <sstream>
#include "Tester.h"
#include "Skeleton.h"
#include <iterator>
////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER=0;
std::string skel_file = "wasp.skel";
std::string skin_file = "wasp.skin";
std::string anim_file = "wasp_walk.anim";
bool hasAnim = true;
bool hasSkin = true;
int main(int argc, char **argv) {
	std::string input="";
	getline(std::cin, input);
	std::string buf;
	std::stringstream ss(input);
	std::vector<std::string> files;
	glutInit(&argc, argv);
	while (ss >> buf)
		files.push_back(buf);
	if (files.size() == 3) {
		skel_file = files[0];
		skin_file = files[1];
		anim_file = files[2];
	}
	else if (files.size() == 2) {
		skel_file = files[0];
		skin_file = files[1];
		hasAnim = false;
	}
	TESTER = new Tester("Skeleton", argc, argv);
	glutMainLoop();
	delete TESTER;
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(const char *windowTitle,int argc,char **argv) {
	DOF_idx = 0;
	WinX=800;
	WinY=600;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 100, 100 );
	WindowHandle = glutCreateWindow( windowTitle );
	glutSetWindowTitle( windowTitle );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize GLEW
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Initialize components
	Program=new ShaderProgram("Model.glsl",ShaderProgram::eRender);
	Cube=new SpinningCube;
	skel = new Skeleton;
	skel->Load(skel_file.c_str());
	skel->Update();
	skin = new Skin(skel);
	skin->Load(skin_file.c_str());
	skin->Update();
	if (hasAnim) {
		clip = new AnimationClip();
		clip->Load(anim_file.c_str());
		pose_values = clip->pose->getValues();
		clip->PreCompute();
		start = clock();
	}
	Cam=new Camera;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	delete Program;
	delete Cube;
	delete Cam;
	delete skel;
	delete skin;
	delete clip;
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	//Cube->Update();
	Cam->Update();
	if (hasAnim) {
		end = clock();
		float time = (float)(end - start) / CLOCKS_PER_SEC;
		clip->Evaluate(time);
		skel->Translate(pose_values->at(0), pose_values->at(1), pose_values->at(2));
		for (int i = 3; i < pose_values->size(); i += 3) {
			skel->EditDof((i - 3) / 3, 0, pose_values->at(i));
			skel->EditDof((i - 3) / 3, 1, pose_values->at(i + 1));
			skel->EditDof((i - 3) / 3, 2, pose_values->at(i + 2));
		}
		skin->Update();
	}
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam->Reset();
	Cam->SetAspect(float(WinX)/float(WinY));

	//Cube->Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw components
	//Cube->Draw(Cam->GetViewProjectMtx(),Program->GetProgramID());
	//skel->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	if (hasSkin) {
		skin->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	}
	else {
		skel->Draw(Cam->GetViewProjectMtx(), Program->GetProgramID());
	}
	//std::cout << "test" << std::endl;
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam->SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case 's':
			hasSkin = !hasSkin;
			break;
	}
	skin->Update();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int maxDelta=100;
	int dx = glm::clamp(nx - MouseX,-maxDelta,maxDelta);
	int dy = glm::clamp(-(ny - MouseY),-maxDelta,maxDelta);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth()+dx*rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline()-dy*rate,-90.0f,90.0f));
	}
	if(RightDown) {
		const float rate=0.005f;
		float dist=glm::clamp(Cam->GetDistance()*(1.0f-dx*rate),0.01f,1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
