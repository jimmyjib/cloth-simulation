#include <iostream>
#include "Viewer.h"

//used to implement dragging
double gClickedPositionX = 0;
double gClickedPositionY = 0;

//input -> camera control
//callbacks : static -> member functions inside : static
void Viewer::cursorPoisitionCallback(GLFWwindow* const window, const double xpos, const double ypos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		cam.rotate((int)gClickedPositionX, (int)gClickedPositionY, (int)xpos, (int)ypos, width, height);
	}
	else
	{
		state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		if (state == GLFW_PRESS)
		{
			cam.translate((int)gClickedPositionX, (int)gClickedPositionY, (int)xpos, (int)ypos);
		}
	}

	gClickedPositionX = xpos;
	gClickedPositionY = ypos;
}

void Viewer::scrollCallback(GLFWwindow* const window, const double xoffset, const double yoffset) {
	if (yoffset > 0) {
		cam.dollyIn();
	}
	else {
		cam.dollyOut();
	}
}

Viewer::Viewer(const int w, const int h) {
	width = w;
	height = h;

	//initialize window
	initGlfwWindow();

	//initialize camera
	cam.initialize();

	//set callback functions
	glfwSetCursorPosCallback(window, cursorPoisitionCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

int Viewer::initGlfwWindow() {

	//check if library was initialized properly
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//setting #samples for multisampling
	glfwWindowHint(GLFW_SAMPLES, 4);
	//sets min/max glfw version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//required for openGL 3.0 and above
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//making window
	window = glfwCreateWindow(width, height, "Cloth Simulation", NULL, NULL);

	//if opening window failed
	if (!window) {
		fprintf(stderr, "Can't open GLFW window. IF you use Intel GPU, OpenGL 3.0 is not supported\n");
		//terminate library
		glfwTerminate();
		return -1;
	}

	//required for openGL API
	glfwMakeContextCurrent(window);

	//initializing GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
	
	//setting input mode
	//sticky keys: only check if it's pressed(order not important)
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//gl funtions
	glEnable(GL_DEPTH_TEST); //enable 3D
	glDepthFunc(GL_LESS);

	return 1;
}

void Viewer::draw(){
	//sets ModelView Matrix, Projection Matrix, View Matrix
	//camera -> Projection Matrix, View Matrix
	//ModelView -> Identity 
	//might implement a way to modify ModelView mat in runtime

}