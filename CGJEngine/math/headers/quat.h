#include <iostream>
#pragma once

#include "vec.h"
#include "mat.h"

namespace CGJM{
#define __Q_t 0
#define __Q_x 1
#define __Q_y 2
#define __Q_z 3

class Quat{
public:
	float t, x, y, z;
	static const float qThreshold;
public:
	Quat();
	Quat(const float& angle, const Vec3& axis);
	Quat(const float& t, const float& x, const float& y, const float& z);
	Quat(const Quat& other);

	std::ostream& write(std::ostream& os) const;

	void getAngleAxis(float& angle, Vec3& axis)const;
	void clean();
	const Quat clean()const;
	float quadrance()const;
	float norm()const;
	void normalize();
	const Quat normalize()const;
	const Quat conjugate()const;
	const Quat inverse()const;

	const Mat4 GLMatrix()const;

	const Quat operator=(const Quat& other);
	bool operator==(const Quat& other)const;
	bool operator!=(const Quat& other)const;
	const Quat operator+(const Quat& right) const;
	const Quat operator-(const Quat& right) const;
	const Quat operator*(const Quat& right) const;
};

const Quat lerp(const Quat& q0, const Quat& q1, float k);
const Quat slerp(const Quat& q0, const Quat& q1, float k);
}


using namespace CGJM;
std::ostream& operator<<(std::ostream &os, const Quat& v);
const Quat operator*(float scalar, const Quat& A);
const Quat operator*(const Quat& A, float scalar);
