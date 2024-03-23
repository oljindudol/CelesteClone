#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

// ==============
// TileMap Shader

// Parameter
#define FACE_X      g_int_0
#define FACE_Y      g_int_1
#define IDX_HightLight      g_int_2
// ��Ʋ�󽺿��� Ÿ���ϳ��� uv
#define vSliceUV    g_vec2_0

//#define TileAtlas0   g_tex_0
//#define TileAtlas1   g_tex_1
#define ArrTileAtlas   g_texarr_0


StructuredBuffer<tTileInfo> g_TileInfo : register(t20);

// ==============
struct VS_IN
{    
    float3 vPos : POSITION; 
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;  
    float2 vUV : TEXCOORD;
};



VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);   
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    int bufferidx; 
    
    // �� ������ŭ _in.vUV �� ������ �ø�
    float2 vUV = _in.vUV * float2(FACE_X, FACE_Y);
        
        //������
    int2 Integer = (int2) floor(vUV);
        //�Ҽ���
    float2 tileUV = vUV - Integer;
        
        //Ÿ���� �ε���
    bufferidx = Integer.y * FACE_X + Integer.x;
        
        // �̹����� �ε����� ����.
    int iImageIdx = g_TileInfo[bufferidx].TileIdx;
    int iAtlasIdx = g_TileInfo[bufferidx].AtlasIdx;
    
    //�����Ϸ�����
    //Texture2D Txture = g_tex_0;
    //if (1 == g_TileInfo[bufferidx].AtlasIdx)
    //{
    //    Txture = g_tex_1;
    //}
    //else
    //{
    //    Txture = g_tex_0;
    //}
    
    //if (Txture)
    //{
        if (-1 == iImageIdx && IDX_HightLight != bufferidx)
            discard;
        
        //��Ʋ��idx���� ���ٰ���
        uint iTileWidthCount = 1.f / vSliceUV.x;
        //��Ʋ�� idx->rowcol ������
        uint iRow = (uint) iImageIdx / iTileWidthCount;
        uint iCol = (uint) iImageIdx % iTileWidthCount;
        
        //�ε��� UV�� �̵�
        float2 IdxUV = vSliceUV * float2(iCol, iRow);
        //Ÿ���� UV��
        tileUV = vSliceUV * tileUV;
        float2 finalUV = IdxUV + tileUV;
        float3 UV3 = float3(finalUV, iAtlasIdx);
        vColor = ArrTileAtlas.Sample(g_sam_0, UV3); //,g_TileInfo[bufferidx].AtlasIdx);
        //if (0 == g_TileInfo[bufferidx].AtlasIdx)
        //{
        //    vColor = TileAtlas0.Sample(g_sam_0, IdxUV + tileUV);
        //}
        //else
        //{
        //    vColor = TileAtlas1.Sample(g_sam_0, IdxUV + tileUV);
        //
        //}
        //���� LT���
        //vUV = g_TileInfo[bufferidx].TileIdx + (vSliceUV * vUV);
        //vColor = TileAtlas.Sample(g_sam_0, vUV);
    //}
    
    if (IDX_HightLight == bufferidx)
    {
        vColor = vColor * 0.5 + float4(0.9f, 0.9f, 0.f, 1.f) * 0.5;
    }
    return vColor;
}

/////////////
//Grid (frame) v shader
/////////////
VS_OUT VS_Grid(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f; // �ʱ�ȭ
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
	
    return output;
}


/////////////
//Grid (frame) pixel shader
/////////////
float4 PS_Grid(VS_OUT _in) : SV_Target
{
    return float4(0.9f, 0.9f, 0.f, 1.f);
}

#endif