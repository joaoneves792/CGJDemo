#version 330 core

//in float q;
out float depth;

void main(){
	//gl_FragDepth = q;
	//gl_FragDepth = gl_FragCoord.z;
	depth = gl_FragCoord.z;
}
