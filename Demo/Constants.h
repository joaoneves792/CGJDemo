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
#define GH3D_SHADER "gh3d"
#define ASPHALT_SHADER "asphalt"
#define SKY_SHADER "sky"
#define QUAD_SHADER "quadShader"
#define FIRE_SHADER "fire"
#define HEAT_SHADER "heat"
#define HEAT_SPOT_REFLECTION_SHADER "heatSpotShader"
#define HEAT_DISTANCE_SHADER "heatDistance"
#define SSAO_SHADER "ssao"
#define SSAO_BLUR_SHADER "ssaoBlur"
#define LIGHTING_SHADER "lightingshader"
#define BLEND_SHADER "blend"

/*Scenes*/
#define SCENE "main"
#define PIPELINE "pipeline"
#define FINAL "final"
#define CREDITS_HUD "credits_hud"
#define CONTROLS_HUD "controls_hud"

/*Lights*/
#define SUN "sun"
#define LAMP_POST "lamp_post"
#define HEADLIGHT_R "headlight_r"
#define HEADLIGHT_L "headlight_l"

/*Cameras*/
#define FREE_CAM "freeCamera"
#define SPHERE_CAM "sphereCamera"
#define ORTHO_CAM "ortho"
#define BOTTOM_RIGHT_CAM "brCamera"
#define TOP_RIGHT_CAM "trCamera"

/*FBOs*/
#define MAIN_FBO "main"
#define SCENE_COLOR_FBO "helper"
#define REFLECTION_FBO "reflection"
#define DEPTH_FBO "depth"
#define NORMALZ_FBO "normalz"
#define SSAO_FBO "ssaofbo"
#define FINAL_FBO "final"
#define SIDE_FBO1 "sidefbo1"
#define SIDE_FBO2 "sidefbo2"

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
#define EXHAUST_REFLECTION "reflection"
#define DISTANCE_HEAT "distanceheat"
#define FRONT_HEAT "frontheat"
#define REAR_HEAT "rearheat"


/*Pipeline Stages*/
#define SSAO_LEVEL (-4)
#define SSAO "ssao"
#define SSAO_BLUR_LEVEL (-3)
#define SSAO_BLUR "ssaoBlur"
#define LIGHTS_LEVEL (-2)
#define LIGHTING "lighting"
#define BLEND_LEVEL (-1)
#define BLEND "blend"

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
