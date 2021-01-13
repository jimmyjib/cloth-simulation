#include "ClothSimulator.h"

//set S matrix based on dimension of freedom
void ClothSimulator::setS(int idx, int ndof, glm::vec3 p, glm::vec3 q)
{
	switch (ndof)
	{
	case 3:
		S[idx] = Identity;
		break;
	case 2:
		S[idx] = Identity - glm::outerProduct(p, p);
		break;
	case 1:
		S[idx] = Identity - glm::outerProduct(p, p) - glm::outerProduct(q, q);
		break;
	case 0:
		S[idx] = glm::mat3();
		break;
	default:
		break;
	}
}

//filter
vec3N_float ClothSimulator::filter(vec3N_float& v)
{
	vec3N_float new_v;
	for (int i = 0; i < v.size(); i++) {
		new_v.push_back(S[i] * v[i]);
	}
	return new_v;
}

vec3N_float ClothSimulator::filter(vec3N_float&& v)
{
	vec3N_float new_v;
	for (int i = 0; i < v.size(); i++) {
		new_v.push_back(S[i] * v[i]);
	}
	return new_v;
}

//main function
vec3N_float ClothSimulator::ConjugateGradient(SparseMatrix&& A_r, vec3N_float&& b_r, float e)
{
	SparseMatrix A = A_r;
	vec3N_float b = b_r;
	vec3N_float x(b.size(), glm::vec3(0, 0, 0));
	vec3N_float P = A.getInvDiagonal();
	vec3N_float P_Inv = A.getDiagonal();
	float delta_zero = Dot(filter(b), P_Inv * b);
	vec3N_float r = filter(b - A * x);
	vec3N_float c = filter(P_Inv * r);
	float delta_new = Dot(r, c);
	int iteration = 0;
	while (delta_new > powf(e, 2)* delta_zero) {
		vec3N_float q = filter(A * c);
		float alpha = delta_new / (Dot(c, q));
		x = x + alpha * c;
		r = r - alpha * q;
		vec3N_float s = P_Inv * r;
		float delta_old = delta_new;
		delta_new = Dot(r, s);
		c = filter(s + (delta_new / delta_old) * c);
		iteration++;
	}

	//cout << "debug : " << delta_new << endl;
	//cout << "debug : " << iteration << endl;
	return x;
}