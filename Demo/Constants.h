//
// Created by joao on 11/21/17.
//

#ifndef CGJDEMO_CONSTANTS_H
#define CGJDEMO_CONSTANTS_H


/*Meshes*/
#define CAR "car"
#define WHEEL "wheel"
#define ROAD "road"
#define ASPHALT "asphalt"
#define SKY "sky"

/*Textures*/
#define SKY_CUBE_MAP "skycubemap"
#define ENVIRONMENT "environment"
#define HAZE_ENVIRONMENT "hazeenvironment"
#define CREDITS "credits"
#define NOISE "noise"

/*Shaders*/
#define H3D_SHADER "h3d"
#define ASPHALT_SHADER "asphalt"
#define SKY_SHADER "sky"
#define FINAL_SHADER "twoDShader"
#define QUAD_SHADER "quadShader"
#define SMOKE_SHADER "smoke"
#define FIRE_SHADER "fire"
#define HEAT_SHADER "heat"
#define HEAT_SPOT_REFLECTION_SHADER "heatSpotShader"
#define HEAT_DISTANCE_SHADER "heatDistance"

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
#define REFLECTION_FBO "reflection"

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
#define REFLECTION_SPOT "reflectionSpot"
#define DISTANCE_HEAT "distanceheat"
#define FRONT_HEAT "frontheat"
#define REAR_HEAT "rearheat"

/*Misc constants*/
#define PI 3.14159f
#define ROAD_SEGMENTS 40
#define ACTIVE_LAMPS 12
#define ROAD_LENGTH 60.0f
#define HEAT_HAZE_LEVEL 10
#define REFLECTIONS_LEVEL 9
#define GROUND_LEVEL 0.0f
#define ROAD_LEVEL 2
#define REFLECTION_RESOLUTION 512
#define HEAT_HAZE_DISTANCE 300.0f





#endif //CGJDEMO_RESOURCENAMES_H
