//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_CONSTANTS_H
#define CGJDEMO_CONSTANTS_H


/*Meshes*/
#define CAR "car"
#define WHEEL "wheel"
#define ROAD "road"
#define SKY "sky"

/*Textures*/
#define SKY_CUBE_MAP "skycubemap"
#define ENVIRONMENT "environment"
#define CREDITS "credits"

/*Shaders*/
#define H3D_SHADER "h3d"
#define SKY_SHADER "sky"
#define FINAL_SHADER "twoDShader"
#define QUAD_SHADER "quadShader"
#define HEAT_SHADER "heat"
#define SMOKE_SHADER "smoke"
#define FIRE_SHADER "fire"

/*Scenes*/
#define SCENE "main"
#define POST "post"
#define HUD "hud"

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
#define SMOKE_PARTICLE "res/smoke.png"
#define LEFT_EXHAUST "exhaust_l"
#define RIGHT_EXHAUST "exhaust_r"
#define FIRE_PARTICLE "res/fireAtlas.png"
#define HEAT_EMITTER "heatHaze"

/*Window properties*/
#define WIN_X 1024
#define WIN_Y 988
#define MSAA 4

/*Important Nodes*/
#define REAR_AXIS "rear_axis"
#define FRONT_AXIS "front_axis"
#define RRWHEEL "rr"
#define RLWHEEL "rl"
#define FRWHEEL "fr"
#define FLWHEEL "fl"

/*Misc constants*/
#define PI 3.14159f
#define ROAD_SEGMENTS 12
#define HEAT_HAZE_LEVEL 10





#endif //CGJDEMO_RESOURCENAMES_H
