#ifndef _DRAWLINE
#define _DRAWLINE

#include "value.fx"
#include "struct.fx"

#define from         g_vec4_0
#define to           g_vec4_1
#define linecolor    g_vec4_2
#define linewidth    g_float_0
#define b_woobleline g_int_0

struct VS_IN
{
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
};

//138 = (320�ȼ�(ȭ�鰡���ִ�) / 16�ȼ������α׸���) * ���δ� 6���� ��������
[maxvertexcount(138)]
void GS_DrawLine(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    //���� or ���� ����
    float4 posdiff = abs(to - from);
    bool vertical;
    float length;
    if(posdiff.x < posdiff.y)
    {
        vertical = true;
        length = posdiff.y;
    }
    else
    {
        vertical = false;
        length = posdiff.x;
    }
    

    //����������    
    float ver = (true == vertical) ? length : linewidth;
    float hor = (true == vertical) ? linewidth : length;
    output[0].vPosition = float4(hor * -0.5f, (ver * 0.5f), 0.f, 1.f);
    output[1].vPosition = float4(hor * 0.5f, (ver * 0.5f), 0.f, 1.f);
    output[2].vPosition = float4(hor * 0.5f, (ver * -0.5f), 0.f, 1.f);
    output[3].vPosition = float4(hor * -0.5f, (ver * -0.5f), 0.f, 1.f);
    
    // ViewSpace �󿡼��� �߽� ������ ���ϱ�
    float4 centerpos = (to + from) / 2.f;
    float3 vCenterWorldPos = centerpos.xyz;
    float4 vViewPos = mul(float4(vCenterWorldPos.xyz, 1.f), g_matView);
    
    // View ��ǥ�� �̵�, ������� ����
    for (int i = 0; i < 4; ++i)
    {
        output[i].vPosition.xyz += vViewPos.xyz;
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
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


float4 PS_DrawLine(GS_OUT _in) : SV_Target
{
    return linecolor;
}

#endif