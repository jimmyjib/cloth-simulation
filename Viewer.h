#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>

//sets window, initialize camera, callaback functions
class Viewer
{
public:
	//all initializing functions goes in Constructor
	Viewer(const int w, const int h);
	~Viewer() {}

	//initialize window
	int initGlfwWindow();

	//callback functions
	//(needs to be static to use the set Callback func of glfw)
	//drag -> rotate, transition
	static void cursorPoisitionCallback(GLFWwindow* const window, const double xpos, const double ypos);
	//scroll -> zoom
	static void scrollCallback(GLFWwindow* const window, const double xoffset, const double yoffset);
private:
	GLFWwindow* window;
	int width, height;

};
