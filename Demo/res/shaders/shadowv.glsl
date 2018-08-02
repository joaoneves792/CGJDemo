#version 330 core

in vec3 position;
/*in vec3 inJointIndex;
in vec3 inJointWeight;*/

//out float q;

/*uniform mat4 MV;
uniform mat4 Projection;*/
//uniform mat4[72] bones;
uniform mat4 MVP;

void main(){
    /*vec4 newPosition;
    if(inJointIndex.x >= 0){
        newPosition = (bones[int(inJointIndex.x)] * vec4(position, 1.0f)) * inJointWeight.x;
        newPosition += (bones[int(inJointIndex.y)] * vec4(position, 1.0f)) * inJointWeight.y;
        newPosition += (bones[int(inJointIndex.z)] * vec4(position, 1.0f)) * inJointWeight.z;
        newPosition.w = 1.0f;
    }else{
        newPosition = vec4(position, 1.0f);
    }
    vec4 p_c = MV * newPosition; //Position cameraspace
    q = (p_c.x*p_c.x+p_c.y*p_c.y+p_c.z*p_c.z)*0.0005;
	gl_Position =  Projection * p_c;*/
	gl_Position =  MVP * vec4(position, 1.0f);
}
