#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"

using namespace CGJM;

Vec4::Vec4(){
	vec[0]=0;
	vec[1]=0;
	vec[2]=0;
	vec[3]=0;
}

Vec4::Vec4(const float& x, const float& y, const float& z, const float& w){
	vec[0]=x;
	vec[1]=y;
	vec[2]=z;
	vec[3]=w;
}


Vec4::Vec4(const Vec4& other){
	vec[0]=other[0];
	vec[1]=other[1];
	vec[2]=other[2];
	vec[3]=other[3];
}


float Vec4::quadrance()const{
	return vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]+vec[3]*vec[3];
}

float Vec4::norm()const{
	return std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]+vec[3]*vec[3]);
}

const Vec4 Vec4::normalize()const {
	float length = norm();
	return Vec4(vec[0] / length, vec[1] / length, vec[2] / length, vec[3] / length);
}


float Vec4::operator[](std::size_t i) const{
	assert(i<4);
	return (vec)[i];
}

float& Vec4::operator[](std::size_t i){
	assert(i<4);
	return (vec)[i];
}

std::ostream& Vec4::write(std::ostream& os) const{
	return os << "[ " << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] <<  " ]";
}


std::ostream& operator<<(std::ostream& os, const Vec4& v){
	return v.write(os);
}

std::istream& Vec4::read(std::istream& is){
	std::string line;
	getline(is, line);
	for(unsigned int i=0;i<line.length();i++)
		if(line[i] == '[' || line[i] == ']')
			line[i] = ' ';
	std::istringstream iss(line);
	iss >> vec[0] >> vec[1] >> vec[2] >> vec[3];
	return is;
}

std::istream& operator>>(std::istream& is, Vec4& v){
	return v.read(is);
}


const Vec4 Vec4::operator=(const Vec4& other){
	vec[0] = other[0];
	vec[1] = other[1];
	vec[2] = other[2];
	vec[3] = other[3];
	return *this;
}

bool Vec4::operator==(const Vec4& other)const{
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(vec[0]-other[0]) < EPSILON) &&
		(std::abs(vec[1]-other[1]) < EPSILON) &&
		(std::abs(vec[2]-other[2]) < EPSILON) &&
		(std::abs(vec[3]-other[3]) < EPSILON));

}

bool Vec4::operator!=(const Vec4& other) const{
	return !(*this==other);
}
const Vec4 Vec4::operator+(const Vec4& right) const{
	return Vec4(vec[0]+right[0], vec[1]+right[1], vec[2]+right[2], vec[3]+right[3]);
}

const Vec4 Vec4::operator-(const Vec4& right) const{
	return Vec4(vec[0]-right[0], vec[1]-right[1], vec[2]-right[2], vec[3]-right[3]);
}

const Vec4 operator*(const Vec4& v, float scalar){
	return Vec4(v[0]*scalar, v[1]*scalar, v[2]*scalar, v[3]*scalar);
}

const Vec4 operator*(float scalar, const Vec4& v){
	return Vec4(v[0]*scalar, v[1]*scalar, v[2]*scalar, v[3]*scalar);
}

const Vec4 Vec4::rotate(const Vec3& a, const float& t)const{
	Vec3 me = Vec3(*this);
	Vec3 r= me*std::cos(t)+
            CGJM::cross(a, me)*std::sin(t)+
            a*CGJM::dot(a, me)*(1-std::cos(t));

	return Vec4(r[0], r[1], r[2], 1.0f);
}
