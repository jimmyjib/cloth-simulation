#include <iostream>
#include "Viewer.h"

//input -> camera control
void Viewer::cursorPoisitionCallback(GLFWwindow* const window, const double xpos, const double ypos) {
	
}

void Viewer::scrollCallback(GLFWwindow* const window, const double xoffset, const double yoffset) {

}

Viewer::Viewer(const int w, const int h) {
	width = w;
	height = h;

	//initialize window
	initGlfwWindow();

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