#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

//std::vector of 3D/2D vectors
typedef std::vector<glm::vec3> vec3N_float;
typedef std::vector<glm::ivec2> vec2N_int;
typedef std::vector<glm::ivec3> vec3N_int;

vec3N_float operator+(vec3N_float& v1, vec3N_float& v2);
vec3N_float operator*(float c, vec3N_float& v1);
vec3N_float operator*(vec3N_float& v1, vec3N_float& v2);
vec3N_float operator-(vec3N_float& v1, vec3N_float& v2);
vec3N_float operator+(vec3N_float& v1, vec3N_float&& v2);
vec3N_float operator*(float c, vec3N_float&& v1);
vec3N_float operator*(vec3N_float& v1, vec3N_float&& v2);
vec3N_float operator-(vec3N_float& v1, vec3N_float&& v2);

//dot product
float Dot(vec3N_float& v1, vec3N_float& v2);
float Dot(vec3N_float& v1, vec3N_float&& v2);
float Dot(vec3N_float&& v1, vec3N_float&& v2);