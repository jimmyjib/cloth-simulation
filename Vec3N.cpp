#include "Vec3N.h"

vec3N_float operator+(vec3N_float& v1, vec3N_float& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(v1[i] + v2[i]);
	}
	return v;
}

vec3N_float operator+(vec3N_float& v1, vec3N_float&& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(v1[i] + v2[i]);
	}
	return v;
}

vec3N_float operator*(float c, vec3N_float& v1)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(c * v1[i]);
	}
	return v;
}

vec3N_float operator*(float c, vec3N_float&& v1)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(c * v1[i]);
	}
	return v;
}

vec3N_float operator*(vec3N_float& v1, vec3N_float& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(glm::vec3(v1[i][0] * v2[i][0], v1[i][1] * v2[i][1], v1[i][2] * v2[i][2]));
	}
	return v;
}

vec3N_float operator*(vec3N_float& v1, vec3N_float&& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(glm::vec3(v1[i][0] * v2[i][0], v1[i][1] * v2[i][1], v1[i][2] * v2[i][2]));
	}
	return v;
}

vec3N_float operator-(vec3N_float& v1, vec3N_float& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(glm::vec3(v1[i][0] - v2[i][0], v1[i][1] - v2[i][1], v1[i][2] - v2[i][2]));
	}
	return v;
}

vec3N_float operator-(vec3N_float& v1, vec3N_float&& v2)
{
	vec3N_float v;
	for (int i = 0; i < v1.size(); i++) {
		v.push_back(glm::vec3(v1[i][0] - v2[i][0], v1[i][1] - v2[i][1], v1[i][2] - v2[i][2]));
	}
	return v;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//dot product
float Dot(vec3N_float& v1, vec3N_float& v2)
{
	float sum = 0;
	for (int i = 0; i < v1.size(); i++) {
		sum += glm::dot(v1[i], v2[i]);
	}
	return sum;
}

float Dot(vec3N_float& v1, vec3N_float&& v2)
{
	float sum = 0;
	for (int i = 0; i < v1.size(); i++) {
		sum += glm::dot(v1[i], v2[i]);
	}
	return sum;
}

float Dot(vec3N_float&& v1, vec3N_float&& v2)
{
	float sum = 0;
	for (int i = 0; i < v1.size(); i++) {
		sum += glm::dot(v1[i], v2[i]);
	}
	return sum;
}