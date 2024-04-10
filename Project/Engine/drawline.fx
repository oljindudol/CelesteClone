#ifndef _DRAWLINE
#define _DRAWLINE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tDreamParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<tDreamParticleModule> g_ParticleModule : register(t21);

#define Particle g_ParticleBuffer[_in.iInstID]
#define ParticleSystemCenterPos   g_vec4_0
#define ParticleSystemCenterScale g_vec4_1
#define CameraPos g_vec4_2

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint iInstID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};




#endif