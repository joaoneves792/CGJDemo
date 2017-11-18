#include "quat.h"
#include "vec.h"
#include "mat.h"

#include <cmath>

using namespace CGJM;

#define interpolate(v1, v2, t)  (1 - t) * v1 + t * v2

const float Quat::qThreshold = (float)1.0e-5;

Quat::Quat(){
    t=0;
    x=0;
    y=0;
    z=0;
}

Quat::Quat(const float& angle, const Vec3& axis){
    Vec3 nAxis = axis.normalize();
    t = std::cos(angle / 2.0f);
    float s = std::sin(angle / 2.0f);
    x = nAxis[0] * s;
    y = nAxis[1] * s;
    z = nAxis[2] * s;

    clean();
    normalize();
}
Quat::Quat(const Quat &other) {
    t = other.t;
    x = other.x;
    y = other.y;
    z = other.z;
}

Quat::Quat(const float &t, const float &x, const float &y, const float &z) {
    this->t = t;
    this->x = x;
    this->y = y;
    this->z = z;

}

std::ostream& Quat::write(std::ostream &os) const {
    Vec3 axis;
    float angle;
    getAngleAxis(angle, axis);
    return os << "t = " << t << std::endl <<
                 "x = " << x << std::endl <<
                 "y = " << y << std::endl <<
                 "z = " << z << std::endl <<
                 "Angle = " << angle << std::endl <<
                 "Axis = " << axis << std::endl;
}

void Quat::getAngleAxis(float &angle, Vec3 &axis) const {
    Quat nQuat = normalize();
    angle = 2.0f * std::acos(nQuat.t);
    float s = std::sqrt(1.0f - nQuat.t*nQuat.t);
    if(s < qThreshold) {
        axis[0] = 1.0f;
        axis[1] = 0.0f;
        axis[2] = 0.0f;
    } else {
        axis[0] = nQuat.x / s;
        axis[1] = nQuat.y / s;
        axis[2] = nQuat.z / s;
    }
}

void Quat::clean(){
    if(std::fabs(t) < qThreshold) t = 0.0f;
    if(std::fabs(x) < qThreshold) x = 0.0f;
    if(std::fabs(y) < qThreshold) y = 0.0f;
    if(std::fabs(z) < qThreshold) z = 0.0f;
}

const Quat Quat::clean() const {
    Quat other((*this));
    other.clean();
    return other;
}

float Quat::quadrance() const {
    return t*t + x*x + y*y + z*z;
}

float Quat::norm() const {
    return std::sqrt(quadrance());
}

const Quat Quat::normalize()const{
    float s = 1 / norm();
    return (*this)*s;
}

void Quat::normalize() {
    float s = 1 / norm();
    t = t*s;
    x = x*s;
    y = y*s;
    z = z*s;
}

const Quat Quat::conjugate() const {
    return Quat(t, -x, -y, -z);
}

const Quat Quat::inverse() const {
    return conjugate()*(1.0f/quadrance());
}

const Mat4 Quat::GLMatrix() const {
    Quat qn = normalize();
    float xx = qn.x * qn.x;
    float xy = qn.x * qn.y;
    float xz = qn.x * qn.z;
    float xt = qn.x * qn.t;
    float yy = qn.y * qn.y;
    float yz = qn.y * qn.z;
    float yt = qn.y * qn.t;
    float zz = qn.z * qn.z;
    float zt = qn.z * qn.t;

    return Mat4(1.0f - 2.0f * (yy + zz), 2.0f * (xy + zt), 2.0f * (xz - yt), 0.0f,
                2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + xt), 0.0f,
                2.0f * (xz + yt), 2.0f * (yz - xt), 1.0f - 2.0f * (xx+yy), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f);
}

const Quat Quat::operator=(const Quat &other) {
    t = other.t;
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

bool Quat::operator==(const Quat &other) const {
    return ( std::fabs(t-other.t) < qThreshold && std::fabs(x-other.x) < qThreshold &&
             std::fabs(y-other.y) < qThreshold && std::fabs(z-other.z) < qThreshold );
}

bool Quat::operator!=(const Quat &other) const {
    return !((*this)==other);
}

const Quat Quat::operator+(const Quat &right) const {
    return Quat(t+right.t, x+right.x, y+right.y, z+right.z);
}
const Quat Quat::operator-(const Quat &right) const {
    return Quat(t-right.t, x-right.x, y-right.y, z-right.z);
}

const Quat Quat::operator*(const Quat &right) const {
    return Quat(t*right.t - x*right.x - y*right.y - z*right.z,
                t*right.x + x*right.t + y*right.z - z*right.y,
                t*right.y + y*right.t + z*right.x - x*right.z,
                t*right.z + z*right.t + x*right.y - y*right.x);
}

const Quat CGJM::lerp(const Quat& q0, const Quat& q1, float k){
    float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
    float k0 = 1.0f - k;
    float k1 = (cos_angle > 0) ? k : -k;
    Quat qi = ((q0*k0) + (q1*k1)).normalize();
    return qi;
}
const Quat CGJM::slerp(const Quat& q0, const Quat& q1, float k){
    Quat z = q1;
    float cosTheta = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;

    // If cosTheta < 0, the interpolation will take the long way around the sphere.
    // To fix this, one quat must be negated.
    if(cosTheta < 0){
        z = Quat(-q1.t, -q1.x, -q1.y, -q1.z);
        cosTheta = -cosTheta;
    }
    // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a zero denominator
    if(cosTheta > (1-Quat::qThreshold)){
        return Quat(
                interpolate(q0.t, q1.t, k),
                interpolate(q0.x, q1.x, k),
                interpolate(q0.y, q1.y, k),
                interpolate(q0.z, q1.z, k)
        );
    }else{
        float angle = std::acos(cosTheta);
        return (std::sin((1-k) *angle) * q0 + std::sin(k*angle)*z* (1/std::sin(angle)));
    }

    /*float angle = std::acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
    float k0 = std::sin((1-k)*angle) / std::sin(angle);
    float k1 = std::sin(k*angle) / std::sin(angle);
    Quat qi = ((q0*k0) + (q1*k1)).normalize();
    return qi;*/
}

std::ostream& operator<<(std::ostream &os, const Quat& v){
    return v.write(os);
}

const Quat operator*(float scalar, const Quat& A){
    return Quat(scalar*A.t, scalar*A.x, scalar*A.y, scalar*A.z);
}
const Quat operator*(const Quat& A, float scalar) {
    return Quat(scalar * A.t, scalar * A.x, scalar * A.y, scalar * A.z);
}
