#include "SparseMatrix.h"

bool SparseMatrix::isNonzero(const int i, const int j) const
{
	for (int k = ri[i]; k < ri[i + 1]; k++)
		if (ci[k] == j) return true;

	return false;
}

void SparseMatrix::increaseValue(const int i, const int j, const glm::mat3 m)
{
	for (int k = ri[i]; k < ri[i + 1]; k++)
	{
		if (ci[k] == j)
		{
			val[k] += m;
			return;
		}
	}
}

void SparseMatrix::insertValue(const int i, const int j, const glm::mat3 m) {
	if (ri[i] == ri[i + 1]) {
		ci.insert(ci.begin() + ri[i], j);
		val.insert(val.begin() + ri[i], m);
	}
	else {
		for (int k = ri[i]; k < ri[i + 1]; k++) {
			if (k == ri[i + 1] - 1) {
				if (ci[k] <= j) {
					ci.insert(ci.begin() + k + 1, j);
					val.insert(val.begin() + k + 1, m);
					break;
				}
			}
			else if (k == ri[i] && j <= ci[k]) {
				ci.insert(ci.begin() + k, j);
				val.insert(val.begin() + k, m);
				break;
			}
			else if (ci[k] <= j && ci[k + 1] >= j) {
				ci.insert(ci.begin() + k + 1, j);
				val.insert(val.begin() + k + 1, m);
				break;
			}
		}
	}
	for (int k = i + 1; k < ri.size(); k++) ri[k]++;
}

glm::mat3 SparseMatrix::getMatrix(int i, int j) {
	for (int k = ri[i]; k < ri[i + 1]; k++)
	{
		if (ci[k] == j)
		{
			return val[k];
		}
	}
}

vec3N_float SparseMatrix::getDiagonal() {
	vec3N_float diagonal;
	for (int i = 0; i < getDim(); i++) {
		glm::mat3 diag = getMatrix(i, i);
		diagonal.push_back(glm::vec3(diag[0][0], diag[1][1], diag[2][2]));
	}
	return diagonal;
}

vec3N_float SparseMatrix::getInvDiagonal() {
	vec3N_float diagonal;
	for (int i = 0; i < getDim(); i++) {
		glm::mat3 diag = getMatrix(i, i);
		diagonal.push_back(glm::vec3(1.0f / diag[0][0], 1.0f / diag[1][1], 1.0f / diag[2][2]));
	}
	return diagonal;
}

///////////////////////////////////////////////////////////////////////////////////
//operator overloading
SparseMatrix SparseMatrix::operator+(const SparseMatrix&& m) {
	SparseMatrix M = m;
	for (int i = 0; i < getDim(); i++) {
		for (int k = ri[i]; k < ri[i + 1]; k++) {
			int j = ci[k];
			if (M.isNonzero(i, j)) M.increaseValue(i, j, val[k]);
			else M.insertValue(i, j, val[k]);
		}
	}
	return M;
}

SparseMatrix SparseMatrix::operator+(const SparseMatrix& m) {
	SparseMatrix M = m;
	for (int i = 0; i < getDim(); i++) {
		for (int k = ri[i]; k < ri[i + 1]; k++) {
			int j = ci[k];
			if (M.isNonzero(i, j)) M.increaseValue(i, j, val[k]);
			else M.insertValue(i, j, val[k]);
		}
	}
	return M;
}

SparseMatrix SparseMatrix::operator*(const float& c) {
	SparseMatrix M = *(this);
	for (int i = 0; i < M.getDim(); i++) {
		for (int k = M.ri[i]; k < M.ri[i + 1]; k++) {
			M.val[k] *= c;
		}
	}
	return M;
}

SparseMatrix SparseMatrix::operator*(const float&& c) {
	SparseMatrix M = *(this);
	for (int i = 0; i < M.getDim(); i++) {
		for (int k = M.ri[i]; k < M.ri[i + 1]; k++) {
			M.val[k] *= c;
		}
	}
	return M;
}

vec3N_float SparseMatrix::operator*(const vec3N_float& v) {
	vec3N_float new_v;
	for (int i = 0; i < v.size(); i++) {
		glm::vec3 temp(0, 0, 0);
		for (int k = ri[i]; k < ri[i + 1]; k++) {
			temp += v[ci[k]] * val[k];
		}
		new_v.push_back(temp);
	}
	return new_v;
}

vec3N_float SparseMatrix::operator*(const vec3N_float&& v) {
	vec3N_float new_v;
	for (int i = 0; i < v.size(); i++) {
		glm::vec3 temp(0, 0, 0);
		for (int k = ri[i]; k < ri[i + 1]; k++) {
			temp += val[k] * v[ci[k]];
		}
		new_v.push_back(temp);
	}
	return new_v;
}