#ifndef _DREAM_PARTICLE_UPDATE
#define _DREAM_PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

StructuredBuffer<tDreamParticleModule> g_Module : register(t20);
RWStructuredBuffer<tDreamParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_SpawnCount : register(u1);

#define MAX_COUNT   g_int_0 
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]
#define CenterPos   g_vec4_0.xyz

[numthreads(32, 1, 1)]
void CS_DreamParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;
    
    // ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (0 == Particle.Active)
    {
        
        while (0 < SpawnCount)
        {
            // Atomic �Լ� 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
           
            // InterlockedExchange �Լ��� �Ἥ SpawnCount �� ��ü, �����ϸ�
            // �ʱ� �õ��� ���尡 ���� �����尡 ������ ���Ŀ� �����ϴ� ��찡 �ִ�. 
            // �̶� SpawnCount �� ������ �÷������� ������ �߻��� �� �ִ�. 
            // InterlockedCompareExchange �� ���ؼ� ������ ���� ��ġ�� ��쿡�� 
            // ��ü�� �ϵ��� �ϴ� �Լ��� ����Ѵ�.
            InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            //InterlockedExchange(SpawnCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // ����Generate
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUV.x += g_time * 0.2f;
                //                 ( ���ļ� )    (����)  (V �� offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                
                // ���� :���̾� �Ҵ�(���� �������)
                float layersum = Module.vSpawnRate[0] + Module.vSpawnRate[1] + Module.vSpawnRate[2];
                float layer0 = Module.vSpawnRate[0] / layersum;
                float layer1 = Module.vSpawnRate[1] / layersum + layer0;
                
                if (layer0 > vRand1[0] )
                {
                    Particle.Layer = 0;
                }
                else if (layer1 < vRand1[0])
                {
                    Particle.Layer = 2;
                }
                else
                {
                    Particle.Layer = 1;
                }
                float layer = Particle.Layer;
                
                // ���� : ������ġ,Depth�� �������  (SpawnShape�� box���� ����)
                Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale[layer].x - (Module.vSpawnBoxScale[layer].x / 2.f);
                Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale[layer].y - (Module.vSpawnBoxScale[layer].y / 2.f);
                Particle.vLocalPos.z = vRand[2] * Module.vSpawnBoxScale[layer].z - (Module.vSpawnBoxScale[layer].z / 2.f) + Module.vSpawnDepth[layer];

                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // ���� ũ�� ����                
                Particle.vWorldScale = Module.vSpawnScale[layer];
                
                // ���� : ���� �÷� 
                if (0.3333333f >  vRand2[2])
                {
                    Particle.vColor = Module.vSpawnColor[3 * layer + 0];
                }
                else if(0.6666666f < vRand2[2])
                {
                    Particle.vColor = Module.vSpawnColor[3 * layer + 2];
                }
                else
                {
                    Particle.vColor = Module.vSpawnColor[3 * layer + 1];
                }
                
                // ���� : �ִϸ��̼� ����������
                if(0 == layer)
                {
                    float ran = vRand[0] * 6 - 0.01f;
                    Particle.CurFrameIdx = floor(ran);
                }
                else if(1 == layer)
                {
                    float ran = vRand[0] * 4 - 0.01f;
                    Particle.CurFrameIdx = floor(ran);
                }
                else
                {
                    float ran = vRand[0] * 2 - 0.01f;
                    Particle.CurFrameIdx = floor(ran);
                }
                
                // ���� : �ִϸ��̼� �����ð�
                Particle.AccTime = g_time +  vRand[2] * Module.FrameDuration*2;
                
                // ���� Mass 1����
                Particle.Mass = 1.f; 
                // velocity 0����
                Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                break;
            }
        }
    }
    
    // ��ƼŬ�� Ȱ��ȭ ���¶��
    else
    {
        //Particle.Age += g_dt;
        if (-1 == SpawnCount)
        {
            Particle.Active = 0;
            return;
        }
        
        // ������ ����
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
        vUV.x += g_time * 0.2f;
        vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        // �̹� �����ӿ��� �޴� ���� �ʱ�ȭ
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        //int maxframe0 = 6;
        //int maxframe1 = 4;
        //int maxframe2 = 2;
        //int layer = Particle.Layer;
        //int trueidx;
        
        // 2. Animation
        if (Particle.AccTime == 0.f
        || (Module.FrameDuration < g_time - Particle.AccTime))
        {
            int layer = Particle.Layer;
            int maxframe = (3 - layer) * 2;
            
            Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % maxframe;
            Particle.AccTime = g_time;
            
            //trueidx ���
            int trueidx;
            if (Particle.CurFrameIdx < (maxframe / 2))
            {
                trueidx = Particle.CurFrameIdx;
            }
            else
            {
                trueidx = maxframe - Particle.CurFrameIdx - 1;
            }
            
             Particle.CurTrueIdx = trueidx;
        }
        
        
        
        //if (0 == layer)
        //{
        //    if (Particle.AccTime == 0.f
        //    || (Module.FrameDuration < g_time - Particle.AccTime))
        //    {
        //    Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % maxframe0;
        //    Particle.AccTime = g_time;
        //    }
        //    //trueframe ���
        //    if (Particle.CurFrameIdx < (maxframe0 / 2))
        //    {
        //        trueidx = Particle.CurFrameIdx;
        //    }
        //    else
        //    {
        //        trueidx = maxframe0 - Particle.CurFrameIdx - 1;
        //    }
        //}
        //else if (1 == layer)
        //{
        //    if (Particle.AccTime == 0.f
        //    || (Module.FrameDuration < g_time - Particle.AccTime))
        //    {
        //    Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % maxframe1;
        //    Particle.AccTime = g_time;
        //    }
        //}
        //else
        //{
        //    if (Particle.AccTime == 0.f
        //    || (Module.FrameDuration < g_time - Particle.AccTime))
        //    {
        //    Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % maxframe2;
        //    Particle.AccTime = g_time;
        //    }
        //}

 
        
        
        //���̱׷��̼�1
        // Noise Force
        //if (Module.arrModuleCheck[4])
        //{
        //    if (Particle.NoiseForceTime == 0.f)
        //    {
        //        Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
        //        Particle.NoiseForceTime = g_time;
        //    }
        //    else if (Module.NoiseForceTerm < g_time - Particle.NoiseForceTime)
        //    {
        //        Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
        //        Particle.NoiseForceTime = g_time;
        //    }
        //}
                
     
        //���̱׷��̼�2
        // Calculate Force
        //if (Module.arrModuleCheck[5])
        //{
        //    Particle.vForce.xyz += Particle.vNoiseForce.xyz;
        //    
        //    // Force ����
        //    // F = M x A
        //    float3 vAccel = Particle.vForce.xyz / Particle.Mass;
        //    
        //    // Accel ����
        //    Particle.vVelocity.xyz += vAccel * g_dt;
        //    
        //    // Drag ����� ����������
        //    if (Module.arrModuleCheck[1])
        //    {
        //        float LimitTime = Module.DragTime - Particle.Age;
        //    
        //        if (LimitTime <= 0.f)
        //        {
        //            Particle.vVelocity = 0.f;
        //        }
        //        else
        //        {
        //            float DT = g_dt / LimitTime;
        //            Particle.vVelocity -= Particle.vVelocity * DT;
        //        }
        //    }
        //    
        //    // Velocity �� ���� ��ġ�̵� ����
        //    if (0 == Module.SpaceType)
        //    {
        //        Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_dt;
        //        Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
        //    }
        //    else if (1 == Module.SpaceType)
        //    {
        //        Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_dt;
        //    }
        //}
    }
}


#endif