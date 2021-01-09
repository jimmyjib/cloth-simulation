#pragma once
#include <vector>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Object.h"

//sets window, initialize camera/shader , callback functions
//main function of Viewer class is integrating other classes
//actual caculations are done inside other classes
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

	//add drawing object to 'objs' vector
	//draw() function draws objects in 'objs' vector
	void addDrawingObject(Object obj) {
		objs.push_back(obj);
	}

	//returns escape trigger
	//if return value is "false" it breaks while loop
	bool escapeTrigger() {
		//when escape key is pressed or close flag is sent through the window
		//the return value will be false
		return glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0;
	}

private:
	GLFWwindow* window;
	static int width, height;

	//datas change when callback is called
	static Camera cam;

	//shader program 
	Shader shader;

	//vector containing objects
	std::vector<Object> objs;
};

