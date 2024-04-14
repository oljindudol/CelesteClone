#pragma once

#include <Engine/CScript.h>
class CPhysics;

template <typename T>
class CCustomStateMachine;

class CPlayerScript :
    public CScript
{
private:
    Ptr<CPrefab>    m_Missile;
    CCustomStateMachine<CPlayerScript>* StateMachine;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();


#pragma region Const
public:
    // TODO:
    //static ParticleType P_DashA;
    //static ParticleType P_DashB;
    //static ParticleType P_CassetteFly;
    //static ParticleType P_Split;
    //static ParticleType P_SummitLandA;
    //static ParticleType P_SummitLandB;
    //static ParticleType P_SummitLandC;

    const float MaxFall = 160.f;
    const float MaxRun = 90.f;
    const float RunAccel = 1000.f;
    const float WallSlideStartMax = 20.f;
    const float BoostTime = .25f;
    const float ClimbMaxStamina = 110;
    const float ClimbTiredThreshold = 20.f;
    const float WalkSpeed = 64.f;
    const string TalkSfx = "player_talk";

private:
    const float Gravity = 900.f;
    const float HalfGravThreshold = 40.f;

    const float FastMaxFall = 240.f;
    const float FastMaxAccel = 300.f;

    const float RunReduce = 400.f;
    const float AirMult = .65f;

    const float HoldingMaxRun = 70.f;
    const float HoldMinTime = .35f;

    const float BounceAutoJumpTime = .1f;

    const float DuckFriction = 500.f;
    const int DuckCorrectCheck = 4;
    const float DuckCorrectSlide = 50.f;

    const float DodgeSlideSpeedMult = 1.2f;
    const float DuckSuperJumpXMult = 1.25f;
    const float DuckSuperJumpYMult = .5f;

    const float JumpGraceTime = 0.1f;
    const float JumpSpeed = -105.f;
    const float JumpHBoost = 40.f;
    const float VarJumpTime = .2f;
    const float CeilingVarJumpGrace = .05f;
    const int UpwardCornerCorrection = 4;
    const float WallSpeedRetentionTime = .06f;
    const int WallJumpCheckDist = 3;
    const float WallJumpForceTime = .16f;
    const float WallJumpHSpeed = MaxRun + JumpHBoost;

    const float WallSlideTime = 1.2f;

    const float BounceVarJumpTime = .2f;
    const float BounceSpeed = -140.f;
    const float SuperBounceVarJumpTime = .2f;
    const float SuperBounceSpeed = -185.f;

    const float SuperJumpSpeed = JumpSpeed;
    const float SuperJumpH = 260.f;
    const float SuperWallJumpSpeed = -160.f;
    const float SuperWallJumpVarTime = .25f;
    const float SuperWallJumpForceTime = .2f;
    //const float SuperWallJumpH = MaxRun + JumpHBoost * 2;

    const float DashSpeed = 240.f;
    const float EndDashSpeed = 160.f;
    const float EndDashUpMult = .75f;
    const float DashTime = .15f;
    const float DashCooldown = .2f;
private:
    //여기부터 들여쓰기오류
    const float DashRefillCooldown = .1f;
    const int DashHJumpThruNudge = 6;
    const int DashCornerCorrection = 4;
    const int DashVFloorSnapDist = 3;
    const float DashAttackTime = .3f;
    const float BoostMoveSpeed = 80.f;
    const float DuckWindMult = 0.f;
    const int WindWallDistance = 3;

    const float ReboundSpeedX = 120.f;
    const float ReboundSpeedY = -120.f;
    const float ReboundVarJumpTime = .15f;

    const float ReflectBoundSpeed = 220.f;

    const float DreamDashSpeed = DashSpeed;
    const int DreamDashEndWiggle = 5;
    const float DreamDashMinTime = .1f;

    const float ClimbUpCost = 100 / 2.2f;
    const float ClimbStillCost = 100 / 10.f;
    const float ClimbJumpCost = 110 / 4.f;
    const int ClimbCheckDist = 2;
    const int ClimbUpCheckDist = 2;
    const float ClimbNoMoveTime = .1f;
    const float ClimbUpSpeed = -45.f;
    const float ClimbDownSpeed = 80.f;
    const float ClimbSlipSpeed = 30.f;
    const float ClimbAccel = 900.f;
    const float ClimbGrabYMult = .2f;
    const float ClimbHopY = -120.f;
    const float ClimbHopX = 100.f;

    const float ClimbHopForceTime = .2f;
    const float ClimbJumpBoostTime = .2f;
    const float ClimbHopNoWindTime = .3f;

    const float LaunchSpeed = 280.f;
    const float LaunchCancelThreshold = 220.f;

    const float LiftYCap = -130.f;
    const float LiftXCap = 250.f;

    const float JumpThruAssistSpeed = -40.f;

    const float InfiniteDashesTime = 2.f;
    const float InfiniteDashesFirstTime = .5f;
    const float FlyPowerFlashTime = .5f;

    const float ThrowRecoil = 80.f;
    //static Vector2 CarryOffsetTarget = Vector2(0.f, -12.f);

    const float ChaserStateMaxTime = 4.f;
#pragma endregion


#pragma region Vars
public:
    Vec4 Collider;
    Vector2 Speed;
    Facings Facing;
    // TODO:
    class CPlayerSprite*        Sprite;
    CGameObject*                m_pPlayerHairGO;
    class CHair*                m_pHairComp;
    class CPlayerHairUpdate*    m_pHairUpdate;

    //CCustomStateMachine<CPlayerScript> StateMachine;
    Vector2 CameraAnchor;
    bool CameraAnchorIgnoreX;
    bool CameraAnchorIgnoreY;
    Vector2 CameraAnchorLerp;
    bool ForceCameraUpdate;
    // TODO:
    //Leader Leader;
    // TODO:
    //VertexLight Light;
    int Dashes;
    float Stamina = ClimbMaxStamina;
    bool StrawberriesBlocked;
    Vector2 PreviousPosition;
    bool DummyAutoAnimate = true;
    Vector2 ForceStrongWindHair;
    Vector2 OverrideDashDirection;
    bool FlipInReflection = false;
    bool JustRespawned;  // True if the player hasn't moved since respawning
    bool m_Dead;
    void SetDead(bool _Dead) { m_Dead = _Dead; }
    bool GetDead() { return m_Dead; }
    Vec2 Position;

    int StrawberryCollectIndex = 0;
    float StrawberryCollectResetTimer = 0.f;

    bool AutoJump;
    float AutoJumpTimer;
    Vector2 DashDir;

    // TODO:
    //Booster CurrentBooster;
    Color OverrideHairColor = HexToColor("9B3FB5");
    const Color NormalBadelineHairColor = HexToColor("9B3FB5");

    // hair
    const Color UsedHairColor = HexToColor("44B7FF");
    const Color UsedBadelineHairColor = HexToColor("44B7FF");

    const Color TwoDashesHairColor = HexToColor("ff6def");
    const Color TwoDashesBadelineHairColor = TwoDashesHairColor;//HexToColor("ff6def");

    const Color NormalHairColor = HexToColor("AC3232");
    const Color FlyPowerHairColor = HexToColor("F2EB6D");

    const Color FlashHairColor = HexToColor("FFFFFF");
    // level-start intro
    enum IntroTypes { Transition, Respawn, WalkInRight, WalkInLeft, Jump, WakeUp, Fall, TempleMirrorVoid, None };
    IntroTypes IntroType;

    
private:
    int inputx;
    int inputy;
    //Level level;
    // TODO:
    //Collision onCollideH;
    //Collision onCollideV;
public:
    bool onGround;
    bool OnSafeGround;
    bool wasOnGround;
    int moveX;
    bool flash;
    bool wasDucking;

    float idleTimer;
    static std::unordered_map<string, float> idleColdOptions; //new Chooser<string>().Add("idleA", 5f).Add("idleB", 3f).Add("idleC", 1f);
    static std::unordered_map<string, float> idleNoBackpackOptions;// = new Chooser<string>().Add("idleA", 1f).Add("idleB", 3f).Add("idleC", 3f);
    static std::unordered_map<string, float> idleWarmOptions;// = new Chooser<string>().Add("idleA", 5f).Add("idleB", 3f);

    Vec4 hurtbox;
    float jumpGraceTimer;
    float varJumpSpeed;
    float varJumpTimer;
    int forceMoveX;
    float forceMoveXTimer;
    int hopWaitX;   // If you climb hop onto a moving solid, snap to beside it until you get above it
    float hopWaitXSpeed;
    Vector2 lastAim;
    float dashCooldownTimer;
    float dashRefillCooldownTimer;
    float wallSlideTimer = WallSlideTime;
    int wallSlideDir;
    float climbNoMoveTimer;
    Vector2 carryOffset;
    Vector2 deadOffset;
    float introEase;
    float wallSpeedRetentionTimer; // If you hit a wall, start this timer. If coast is clear within this timer, retain h-speed
    float wallSpeedRetained;
    int wallBoostDir;
    float wallBoostTimer;   // If you climb jump and then do a sideways input within this timer, switch to wall jump
    float maxFall;
    float dashAttackTimer;
    //list<ChaserState> chaserStates;
    bool wasTired;
    //HashSet<Trigger> triggersInside;
    float highestAirY;
    bool dashStartedOnGround;
    bool fastJump;
    int lastClimbMove;
    float noWindTimer;
    float dreamDashCanEndTimer;
    //Solid climbHopSolid;
    Vector2 climbHopSolidPosition;
    //SoundSource wallSlideSfx;
    //SoundSource swimSurfaceLoopSfx;
    float playFootstepOnLand;
    float minHoldTimer;
    //Booster lastBooster;
    bool calledDashEvents;
    int lastDashes;
    //Sprite sweatSprite;
    int startHairCount;
    bool launched;
    float launchedTimer;
    float dashTrailTimer;
    //list<ChaserStateSound> activeSounds = new List<ChaserStateSound>();
    //FMOD.Studio.EventInstance idleSfx;

    const Vec4 normalHitbox = Vec4(8, 11, -4, -11);
    const Vec4 duckHitbox = Vec4(8, 6, -4, -6);
    const Vec4 normalHurtbox = Vec4(8, 9, -4, -11);
    const Vec4 duckHurtbox = Vec4(8, 4, -4, -6);
    const Vec4 starFlyHitbox = Vec4(8, 8, -4, -10);
    const Vec4 starFlyHurtbox = Vec4(6, 6, -3, -9);

    const Vector2 normalLightOffset = Vector2(0, -8);
    const Vector2 duckingLightOffset = Vector2(0, -3);

    //list<CEntity> temp;

    float hairFlashTimer;
    Vector2 windDirection;
    float windTimeout;
    float windHairTimer;
    //MirrorReflection reflection;
    int MaxDashes = 2;
    void* Holding = nullptr;

#pragma endregion

private:
    bool startHairCalled = false;
    class CEngine* Celeste;

    bool  m_bAfterImageRequest = false;

    class CAfterImage* m_AfterImage;
    class CShockWave* m_ShockWave;
    class CCustomParticleSystem* m_Particle;
    float gliderBoostTimer;
    void CreateSplitParticles();
    void CreateTrail();
    void PushAfterImageEvent();
    void SetShockWaveEvent(float _TimeAccel, float _MaxDist);

#pragma region functions
public:
    void Update();
    void UpdateHair(bool applyGravity=true);
    //헤어,스웨트,파티클
    void UpdateRender();
    void UpdateSprite();
    void StartHair();
    void RefillDash() { Dashes = MaxDashes; }
    void NormalBegin();
    int NormalUpdate();
    void NormalEnd();
    void DashBegin();
    int DashUpdate();
    void DashEnd();
    void jump();
    bool StartedDashing= false;
    Vec2 beforeDashSpeed;
    bool demoDashed = false;
    void CallDashEvents();

private:
    void RefillStamina()
    {
        Stamina = ClimbMaxStamina;
    }
    bool dreamJump;
    bool DreamDashCheck()
    {
        if (DreamBlockColCnt > 0)
        {
            return true;
        }
        return false;
    }

public:
    int  GetCurState();
    int  DreamBlockColCnt = 0;
    void DreamDashBegin();
    int DreamDashUpdate();
    void DreamDashEnd();

    //Vec2 LiftBoost()
    //{
    //    Vector2 val = LiftSpeed;
    //    if (Math.Abs(val.X) > LiftXCap)
    //        val.X = LiftXCap * Math.Sign(val.X);
    //    if (val.Y > 0)
    //        val.Y = 0;
    //    else if (val.Y < LiftYCap)
    //        val.Y = LiftYCap;
    //    return val;
    //}

    int DashCoroutineCnt;
    float DashCoroutineTimer;
    bool CanDash()
    {
        return KEY_PRESSED(F) && dashCooldownTimer <= 0 && Dashes > 0;
            //&& (TalkComponent.PlayerOver == null || !Input.Talk.Pressed);
    }

private:
        bool wasDashB;
public:
    int StartDash()
    {
        wasDashB = Dashes == 2;
        Dashes = max(0, Dashes - 1);
        //Input.Dash.ConsumeBuffer();
        return StDash;
    }

#pragma endregion


#pragma region getters&setters
public:

    bool InControl();
    void SetMaxDashes(int _maxdashes) {MaxDashes = _maxdashes;}
    int GetMaxDashes() { return MaxDashes; }

    bool Ducking()
    {
        return Collider == duckHitbox || Collider == duckHurtbox;
    }

    void SetDucking(bool value)
    {
        if (value)
        {
            Collider = duckHitbox;
            hurtbox = duckHurtbox;

        }
        else
        {
            Collider = normalHitbox;
            hurtbox = normalHurtbox;
        }
    }


    float CheckStamina()
    {
        if (wallBoostTimer > 0)
            return Stamina + ClimbJumpCost;
        else
            return Stamina;
    }

    bool IsTired()
    {
        return CheckStamina() < ClimbTiredThreshold;
    }

    bool CanUnDuck()
    {
        if (!Ducking())
            return true;

        Vec4 Colliderwas = Collider;
        Collider = normalHitbox;
        bool ret = true;//!CollideCheck<Solid>();
        Collider = Colliderwas;
        return ret;
    }

    void CreateWallSlideParticles(int _val) {  };
    bool LaunchedBoostCheck() {
        launched = false;
        return false;
        //if (LiftBoost.LengthSquared() >= LaunchedBoostCheckSpeedSq && Speed.LengthSquared() >= LaunchedJumpCheckSpeedSq)
        //{
        //    launched = true;
        //    return true;
        //}
        //else
        //{
        //    launched = false;
        //    return false;
        //}
    }

    void SetGround();
    void UnSetGround();

#pragma endregion

#pragma region DebugVariables

//Visibility
private:
    bool m_bDashParticleVisible = false;
    bool m_bAfterImageVisible = false;
    bool m_bDistortionVisible = false;

#pragma endregion





};

