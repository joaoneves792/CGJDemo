#version 330 core

in vec2 uv;
in vec2 position_modelspace;
in float life;

uniform sampler2D renderedTexture;
uniform float noise_blur_black;

out vec4 color;

#define PI 3.14159f
#define AMPLITUDE 1.0f/800.0f
#define FREQUENCY PI/2.0f
#define PHASE 100.0f

#define X_AMPLITUDE 0.2f
#define Y_AMPLITUDE 0.3f

#define HARMONICS_A 1.2f
#define HARMONICS_B 1.5f
#define HARMONICS_N 6

#define NEXT_TEXEL_STEP 0.0025f

#define NOISE_FREQUENCY 64.0f
#define NOISE_PERIOD  40.0f
#define NOISE_AMPLITUDE 0.7f

float blur_kernel[9] = float[](0.05f, 0.15f, 0.05f,
                               0.15f, 0.15f, 0.15f,
                               0.05f, 0.15f, 0.05f);


// Description : Array and textureless GLSL 3D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 mod289(vec4 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec4 permute(vec4 x) { return mod289(((x*34.0)+1.0)*x); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }
float snoise(vec3 v)
{
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  // First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

  // Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

  // Permutations
  i = mod289(i);
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  // Gradients: 7x7 points over a square, mapped onto an octahedron.
  // The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  //Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  // Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
}


vec3 convolute(float[9] kernel, vec2 pos){
    vec3 t[9] = vec3[](
        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y+NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y+NEXT_TEXEL_STEP)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y)).rgb,

        texture(renderedTexture, vec2(pos.x-NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x, pos.y-NEXT_TEXEL_STEP)).rgb,
        texture(renderedTexture, vec2(pos.x+NEXT_TEXEL_STEP, pos.y-NEXT_TEXEL_STEP)).rgb
    );

    return kernel[8]*t[0] + kernel[7]*t[1] + kernel[6]*t[2] +
           kernel[5]*t[3] + kernel[4]*t[4] + kernel[3]*t[5] +
           kernel[2]*t[6] + kernel[1]*t[7] + kernel[0]*t[8];
}

float harmonic_sin(int i, float a, float f){
         return (AMPLITUDE*sin(pow(HARMONICS_B, i)*FREQUENCY*a + PHASE*f))/pow(HARMONICS_A, i);
}

void main() {
    vec2 pos = position_modelspace;
    vec2 wave_offset = vec2(0.0f, 0.0f);
    for(int i=0; i<HARMONICS_N; i++){
        float hs = harmonic_sin(i, pos.x, life);
        wave_offset.x += X_AMPLITUDE*hs;
        wave_offset.y += Y_AMPLITUDE*hs;
    }

    float noise = snoise(vec3((pos * NOISE_FREQUENCY), life * NOISE_PERIOD)) * PI;
    vec2 noise_offset = vec2(cos(noise), sin(noise)) * NOISE_AMPLITUDE * NEXT_TEXEL_STEP;
    vec3 noise_component = texture(renderedTexture, uv+wave_offset+noise_offset).rgb;
    vec3 blur_component = convolute(blur_kernel, uv+wave_offset);
    if(noise_blur_black == 0.0){
        color.rgb = 0.6f*noise_component+0.4*blur_component;
    }else if(noise_blur_black == 1.0f){
        color.rgb = noise_component;
    }else if(noise_blur_black == 2.0f){
        color.rgb = blur_component;
    }else{
            color.rgb = vec3(0.0f, 0.0f, 0.0f);
    }

    color.a = 1.0f-length(position_modelspace)*2;
}
