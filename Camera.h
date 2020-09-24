#pragma once
#include <glm/glm.hpp>
#include "Quaternion.h"

#define TRANSLATE_SENSITIVITY	0.1f
#define DOLLY_SENSITIVITY		1.f
#define DOLLY_MINIMUM			1.f
#define DOLLY_MAXIMUM			1000.f
#define ROTATE_SENSITIVITY      1.f

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
	glm::vec3 getPosition();
	void setPosition(glm::vec3 _src);
	void setPosition(float x, float y, float z);

	//LookAt
	glm::vec3 getLookAt();
	void setLookAt(glm::vec3 _src);
	void setLookAt(float x, float y, float z);

	//Up
	glm::vec3 getUpVec();
	void setUpVec(glm::vec3 _src);
	void setUpVec(float x, float y, float z);

	//Orientation
	Quaternion getOrientation();
	void setOrientation(Quaternion _src);
	void setOrientation(float x, float y, float z, float theta);
	void setOrientation(glm::vec3 axis, float theta);

	//Near
	float getNear();
	void setNear(float _near);

	//Far
	float getFar();
	void setFar(float _far);

	//Angle
	float getViewingAngle();
	void setViewingAngle(float _vangle);

	//Local Coordinate
	//Y: up vector
	//Z: position - lookat
	//(a vector that points the target position from camera location)
	//X: Y cross Z 
	glm::vec3 getLocalXAxis();
	glm::vec3 getLocalYAxis();
	glm::vec3 getLocalZAxis();

	//Get Final Matrix
	glm::mat4 viewMat();

	///////////////////////////////////////////////////////////////////////////////////
	//Camera Manipulation

	//Translation
	void translate(float x, float y, float z);
	void translate(glm::vec3 vec);
	//Middle Mouse Dragging
	void translate(int prevX, int prevY, int curX, int curY);

	//Rotation
	void rotate(glm::vec3 axis, float theta);
	void rotate(Quaternion rotation);
	//Left Mouse Dragging
	void rotate(int prevX, int prevY, int curX, int curY, int window_w, int window_h);
	//2D Mouse(Viewport) -> 3D Sphere
	glm::vec3 viewPortPositionToTrackBall(int vp_w, int vp_h, int x, int y);

	//Dolly (moving camera fw/bw)
	void dollyIn();
	void dollyOut();

private:
	float zNear;
	float zFar;
	float viewingAngle;

	glm::vec3 pos;
	glm::vec3 lookat;
	glm::vec3 up;
	Quaternion ori;
};

