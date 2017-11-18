#include <iostream>
#pragma once

#include "vec.h"

namespace CGJM{

class Mat4{
private:
	//Internally row-major
	float mat[4][4];
public:
	Mat4();
	Mat4(const float& d);	
	Mat4(const float& a11, const float& a12, const float& a13, const float& a14,
			const float& a21, const float& a22, const float& a23, const float& a24,
			const float& a31, const float& a32, const float& a33, const float& a34,
			const float& a41, const float& a42, const float& a43, const float& a44 );
	Mat4(const Mat4& other);

	std::ostream& write(std::ostream& os) const;

	const Mat4 transpose() const;
	const Mat4 inverse() const;
	float determinant() const;

	const float* operator[](int i) const;
	const Mat4 operator=(const Mat4& other);
	bool operator==(const Mat4& other)const;
	bool operator!=(const Mat4& other)const;
	const Mat4 operator+(const Mat4& right) const;
	const Mat4 operator-(const Mat4& right) const;
	const Mat4 operator*(const Mat4& right) const;
	const Vec3 operator*(const Vec3& v) const;
	operator const float*() const;
};

class Mat3{
private:
	//Internally row-major
	float mat[3][3];
public:
	Mat3();
	Mat3(const float& d);	
	Mat3(const float& a11, const float& a12, const float& a13,
			const float& a21, const float& a22, const float& a23,
			const float& a31, const float& a32, const float& a33);
	Mat3(const Mat3& other);
	Mat3(const Mat4& other);

	std::ostream& write(std::ostream& os) const;

	const Mat3 transpose() const;
	const Mat3 inverse() const;
	float determinant() const;

	const float* operator[](int i) const;
	const Mat3 operator=(const Mat3& other);
	bool operator==(const Mat3& other)const;
	bool operator!=(const Mat3& other)const;
	const Mat3 operator+(const Mat3& right) const;
	const Mat3 operator-(const Mat3& right) const;
	const Mat3 operator*(const Mat3& right) const;
	const Vec3 operator*(const Vec3& v) const;
	operator const float*() const;
};

class Mat2{
private:
	//Internally row-major
	float mat[2][2];
public:
	Mat2();
	Mat2(const float& d);	
	Mat2(const float& a11, const float& a12,
			const float& a21, const float&a22);
	Mat2(const Mat2& other);
	Mat2(const Mat3& other);
	Mat2(const Mat4& other);

	std::ostream& write(std::ostream& os) const;

	const Mat2 transpose() const;
	const Mat2 inverse() const;
	float determinant() const;

	const float* operator[](int i) const;
	const Mat2 operator=(const Mat2& other);
	bool operator==(const Mat2& other)const;
	bool operator!=(const Mat2& other)const;
	const Mat2 operator+(const Mat2& right) const;
	const Mat2 operator-(const Mat2& right) const;
	const Mat2 operator*(const Mat2& right) const;
	const Vec2 operator*(const Vec2& v) const;
	operator const float*() const;
};

const Mat4 scale(const float& x, const float& y, const float& z);
const Mat4 rotate(const Vec3& axis, const float& angle);
const Mat4 translate(const float& x, const float& y, const float& z);
const Mat3 dualMatrix(const Vec3& v);

const Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
const Mat4 ortho(const float& left, const float& right, const float& top, const float& bottom, const float& near, const float& far);
const Mat4 perspective(const float& fovy, const float& aspectRatio, const float& near, const float& far);
}


using namespace CGJM;
std::ostream& operator<<(std::ostream &os, const Mat2& v);
std::ostream& operator<<(std::ostream &os, const Mat3& v);
std::ostream& operator<<(std::ostream &os, const Mat4& v);
const Mat2 operator*(const Mat2& A, float scalar);
const Mat2 operator*(float scalar, const Mat2& A);
const Mat3 operator*(const Mat3& A, float scalar);
const Mat4 operator*(float scalar, const Mat4& A);
const Mat4 operator*(const Mat4& A, float scalar);
const Mat3 operator*(float scalar, const Mat3& A);
