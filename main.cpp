#include <iostream>
#include "Viewer.h"
#include "ClothSimulator.h"

int main()
{
	//make 1280x960 window;
	Viewer v(1280, 960);

	//make clothsimulator object
	ClothSimulator c;
	//generate cloth mesh
	c.generateMesh();
	//put cloth object in viewer
	v.addDrawingObject(c.object());

	//simulate cloth until escape Trigger
	do {
		//calculate positions of next step
		c.solve();

		//since vertex informations are saved in VBO
		//updating values in "ClothSimulator" class will also affect
		//values used for drawing in "Viewer"
		v.draw();

		//process events(callback functions - keyboard/mouse)
		glfwPollEvents();
	} while (v.escapeTrigger());
}