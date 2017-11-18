#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"

using namespace CGJM;

Vec3::Vec3(){
	vec[0]=0;
	vec[1]=0;
	vec[2]=0;
}

Vec3::Vec3(const float& x, const float& y, const float& z){
	vec[0]=x;
	vec[1]=y;
	vec[2]=z;
}


Vec3::Vec3(const Vec3& other){
	vec[0]=other[0];
	vec[1]=other[1];
	vec[2]=other[2];
}

Vec3::Vec3(const Vec4& other){
	vec[0]=other[0];
	vec[1]=other[1];
	vec[2]=other[2];
}

float Vec3::quadrance()const{
	return vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2];
}

float Vec3::norm()const{
	return std::sqrt(vec[0]*vec[0]+vec[1]*vec[1]+vec[2]*vec[2]);
}

const Vec3 Vec3::normalize()const {
	float length = norm();
	return Vec3( vec[0]/length, vec[1]/length, vec[2]/length);
}


float Vec3::operator[](std::size_t i) const{
	assert(i<3);
	return (vec)[i];
}

float& Vec3::operator[](std::size_t i){
	assert(i<3);
	return (vec)[i];
}

std::ostream& Vec3::write(std::ostream& os) const{
	return os << "[ " << vec[0] << " " << vec[1] << " " << vec[2] << " ]";
}


std::ostream& operator<<(std::ostream& os, const Vec3& v){
	return v.write(os);
}

std::istream& Vec3::read(std::istream& is){
	std::string line;
	getline(is, line);
	for(unsigned int i=0;i<line.length();i++)
		if(line[i] == '[' || line[i] == ']')
			line[i] = ' ';
	std::istringstream iss(line);
	iss >> vec[0] >> vec[1] >> vec[2];
	return is;
}

std::istream& operator>>(std::istream& is, Vec3& v){
	return v.read(is);
}


const Vec3 Vec3::operator=(const Vec3& other){
	vec[0] = other[0];
	vec[1] = other[1];
	vec[2] = other[2];
	return *this;
}

bool Vec3::operator==(const Vec3& other)const{
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(vec[0]-other[0]) < EPSILON) &&
		(std::abs(vec[1]-other[1]) < EPSILON) &&
		(std::abs(vec[2]-other[2]) < EPSILON));

}

bool Vec3::operator!=(const Vec3& other) const{
	return !(*this==other);
}
const Vec3 Vec3::operator+(const Vec3& right) const{
	return Vec3(vec[0]+right[0], vec[1]+right[1], vec[2]+right[2]);
}

const Vec3 Vec3::operator-(const Vec3& right) const{
	return Vec3(vec[0]-right[0], vec[1]-right[1], vec[2]-right[2]);
}

const Vec3 operator*(const Vec3& v, float scalar){
	return Vec3(v[0]*scalar, v[1]*scalar, v[2]*scalar);
}

const Vec3 operator*(float scalar, const Vec3& v){
	return Vec3(v[0]*scalar, v[1]*scalar, v[2]*scalar);
}


float CGJM::dot(const Vec3& v1, const Vec3& v2){
	return v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2];
}

const Vec3 CGJM::cross(const Vec3& v1, const Vec3& v2){
	return Vec3(v1[1]*v2[2]-v1[2]*v2[1], 
			v1[2]*v2[0]-v1[0]*v2[2], 
			v1[0]*v2[1]-v1[1]*v2[0]);
}

const Vec3 Vec3::rotate(const Vec3& a, const float& t)const{
    return (*this)*std::cos(t)+
            CGJM::cross(a,(*this))*std::sin(t)+
            a*CGJM::dot(a, (*this))*(1-std::cos(t));
}
