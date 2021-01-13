#pragma once
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include "Vec3N.h"

//Compressed Sparse Row
class SparseMatrix{
	//linked list structure
	//each entry of Matrix is a 3x3 matrix

	//for commenting M stands for current SparseMatrix
	//Mij stands for ith row & jth col of M 
public:
	void init(int dim) {
		ri.resize(dim + 1);
	}
	void zero()
	{
		for (glm::mat3& m : val)
			m = glm::mat3(0, 0, 0, 0, 0, 0, 0, 0, 0);
	}

	//searching is done from ri[i]~ri[i+1]
	//ri.size()-1 = num of nonzero row
	size_t getDim() const { return ri.size() - 1; };

	//number of nonzero 3x3 matrix
	size_t numNonzero() const { return ci.size(); };

	//check if Mij is nonzero
	bool isNonzero(const int i, const int j) const;

	//add m to Mij
	void increaseValue(const int i, const int j, const glm::mat3 m);

	//insert m into M
	void insertValue(const int i, const int j, const glm::mat3 m);

	//return Mij
	glm::mat3 getMatrix(int i, int j);

	//return vec3N of Diagonal entries
	vec3N_float getDiagonal();

	//return vec3N of 1/Diagonal entries(inverse)
	vec3N_float getInvDiagonal();
	
	///////////////////////////////////////////////////////////////////////////////////
	//SparseMatrix & SparseMatrix, SparseMatrix & vec3N operations

	//SparseMatrix + SparseMatrix
	SparseMatrix operator+(const SparseMatrix&& m);
	SparseMatrix operator+(const SparseMatrix& m);

	//SparseMatrix * scalar
	SparseMatrix operator*(const float& c);
	SparseMatrix operator*(const float&& c);

	//SparseMatrix * vec3N
	vec3N_float operator*(const vec3N_float& v);
	vec3N_float operator*(const vec3N_float&& v);

private:
	//linked list - vector도 insert 등이 가능
	std::vector<int> ri;
	std::vector<int> ci;
	std::vector<glm::mat3> val;
};
