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
    
    // 파티클이 비활성화 상태라면
    if (0 == Particle.Active)
    {
        
        while (0 < SpawnCount)
        {
            // Atomic 함수 
            int AliveCount = SpawnCount;
            int Exchange = SpawnCount - 1;
            int Origin = 0;
           
            // InterlockedExchange 함수를 써서 SpawnCount 를 교체, 수정하면
            // 초기 시도인 스드가 여러 스레드가 성공한 이후에 진입하는 경우가 있다. 
            // 이때 SpawnCount 를 오히려 늘려버리는 현상이 발생할 수 있다. 
            // InterlockedCompareExchange 를 통해서 예상한 값과 일치할 경우에만 
            // 교체를 하도록 하는 함수를 사용한다.
            //InterlockedCompareExchange(SpawnCount, AliveCount, Exchange, Origin);
            InterlockedExchange(SpawnCount, Exchange, Origin);
            
            if (AliveCount == Origin)
            {
                Particle.Active = 1;
                Particle.vNoiseForce = (float3) 0.f;
                Particle.NoiseForceTime = 0.f;
                
                // 랜덤
                float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
                
                vUV.x += g_time * 0.2f;
                //                 ( 주파수 )    (진폭)  (V 축 offset)
                vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
                                
                float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
                float4 vRand1 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.1f, 0.1f), 0);
                float4 vRand2 = g_NoiseTex.SampleLevel(g_sam_0, vUV - float2(0.2f, 0.2f), 0);
                
                // SpawnShape 가 box고정
                Particle.vLocalPos.x = vRand[0] * Module.vSpawnBoxScale[0].x - (Module.vSpawnBoxScale[0].x / 2.f);
                Particle.vLocalPos.y = vRand[1] * Module.vSpawnBoxScale[0].y - (Module.vSpawnBoxScale[0].y / 2.f);
                Particle.vLocalPos.z = vRand[2] * Module.vSpawnBoxScale[0].z - (Module.vSpawnBoxScale[0].z / 2.f) + 180.f;
                
                Particle.vWorldPos.xyz = Particle.vLocalPos.xyz + CenterPos;
                
                // 스폰 컬러 설정
                Particle.vColor = Module.vSpawnColor[0];
                
                // 스폰 크기 설정                
                Particle.vWorldScale = Module.vSpawnScale[0];
                                
                // 스폰 Life 설정
                //Particle.Age = 0.f;
                //Particle.Life = (Module.MaxLife - Module.MinLife) * vRand[0] + Module.MaxLife;
                      
                // 스폰 Mass 설정
                Particle.Mass = 1.f; // clamp(vRand1[0], Module.MinMass, Module.MaxMass);
                //float newspeed = Module.MinSpeed + vRand[2] * (Module.MaxSpeed - Module.MinSpeed);
                
                
                Particle.vVelocity.xyz = float3(0.f, 0.f, 0.f);
                // Add VelocityModule
                //if (Module.arrModuleCheck[3])
                //{
                //    // 0 : From Center
                //    if (0 == Module.AddVelocityType)
                //    {
                //        float3 vDir = normalize(Particle.vLocalPos.xyz);
                //        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                //    }
                //    if (1 == Module.AddVelocityType)
                //    {
                //        float3 vDir = -normalize(Particle.vLocalPos.xyz);
                //        Particle.vVelocity.xyz = vDir * clamp(vRand[2], Module.MinSpeed, Module.MaxSpeed);
                //    }
                //    if (2 == Module.AddVelocityType)
                //    {
                //        float3 vDir = normalize(Module.FixedDirection);
                //        Particle.vVelocity.xyz = vDir * newspeed;
                //    }
                //
                //}
                //else
                //{
                //}
                
                //2. 애니메이션 파티클

                Particle.CurFrameIdx = 0;
                Particle.AccTime = 0.f;
                
                break;
            }
        }
    }
    
    // 파티클이 활성화 상태라면
    else
    {
        //Particle.Age += g_dt;
        if (-1 == SpawnCount)
        {
            Particle.Active = 0;
            return;
        }
        
        // 랜덤값 추출
        float2 vUV = float2((1.f / (MAX_COUNT - 1)) * id.x, 0.f);
        float3 Rand;
        GaussianSample(g_NoiseTex, g_NoiseTexResolution, vUV.x, Rand);
        
        //vUV.x += g_time * 0.2f;
        //vUV.y = sin(vUV.x * 20.f * PI) * 0.2f + g_time * 0.1f;
        //float4 vRand = g_NoiseTex.SampleLevel(g_sam_0, vUV, 0);
        
        
        // 이번 프레임에서 받는 힘을 초기화
        Particle.vForce.xyz = float3(0.f, 0.f, 0.f);
        
        // Normalize Age 계산
        //Particle.NomalizedAge = Particle.Age / Particle.Life;
        

        
        // 2. Animation

        if (Particle.AccTime == 0.f
        || (Module.FrameDuration < g_time - Particle.AccTime))
        {
            Particle.CurFrameIdx = (Particle.CurFrameIdx + 1) % 3;
            Particle.AccTime = g_time;
        }
        else
        {

        }
        
        
        //마이그레이션1
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
                
     
        //마이그레이션2
        // Calculate Force
        //if (Module.arrModuleCheck[5])
        //{
        //    Particle.vForce.xyz += Particle.vNoiseForce.xyz;
        //    
        //    // Force 연산
        //    // F = M x A
        //    float3 vAccel = Particle.vForce.xyz / Particle.Mass;
        //    
        //    // Accel 연산
        //    Particle.vVelocity.xyz += vAccel * g_dt;
        //    
        //    // Drag 모듈이 켜져있으면
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
        //    // Velocity 에 따른 위치이동 연산
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