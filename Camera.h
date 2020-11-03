#pragma once
#include <glm/glm.hpp>
#include "Quaternion.h"

#define TRANSLATE_SENSITIVITY	0.1f
#define DOLLY_SENSITIVITY		1.f
#define DOLLY_MINIMUM			1.f
#define DOLLY_MAXIMUM			1000.f
#define ROTATE_SENSITIVITY      1.f

//class that has informations about camera
//and methods to manipulate it
class Camera
{
public:
	Camera();
	~Camera() {}

	//sets initial information
	//camera position, lookat, up (information need for gluLookAt)
	//near, far, angle (information need for gluPerspective)
	//camera orientation - uses quaternion
	void initialize();

	///////////////////////////////////////////////////////////////////////////////////
	//Getter & Setter

	//Position
	static glm::vec3 getPosition();
	void setPosition(glm::vec3 _src);
	void setPosition(float x, float y, float z);

	//LookAt
	static glm::vec3 getLookAt();
	void setLookAt(glm::vec3 _src);
	void setLookAt(float x, float y, float z);

	//Up
	static glm::vec3 getUpVec();
	void setUpVec(glm::vec3 _src);
	void setUpVec(float x, float y, float z);

	//Orientation
	static Quaternion getOrientation();
	void setOrientation(Quaternion _src);
	void setOrientation(float x, float y, float z, float theta);
	void setOrientation(glm::vec3 axis, float theta);

	//Near
	static float getNear();
	void setNear(float _near);

	//Far
	static float getFar();
	void setFar(float _far);

	//Angle
	float getViewingAngle();
	void setViewingAngle(float _vangle);

	//Local Coordinate
	//Y: up vector
	//Z: position - lookat
	//(a vector that points the target position from camera location)
	//X: Y cross Z 
	static glm::vec3 getLocalXAxis();
	static glm::vec3 getLocalYAxis();
	static glm::vec3 getLocalZAxis();

	//Get Final Matrix
	glm::mat4 viewMat();

	///////////////////////////////////////////////////////////////////////////////////
	//Camera Manipulation

	//Translation
	static void translate(float x, float y, float z);
	static void translate(glm::vec3 vec);
	//Middle Mouse Dragging
	static void translate(int prevX, int prevY, int curX, int curY);

	//Rotation
	static void rotate(glm::vec3 axis, float theta);
	static void rotate(Quaternion rotation);
	//Left Mouse Dragging
	static void rotate(int prevX, int prevY, int curX, int curY, int window_w, int window_h);
	//2D Mouse(Viewport) -> 3D Sphere
	static glm::vec3 viewPortPositionToTrackBall(int vp_w, int vp_h, int x, int y);

	//Dolly (moving camera fw/bw)
	void dollyIn();
	void dollyOut();

private:
	static float zNear;
	static float zFar;
	static float viewingAngle;

	static glm::vec3 pos;
	static glm::vec3 lookat;
	static glm::vec3 up;
	static Quaternion ori;
};

