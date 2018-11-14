//
// Created by joao on 11/26/17.
//

#ifndef CGJDEMO_GLM_WRAPPER_H
#define CGJDEMO_GLM_WRAPPER_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/detail/type_vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
typedef glm::mat4 Mat4;
typedef glm::mat3 Mat3;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;
typedef glm::quat Quat;

/**
   Decompose the rotation on to 2 parts.
   1. Twist - rotation around the "direction" vector
   2. Swing - rotation around axis that is perpendicular to "direction" vector
   The rotation can be composed back by
   rotation = swing * twist

   has singularity in case of swing_rotation close to 180 degrees rotation.
   if the input quaternion is of non-unit length, the outputs are non-unit as well
   otherwise, outputs are both unit
*/
inline void swing_twist_decomposition( const Quat& rotation,
                                       const Vec3&      direction,
                                       Quat&       swing,
                                       Quat&       twist){
    Quat normalizedRotation = glm::normalize(rotation);
    Vec3 normalizedDirection = glm::normalize(direction);
    Vec3 ra( normalizedRotation.x, normalizedRotation.y, normalizedRotation.z ); // rotation axis
    Vec3 p = dot( normalizedDirection, ra)*normalizedDirection; // return projection v1 on to v2  (parallel component)
    twist = Quat( rotation.w ,p.x, p.y, p.z );
    glm::normalize(twist);
    //swing = rotation * glm::conjugate(twist);
    swing = rotation * glm::conjugate(twist);
}


#endif //CGJDEMO_GLM_WRAPPER_H
