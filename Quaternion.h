#pragma once
#include <glm/glm.hpp>
#include <math.h>

const float PI = 3.14159265358979323846f;

class Quaternion
{
public:
	///////////////////////////////////////////////////////////////////////////////////
	//Constructors
	//Identity rotation
	Quaternion() { q[0] = q[1] = q[2] = 0.0; q[3] = 1.0; }
	//rotation axis/angle(radian)
	Quaternion(const glm::vec3& axis, float angle) { setAxisAngle(axis, angle); }
	//setting 4 elements
	Quaternion(float q0, float q1, float q2, float q3) { q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3; }
	//from -> to vector
	Quaternion(const glm::vec3& from, const glm::vec3& to);
	//copy constructor
	Quaternion(const Quaternion& Q) { for (int i = 0; i < 4; i++) q[i] = Q.q[i]; }

	///////////////////////////////////////////////////////////////////////////////////
	//Setter
	void setAxisAngle(const glm::vec3& axis, float angle);
	void setValue(float q0, float q1, float q2, float q3) { q[0] = q0; q[1] = q1; q[2] = q2; q[3] = q3; }
	void setFromRotationMatrix(const float m[3][3]);
	void setFromRotatedBasis(const glm::vec3& X, const glm::vec3& Y, const glm::vec3& Z);
	void identity() { q[0] = q[1] = q[2] = 0.0;  q[3] = 1.0; }

	///////////////////////////////////////////////////////////////////////////////////
	//Getter
	glm::vec3 axis() const;
	inline float angle() const;
	inline void getAxisAngle(glm::vec3& axis, float& angle) const;

	///////////////////////////////////////////////////////////////////////////////////
	//Operator
	float& operator[](int i) { return q[i]; }
	const float& operator[](int i) const { return q[i]; }

	friend Quaternion operator*(const Quaternion& a, const Quaternion& b) {
		return Quaternion(a.q[3] * b.q[0] + b.q[3] * a.q[0] + a.q[1] * b.q[2] - a.q[2] * b.q[1],
			a.q[3] * b.q[1] + b.q[3] * a.q[1] + a.q[2] * b.q[0] - a.q[0] * b.q[2],
			a.q[3] * b.q[2] + b.q[3] * a.q[2] + a.q[0] * b.q[1] - a.q[1] * b.q[0],
			a.q[3] * b.q[3] - b.q[0] * a.q[0] - a.q[1] * b.q[1] - a.q[2] * b.q[2]);
	}
	friend glm::vec3 operator*(const Quaternion& Q, const glm::vec3& v) { return Q.rotate(v); }
	Quaternion& operator=(const Quaternion& Q) {
		memcpy(q, Q.q, 4 * sizeof(float));
		return (*this);
	}
	Quaternion& operator*=(const Quaternion& Q) {
		*this = (*this) * Q;
		return *this;
	}
	
	///////////////////////////////////////////////////////////////////////////////////
	//Arithmatic

	//rotate
	glm::vec3 rotate(const glm::vec3& v) const;
	glm::vec3 inverseRotate(const glm::vec3& v) const { return inverse().rotate(v); }

	//inverse, invert, negate 
	Quaternion inverse() const { return Quaternion(-q[0], -q[1], -q[2], q[3]); }
	void invert() { q[0] = -q[0]; q[1] = -q[1]; q[2] = -q[2]; }
	void negate() { invert(); q[3] = -q[3]; }

	//normalize
	float normalize();
	Quaternion normalized() const;

	//Matrix
	void getMatrix(float m[4][4]) const;
	void getInverseMatrix(float m[4][4]) const { inverse().getMatrix(m); }
	void getRotationMatrix(float m[3][3]) const;
	glm::mat3 getRotationMatrix() const;
	void getInverseRotationMatrix(float m[3][3]) const;
	glm::mat3 getInverseRotationMatrix() const;
	glm::mat4 getSE3();

	//other
	static Quaternion slerp(const Quaternion& a, const Quaternion& b, float t, bool allowFlip = true);
	static Quaternion squad(const Quaternion& a, const Quaternion& tgA, const Quaternion& tgB, const Quaternion& b, float t);
	static float dot(const Quaternion& a, const Quaternion& b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]; }

	Quaternion log();
	Quaternion exp();
	Quaternion lnDif(const Quaternion& a, const Quaternion& b);
	Quaternion squadfloatangent(const Quaternion& before, const Quaternion& center, const Quaternion& after);

	Quaternion randomQuaternion();

private:
	//4D vector of quaternion
	float q[4];
};

