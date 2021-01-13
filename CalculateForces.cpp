#include "ClothSimulator.h"

///////////////////////////////////////////////////////////////////////////////////////////
//external forces
void ClothSimulator::calcGravityForce() {
	glm::vec3 g(0.f, -9.8f, 0.f);
#pragma omp parallel for
	for (int i = 0; i < nV; i++)
	{
		if (w[i] == 0) continue;
		force[i] += g / w[i];
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
//internal forces
//calculate force & matrix K
void ClothSimulator::calcStretchShearForce()
{
	float kst = 30000;
	float ksh = 1000;
	//0:i 1:j 2:k
	for (int t = 0; t < nT; t++) {

		float v1, v2, u1, u2;
		glm::ivec3 idx = triangle[t];
		v1 = init_pos[idx[1]][2] - init_pos[idx[0]][2];
		v2 = init_pos[idx[2]][2] - init_pos[idx[0]][2];
		u1 = init_pos[idx[1]][0] - init_pos[idx[0]][0];
		u2 = init_pos[idx[2]][0] - init_pos[idx[0]][0];

		float det = u1 * v2 - u2 * v1;
		glm::vec3 Su, Sv;
		float Cv[3], Cu[3];
		Cu[0] = (v1 - v2) / det; Cu[1] = v2 / det; Cu[2] = -v1 / det;
		Cv[0] = (u2 - u1) / det; Cv[1] = -u2 / det; Cv[2] = u1 / det;
		Su = Cu[0] * pos[idx[0]] + Cu[1] * pos[idx[1]] + Cu[2] * pos[idx[2]];
		Sv = Cv[0] * pos[idx[0]] + Cv[1] * pos[idx[1]] + Cv[2] * pos[idx[2]];
		//float A = powf(abs(det/2), 2);
		float A = abs(det / 2);
		float Sv_norm = glm::length(Sv);
		float Su_norm = glm::length(Su);
		for (int i = 0; i < 3; i++) {
			force[idx[i]] += -kst * A * Cv[i] * (1 - 1.0f / Sv_norm) * Sv;
			force[idx[i]] += -kst * A * Cu[i] * (1 - 1.0f / Su_norm) * Su;
			force[idx[i]] += -2 * ksh * A * glm::dot(Su, Sv) * (Cu[i] * Sv + Cv[i] * Su);
		}

		glm::mat3 Kij;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				Kij = glm::mat3();
				Kij += -kst * A * Cv[i] * Cv[j] * ((1 - 1.0f / Sv_norm) * Identity
					+ 1.0f / Sv_norm * glm::outerProduct(Sv, Sv) / glm::dot(Sv, Sv));
				Kij += -kst * A * Cu[i] * Cu[j] * ((1 - 1.0f / Su_norm) * Identity
					+ 1.0f / Su_norm * glm::outerProduct(Su, Su) / glm::dot(Su, Su));
				Kij += -2 * ksh * A * (glm::outerProduct((Cu[i] * Sv + Cv[i] * Su), (Cu[j] * Sv + Cv[j] * Su))
					+ glm::dot(Su, Sv) * (Cu[i] * Cv[j] + Cv[i] * Cu[j]) * Identity);
				if (Kij != glm::mat3()) {
					if (K.isNonzero(idx[i], idx[j])) K.increaseValue(idx[i], idx[j], Kij);
					else K.insertValue(idx[i], idx[j], Kij);
				}
			}
		}
	}
}

glm::mat3 getS(glm::vec3 v)
{
	//col_type 
	return glm::mat3(0, -v[2], v[1], v[2], 0, -v[0], -v[1], v[0], 0);
}

void ClothSimulator::calcBendingForce()
{
	float ku = 2;
	float kv = 2;
	for (int ed = 0; ed < nE; ed++) {
		glm::ivec3 t1 = triangle[edge_triangle[ed][0]];
		glm::ivec3 t2 = triangle[edge_triangle[ed][1]];

		int x_idx[4];
		if (t1[0] == t2[0]) {
			//diagonal
			x_idx[0] = t2[2];
			x_idx[1] = t2[1];
			x_idx[2] = t1[0];
			x_idx[3] = t1[1];
		}
		else if (t1[1] == t2[1]) {
			//horizontal
			x_idx[0] = t1[0];
			x_idx[1] = t1[2];
			x_idx[2] = t2[1];
			x_idx[3] = t2[2];
		}
		else if (t1[2] == t2[2]) {
			//vertical
			x_idx[0] = t1[0];
			x_idx[1] = t2[2];
			x_idx[2] = t1[1];
			x_idx[3] = t2[1];
		}
		glm::vec3 x[4];
		for (int i = 0; i < 4; i++) {
			x[i] = pos[x_idx[i]];
		}
		glm::vec3 na = glm::cross(x[2] - x[0], x[1] - x[0]);
		glm::vec3 nb = glm::cross(x[1] - x[3], x[2] - x[3]);
		glm::vec3 e = x[1] - x[2];
		float na_norm = glm::length(na);
		float nb_norm = glm::length(nb);
		float e_norm = glm::length(e);
		//omit hat
		na /= na_norm;
		nb /= nb_norm;
		e /= e_norm;
		glm::mat3 na_x[4], nb_x[4], e_x[4];
		//na_x
		na_x[0] = getS(x[2] - x[1]) / na_norm;
		na_x[1] = getS(x[0] - x[2]) / na_norm;
		na_x[2] = getS(x[1] - x[0]) / na_norm;
		na_x[3] = getS(glm::vec3()) / na_norm;
		//nb_x
		nb_x[0] = getS(glm::vec3()) / nb_norm;
		nb_x[1] = getS(x[2] - x[3]) / nb_norm;
		nb_x[2] = getS(x[3] - x[1]) / nb_norm;
		nb_x[3] = getS(x[1] - x[2]) / nb_norm;
		//e_x
		e_x[0] = glm::mat3() / e_norm;
		e_x[1] = Identity / e_norm;
		e_x[2] = -Identity / e_norm;
		e_x[3] = glm::mat3() / e_norm;
		glm::vec3 sin_x[4], cos_x[4];
		for (int i = 0; i < 4; i++) {
			cos_x[i] = nb * na_x[i] + na * nb_x[i];
		}
		for (int i = 0; i < 4; i++) {
			for (int m = 0; m < 3; m++) {
				sin_x[i][m] = glm::dot(glm::cross(na_x[i][m], nb) + glm::cross(na, nb_x[i][m]), e)
					+ glm::dot(glm::cross(na, nb), e_x[i][m]);
			}
		}
		glm::vec3 theta_x[4];
		float cos_th = glm::dot(na, nb);
		float sin_th = glm::dot(glm::cross(na, nb), e);
		for (int i = 0; i < 4; i++) {
			theta_x[i] = cos_th * sin_x[i] - sin_th * cos_x[i];
		}
		float u = init_pos[edge[ed][0]][0] - init_pos[edge[ed][1]][0];
		float v = init_pos[edge[ed][0]][2] - init_pos[edge[ed][1]][2];
		float kb = (ku * powf(u, 2) + kv * powf(v, 2)) / (powf(u, 2) + powf(v, 2));
		float theta = atan2f(sin_th, cos_th);

		for (int i = 0; i < 4; i++) {
			force[x_idx[i]] += -kb * theta * theta_x[i];
		}

		glm::mat3 Kij;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Kij = -kb * glm::outerProduct(theta_x[i], theta_x[j]);
				if (Kij != glm::mat3()) {
					if (K.isNonzero(x_idx[i], x_idx[j])) K.increaseValue(x_idx[i], x_idx[j], Kij);
					else K.insertValue(x_idx[i], x_idx[j], Kij);
				}
			}
		}
	}
}