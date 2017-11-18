#include <iostream>
#pragma once

namespace CGJM{

class Vec2{
private:
	float vec[2];
public:
	Vec2();
	Vec2(const float& x, const float& y);
	Vec2(const Vec2& other);

	std::ostream& write(std::ostream& os) const;
	std::istream& read(std::istream& is);

	float quadrance()const;
	float norm() const;
	const Vec2 normalize() const;

	float& operator[](std::size_t i);
	float operator[](std::size_t i) const;
	bool operator==(const Vec2& other)const;
	bool operator!=(const Vec2& other)const;
	const Vec2 operator=(const Vec2& other);
	const Vec2 operator+(const Vec2& right) const;
	const Vec2 operator-(const Vec2& right) const;
};

class Vec3;

class Vec4{
private:
	float vec[4];
public:
	Vec4();
	Vec4(const float& x, const float& y, const float& z, const float& w);
	Vec4(const Vec4& other);

	std::ostream& write(std::ostream& os) const;
	std::istream& read(std::istream& is);

	float quadrance()const;
	float norm() const;
	const Vec4 normalize() const;

	const Vec4 rotate(const Vec3& a, const float& t)const ;

	float& operator[](std::size_t i);
	float operator[](std::size_t i) const;
	bool operator==(const Vec4& other)const;
	bool operator!=(const Vec4& other)const;
	const Vec4 operator=(const Vec4& other);
	const Vec4 operator+(const Vec4& right) const;
	const Vec4 operator-(const Vec4& right) const;
};

class Vec3{
private:
	float vec[3];
public:
	Vec3();
	Vec3(const float& x, const float& y, const float& z);
	Vec3(const Vec3& other);
	Vec3(const Vec4& other);

	std::ostream& write(std::ostream& os) const;
	std::istream& read(std::istream& is);

	float quadrance()const;
	float norm() const;
	const Vec3 normalize() const;

	const Vec3 rotate(const Vec3& a, const float& t)const ;

	float& operator[](std::size_t i);
	float operator[](std::size_t i) const;
	bool operator==(const Vec3& other)const;
	bool operator!=(const Vec3& other)const;
	const Vec3 operator=(const Vec3& other);
	const Vec3 operator+(const Vec3& right) const;
	const Vec3 operator-(const Vec3& right) const;
};


float dot(const Vec2& v1, const Vec2& v2);
float dot(const Vec3& v1, const Vec3& v2);
const Vec3 cross(const Vec3& v1, const Vec3& v2);
}



using namespace CGJM;
std::ostream& operator<<(std::ostream &os, const Vec2& v);
std::ostream& operator<<(std::ostream &os, const Vec3& v);
std::ostream& operator<<(std::ostream &os, const Vec4& v);
std::istream& operator>>(std::istream &is, Vec2& v);
std::istream& operator>>(std::istream &is, Vec3& v);
std::istream& operator>>(std::istream &is, Vec4& v);
const Vec2 operator*(const Vec2& v, float scalar);
const Vec3 operator*(const Vec3& v, float scalar);
const Vec4 operator*(const Vec4& v, float scalar);
const Vec2 operator*(float scalar,const Vec2& v);
const Vec3 operator*(float scalar,const Vec3& v);
const Vec4 operator*(float scalar,const Vec4& v);



