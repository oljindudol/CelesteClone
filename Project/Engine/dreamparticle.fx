#ifndef _DREAMPARTICLE
#define _DREAMPARTICLE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tDreamParticle> g_ParticleBuffer : register(t20);
StructuredBuffer<tDreamParticleModule> g_ParticleModule : register(t21);

#define Particle g_ParticleBuffer[_in.iInstID]
#define ParticleSystemCenterPos   g_vec4_0
#define ParticleSystemCenterScale g_vec4_1

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
    float ClipDistance : SV_ClipDistance;
};


// DreamParticle 전용 GS
[maxvertexcount(12)]
void GS_DreamParticle(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    GS_OUT output_cross[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
        
    // GS 가 담당하는 파티클 정보를 가져온다.    
    tDreamParticle particle = g_ParticleBuffer[_in[0].InstID];
    if (0 == particle.Active)
    {
        return;
    }
    
    // 파티클의 중심점을 월드로 이동    
    float3 vWorldPos = particle.vWorldPos.xyz;
    
    // 파티클의 ViewSpace 상에서의 중심 포지션 구하기
    float4 vViewPos = mul(float4(vWorldPos, 1.f), g_matView);
    
    float3 vCenterWorldPos = ParticleSystemCenterPos.xyz;
    float4 vCenterViewPos = mul(float4(vCenterWorldPos, 1.f), g_matView);
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    float4 vClip[2];
    vClip[0] = float4((ParticleSystemCenterScale.x * -0.5f), (ParticleSystemCenterScale.y * 0.5f), 0.f, 1.f);
    vClip[1] = float4((ParticleSystemCenterScale.x *  0.5f), (ParticleSystemCenterScale.y * -0.5f), 0.f, 1.f);
   
    output[0].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4((particle.vWorldScale.x * 0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4((particle.vWorldScale.x * -0.5f), (particle.vWorldScale.y * -0.5f), 0.f, 1.f);
          
    output_cross[0].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[1].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * 0.5f), 1.f);
    output_cross[2].vPosition = float4((particle.vWorldScale.x * 0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    output_cross[3].vPosition = float4((particle.vWorldScale.x * -0.5f), 0.f, (particle.vWorldScale.y * -0.5f), 1.f);
    
    output_cross[0].vUV = output[0].vUV = float2(0.f, 0.f);
    output_cross[1].vUV = output[1].vUV = float2(1.f, 0.f);
    output_cross[2].vUV = output[2].vUV = float2(1.f, 1.f);
    output_cross[3].vUV = output[3].vUV = float2(0.f, 1.f);
        
    
    // 렌더모듈 기능
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            float3 vR = normalize(mul(float4(particle.vVelocity.xyz, 0.f), g_matView).xyz);
            float3 vF = normalize(cross(vR, float3(0.f, 1.f, 0.f)));
            float3 vU = normalize(cross(vF, vR));
            
            float3x3 vRot =
            {
                vR,
                vU,
                vF
            };
            
            for (int i = 0; i < 4; ++i)
            {
                output[i].vPosition.xyz = mul(output[i].vPosition.xyz, vRot);
                output_cross[i].vPosition.xyz = mul(output_cross[i].vPosition.xyz, vRot);
            }
        }
    }
    
    //center view 좌표로이동, 투영행렬 적용
    for (int k = 0;k < 2; ++k)
    {
        vClip[k].xyz += vCenterViewPos.xyz;
        vClip[k] = mul(vClip[k], g_matProj);
    }
    
    // View 좌표로 이동, 투영행렬 적용
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
        
        output_cross[i].vPosition.xyz += vViewPos.xyz;
        output_cross[i].vPosition = mul(output_cross[i].vPosition, g_matProj);
        
        output[i].InstID = _in[0].InstID;
        output_cross[i].InstID = _in[0].InstID;

        //output[i].ClipDistance = (vClip[0].x > output[i].vPosition.x) ? -1 : 1;
        //output[i].ClipDistance = (vClip[1].x < output[i].vPosition.x) || (-1 == output[i].ClipDistance) ? -1 : 1;
        //output[i].ClipDistance = (vClip[0].y < output[i].vPosition.y) || (-1 == output[i].ClipDistance) ? -1 : 1;
        //output[i].ClipDistance = (vClip[1].y > output[i].vPosition.y) || (-1 == output[i].ClipDistance) ? -1 : 1;
        
        //output_cross[i].ClipDistance = (vClip[0].x > output_cross[i].vPosition.x) ? -1 : 1;
        //output_cross[i].ClipDistance = (vClip[1].x < output_cross[i].vPosition.x) || (-1 == output_cross[i].ClipDistance) ? -1 : 1;
        //output_cross[i].ClipDistance = (vClip[0].y < output_cross[i].vPosition.y) || (-1 == output_cross[i].ClipDistance) ? -1 : 1;
        //output_cross[i].ClipDistance = (vClip[1].y > output_cross[i].vPosition.y) || (-1 == output_cross[i].ClipDistance) ? -1 : 1;
    }
      
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
        
    if (g_ParticleModule[0].arrModuleCheck[6])
    {
        // 속도에 따른 정렬 기능
        if (g_ParticleModule[0].VelocityAlignment)
        {
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[2]);
            _OutStream.Append(output_cross[3]);
            _OutStream.RestartStrip();
    
            _OutStream.Append(output_cross[0]);
            _OutStream.Append(output_cross[1]);
            _OutStream.Append(output_cross[2]);
            _OutStream.RestartStrip();
        }
    }
}



// DreamParticle 전용 PS
float4 PS_DreamParticle(GS_OUT _in) : SV_Target
{
    tDreamParticle particle = g_ParticleBuffer[_in.InstID];
    tDreamParticleModule module = g_ParticleModule[0];
    
    
    // 0 -- 1	     
	// | \  |	     
	// 3 -- 2
    //if (_in.vClip[0].x > _in.vPosition.x)
    // || _in.vClip[1].x < _in.vPosition.x)
    //    discard;
    //if (_in.vClip[0].y < _in.vPosition.y 
    //    || _in.vClip[1].y > _in.vPosition.y)
    //    discard;
        
    
    
    // 출력 색상
    float4 vOutColor;
    if (module.arrModuleCheck[9])
    {
        vOutColor = float4(1.f, 1.f, 1.f, 1.f);
    }
    else
    {
        vOutColor = particle.vColor;
        vOutColor.a = 1.f;
    }
    
    //if (g_btex_0)
    //{
    //    float4 vSampleColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    //    vOutColor.rgb *= vSampleColor.rgb;
    //    vOutColor.a = vSampleColor.a;
    //}
    
    //particle.CurFrameIdx;
    //particle.AtlasIdx;
    //module.NumberOfAtlas;
    //module.NumberOfFrame;
    
    float uvunit = 1.f / module.NumberOfFrame[particle.AtlasIdx];
    float left = uvunit * particle.CurFrameIdx;
    float newUVx = _in.vUV.x * uvunit + left;
    
    float4 vSampleColor = g_texarr_0.Sample(g_sam_0, float3(float2(newUVx, _in.vUV.y), particle.AtlasIdx));
    vOutColor.rgb *= vSampleColor.rgb;
    vOutColor.a = vSampleColor.a;
    
    
    // 렌더모듈이 켜져 있으면
    if (module.arrModuleCheck[6])
    {
        if (1 == module.AlphaBasedLife)
        {
            vOutColor.a *= saturate(1.f - clamp(particle.NomalizedAge, 0.f, 1.f));
        }
        else if (2 == module.AlphaBasedLife)
        {
            float fRatio = particle.Age / module.AlphaMaxAge;
            vOutColor.a *= saturate(1.f - clamp(fRatio, 0.f, 1.f));
        }
    }
    
    return vOutColor;
}


#endif