#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>
#include "vec.h"
#include "mat.h"

using namespace CGJM;

Mat4::Mat4(){
	mat[0][0]=0;
	mat[0][1]=0;
	mat[0][2]=0;
	mat[0][3]=0;

	mat[1][0]=0;
	mat[1][1]=0;
	mat[1][2]=0;
	mat[1][3]=0;

	mat[2][0]=0;
	mat[2][1]=0;
	mat[2][2]=0;
	mat[2][3]=0;
	
	mat[3][0]=0;
	mat[3][1]=0;
	mat[3][2]=0;
	mat[3][3]=0;
}

Mat4::Mat4(const float& d){
	mat[0][0]=d;
	mat[0][1]=0;
	mat[0][2]=0;
	mat[0][3]=0;

	mat[1][0]=0;
	mat[1][1]=d;
	mat[1][2]=0;
	mat[1][3]=0;

	mat[2][0]=0;
	mat[2][1]=0;
	mat[2][2]=d;
	mat[2][3]=0;
	
	mat[3][0]=0;
	mat[3][1]=0;
	mat[3][2]=0;
	mat[3][3]=d;
}

Mat4::Mat4(const float& a11, const float& a12, const float& a13, const float& a14,
			const float& a21, const float& a22, const float& a23, const float& a24,
			const float& a31, const float& a32, const float& a33, const float& a34,
			const float& a41, const float& a42, const float& a43, const float& a44 ){
	mat[0][0]=a11;
	mat[0][1]=a12;
	mat[0][2]=a13;
	mat[0][3]=a14;

	mat[1][0]=a21;
	mat[1][1]=a22;
	mat[1][2]=a23;
	mat[1][3]=a24;

	mat[2][0]=a31;
	mat[2][1]=a32;
	mat[2][2]=a33;
	mat[2][3]=a34;
	
	mat[3][0]=a41;
	mat[3][1]=a42;
	mat[3][2]=a43;
	mat[3][3]=a44;
}


Mat4::Mat4(const Mat4& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];
	mat[0][2]=other[0][2];
	mat[0][3]=other[0][3];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
	mat[1][2]=other[1][2];
	mat[1][3]=other[1][3];

	mat[2][0]=other[2][0];
	mat[2][1]=other[2][1];
	mat[2][2]=other[2][2];
	mat[2][3]=other[2][3];
	
	mat[3][0]=other[3][0];
	mat[3][1]=other[3][1];
	mat[3][2]=other[3][2];
	mat[3][3]=other[3][3];
}


const float* Mat4::operator[](int i) const{
	assert(i<4);
	return (mat)[i];
}

Mat4::operator const float*() const{
	return (mat)[0];
}
std::ostream& Mat4::write(std::ostream& os) const{
	return os << "| " << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << " " << mat[0][3] << " |" << std::endl
		  << "| " << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << " " << mat[1][3] << " |" << std::endl
		  << "| " << mat[2][0] << " " << mat[2][1] << " " << mat[2][2] << " " << mat[2][3] << " |" << std::endl
		  << "| " << mat[3][0] << " " << mat[3][1] << " " << mat[3][2] << " " << mat[3][3] << " |" << std::endl;
}


std::ostream& operator<<(std::ostream& os, const Mat4& v){
	return v.write(os);
}

const Mat4 Mat4::operator=(const Mat4& other){
	mat[0][0]=other[0][0];
	mat[0][1]=other[0][1];
	mat[0][2]=other[0][2];
	mat[0][3]=other[0][3];

	mat[1][0]=other[1][0];
	mat[1][1]=other[1][1];
	mat[1][2]=other[1][2];
	mat[1][3]=other[1][3];

	mat[2][0]=other[2][0];
	mat[2][1]=other[2][1];
	mat[2][2]=other[2][2];
	mat[2][3]=other[2][3];
	
	mat[3][0]=other[3][0];
	mat[3][1]=other[3][1];
	mat[3][2]=other[3][2];
	mat[3][3]=other[3][3];
	return *this;
}

bool Mat4::operator==(const Mat4& other)const{
#ifndef EPSILON
#define EPSILON 0.001
#endif
	return ((std::abs(mat[0][0]-other[0][0]) < EPSILON) &&
		(std::abs(mat[0][1]-other[0][1]) < EPSILON) &&
		(std::abs(mat[0][2]-other[0][2]) < EPSILON) &&
		(std::abs(mat[0][3]-other[0][3]) < EPSILON) &&
		(std::abs(mat[1][0]-other[1][0]) < EPSILON) &&
		(std::abs(mat[1][1]-other[1][1]) < EPSILON) &&
		(std::abs(mat[1][2]-other[1][2]) < EPSILON) && 
		(std::abs(mat[1][3]-other[1][3]) < EPSILON) && 
		(std::abs(mat[2][0]-other[2][0]) < EPSILON) &&
		(std::abs(mat[2][1]-other[2][1]) < EPSILON) &&
		(std::abs(mat[2][2]-other[2][2]) < EPSILON) && 
		(std::abs(mat[2][3]-other[2][3]) < EPSILON) && 
		(std::abs(mat[3][0]-other[3][0]) < EPSILON) &&
		(std::abs(mat[3][1]-other[3][1]) < EPSILON) &&
		(std::abs(mat[3][2]-other[3][2]) < EPSILON) &&
		(std::abs(mat[3][3]-other[3][3]) < EPSILON) );


}

bool Mat4::operator!=(const Mat4& other) const{
	return !(*this==other);
}


const Mat4 Mat4::operator+(const Mat4& right) const{
	return Mat4(mat[0][0]+right[0][0], mat[0][1]+right[0][1], mat[0][2]+right[0][2], mat[0][3]+right[0][3],
		    mat[1][0]+right[1][0], mat[1][1]+right[1][1], mat[1][2]+right[1][2], mat[1][3]+right[1][3],
		    mat[2][0]+right[2][0], mat[2][1]+right[2][1], mat[2][2]+right[2][2], mat[2][3]+right[2][3],
		    mat[3][0]+right[3][0], mat[3][1]+right[3][1], mat[3][2]+right[3][2], mat[3][3]+right[3][3]);
}

const Mat4 Mat4::operator-(const Mat4& right) const{
	return Mat4(mat[0][0]-right[0][0], mat[0][1]-right[0][1], mat[0][2]-right[0][2], mat[0][3]-right[0][3],
		    mat[1][0]-right[1][0], mat[1][1]-right[1][1], mat[1][2]-right[1][2], mat[1][3]-right[1][3],
		    mat[2][0]-right[2][0], mat[2][1]-right[2][1], mat[2][2]-right[2][2], mat[2][3]-right[2][3],
		    mat[3][0]-right[3][0], mat[3][1]-right[3][1], mat[3][2]-right[3][2], mat[3][3]-right[3][3]);
}

const Mat4 operator*(const Mat4& A, float scalar){
	return Mat4(A[0][0]*scalar, A[0][1]*scalar, A[0][2]*scalar, A[0][3]*scalar,
                    A[1][0]*scalar, A[1][1]*scalar, A[1][2]*scalar, A[1][3]*scalar,
                    A[2][0]*scalar, A[2][1]*scalar, A[2][2]*scalar, A[2][3]*scalar,
                    A[3][0]*scalar, A[3][1]*scalar, A[3][2]*scalar, A[3][3]*scalar);
}

const Mat4 operator*(float scalar, const Mat4& A){
	return Mat4(A[0][0]*scalar, A[0][1]*scalar, A[0][2]*scalar, A[0][3]*scalar,
                    A[1][0]*scalar, A[1][1]*scalar, A[1][2]*scalar, A[1][3]*scalar,
                    A[2][0]*scalar, A[2][1]*scalar, A[2][2]*scalar, A[2][3]*scalar,
                    A[3][0]*scalar, A[3][1]*scalar, A[3][2]*scalar, A[3][3]*scalar);
}

const Vec3 Mat4::operator*(const Vec3& v) const{
	return Vec3(mat[0][0]*v[0]+mat[0][1]*v[1]+mat[0][2]*v[2]+mat[0][3]*1,
                    mat[1][0]*v[0]+mat[1][1]*v[1]+mat[1][2]*v[2]+mat[1][3]*1,
                    mat[2][0]*v[0]+mat[2][1]*v[1]+mat[2][2]*v[2]+mat[2][3]*1);
}

//TODO -->>

const Mat4 Mat4::operator*(const Mat4& right) const{
	return Mat4(mat[0][0]*right[0][0]+mat[0][1]*right[1][0]+mat[0][2]*right[2][0]+mat[0][3]*right[3][0],
	            mat[0][0]*right[0][1]+mat[0][1]*right[1][1]+mat[0][2]*right[2][1]+mat[0][3]*right[3][1],
	            mat[0][0]*right[0][2]+mat[0][1]*right[1][2]+mat[0][2]*right[2][2]+mat[0][3]*right[3][2],
	            mat[0][0]*right[0][3]+mat[0][1]*right[1][3]+mat[0][2]*right[2][3]+mat[0][3]*right[3][3],
	            
		    mat[1][0]*right[0][0]+mat[1][1]*right[1][0]+mat[1][2]*right[2][0]+mat[1][3]*right[3][0],
	            mat[1][0]*right[0][1]+mat[1][1]*right[1][1]+mat[1][2]*right[2][1]+mat[1][3]*right[3][1],
	            mat[1][0]*right[0][2]+mat[1][1]*right[1][2]+mat[1][2]*right[2][2]+mat[1][3]*right[3][2],
	            mat[1][0]*right[0][3]+mat[1][1]*right[1][3]+mat[1][2]*right[2][3]+mat[1][3]*right[3][3],
		    
		    mat[2][0]*right[0][0]+mat[2][1]*right[1][0]+mat[2][2]*right[2][0]+mat[2][3]*right[3][0],
	            mat[2][0]*right[0][1]+mat[2][1]*right[1][1]+mat[2][2]*right[2][1]+mat[2][3]*right[3][1],
	            mat[2][0]*right[0][2]+mat[2][1]*right[1][2]+mat[2][2]*right[2][2]+mat[2][3]*right[3][2],
	            mat[2][0]*right[0][3]+mat[2][1]*right[1][3]+mat[2][2]*right[2][3]+mat[2][3]*right[3][3],
		    
		    mat[3][0]*right[0][0]+mat[3][1]*right[1][0]+mat[3][2]*right[2][0]+mat[3][3]*right[3][0],
	            mat[3][0]*right[0][1]+mat[3][1]*right[1][1]+mat[3][2]*right[2][1]+mat[3][3]*right[3][1],
	            mat[3][0]*right[0][2]+mat[3][1]*right[1][2]+mat[3][2]*right[2][2]+mat[3][3]*right[3][2],
	            mat[3][0]*right[0][3]+mat[3][1]*right[1][3]+mat[3][2]*right[2][3]+mat[3][3]*right[3][3]);
}

const Mat4 Mat4::transpose() const{
	return Mat4(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
		    mat[0][1], mat[1][1], mat[2][1], mat[3][1],
		    mat[0][2], mat[1][2], mat[2][2], mat[3][2],
		    mat[0][3], mat[1][3], mat[2][3], mat[3][3] );
}

float Mat4::determinant() const{
	/*var det = a11 * (a22*(a33*a44 - a43*a34) - a23*(a32*a44 - a42*a34) + a24*(a32*a43 - a42*a33)) 
                  - a12 * (a21*(a33*a44 - a43*a34) - a23*(a31*a44 - a41*a34) + a24*(a31*a43 - a41*a33)) 
		  + a13 * (a21*(a32*a44 - a42*a34) - a22*(a31*a44 - a41*a34) + a24 *(a31*a42 - a41*a32))
		  - a14 * (a21*(a32*a43 - a42*a33) - a22*(a31*a43 - a41*a33) + a23 *(a31*a42 - a41*a32));*/

	return mat[0][0] * (mat[1][1]*(mat[2][2]*mat[3][3] - mat[3][2]*mat[2][3]) 
			  - mat[1][2]*(mat[2][1]*mat[3][3] - mat[3][1]*mat[2][3])
			  + mat[1][3]*(mat[2][1]*mat[3][2] - mat[3][1]*mat[2][2]))
	     - mat[0][1] * (mat[1][0]*(mat[2][2]*mat[3][3] - mat[3][2]*mat[2][3])
			  - mat[1][2]*(mat[2][0]*mat[3][3] - mat[3][0]*mat[2][3])
			  + mat[1][3]*(mat[2][0]*mat[3][2] - mat[3][0]*mat[2][2]))
	     + mat[0][2] * (mat[1][0]*(mat[2][1]*mat[3][3] - mat[3][1]*mat[2][3])
			  - mat[1][1]*(mat[2][0]*mat[3][3] - mat[3][0]*mat[2][3])
			  + mat[1][3]*(mat[2][0]*mat[3][1] - mat[3][0]*mat[2][1]))
	     - mat[0][3] * (mat[1][0]*(mat[2][1]*mat[3][2] - mat[3][1]*mat[2][2])
			  - mat[1][1]*(mat[2][0]*mat[3][2] - mat[3][0]*mat[2][2])
			  + mat[1][2]*(mat[2][0]*mat[3][1] - mat[3][0]*mat[2][1]));
		
}

const Mat4 Mat4::inverse() const{
	float detInverse = 1/determinant();
	return Mat4( (Mat3(mat[1][1], mat[2][1], mat[3][1],
		           mat[1][2], mat[2][2], mat[3][2],
			   mat[1][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		    -(Mat3(mat[0][1], mat[2][1], mat[3][1],
		           mat[0][2], mat[2][2], mat[3][2],
			   mat[0][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		     (Mat3(mat[0][1], mat[1][1], mat[3][1],
		           mat[0][2], mat[1][2], mat[3][2],
			   mat[0][3], mat[1][3], mat[3][3]).determinant()*detInverse),
		    -(Mat3(mat[0][1], mat[1][1], mat[2][1],
		           mat[0][2], mat[1][2], mat[2][2],
			   mat[0][3], mat[1][3], mat[2][3]).determinant()*detInverse),

		    -(Mat3(mat[1][0], mat[2][0], mat[3][0],
		           mat[1][2], mat[2][2], mat[3][2],
			   mat[1][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		     (Mat3(mat[0][0], mat[2][0], mat[3][0],
		           mat[0][2], mat[2][2], mat[3][2],
			   mat[0][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		    -(Mat3(mat[0][0], mat[1][0], mat[3][0],
		           mat[0][2], mat[1][2], mat[3][2],
			   mat[0][3], mat[1][3], mat[3][3]).determinant()*detInverse),
		     (Mat3(mat[0][0], mat[1][0], mat[2][0],
		           mat[0][2], mat[1][2], mat[2][2],
			   mat[0][3], mat[1][3], mat[2][3]).determinant()*detInverse),

		     (Mat3(mat[1][0], mat[2][0], mat[3][0],
		           mat[1][1], mat[2][1], mat[3][1],
			   mat[1][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		    -(Mat3(mat[0][0], mat[2][0], mat[3][0],
		           mat[0][1], mat[2][1], mat[3][1],
			   mat[0][3], mat[2][3], mat[3][3]).determinant()*detInverse),
		     (Mat3(mat[0][0], mat[1][0], mat[3][0],
		           mat[0][1], mat[1][1], mat[3][1],
			   mat[0][3], mat[1][3], mat[3][3]).determinant()*detInverse),
		    -(Mat3(mat[0][0], mat[1][0], mat[2][0],
		           mat[0][1], mat[1][1], mat[2][1],
			   mat[0][3], mat[1][3], mat[2][3]).determinant()*detInverse),
		    
		    -(Mat3(mat[1][0], mat[2][0], mat[3][0],
		           mat[1][1], mat[2][1], mat[3][1],
			   mat[1][2], mat[2][2], mat[3][2]).determinant()*detInverse),
		     (Mat3(mat[0][0], mat[2][0], mat[3][0],
		           mat[0][1], mat[2][1], mat[3][1],
			   mat[0][2], mat[2][2], mat[3][2]).determinant()*detInverse),
		    -(Mat3(mat[0][0], mat[1][0], mat[3][0],
		           mat[0][1], mat[1][1], mat[3][1],
			   mat[0][2], mat[1][2], mat[3][2]).determinant()*detInverse),
		     (Mat3(mat[0][0], mat[1][0], mat[2][0],
		           mat[0][1], mat[1][1], mat[2][1],
			   mat[0][2], mat[1][2], mat[2][2]).determinant()*detInverse));
}
