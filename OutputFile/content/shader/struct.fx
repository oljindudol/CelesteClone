#ifndef _STRUCT
#define _STRUCT

struct tLightColor
{
    float4 vColor; // ���� ����	- ������ ���� ����
    float4 vSpecular; // �ݻ籤	- ������ ��ü ǥ���� �ݻ�� ����
    float4 vAmbient; // ȯ�汤(�ֺ���) - ������ ���ؼ� ����Ǵ� �ּ����� ��
};

struct tLightInfo
{
    tLightColor Color;

    float3 vWorldPos;   // ������ ��ġ
    float3 vWorldDir;   // ������ ���ϴ� ����
    float fRadius;      // ������ ���� �ݰ�, �Ÿ� ����
    float fAngle;       // ������ ���� ����

    int LightType;      // ���� Ÿ�� (0 : Directional, 1 : Point, 2 : Spot)
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
	int		Active;					// Ȱ��ȭ, ��Ȱ��ȭ ����
	int		Layer;					//���̾� 0, 1 ,2 
	float	Mass;					// ���� 1����

	float4	vColor;					// ����
	float4	vWorldScale;			// ũ��
	float4	vLocalPos;				// ���� ��ġ
	float4	vWorldPos;				// ���� ��ġ
	float3	vVelocity;				// �ӵ�
	float4	vWorldRotation;			// ȸ����
	float3	vRotationVelocity;		// ���ӵ�

	int		CurFrameIdx;			// ����Frame
	float	AccTime;				// ����frame�����ð�

	float4	vForce;					// ���ڿ� ����� ���� �� �ѷ�
	float3	vNoiseForce;			// NoiseForce ���� ���� ���� ��
	float	NoiseForceTime;			// NoiseForce �� ���ù��� �ð�

	float	NoiseRotTime;			// NoiseRotVel �� ���ù��� �ð�
};

struct tDreamParticleModule
{
	float	SpawnCount;			// ũ������Ʈ�� ���� ����

	// ���̾�����
	float	vSpawnRate[3];		// ���̾�1~3��������
	float4	vSpawnBoxScale[3];	// ���̾�1~3 �ڽ�ũ��
	float4	vSpawnColor[9];		// ���̾�1~3 �����÷�
	float	vSpawnScale[3];		// ���̾�1~3������
	float	vSpawnDepth[3];		// ���̾�1~3��

	//�ִԸ��
	float FrameDuration;		// �෹�̼�

	// Noise Force
	float	NoiseForceScale;
	float	NoiseForceTerm;

	// Noise Angle
	float	Angle;				// �ش� ���⿡�� �������� ����
	float	NoiseScaleTerm;

	// ����������(cpu���� ���Ǿ���°�
	// (ndc��ǥ�� scale�� �����ϴ� �뵵.)
	float NdcBoxbyDepth[3]; // 0=0.5 90=1 180 = 1.5
	float ScaleMultibyDepth[3];// 0 = 0 ,90 = 1 ,180 = 2;

	// Module On / Off
	int arrModuleCheck[2];
	int padding;
};


struct tParticle
{
    float4  vLocalPos;
    float4  vWorldPos; // ��ġ
    float4  vWorldInitScale; // �ʱ� ũ��
    float4  vWorldScale; // ũ��
    float4  vWorldRotation; // ȸ����
    float3  vVelocity; // �ӵ�
    float4  vColor; // ����
    float4  vForce; // ���� ��
    float3  vNoiseForce; // NoiseForce ���� ���� ���� ��
    float   NoiseForceTime; // NoiseForce �� ���ù��� �ð�

    float   NomalizedAge;
    float   Mass; // ����
    float   Age; // ���� ����
    float   Life; // ����
    int     Active; // Ȱ��ȭ, ��Ȱ��ȭ ����
    
    float4 ColorOrigin;
    float4 ColorVibra; // ���� ����� ������ ����
    float  ColorAcctime; // ������������ �����ð�
    
    int AtlasIdx; // �ο����� Anim
    int CurFrameIdx; // ����Frame
    float AccTime; // ����frame�����ð�
};

struct tParticleModule
{
	// Sapwn ���
    float4  vSpawnColor; // �ʱ� �÷�
    float4  vSpawnMinScale; // �ʱ� �ּ� ũ��
    float4  vSpawnMaxScale; // �ʱ� �ִ� ũ��

    float   MinLife; // �ּ� ����
    float   MaxLife; // �ִ� ����
    float   MinMass; // �ּ� ����
    float   MaxMass; // �ִ� ����
    float   SpawnRate; // �ʴ� ���� ����
    int     SpaceType; // ��ǥ��(0 : LocalSpace, 1 : WorldSpace)
    
    int     SpawnShape; // ���� ����(0 : Sphere, 1 : Box)
    float   Radius; // SpawnShape �� Sphere �� ���, ������ ����
    float4  vSpawnBoxScale; // SpawnShape �� Box �� ���, Box �� ũ��   
    
    // Add Velocity
    int     AddVelocityType; // 0 : From Center, 1: To Center, 2: Fix Direction
    float   MinSpeed;
    float   MaxSpeed;
    float   FixedAngle; // �ش� ���⿡�� �������� ����
    float4  FixedDirection; // ���� ����
    
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
    float   AlphaMaxAge;        // ���İ� 0 �� �Ǳ������ ����
    
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
