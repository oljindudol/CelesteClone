#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// mesh : RectMesh
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;    
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float2 vCenter : CENTER;
};

#define TrueRender float2(160.f, 85.f)
// ==========
// Shock Wave
// ==========
VS_OUT VS_ShockWave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    float2 center = float2(0.f, 0.f);
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    float2 proj = mul(float4(center, 1.f, 1.f), g_matWVP);
    center = proj.xy;
    center = (center + (float2(TrueRender.x, TrueRender.y) / 2.f)) / TrueRender;
    output.vCenter = float2(center.x,  1- center.y);
    
    return output;
    
    //VS_OUT output = (VS_OUT) 0.f;
    //float3 center = float3(0.f, 0.f, 0.f);
    //
    //_in.vPos = _in.vPos - float3(g_vOffset, 0.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    //output.vCenter = mul(float4(center, 1.f), g_matWVP);
    //
    ////output.vUV = _in.vUV / float2(2.f, 2.f); // / g_RenderResolution * float2(100.f, 100.f);
    //output.vUV = _in.vUV; // * float2(100.f / g_RenderResolution.x, 100.f / g_RenderResolution.y);
    //return output;
}

float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898f, 78.233f))) * 43758.5453);
}



float4 PS_ShockWave(VS_OUT _in) : SV_Target
{
     //Sawtooth function to pulse from centre.
    float offset = (g_ShockAcctime - floor(g_ShockAcctime)) / g_ShockAcctime;
    float CurrentTime = (g_ShockAcctime) * (offset);
    CurrentTime /= 12.f;
    
    
    //float3 WaveParams = float3(10.0, 0.8, 0.1);
    //y: 왜곡되는 강도 , z:왜곡되는 범위
    float3 WaveParams = float3(.5f, 0.1, 0.1f);
    
    float ratio = TrueRender.y / TrueRender.x;
    
    //Use this if you want to place the centre with the mouse instead
    float2 Wafloatentre = _in.vCenter.xy * float2(1.f, 0.5f) - float2(0.f, -0.25f); // float2( _in.vCenter.xy / TrueRender.xy);
       
    //float2 Wafloatentre = float2(0.5, 0.5);
    //Wafloatentre.y *= ratio;
   
    float2 texCoord = _in.vUV; //_in.vPosition.xy / g_RenderResolution.xy;
    //texCoord.y *= ratio;
    float Dist = distance(float2(texCoord.x, texCoord.y * ratio), float2(Wafloatentre.x, Wafloatentre.y * ratio));
    //float Dist = distance(texCoord, Wafloatentre);
    
	
    float4 Color = g_postprocess.Sample(g_sam_0, texCoord);
    float minDist = 0.045f;
    float maxDist = .1f;
    float fadeOutFactor = 1.0 - (Dist / maxDist);
    
    //Only distort the pixels within the parameter distance from the centre
    if ((Dist <= ((CurrentTime) + (WaveParams.z))) &&
	(Dist >= ((CurrentTime) - (WaveParams.z)))
     && (Dist <= maxDist))
    {
        //The pixel offset distance based on the input parameters
        float Diff = (Dist - CurrentTime);
        float ScaleDiff = (1.0 - pow(abs(Diff * WaveParams.x), WaveParams.y));
        float DiffTime = (Diff * ScaleDiff);
        
        //The direction of the distortion
        float2 DiffTexCoord = normalize(texCoord - Wafloatentre);
        
        //Perform the distortion and reduce the effect over time
        //파형의 왜곡정도(작을때큼)
        float degree = 10.f;
        // 감쇠 계수 계산
        // 왜곡 강도 최대치를 결정하는 반지름의 절반 값
        float halfMaxDist = maxDist * .5f;
        // 거리에 따른 왜곡 강도 조절을 위한 계수 계산
        float distortionStrength = 1.0 - abs(Dist - halfMaxDist) / halfMaxDist;
        //float distortionStrength = max(0.0, (Dist - minDist) / (maxDist - minDist));
        
        // 시간에 따른 왜곡 강도 조절
        float timeFactor = 1.f - smoothstep(0.0, 1.f, g_ShockAcctime);
        
        texCoord += ((DiffTexCoord * DiffTime) * timeFactor / (CurrentTime * Dist * 40.0 * degree)) * distortionStrength;
        Color = g_postprocess.Sample(g_sam_0, texCoord);
        float4 OriginColor = Color;
        
        //Blow out the color and reduce the effect over time
        //파형의 보간감소
        float decline = 10000.f;
        Color += (Color * ScaleDiff) / (CurrentTime * Dist * 40.0 * decline);
        
        //거리에따른 색상강도조정
        Color *= fadeOutFactor;

        
        //// 노이즈 텍스처 샘플링
        //float4 noise = g_tex_0.Sample(g_sam_0, texCoord);
        //// 노이즈 강도 조절
        //float noiseIntensity = 0.01 + 0.99 * (1 - fadeOutFactor); // 예시로, fadeOutFactor에 따라 노이즈 강도가 0.5에서 1.0 사이로 조절됩니다.
        //// 최종 색상에 노이즈 적용
        //Color += noise * noiseIntensity;
        
        
        //최대밝기조절
        Color = min(OriginColor * 1.2f, Color);
        //최소밝기조절
        Color = max(OriginColor, Color / 1.f);
    }
    
    return Color;

}











// ==========
// GrayFilter
// ==========
VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;    
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    
    vColor.rgb = float3(aver, aver, aver);
    
    return vColor;
}


// 
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    //_in.vPos = _in.vPos - float3(g_vOffset, 0.f);
    //output.vPosition = float4(_in.vPos * .5f, 1.f);
    //output.vUV = _in.vUV;
    //
    //return output;
    
    _in.vPos = _in.vPos - float3(g_vOffset, 0.f);
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
        
    // 픽셀쉐이더에 SV_Position 으로 입력된 값은 픽셀 쉐이더를 호출한 해당 픽셀의 좌표가 들어있다.
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    
    //vScreenUV.y += cos((vScreenUV.x + (g_time * (속도) )) * (주파수)) * (진폭);
    //vScreenUV.y += cos((vScreenUV.x + (g_time *  0.1f))   *   40.f)  *  0.1f;
    
    if (g_btex_0)
    {
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
        
        float2 vNoise = g_tex_0.Sample(g_sam_0, vUV);
        vNoise = (vNoise.xy - 0.5f) * 0.1f;
        
        vScreenUV += vNoise;
    }
        
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}






#endif