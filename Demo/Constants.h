//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_CONSTANTS_H
#define CGJDEMO_CONSTANTS_H

#ifndef M_PI
#define M_PI 3.14156
#endif

/*Meshes*/
#define CAR "car"
#define ROAD "road"
#define SKY "sky"

/*Shaders*/
#define H3D_SHADER "h3d"
#define SKY_SHADER "sky"
#define TWO_D_SHADER "twoDShader"
#define QUAD_SHADER "quad"
#define SMOKE_SHADER "smoke"

/*Scenes*/
#define SCENE "main"
#define POST "post"
#define FINAL "final"

/*Lights*/
#define SUN "sun"
#define LAMP_POST "lamp_post"
#define HEADLIGHT_R "headlight_r"
#define HEADLIGHT_L "headlight_l"

/*Cameras*/
#define FREE_CAM "freeCamera"
#define SPHERE_CAM "sphereCamera"
#define ORTHO_CAM "orthoCamera"

/*FBOs*/
#define MAIN_FBO "main"
#define HELPER_FBO "helper"

/*Particles*/
#define MAX_PARTICLES 1024
#define POOL "pool"
#define SMOKE_EMITTER "smoke"

/*Initial window size*/
#define WIN_X 1024
#define WIN_Y 1024

/*Misc constants*/
#define ROAD_SEGMENTS 12
#define HEAT_HAZE_LEVEL 10





#endif //CGJDEMO_RESOURCENAMES_H
