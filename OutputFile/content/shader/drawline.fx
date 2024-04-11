#ifndef _DRAWLINE
#define _DRAWLINE

#include "value.fx"
#include "struct.fx"

#define from         g_vec4_0
#define to           g_vec4_1
#define linecolor    g_vec4_2
#define linewidth    g_float_0
#define b_woobleline g_int_0
#define wobbleFrom   g_float_1
#define wobbleTo     g_float_2
#define wobbleEase   g_float_3
#define vertical_in  g_int_1

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct GS_OUT
{
    float4 vPosition : SV_Position;
    //float2 vUV : TEXCOORD;
    int    color  : col;
};


VS_OUT VS_DrawLine(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    return output;
}


float LineAmplitude(float seed, float index)
{
    return (float) (sin((double) (seed + index / 16.f) + sin((double) (seed * 2.f + index / 32.f)) * 6.2831854820251465f) + 1.0f) * 1.5f;
}

void DrawLine(float2 _start, float2 _end, int _color, bool _vertical,inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    float len = distance(_end, _start);
    
    //로컬포지션    
    float ver = (true == _vertical) ? len : linewidth;
    float hor = (true == _vertical) ? linewidth : len;
    
    output[0].vPosition = float4(hor * -0.5f, (ver * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4(hor * 0.5f, (ver * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4(hor * 0.5f, (ver * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4(hor * -0.5f, (ver * -0.5f), 0.f, 1.f);
    
    // ViewSpace 상에서의 중심 포지션 구하기
    float4 centerpos = float4((_start + _end) / 2.f, from.z, 0.f);
    float3 vCenterWorldPos = centerpos.xyz;
    float4 vViewPos = mul(float4(vCenterWorldPos.xyz, 1.f), g_matView);
    
    // View 좌표로 이동, 투영행렬 적용
    for (int j = 0; j < 4; ++j)
    {
        output[j].vPosition.xyz += vViewPos.xyz;
        output[j].vPosition = mul(output[j].vPosition, g_matProj);
        output[j].color = _color;
    }
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
}



//138 = (320픽셀(화면가로최대) / 16픽셀씩라인그리기) * 라인당 6개의 정점생성
[maxvertexcount(200)]
void GS_DrawLine(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    
    //가로 or 세로 판정
    float4 posdiff = abs(to - from);
    bool vertical;
    if(posdiff.x < posdiff.y)
    {
        vertical = true;
    }
    else
    {
        vertical = false;
    }
    
    if (0 == b_woobleline)
    {
        DrawLine(from.xy, to.xy, 1, (1 == vertical_in)? true:false, _OutStream);
        return;
    }
    
    
    float length = distance(to.xyz, from.xyz);
    float2 normal = normalize(to.xy - from.xy);
    float2 perp = float2(normal.y, -normal.x);
    float lastAmp = 0.f;
    int   interval = 8;
    int   i = 2;
    
    while ((float) i < length - 2.f)
    {
        float amp = lerp(LineAmplitude(wobbleFrom, (float) i), LineAmplitude(wobbleTo, (float) i), wobbleEase ) /1.5f;
        if ((float) (i + interval) >= length)
        {
            amp = 0.f;
        }
        float len = min((float) interval, length - 2.f - (float) i);
        float2 start = from.xy + normal * (float) i + perp * float2(lastAmp, lastAmp);
        float2 end = from.xy + normal * ((float) i + len) + perp * float2(amp, amp);

        //DrawLine(start - perp, end - perp, 2, vertical, _OutStream);
        //DrawLine(start - perp * 2.f, end - perp * 2.f, 3, vertical, _OutStream);
        DrawLine(start, end, 1, vertical, _OutStream);
        
        lastAmp = amp;
        i += interval;
    }
    
    
}


float4 PS_DrawLine(GS_OUT _in) : SV_Target
{
    if(1 == _in.color)
    {
        return linecolor;
    }
    if (2 == _in.color)
    {
        return float4(1.f, 1.f, 0.f, 1.f);;
    }
    if (3 == _in.color)
    {
        return float4(0.f, 1.f, 0.f, 1.f);;
    }
    return float4(0.f, 0.f, 0.f, 1.f);
}

#endif