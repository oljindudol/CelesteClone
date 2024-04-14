#ifndef _TRANSITION_PARTICLE_UPDATE
#define _TRANSITION_PARTICLE_UPDATE

#include "value.fx"
#include "struct.fx"
#include "func.fx"

StructuredBuffer<tParticleModule> g_Module : register(t20);
RWStructuredBuffer<tParticle> g_ParticleBuffer : register(u0);
RWStructuredBuffer<tSpawnCount> g_SpawnCount : register(u1);

#define MAX_COUNT   g_int_0 
#define SpawnCount  g_SpawnCount[0].iSpawnCount
#define Particle    g_ParticleBuffer[id.x]
#define Module      g_Module[0]
#define CenterPos   g_vec4_0.xyz
#define ToRight             g_int_1
#define EventDuration       g_float_0
#define EventCurTime        g_float_1
#define ScaleIncreaseRatio  g_float_2


[numthreads(32, 1, 1)]
void CS_TransitionParticleUpdate(uint3 id : SV_DispatchThreadID)
{
    if (MAX_COUNT <= id.x)
        return;
    
    // ��ƼŬ�� ��Ȱ��ȭ ���¶��
    if (0 == Particle.Active)
    {
        // ���� ��� Ȱ��ȭ üũ
        if (0 == Module.arrModuleCheck[0])
            return;
        
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
                //1. ��������
                Particle.ColorOrigin = Module.vSpawnColor;
                Particle.ColorVibra = Module.VibColor;
                Particle.ColorAcctime = 0.f;
                
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // ����
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUV.x += g_time * 0.2f;
                //                 ( ���ļ� )    (����)  (V �� offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                
                // SpawnShape �� Sphere Ÿ���̶��
                if (0 == Module.SpawnShape)
                {
                    float RandomRadius = vRand[0] * Module.Radius;
                    float RandomAngle = vRand[1] * 2 * PI;
                   
                    // Particle ������Ʈ(��ü) �� �߽���ġ(����) ����
                    // ���� ����, ���� �������� �ش��ϴ� ��ġ�� ����ؼ� ��ƼŬ�� �ʱ� ��ġ�� �ش�.
                    Particle.vLocalPos.xyz = float3(cos(RandomAngle), sin(RandomAngle), 0.f) * RandomRadius;
                }
                else
                {
                    //ToRight Event ���
                    if (1 == ToRight)
                    {
                        //�����ð��� Normalize�Ѵ�.
                        float nor = EventCurTime / EventDuration;
                        float w = 15.f;
                        Particle.vLocalPos.x = vRand[0] * w - (Module.vSpawnBoxScale.x ) * (nor - .5f);
                    }
                    else
                    {
                        Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale.x - (Module.vSpawnBoxScale.x / 2.f);
                    }
                    Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale.y - (Module.vSpawnBoxScale.y / 2.f);
                    Particle.vLocalPos.z = vRand[2] * Module.vSpawnBoxScale.z - (Module.vSpawnBoxScale.z / 2.f);
                    //Particle.vLocalPos.z = 0.f;
                }
                
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // ���� �÷� ����
                Particle.vColor = Module.vSpawnColor;
                
                // ���� ũ�� ����                
                Particle.vWorldInitScale = (Module.vSpawnMaxScale - Module.vSpawnMinScale) * vRand[2] + Module.vSpawnMinScale;
                Particle.vWorldScale = float4(0.f, 0.f, 0.f, 0.f);
                // ���� Life ����
                Particle.Age = 0.f;
                Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MaxLife;
                      
                // ���� Mass ����
                Particle.Mass = clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                float newspeed = Module.MinSpeed + vRand[2] * (Module.MaxSpeed - Module.MinSpeed);
                
                
                // Add VelocityModule
                if (Module.arrModuleCheck[3])
                {
                    // 0 : From Center
                    if (0 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    if (1 == Module.AddVelocityType)
                    {
                        float3 vDir = -normalize(Particle.vLocalPos.xyz);
                        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                    }
                    if (2 == Module.AddVelocityType)
                    {
                        float3 vDir = normalize(Module.FixedDirection);
                        Particle.vVelocity.xyz = vDir * newspeed;
                    }

                }
                else
                {
                     Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                }
                
                //2. �ִϸ��̼� ��ƼŬ
                if (1 == Module.arrModuleCheck[8] && 0 != Module.NumberOfAtlas)
                {
                    float ran = vRand[2];
                    if (Module.NumberOfAtlas == ran)
                    {
                        ran - 1;
                    }
                    ran = ran * Module.NumberOfAtlas;
                    
                    Particle.AtlasIdx = floor(ran);
                    Particle.CurFrameIdx = 0;
                    Particle.AccTime = 0.f;
                }
                
                break;
            }
        }
    }
    
    // ��ƼŬ�� Ȱ��ȭ ���¶��
    else
    {
        Particle.Age += g_dt;
        if (Particle.Life < Particle.Age)
        {
            Particle.Active = 0;
            return;
        }
        
        // ������ ����
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
        //vUV.x += g_time * 0.2f;
        //vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        //float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        
        // �̹� �����ӿ��� �޴� ���� �ʱ�ȭ
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // Normalize Age ���
        Particle.NomalizedAge = Particle.Age / Particle.Life;
        
        // Scale ���
        float a = ScaleIncreaseRatio;
        if (Module.arrModuleCheck[2])
        {
            if (0.5f > Particle.NomalizedAge)
            {
                Particle.vWorldScale = Particle.vWorldInitScale * exp(a * Particle.NomalizedAge);
            }
            else
            {
                Particle.vWorldScale = Particle.vWorldInitScale * exp(a * (1 - Particle.NomalizedAge));
            }
            //if (1.f <= Particle.NomalizedAge || 0.f >= Particle.NomalizedAge || Particle.Life <0.1f)
            //{
            //    Particle.vWorldScale = float4(0.f, 0.f, 0.f, 0.f);
            //}
            //else
            //{
            //  Particle.vWorldScale = Particle.vWorldInitScale * 4 *(Particle.NomalizedAge) * (1 - Particle.NomalizedAge);
            //}
        }
        
        // 1. �������� ����� ����������
        if (1 == Module.arrModuleCheck[7])
        {
            if (0.f == Particle.ColorAcctime
            || (Module.VibTime < g_time - Particle.ColorAcctime))
            {
                Particle.ColorAcctime = g_time;
                float iout;
                float2 uv;
                uv.x = modf(g_time, iout);
                uv.y = modf(g_time, iout);
            
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, uv, 0);
                Particle.vColor = Particle.ColorOrigin * vRand.x + Particle.ColorVibra * (1 - vRand.x);
            }
            else
            {
            
            }
        }
        
        // 2. Animation
        if (1 == Module.arrModuleCheck[8])
        {
            if (Particle.AccTime == 0.f
            || (Module.FrameDuration < g_time - Particle.AccTime))
            {
                Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % Module.NumberOfFrame[Particle.AtlasIdx];
                Particle.AccTime = g_time;
            }
            else
            {

            }
        }
        
        
        
        // Noise Force
        if (Module.arrModuleCheck[4])
        {
            if (Particle.NoiseForceTime == 0.f)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
            else if (Module.NoiseForceTerm < g_time - Particle.NoiseForceTime)
            {
                Particle.vNoiseForce = normalize(Rand.xyz * 2.f - 1.f) * Module.NoiseForceScale;
                Particle.NoiseForceTime = g_time;
            }
        }
                
     
        
        // Calculate Force
        if (Module.arrModuleCheck[5])
        {
            Particle.vForce.xyz += Particle.vNoiseForce.xyz;
            
            // Force ����
            // F = M x A
            float3 vAccel = Particle.vForce.xyz / Particle.Mass;
            
            // Accel ����
            Particle.vVelocity.xyz += vAccel * g_dt;
            
            // Drag ����� ����������
            if (Module.arrModuleCheck[1])
            {
                float LimitTime = Module.DragTime - Particle.Age;
            
                if (LimitTime <= 0.f)
                {
                    Particle.vVelocity = 0.f;
                }
                else
                {
                    float DT = g_dt / LimitTime;
                    Particle.vVelocity -= Particle.vVelocity * DT;
                }
            }
            
            // Velocity �� ���� ��ġ�̵� ����
            if (0 == Module.SpaceType)
            {
                Particle.vLocalPos.xyz += Particle.vVelocity.xyz * g_dt;
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
            }
            else if (1 == Module.SpaceType)
            {
                Particle.vWorldPos.xyz += Particle.vVelocity.xyz * g_dt;
            }
        }
    }
}

#endif