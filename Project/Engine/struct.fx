#ifndef _STRUCT
#define _STRUCT

struct tLightColor
{
    float4 vColor; // 빛의 색상	- 광원의 순수 색상
    float4 vSpecular; // 반사광	- 광원이 물체 표변에 반사된 색상
    float4 vAmbient; // 환경광(주변광) - 광원에 의해서 보장되는 최소한의 빛
};

struct tLightInfo
{
    tLightColor Color;

    float3 vWorldPos;   // 광원의 위치
    float3 vWorldDir;   // 광원이 향하는 방향
    float fRadius;      // 광원의 영향 반경, 거리 정보
    float fAngle;       // 광원의 범위 각도

    int LightType;      // 광원 타입 (0 : Directional, 1 : Point, 2 : Spot)
    float3 vPadding;
};


struct tTileInfo
{
    int AtlasType;
    int AtlasIdx;
    int TileIdx;
    int padding;
};

struct tDreamParticle
{
	int		Active;					// 활성화, 비활성화 여부
	int		Layer;					//레이어 0, 1 ,2 
	float	Mass;					// 질량 1고정

	float4	vColor;					// 색상
	float4	vWorldScale;			// 크기
	float4	vLocalPos;				// 로컬 위치
	float4	vWorldPos;				// 윌드 위치
	float3	vVelocity;				// 속도
	float4	vWorldRotation;			// 회전값
	float3	vRotationVelocity;		// 각속도

	int		CurFrameIdx;			// 현재Frame
	float	AccTime;				// 현재frame누적시간

	float4	vForce;					// 입자에 적용된 누적 힘 총량
	float3	vNoiseForce;			// NoiseForce 모듈로 인한 랜덤 힘
	float	NoiseForceTime;			// NoiseForce 를 세팅받은 시간

	float	NoiseRotTime;			// NoiseRotVel 를 세팅받은 시간
};

struct tDreamParticleModule
{
	float	SpawnCount;			// 크리에이트시 생성 개수

	// 레이어정보
	float	vSpawnRate[3];		// 레이어1~3스폰비율
	float4	vSpawnBoxScale[3];	// 레이어1~3 박스크기
	float4	vSpawnColor[9];		// 레이어1~3 랜덤컬러
	float	vSpawnScale[3];		// 레이어1~3스케일
	float	vSpawnDepth[3];		// 레이어1~3댑스

	//애님모듈
	float FrameDuration;		// 듀레이션

	// Noise Force
	float	NoiseForceScale;
	float	NoiseForceTerm;

	// Noise Angle
	float	Angle;				// 해당 방향에서 랜덤범위 각도
	float	NoiseScaleTerm;

	// 시차보정용(cpu에서 계산되어나오는값
	// (ndc좌표와 scale을 보정하는 용도.)
	float NdcBoxbyDepth[3]; // 0=0.5 90=1 180 = 1.5
	float ScaleMultibyDepth[3];// 0 = 0 ,90 = 1 ,180 = 2;

	// Module On / Off
	int arrModuleCheck[2];
	int padding;
};


struct tParticle
{
    float4  vLocalPos;
    float4  vWorldPos; // 위치
    float4  vWorldInitScale; // 초기 크기
    float4  vWorldScale; // 크기
    float4  vWorldRotation; // 회전값
    float3  vVelocity; // 속도
    float4  vColor; // 색상
    float4  vForce; // 누적 힘
    float3  vNoiseForce; // NoiseForce 모듈로 인한 랜덤 힘
    float   NoiseForceTime; // NoiseForce 를 세팅받은 시간

    float   NomalizedAge;
    float   Mass; // 질량
    float   Age; // 현재 나이
    float   Life; // 수명
    int     Active; // 활성화, 비활성화 여부
    
    float4 ColorOrigin;
    float4 ColorVibra; // 원래 색상과 진동할 색상
    float  ColorAcctime; // 진동하지않은 누적시간
    
    int AtlasIdx; // 부여받은 Anim
    int CurFrameIdx; // 현재Frame
    float AccTime; // 현재frame누적시간
};

struct tParticleModule
{
	// Sapwn 모듈
    float4  vSpawnColor; // 초기 컬러
    float4  vSpawnMinScale; // 초기 최소 크기
    float4  vSpawnMaxScale; // 초기 최대 크기

    float   MinLife; // 최소 수명
    float   MaxLife; // 최대 수명
    float   MinMass; // 최소 질량
    float   MaxMass; // 최대 질량
    float   SpawnRate; // 초당 생성 개수
    int     SpaceType; // 좌표계(0 : LocalSpace, 1 : WorldSpace)
    
    int     SpawnShape; // 스폰 범위(0 : Sphere, 1 : Box)
    float   Radius; // SpawnShape 가 Sphere 인 경우, 반지름 길이
    float4  vSpawnBoxScale; // SpawnShape 가 Box 인 경우, Box 의 크기   
    
    // Add Velocity
    int     AddVelocityType; // 0 : From Center, 1: To Center, 2: Fix Direction
    float   MinSpeed;
    float   MaxSpeed;
    float   FixedAngle; // 해당 방향에서 랜덤범위 각도
    float4  FixedDirection; // 지정 방향
    
    // Scale
    float4  vScaleRatio;
    
    // Noise Force
    float   NoiseForceScale;
    float   NoiseForceTerm;
        
    // Drag
    float   DragTime;
        
    // Render
    int     VelocityAlignment;  // 1 : On, 0 : Off
    int     AlphaBasedLife;     // 0 : off, 1 : NomrlizedAge, 2: Age
    float   AlphaMaxAge;        // 알파가 0 가 되기까지의 수명
    
    // Vib Color
    float4  VibColor;
    float   VibTime;
    
    // Animation
    float FrameDuration;
    int NumberOfAtlas;
    int NumberOfFrame[5];
    
    // Module Active Check
    int arrModuleCheck[10];
};


struct tSpawnCount
{
    int iSpawnCount;
    int3 iPadding;
};

#endif
