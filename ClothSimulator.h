#pragma once
#include "SparseMatrix.h"
#include "Vec3N.h"
#include "Object.h"
#define Identity glm::mat3(1,0,0,0,1,0,0,0,1) //identity matrix

//process in main function: generateMesh() -> solve()
class ClothSimulator {
	//calculates positions for each timestep
	//and saves it in VBO
public:
	ClothSimulator() {}
	~ClothSimulator() {}

	//generate triagular mesh of rectangular cloth
	void generateMesh(const float width, const float height, const int w_div, const int h_div, const float density);

	//initialize values 
	void initSolver();

	//main solve function
	void solve();

	//return object instance containing information of cloth
	Object object() const {
		return obj;
	}

	//size of unit time step
	float h = 0.02;
private:
	//object for VAO/VBO
	Object obj;

	//4 arrays for obj.genVAO();
	vec3N_float pos;
	vec3N_float normal;
	vec3N_float color;
	vec3N_int triangle;

	//////////////////////////////////////////////////////////////////////////////////////////
	//calculate forces
	void calcExternalForce();
	void calcInternalForce();

	//external force
	void calcGravityForce();

	//internal force
	void calcStretchShearForce();
	void calcBendingForce();

	//Conjugate Gradient -> ConjugateGradient.cpp
	void setS(int idx, int ndof, glm::vec3 p = glm::vec3(0, 0, 0), glm::vec3 q = glm::vec3(0, 0, 0));
	vec3N_float filter(vec3N_float& v);
	vec3N_float filter(vec3N_float&& v);
	vec3N_float ConjugateGradient(SparseMatrix&& A, vec3N_float&& b, float e);

	//////////////////////////////////////////////////////////////////////////////////////////
	//variables
	SparseMatrix K;
	SparseMatrix M;
	std::vector<glm::mat3> S;

	vec3N_float init_pos; 
	vec3N_float vel;
	vec3N_float force;
	vec2N_int edge;
	vec2N_int edge_triangle;
	std::vector<float> w;

	size_t nT;
	size_t nV;
	size_t nE;
};

