#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"

using namespace CGJM;

Vec2::Vec2(){
	vec[0]=0;
	vec[1]=0;
}

Vec2::Vec2(const float& x, const float& y){
	vec[0]=x;
	vec[1]=y;
}


Vec2::Vec2(const Vec2& other){
	vec[0]=other[0];
	vec[1]=other[1];
}


float Vec2::quadrance()const{
	return vec[0]*vec[0]+vec[1]*vec[1];
}

float Vec2::norm() const{
	return std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]);
}

const Vec2 Vec2::normalize() const{
	float length = norm();
	return Vec2(vec[0]/length, vec[1]/length);
}


float Vec2::operator[](std::size_t i) const{
	assert(i<2);
	return (vec)[i];
}

float& Vec2::operator[](std::size_t i){
	assert(i<2);
	return (vec)[i];
}

std::ostream& Vec2::write(std::ostream& os) const{
	return os << "[ " << vec[0] << " " << vec[1] << " ]";
}


std::ostream& operator<<(std::ostream& os, const Vec2& v){
	return v.write(os);
}

std::istream& Vec2::read(std::istream& is){
	std::string line;
	getline(is, line);
	for(unsigned int i=0;i<line.length();i++)
		if(line[i] == '[' || line[i] == ']')
			line[i] = ' ';
	std::istringstream iss(line);
	iss >> vec[0] >> vec[1];
	return is;
}

std::istream& operator>>(std::istream& is, Vec2& v){
	return v.read(is);
}


const Vec2 Vec2::operator=(const Vec2& other){
	vec[0] = other[0];
	vec[1] = other[1];
	return *this;
}

bool Vec2::operator==(const Vec2& other)const{
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(vec[0]-other[0]) < EPSILON) &&
		(std::abs(vec[1]-other[1]) < EPSILON));

}

bool Vec2::operator!=(const Vec2& other) const{
	return !(*this==other);
}

const Vec2 Vec2::operator+(const Vec2& right) const{
	return Vec2(vec[0]+right[0], vec[1]+right[1]);
}

const Vec2 Vec2::operator-(const Vec2& right) const{
	return Vec2(vec[0]-right[0], vec[1]-right[1]);
}

const Vec2 operator*(const Vec2& v, float scalar){
	return Vec2(v[0]*scalar, v[1]*scalar);
}

const Vec2 operator*(float scalar, const Vec2& v){
	return Vec2(v[0]*scalar, v[1]*scalar);
}


float CGJM::dot(const Vec2& v1, const Vec2& v2){
	return v1[0]*v2[0]+v1[1]*v2[1];
}
