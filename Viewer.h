#pragma once
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"

//sets window, initialize camera/shader , callaback functions
//main function is integrating classes
//actual caculations are done inside each class
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

	//calculates info of each pixel using Projection, View, Model Matrices
	//called inside a while loop in the main function
	//update matrices with datas in camera object 
	void draw();
private:
	GLFWwindow* window;
	static int width, height;

	//datas change when callback is called
	static Camera cam;

	//shader program 
	Shader shader;
};

