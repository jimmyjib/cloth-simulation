#include <iostream>
#include "Viewer.h"
#include "ClothSimulator.h"

bool Viewer::pause;

int main()
{
	//make 1280x960 window;
	Viewer v(1280, 960);

	//make clothsimulator object
	ClothSimulator c;
	//generate cloth mesh
	c.generateMesh(3.f, 3.f, 33, 33, 0.1f);
	//put cloth object in viewer
	v.addDrawingObject(c.object());

	v.pause = true;

	//simulate cloth until escape Trigger
	do {
		//calculate positions of next step
		if (!v.pause) c.solve();

		//since vertex informations are saved in VBO
		//updating values in "ClothSimulator" class will also affect
		//values used for drawing in "Viewer"
		v.draw();

		//process events(callback functions - keyboard/mouse)
		glfwPollEvents();
	} while (v.escapeTrigger());
}