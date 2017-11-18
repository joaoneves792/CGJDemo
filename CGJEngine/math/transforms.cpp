#include <cmath>
#include "vec.h"
#include "mat.h"

using namespace CGJM;

const Mat4 CGJM::scale(const float& x, const float& y, const float& z){
	return Mat4(x, 0, 0, 0,
		    0, y, 0, 0,
		    0, 0, z, 0,
		    0, 0, 0, 1);
}

const Mat4 CGJM::rotate(const Vec3& axis, const float& angle){
#define __COS std::cos(angle)
#define __1_COS (1-std::cos(angle))
#define __SIN std::sin(angle)
#define __l axis[0]
#define __m axis[1]
#define __n axis[2]
	return Mat4( __l*__l*__1_COS+__COS, __m*__l*__1_COS-__n*__SIN, __n*__l*__1_COS+__m*__SIN, 0,
		     __l*__m*__1_COS+__n*__SIN, __m*__m*__1_COS+__COS, __n*__m*__1_COS-__l*__SIN, 0,
		     __l*__n*__1_COS-__m*__SIN, __m*__n*__1_COS+__l*__SIN, __n*__n*__1_COS+__COS, 0,
		     0, 0, 0, 1);
}

const Mat4 CGJM::translate(const float& x, const float& y, const float& z){
	return Mat4(1, 0, 0, x,
		    0, 1, 0, y,
		    0, 0, 1, z,
		    0, 0, 0, 1);
}

const Mat4 CGJM::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up){
	up.normalize();
	Vec3 v = (center-eye).normalize();
	Vec3 s = cross(v, up);
	s = s.normalize();
	Vec3 u = cross(s, v);
	return Mat4(s[0],  s[1],  s[2], -dot(s, eye),
		    u[0],  u[1],  u[2], -dot(u, eye),
		   -v[0], -v[1], -v[2],  dot(v, eye),
		       0,     0,     0,           1);
}
const Mat4 CGJM::ortho(const float& left, const float& right, const float& top, const float& bottom, const float& near, const float& far) {
	return Mat4(2/(right-left),              0,            0,  (left+right)/(left-right),
		                      0,2/(top-bottom),            0,  (bottom+top)/(bottom-top),
					          0,             0, 2/(far-near),      (near+far)/(far-near),
		                      0,             0,            0,                         1);
}


const Mat4 CGJM::perspective(const float& fovy, const float& aspectRatio, const float& near, const float& far) {
	float t = fovy / 2;
	float d = 1 / std::tan(t);
	return Mat4(d / aspectRatio, 0,                           0,                             0,
		                      0, d,                           0,                             0,
		                      0, 0, (near + far) / (near - far), (2 * far*near) / (near - far),
		                      0, 0,                          -1,                            0);
}
