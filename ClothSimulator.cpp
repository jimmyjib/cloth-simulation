#include "ClothSimulator.h"

void ClothSimulator::generateMesh(
	const float width, 
	const float height, 
	const int w_div, 
	const int h_div, 
	const float density) 
{
	//widthwise starting position 
	float w_st = -width / 2;
	float w_step = width / w_div;
	float h_step = height / h_div;

	//clear weight array(of each vertex)
	w.clear();

	//set initial vertex position of cloth
	for (int j = 0; j < h_div; j++)
	{
		for (int i = 0; i < w_div; i++)
		{
			//xz-plane y=2
			glm::vec3 p(w_st + i * w_step, 2.f, j * h_step);
			init_pos.push_back(p);
			w.push_back(1.f / density);
		}
	}

	//number of vertex
	nV = init_pos.size();

	//initialize M, K matrices
	M.init(nV);
	K.init(nV);
	for (int i = 0; i < nV; i++) {
		M.insertValue(i, i, w[i] * Identity);
	}

	//initialize S matrices
	for (int i = 0; i < nV; i++) {
		S.push_back(Identity);
	}

	/////////////////////////////////////////////////////////////////////////
	//set constraint

	//int shift = w_div * (h_div / 2);
	int shift = 0;
	//int shift = 0;
	setS(shift, 0);
	setS(shift + w_div - 1, 0);

	//used also on external force(gravity)
	w[shift] = w[shift + w_div - 1] = 0; // fixed constraint

	/////////////////////////////////////////////////////////////////////////
	//initialize position, velocity, force
	pos = init_pos;
	vel.resize(nV);
	force.resize(nV);

	/////////////////////////////////////////////////////////////////////////
	//traingle, edge inforamation
	//make triangle (clockwise indexing)
	for (int j = 0; j < h_div - 1; j++)
	{
		for (int i = 0; i < w_div - 1; i++)
		{
			glm::ivec3 index((j * w_div) + i, (j * w_div) + i + 1, ((j + 1) * w_div) + i + 1);
			triangle.push_back(index);
			glm::ivec3 index2((j * w_div) + i, (j + 1) * w_div + i + 1, (j + 1) * w_div + i);
			triangle.push_back(index2);
		}
	}

	//for each edge find triangle that contains the edge
	//diagonal edge
	for (int j = 0; j < h_div - 1; j++)
	{
		for (int i = 0; i < w_div - 1; i++) {
			glm::ivec2 index((j * w_div) + i, ((j + 1) * w_div) + i + 1);
			edge.push_back(index);
			glm::ivec2 t_index(2 * ((j * (w_div - 1)) + i), 2 * ((j * (w_div - 1)) + i) + 1);
			edge_triangle.push_back(t_index);
		}
	}

	//vertical edge
	for (int j = 0; j < h_div - 1; j++)
	{
		for (int i = 1; i < w_div - 1; i++) {
			glm::ivec2 index((j * w_div) + i, ((j + 1) * w_div) + i);
			edge.push_back(index);
			glm::ivec2 t_index(2 * ((j * (w_div - 1)) + (i - 1)), 2 * ((j * (w_div - 1)) + i) + 1);
			edge_triangle.push_back(t_index);
		}
	}

	//horizontal
	for (int j = 1; j < h_div - 1; j++)
	{
		for (int i = 0; i < w_div - 1; i++) {
			glm::ivec2 index((j * w_div) + i, (j * w_div) + i + 1);
			edge.push_back(index);
			glm::ivec2 t_index(2 * (((j - 1) * (w_div - 1)) + i) + 1, 2 * ((j * (w_div - 1)) + i));
			edge_triangle.push_back(t_index);
		}
	}

	//number of edges
	nE = edge.size();
	//number of triangles
	nT = (w_div - 1) * (h_div - 1) * 2;

	//set color
	glm::vec3 c(1.f, 0.f, 0.f);
	for (int i = 0; i < nV; i++)
	{
		color.push_back(c);
	}

	//generate VAO
	normal.resize(nV);
	obj.genVAO(&pos[0][0], &normal[0][0], &color[0][0], &triangle[0][0], nT, nV);
	obj.updateNormal();
}

void ClothSimulator::solve() {
	initSolver();
	calcExternalForce();
	calcInternalForce();

	// Explicit Solver
	/*for (int i = 0; i < nV; i++)
		vel[i] += h * w[i] * force[i];

	for (int i = 0; i < nV; i++)
		pos[i] += h * vel[i];*/


	//calculate delta_vel by modified CG
	//Implicit Solver
	SparseMatrix K_h_2 = K * (-powf(h, 2));
	vec3N_float delta_vel = ConjugateGradient(M + K_h_2, h * (force + (K * h) * vel), 1e-6);

	for (int i = 0; i < nV; i++)
		vel[i] += delta_vel[i];

	for (int i = 0; i < nV; i++)
		pos[i] += h * vel[i];

	obj.updateVBO();
	obj.updateNormal();
}

///////////////////////////////////////////////////////////////////////////////////////////
void ClothSimulator::initSolver() {
	//initialize force & K to 0
	glm::vec3 zero(0.f, 0.f, 0.f);
#pragma omp parallel for
	for (int i = 0; i < nV; i++)
	{
		force[i] = zero;
	}
	K.zero();
}

void ClothSimulator::calcExternalForce() {
	calcGravityForce();
}

void ClothSimulator::calcInternalForce() {
	calcStretchShearForce();
	calcBendingForce();
}


