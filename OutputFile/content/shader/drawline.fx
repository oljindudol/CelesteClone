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
    float2 vUV : TEXCOORD;
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
    return (float) (sin((double) (seed + index / 16.f) + sin((double) (seed * 2.f + index / 32.f)) * 6.2831854820251465) + 1.0f) * 1.5f;
}

//void DrawLine(float2 _start, float2 _end, inout TriangleStream<GS_OUT> _OutStream)
//{
//    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
//    
//    
//    
//    
//}



//138 = (320�ȼ�(ȭ�鰡���ִ�) / 16�ȼ������α׸���) * ���δ� 6���� ��������
[maxvertexcount(138)]
void GS_DrawLine(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    
    float length = distance(to.xyz , from.xyz);
    float2 normal = normalize(to.xy - from.xy);
    float2 perp = float2(normal.y, -normal.x);

    float lastAmp = 0.f;
    int interval = 16;
    int i = 2;
    
    
    //���� or ���� ����
    float4 posdiff = abs(to - from);
    bool vertical;
    if(posdiff.x < posdiff.y)
    {
        vertical = true;
        //length = posdiff.y;
    }
    else
    {
        vertical = false;
        //length = posdiff.x;
    }
    
    while ((float) i < length - 2.f)
    {
        float amp = lerp(LineAmplitude(wobbleFrom, (float) i), LineAmplitude(wobbleTo, (float) i), wobbleEase);
        if ((float) (i + interval) >= length)
        {
            amp = 0.f;
        }
        float len = min((float) interval, length - 2.f - (float) i);
        float2 start= from.xy + normal * (float) i + perp * lastAmp;
        float2 end = from.xy + normal * ((float) i + len) + perp * amp;
        
        float l = distance(end, start);
        //Draw.Line(start, end, line);
        
        //����������    
        float ver = (true == vertical) ? l : linewidth;
        float hor = (true == vertical) ? linewidth : l;
    
        output[0].vPosition = float4(hor * -0.5f, (ver * 0.5f), 0.f, 1.f);
        output[1].vPosition = float4(hor * 0.5f, (ver * 0.5f), 0.f, 1.f);
        output[2].vPosition = float4(hor * 0.5f, (ver * -0.5f), 0.f, 1.f);
        output[3].vPosition = float4(hor * -0.5f, (ver * -0.5f), 0.f, 1.f);
    
        // ViewSpace �󿡼��� �߽� ������ ���ϱ�
        float4 centerpos = float4((start + end) / 2.f, from.z,0.f);
        float3 vCenterWorldPos = centerpos.xyz;
        float4 vViewPos = mul(float4(vCenterWorldPos.xyz, 1.f), g_matView);
    
    // View ��ǥ�� �̵�, ������� ����
        for (int j = 0; j < 4; ++j)
        {
            output[j].vPosition.xyz += vViewPos.xyz;
            output[j].vPosition = mul(output[j].vPosition, g_matProj);
            output[j].color = 0;
        }

        _OutStream.Append(output[0]);
        _OutStream.Append(output[2]);
        _OutStream.Append(output[3]);
        _OutStream.RestartStrip();
    
        _OutStream.Append(output[0]);
        _OutStream.Append(output[1]);
        _OutStream.Append(output[2]);
        _OutStream.RestartStrip();
    
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
    return float4(0.f, 0.f, 0.f, 1.f);
}

#endif