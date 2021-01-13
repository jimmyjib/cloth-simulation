#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

//define static variables
float Camera::zNear;
float Camera::zFar;
float Camera::viewingAngle;

glm::vec3 Camera::pos;
glm::vec3 Camera::lookat;
glm::vec3 Camera::up;
Quaternion Camera::ori;

Camera::Camera() {
    initialize();
}

void Camera::initialize() {
    pos = glm::vec3(3.5f, 3.5f, 7.f);
    lookat = glm::vec3(0.f, 0.f, 0.f);
    up = glm::vec3(0.f, 1.f, 0.f);
    zNear = 0.1f;
    zFar = 1000.f;
    viewingAngle = 45.f;
    ori = Quaternion(0.f, 0.f, 0.f, 1.f);
}

///////////////////////////////////////////////////////////////////////////////////
//Getter & Setter

//Position
glm::vec3 Camera::getPosition() {
    return pos;
}

void Camera::setPosition(glm::vec3 _src) {
    pos = _src;
}

void Camera::setPosition(float x, float y, float z) {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

//LookAt
glm::vec3 Camera::getLookAt() {
    return lookat;
}

void Camera::setLookAt(glm::vec3 _src) {
    lookat = _src;
}

void Camera::setLookAt(float x, float y, float z) {
    lookat[0] = x;
    lookat[1] = y;
    lookat[2] = z;
}

//Up
glm::vec3 Camera::getUpVec() {
    return up;
}

void Camera::setUpVec(glm::vec3 _src) {
    up = _src;
}

void Camera::setUpVec(float x, float y, float z) {
    up[0] = x;
    up[1] = y;
    up[2] = z;
}

//Orientation
inline Quaternion Camera::getOrientation() {
    return ori;
}

void Camera::setOrientation(Quaternion _src) {
    ori = _src;
}

void Camera::setOrientation(float x, float y, float z, float theta) {
    glm::vec3 axis = glm::vec3(x, y, z);
    axis = normalize(axis);
    ori = Quaternion(axis, theta);
}

void Camera::setOrientation(glm::vec3 axis, float theta) {
    ori = Quaternion(axis, theta);
}

//Near
float Camera::getNear() {
    return zNear;
}

void Camera::setNear(float _near) {
    zNear = _near;
}

//Far
float Camera::getFar() {
    return zFar;
}

void Camera::setFar(float _far) {
    zFar = _far;
}

//Angle
float Camera::getViewingAngle() {
    return viewingAngle;
}

void Camera::setViewingAngle(float _vangle) {
    viewingAngle = _vangle;
}

//Local Coordinate
glm::vec3 Camera::getLocalXAxis() {
    return glm::normalize(cross(getLocalYAxis(), getLocalZAxis()));
}

glm::vec3 Camera::getLocalYAxis() {
    return glm::normalize(getUpVec());
}

glm::vec3 Camera::getLocalZAxis() {
    return glm::normalize(getPosition() - getLookAt());
}

//Get Final Matrix
//uses function in glm library
glm::mat4 Camera::viewMat()
{
    return glm::lookAt(pos, lookat, up);
};

///////////////////////////////////////////////////////////////////////////////////
//Camera Manipulation

//Translation
void Camera::translate(float x, float y, float z) {
    glm::vec3 trans(x, y, z);
    trans = normalize(trans);
    pos += TRANSLATE_SENSITIVITY * trans;
    lookat += TRANSLATE_SENSITIVITY * trans;
}

void Camera::translate(glm::vec3 vec) {
    vec = normalize(vec);
    pos += TRANSLATE_SENSITIVITY * vec;
    lookat += TRANSLATE_SENSITIVITY * vec;
}

void Camera::translate(int prevX, int prevY, int curX, int curY) {
    float dx = float(curX - prevX);
    float dy = float(curY - prevY);
    translate(-dx * getLocalXAxis() + dy * getLocalYAxis());
}

//Rotation
void Camera::rotate(glm::vec3 axis, float theta) {
    Quaternion rotateQuat(axis, -theta);

    glm::vec3 lookatVec = lookat - pos;
    lookatVec = rotateQuat.rotate(lookatVec);
    pos = lookat - lookatVec;
    up = rotateQuat.rotate(up);
    ori = rotateQuat * ori;
}

void Camera::rotate(Quaternion rotation) {
    glm::vec3 lookatVec = lookat - pos;
    lookatVec = rotation.rotate(lookatVec);
    pos = lookat - lookatVec;
    up = rotation.rotate(up);
    ori = rotation * ori;
}

void Camera::rotate(int prevX, int prevY, int curX, int curY, int window_w, int window_h) {
    glm::vec3 prevPos = viewPortPositionToTrackBall(window_w, window_h, prevX, prevY);
    glm::vec3 currentPos = viewPortPositionToTrackBall(window_w, window_h, curX, curY);
    Quaternion rotation(currentPos, prevPos);
    rotate(rotation);
}

glm::vec3 Camera::viewPortPositionToTrackBall(int vp_w, int vp_h, int x, int y) {
    // 2D - Viewport -> 3D - sphere

    glm::vec3 tempVec;

    // Translate of Origin Point
    tempVec[0] = (float)x - vp_w / 2.f;
    tempVec[1] = (float)y - vp_h / 2.f;

    // Scaling
    tempVec[0] /= (float)vp_w / sqrt(2.f);
    tempVec[1] /= (float)(-vp_h) / sqrt(2.f);
    tempVec[2] = 0.f;

    // Normalize
    if (glm::length(tempVec) >= 1.f)
        tempVec = normalize(tempVec);
    else
        tempVec[2] = sqrt(1.f - tempVec[0] * tempVec[0] - tempVec[1] * tempVec[1]);

    // Consider ori of Trackball
    tempVec = ori.rotate(tempVec);

    return tempVec;
}

//Dolly (moving camera fw/bw)
void Camera::dollyIn() {
    glm::vec3 vec = lookat - pos;
    vec = normalize(vec);
    if (glm::length(lookat - pos) > DOLLY_MINIMUM)
        pos += DOLLY_SENSITIVITY * vec;
    //lookat += DOLLY_SENSITIVITY * vec;
}

void Camera::dollyOut() {
    glm::vec3 vec = pos - lookat;
    vec = normalize(vec);
    if (glm::length(lookat - pos) < DOLLY_MAXIMUM)
        pos += DOLLY_SENSITIVITY * vec;
    //lookat += DOLLY_SENSITIVITY * vec;
}