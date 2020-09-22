#include "Quaternion.h"

///////////////////////////////////////////////////////////////////////////////////
//Constructer/Setter

Quaternion::Quaternion(const glm::vec3& from, const glm::vec3& to) {
	const float epsilon = 1e-10f;

	const float fromSqNorm = glm::dot(from, from);
	const float toSqNorm = glm::dot(to, to);
	// Identity Quaternion when one vector is null
	if ((fromSqNorm < epsilon) || (toSqNorm < epsilon)) {
		q[0] = q[1] = q[2] = 0.0;
		q[3] = 1.0;
	}
	else {
		glm::vec3 axis = cross(from, to);
		const float axisSqNorm = glm::dot(axis, axis);

		// Aligned vectors, pick any axis, not aligned with from or to
		if (axisSqNorm < epsilon)
			axis = glm::normalize(
				glm::cross((abs(from.x) < 1. ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0)), from)
			);
		//axis = glm::vec3(1,0,0);

		float angle = asin(sqrt(axisSqNorm / (fromSqNorm * toSqNorm)));

		if (glm::dot(from, to) < 0.f)
			angle = PI - angle;

		setAxisAngle(axis, angle);
	}
}

void Quaternion::setAxisAngle(const glm::vec3& axis, float angle) {
	const float norm = glm::length(axis);
	if (norm < 1e-8) {
		// Null rotation
		q[0] = 0.0; q[1] = 0.0; q[2] = 0.0;
		q[3] = 1.0;
	}
	else {
		const float sin_half_angle = (float)sin(0.5f * angle);
		q[0] = sin_half_angle * axis[0] / norm;
		q[1] = sin_half_angle * axis[1] / norm;
		q[2] = sin_half_angle * axis[2] / norm;
		q[3] = (float)cos(angle / 2.0);
	}
}

void Quaternion::setFromRotationMatrix(const float m[3][3]) {
	// Compute one plus the trace of the matrix
	const float onePlusfloatrace = 1.f + m[0][0] + m[1][1] + m[2][2];

	if (onePlusfloatrace > 1E-5) {
		// Direct computation
		const float s = sqrt(onePlusfloatrace) * 2.f;
		q[0] = (m[2][1] - m[1][2]) / s;
		q[1] = (m[0][2] - m[2][0]) / s;
		q[2] = (m[1][0] - m[0][1]) / s;
		q[3] = 0.25f * s;
	}
	else {
		// Computation depends on major diagonal term
		if ((m[0][0] > m[1][1])& (m[0][0] > m[2][2])) {
			const float s = sqrt(1.f + m[0][0] - m[1][1] - m[2][2]) * 2.f;
			q[0] = 0.25f * s;
			q[1] = (m[0][1] + m[1][0]) / s;
			q[2] = (m[0][2] + m[2][0]) / s;
			q[3] = (m[1][2] - m[2][1]) / s;
		}
		else if (m[1][1] > m[2][2]) {
			const float s = sqrt(1.f + m[1][1] - m[0][0] - m[2][2]) * 2.f;
			q[0] = (m[0][1] + m[1][0]) / s;
			q[1] = 0.25f * s;
			q[2] = (m[1][2] + m[2][1]) / s;
			q[3] = (m[0][2] - m[2][0]) / s;
		}
		else {
			const float s = sqrt(1.f + m[2][2] - m[0][0] - m[1][1]) * 2.f;
			q[0] = (m[0][2] + m[2][0]) / s;
			q[1] = (m[1][2] + m[2][1]) / s;
			q[2] = 0.25f * s;
			q[3] = (m[0][1] - m[1][0]) / s;
		}
	}
	normalize();
}

void Quaternion::setFromRotatedBasis(const glm::vec3& X, const glm::vec3& Y, const glm::vec3& Z) {
	float m[3][3];
	float normX = glm::length(X);
	float normY = glm::length(Y);
	float normZ = glm::length(Z);

	for (int i = 0; i < 3; ++i) {
		m[i][0] = X[i] / normX;
		m[i][1] = Y[i] / normY;
		m[i][2] = Z[i] / normZ;
	}

	setFromRotationMatrix(m);
}

///////////////////////////////////////////////////////////////////////////////////
//Getter

void Quaternion::getMatrix(float m[4][4]) const {
	const float q00 = (float)(2.0 * q[0] * q[0]);
	const float q11 = (float)(2.0 * q[1] * q[1]);
	const float q22 = (float)(2.0 * q[2] * q[2]);

	const float q01 = (float)(2.0 * q[0] * q[1]);
	const float q02 = (float)(2.0 * q[0] * q[2]);
	const float q03 = (float)(2.0 * q[0] * q[3]);

	const float q12 = (float)(2.0 * q[1] * q[2]);
	const float q13 = (float)(2.0 * q[1] * q[3]);

	const float q23 = (float)(2.0 * q[2] * q[3]);

	m[0][0] = (float)(1.0l - q11 - q22);
	m[1][0] = (float)(q01 + q23);
	m[2][0] = (float)(q02 - q13);

	m[0][1] = (float)(q01 - q23);
	m[1][1] = (float)(1.0l - q22 - q00);
	m[2][1] = (float)(q12 + q03);

	m[0][2] = (float)(q02 + q13);
	m[1][2] = (float)(q12 - q03);
	m[2][2] = (float)(1.0l - q11 - q00);

	m[0][3] = (float)(0.0l);
	m[1][3] = (float)(0.0l);
	m[2][3] = (float)(0.0l);

	m[3][0] = (float)(0.0l);
	m[3][1] = (float)(0.0l);
	m[3][2] = (float)(0.0l);
	m[3][3] = (float)(1.0l);
}

void Quaternion::getRotationMatrix(float m[3][3]) const {
	static float mat[4][4];
	getMatrix(mat);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			m[i][j] = mat[j][i]; // Beware of transposition
}

glm::mat3 Quaternion::getRotationMatrix() const {
	static float mat[4][4];
	getMatrix(mat);
	return glm::mat3(mat[0][0], mat[1][0], mat[2][0],
		mat[0][1], mat[1][1], mat[2][1],
		mat[0][2], mat[1][2], mat[2][2]);
}

void Quaternion::getInverseRotationMatrix(float m[3][3]) const {
	static float mat[4][4];
	getInverseMatrix(mat);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			m[i][j] = mat[j][i]; // Beware of transposition
}

glm::mat3 Quaternion::getInverseRotationMatrix() const {
	float mat[4][4];
	getInverseMatrix(mat);
	return glm::mat3(mat[0][0], mat[1][0], mat[2][0],
		mat[0][1], mat[1][1], mat[2][1],
		mat[0][2], mat[1][2], mat[2][2]);
}

glm::mat4 Quaternion::getSE3() {
	float mat[4][4];
	getMatrix(mat);
	return glm::mat4(mat[0][0], mat[1][0], mat[2][0], mat[3][0], // col 0
		mat[0][1], mat[1][1], mat[2][1], mat[3][1], // col 1
		mat[0][2], mat[1][2], mat[2][2], mat[3][2], // col 2
		mat[0][3], mat[1][3], mat[2][3], mat[3][3]);// col 3
}

void Quaternion::getAxisAngle(glm::vec3& axis, float& angle) const {
	angle = 2.f * acos(q[3]);
	axis = glm::vec3(q[0], q[1], q[2]);
	const float sinus = glm::length(axis);
	if (sinus > 1E-8)
		axis /= sinus;

	if (angle > PI) {
		angle = 2.f * PI - angle;
		axis = -axis;
	}
}

glm::vec3 Quaternion::axis() const {
	glm::vec3 res(q[0], q[1], q[2]);
	const float sinus = glm::length(res);
	if (sinus > 1e-8) res /= sinus;
	return (acos(q[3]) <= PI / 2.f) ? res : -res;
}

float Quaternion::angle() const {
	const float angle = 2.f * acos(q[3]);
	return (angle <= PI) ? angle : 2.f * PI - angle;
}

///////////////////////////////////////////////////////////////////////////////////
//Arithmatic

float Quaternion::normalize() {
	const float norm = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	for (int i = 0; i < 4; ++i) q[i] /= norm;
	return norm;
}

Quaternion Quaternion::normalized() const {
	float Q[4];
	const float norm = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	for (int i = 0; i < 4; ++i) Q[i] = q[i] / norm;
	return Quaternion(Q[0], Q[1], Q[2], Q[3]);
}

glm::vec3 Quaternion::rotate(const glm::vec3& v) const {
	const float q00 = (float)(2.0l * q[0] * q[0]);
	const float q11 = (float)(2.0l * q[1] * q[1]);
	const float q22 = (float)(2.0l * q[2] * q[2]);

	const float q01 = (float)(2.0l * q[0] * q[1]);
	const float q02 = (float)(2.0l * q[0] * q[2]);
	const float q03 = (float)(2.0l * q[0] * q[3]);

	const float q12 = (float)(2.0l * q[1] * q[2]);
	const float q13 = (float)(2.0l * q[1] * q[3]);

	const float q23 = (float)(2.0l * q[2] * q[3]);

	return glm::vec3(
		(float)((1.0 - q11 - q22) * v[0] + (q01 - q23) * v[1] + (q02 + q13) * v[2]),
		(float)((q01 + q23) * v[0] + (1.0 - q22 - q00) * v[1] + (q12 - q03) * v[2]),
		(float)((q02 - q13) * v[0] + (q12 + q03) * v[1] + (1.0 - q11 - q00) * v[2])
	);
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b, float t, bool allowFlip) {
	float cosAngle = Quaternion::dot(a, b);
	float c1, c2;

	// Linear interpolation for close orientations
	if ((1.0 - fabs(cosAngle)) < 0.01) {
		c1 = (float)1.0 - t;
		c2 = t;
	}
	else {
		// Spherical interpolation
		float angle = acos(fabs(cosAngle));
		float sinAngle = sin(angle);
		c1 = (float)sin(angle * (1.0 - t)) / sinAngle;
		c2 = (float)sin(angle * t) / sinAngle;
	}

	// Use the shortest path
	if (allowFlip && (cosAngle < 0.0))
		c1 = -c1;

	return Quaternion(c1 * a[0] + c2 * b[0], c1 * a[1] + c2 * b[1], c1 * a[2] + c2 * b[2], c1 * a[3] + c2 * b[3]);
}

Quaternion Quaternion::squad(const Quaternion& a, const Quaternion& tgA, const Quaternion& tgB, const Quaternion& b, float t) {
	Quaternion ab = Quaternion::slerp(a, b, t);
	Quaternion tg = Quaternion::slerp(tgA, tgB, t, false);
	return Quaternion::slerp(ab, tg, 2.f * t * (1.f - t), false);
}

Quaternion Quaternion::log() {
	float len = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);

	if (len < 1e-6)
		return Quaternion(q[0], q[1], q[2], 0.0);
	else {
		float coef = acos(q[3]) / len;
		return Quaternion(q[0] * coef, q[1] * coef, q[2] * coef, 0.0);
	}
}

Quaternion Quaternion::exp() {
	float theta = sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2]);

	if (theta < 1E-6)
		return Quaternion(q[0], q[1], q[2], cos(theta));
	else {
		float coef = sin(theta) / theta;
		return Quaternion(q[0] * coef, q[1] * coef, q[2] * coef, cos(theta));
	}
}

Quaternion Quaternion::lnDif(const Quaternion& a, const Quaternion& b) {
	Quaternion dif = a.inverse() * b;
	dif.normalize();
	return dif.log();
}

Quaternion Quaternion::squadfloatangent(const Quaternion& before, const Quaternion& center, const Quaternion& after) {
	Quaternion l1 = Quaternion::lnDif(center, before);
	Quaternion l2 = Quaternion::lnDif(center, after);
	Quaternion e;
	for (int i = 0; i < 4; ++i)
		e.q[i] = -0.25f * (l1.q[i] + l2.q[i]);
	e = center * (e.exp());

	// if (Quaternion::dot(e,b) < 0.0)
	// e.negate();

	return e;
}