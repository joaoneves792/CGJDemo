#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"
#include "mat.h"

using namespace CGJM;

Mat3::Mat3(){
	mat[0][0]=0;
	mat[0][1]=0;
	mat[0][2]=0;

	mat[1][0]=0;
	mat[1][1]=0;
	mat[1][2]=0;

	mat[2][0]=0;
	mat[2][1]=0;
	mat[2][2]=0;
}

Mat3::Mat3(const float& d){
	mat[0][0]=d;
	mat[0][1]=0;
	mat[0][2]=0;

	mat[1][0]=0;
	mat[1][1]=d;
	mat[1][2]=0;

	mat[2][0]=0;
	mat[2][1]=0;
	mat[2][2]=d;
}

Mat3::Mat3(const float& a11, const float& a12, const float& a13,
			const float& a21, const float& a22, const float& a23,
			const float& a31, const float& a32, const float& a33){
	
	mat[0][0]=a11;
	mat[0][1]=a12;
	mat[0][2]=a13;

	mat[1][0]=a21;
	mat[1][1]=a22;
	mat[1][2]=a23;

	mat[2][0]=a31;
	mat[2][1]=a32;
	mat[2][2]=a33;
}


Mat3::Mat3(const Mat3& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];
	mat[0][2]=other[0][2];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
	mat[1][2]=other[1][2];

	mat[2][0]=other[2][0];
	mat[2][1]=other[2][1];
	mat[2][2]=other[2][2];
}

Mat3::Mat3(const Mat4& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];
	mat[0][2]=other[0][2];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
	mat[1][2]=other[1][2];

	mat[2][0]=other[2][0];
	mat[2][1]=other[2][1];
	mat[2][2]=other[2][2];
}


const float* Mat3::operator[](int i) const{
	assert(i<3);
	return (mat)[i];
}

std::ostream& Mat3::write(std::ostream& os) const{
	return os << "| " << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " |" << std::endl
		  << "| " << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " |" << std::endl
		  << "| " << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " |" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const Mat3& v){
	return v.write(os);
}

const Mat3 Mat3::operator=(const Mat3& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];
	mat[0][2]=other[0][2];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
	mat[1][2]=other[1][2];

	mat[2][0]=other[2][0];
	mat[2][1]=other[2][1];
	mat[2][2]=other[2][2];
	return *this;
}

bool Mat3::operator==(const Mat3& other)const{
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(mat[0][0]-other[0][0]) < EPSILON) &&
		(std::abs(mat[0][1]-other[0][1]) < EPSILON) &&
		(std::abs(mat[0][2]-other[0][2]) < EPSILON) &&
		(std::abs(mat[1][0]-other[1][0]) < EPSILON) &&
		(std::abs(mat[1][1]-other[1][1]) < EPSILON) &&
		(std::abs(mat[1][2]-other[1][2]) < EPSILON) && 
		(std::abs(mat[2][0]-other[2][0]) < EPSILON) &&
		(std::abs(mat[2][1]-other[2][1]) < EPSILON) &&
		(std::abs(mat[2][2]-other[2][2]) < EPSILON) );


}

bool Mat3::operator!=(const Mat3& other) const{
	return !(*this==other);
}


const Mat3 Mat3::operator+(const Mat3& right) const{
	return Mat3(mat[0][0]+right[0][0], mat[0][1]+right[0][1], mat[0][2]+right[0][2],
		    mat[1][0]+right[1][0], mat[1][1]+right[1][1], mat[1][2]+right[1][2],
		    mat[2][0]+right[2][0], mat[2][1]+right[2][1], mat[2][2]+right[2][2]);
}

const Mat3 Mat3::operator-(const Mat3& right) const{
	return Mat3(mat[0][0]-right[0][0], mat[0][1]-right[0][1], mat[0][2]-right[0][2],
		    mat[1][0]-right[1][0], mat[1][1]-right[1][1], mat[1][2]-right[1][2],
		    mat[2][0]-right[2][0], mat[2][1]-right[2][1], mat[2][2]-right[2][2]);
}

const Mat3 operator*(const Mat3& A, float scalar){
	return Mat3(A[0][0]*scalar, A[0][1]*scalar, A[0][2]*scalar,
                    A[1][0]*scalar, A[1][1]*scalar, A[1][2]*scalar,
                    A[2][0]*scalar, A[2][1]*scalar, A[2][2]*scalar);
}

const Mat3 operator*(float scalar, const Mat3& A){
	return Mat3(A[0][0]*scalar, A[0][1]*scalar, A[0][2]*scalar,
                    A[1][0]*scalar, A[1][1]*scalar, A[1][2]*scalar,
                    A[2][0]*scalar, A[2][1]*scalar, A[2][2]*scalar);
}

const Vec3 Mat3::operator*(const Vec3& v) const{
	return Vec3(mat[0][0]*v[0]+mat[0][1]*v[1]+mat[0][2]*v[2],
                    mat[1][0]*v[0]+mat[1][1]*v[1]+mat[1][2]*v[2],
                    mat[2][0]*v[0]+mat[2][1]*v[1]+mat[2][2]*v[2]);
}

const Mat3 Mat3::operator*(const Mat3& right) const{
	return Mat3(mat[0][0]*right[0][0]+mat[0][1]*right[1][0]+mat[0][2]*right[2][0],
	            mat[0][0]*right[0][1]+mat[0][1]*right[1][1]+mat[0][2]*right[2][1],
	            mat[0][0]*right[0][2]+mat[0][1]*right[1][2]+mat[0][2]*right[2][2],
	            
		    mat[1][0]*right[0][0]+mat[1][1]*right[1][0]+mat[1][2]*right[2][0],
	            mat[1][0]*right[0][1]+mat[1][1]*right[1][1]+mat[1][2]*right[2][1],
	            mat[1][0]*right[0][2]+mat[1][1]*right[1][2]+mat[1][2]*right[2][2],
		    
		    mat[2][0]*right[0][0]+mat[2][1]*right[1][0]+mat[2][2]*right[2][0],
	            mat[2][0]*right[0][1]+mat[2][1]*right[1][1]+mat[2][2]*right[2][1],
	            mat[2][0]*right[0][2]+mat[2][1]*right[1][2]+mat[2][2]*right[2][2]);
}

const Mat3 Mat3::transpose() const{
	return Mat3(mat[0][0], mat[1][0], mat[2][0],
		    mat[0][1], mat[1][1], mat[2][1],
		    mat[0][2], mat[1][2], mat[2][2] );
}

float Mat3::determinant() const{
	return (mat[0][0]*mat[1][1]*mat[2][2])+
	       (mat[0][1]*mat[1][2]*mat[2][0])+
	       (mat[0][2]*mat[1][0]*mat[2][1])-

	       (mat[0][2]*mat[1][1]*mat[2][0])-
	       (mat[0][0]*mat[1][2]*mat[2][1])-
	       (mat[0][1]*mat[1][0]*mat[2][2]);
}

const Mat3 Mat3::inverse() const{
	float detInverse = 1/determinant();
	return Mat3(
			(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2])*detInverse,
		       -(mat[0][1]*mat[2][2]-mat[2][1]*mat[0][2])*detInverse,
                        (mat[0][1]*mat[1][2]-mat[1][1]*mat[0][2])*detInverse,
		       -(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2])*detInverse,
		        (mat[0][0]*mat[2][2]-mat[2][0]*mat[0][2])*detInverse,
		       -(mat[0][0]*mat[1][2]-mat[1][0]*mat[0][2])*detInverse,
		        (mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1])*detInverse,
		       -(mat[0][0]*mat[2][1]-mat[2][0]*mat[0][1])*detInverse,
		        (mat[0][0]*mat[1][1]-mat[1][0]*mat[0][1])*detInverse
			);
}

const Mat3 CGJM::dualMatrix(const Vec3& v){
		return	CGJM::Mat3(0, -v[2], v[1], 
				   v[2], 0, -v[0],
				  -v[1], v[0], 0);
}

Mat3::operator const float*() const{
	return (mat)[0];
}
