#ifndef _PLAYER
#define _PLAYER

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VS_OUT VS_Hair(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    

    _in.vPos = _in.vPos * float3(g_HairScale, 1.f);
    //_in.vPos = _in.vPos * float3(10.f,10.f, 1.f);

    _in.vPos = _in.vPos + g_HairOffset;

    
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    
    return output;
}



float4 PS_Hair(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_vec4_1.w == 3.14f)
        return float4(1.f, 1.f, 0.f, 1.f);
    //if(g_Int_Bang)
    //{
    //    vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    //    float fAlpha = vColor.a;
    //    if (fAlpha > 0.9f)
    //    {
    //        vColor = g_HairColor;
    //        //vColor = float4(g_HairColor, 1.f);
    //        //vColor = float4(1.f, 0.f, 0.f, 1.f);
    //    }
    //    else
    //    {
    //        discard;
    //    }
    //}
    //else
    //{
        //vColor = g_tex_0.Sample(g_sam_1, _in.vUV + float2(g_time * 0.1, 0.f));
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        float fAlpha = vColor.a;
        //saturate 0 ~ 1 을 넘지 않게 보정
        //float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
    
        if (fAlpha < 0.5f)
        {
        // 픽셀 쉐이더를 중간에 폐기처리
            discard; //clip(-1);            
        }
        else
        {
            //vColor = float4(1.f, 0.f, 0.f, 1.f);
            vColor = g_HairColor;
            //vColor = float4(g_HairColor, 1.f);
            //vColor = float4(1.f, 0.f, 0.f, 1.f);
        }
    //}


    
    // 광원 처리
    // 광원의 타입별 처리
    // 광원이 여러개일 때 처리
    //g_Light2DCount;    
    tLightColor LightColor = (tLightColor) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalLight2D(_in.vWorldPos, i, LightColor);
    }
    
    vColor.rgb *= (LightColor.vColor.rgb + LightColor.vAmbient.rgb);
        
    if (0.f == vColor.a)
        discard;
    
    float x = g_NoiseTex.Sample(g_sam_0, _in.vUV).x;
    if (0 > x - g_float_1)
    {
        discard;
    }
    
    return vColor;
}

#endif