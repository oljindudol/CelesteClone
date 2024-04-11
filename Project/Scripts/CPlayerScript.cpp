#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

//#include "CPhysics.h"
#include "CCustomStateMachine.h"
#include <math.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CAnim.h>
#include "CPlayerSprite.h"
#include "CPlayerHairUpdate.h"
#include <Engine\CHair.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include "CPlayerHairUpdate.h"
#include "Engine\CTaskMgr.h"
#include <Engine\CEngine.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CAfterImage.h>
#include <Engine\CShockWave.h>
#include <Engine\CCustomParticleSystem.h>
#include <Engine\CAnimatedParticleSystem.h>

//key bind
#define GRABKEY KEY_PRESSED(S)
#define JUMPKEY KEY_PRESSED(LALT)
#define LEFTKEY KEY_PRESSED(LEFT)
#define RIGHTKEY KEY_PRESSED(RIGHT)

#define UPKEY KEY_PRESSED(UP)
#define DOWNKEY KEY_PRESSED(DOWN)
#define null nullptr




void CPlayerScript::CreateSplitParticles()
{
}


#pragma region Constructor & Begin

CPlayerScript::CPlayerScript()
    : CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
{
    //Depth = Depths.Player;
    //Tag = Tags.Persistent;

    // sweat sprite
    //sweatSprite = GFX.SpriteBank.Create("player_sweat");
    //Add(sweatSprite);
    Celeste = CEngine::GetInst();

    // physics
    Collider = normalHitbox;
    hurtbox = normalHurtbox;
    // physics델리게이트 함수
    //onCollideH = OnCollideH;
    //onCollideV = OnCollideV;

    StateMachine = new CCustomStateMachine<CPlayerScript>(this, (int)PLAYER_STATE::StEND);
    StateMachine->SetCallbacks(StNormal, ToString(magic_enum::enum_name(StNormal)), &CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd, nullptr);
    StateMachine->SetCallbacks(StDash, ToString(magic_enum::enum_name(StDash)), &CPlayerScript::DashUpdate, &CPlayerScript::DashBegin, &CPlayerScript::DashEnd, nullptr);

    // other stuff
    // Leader = 아이템따라오는것 , wind,light 
    //Add(Leader = new Leader(new Vector2(0, -8)));
    lastAim = Vector2(1, 0);
    Facing = FacingsRight;
    //chaserStates = new List<ChaserState>();
    //triggersInside = new HashSet<Trigger>();
    //Add(Light = new VertexLight(normalLightOffset, Color.White, 1f, 32, 64));
    //Add(new WaterInteraction(() = > { return StateMachine.State == StDash || StateMachine.State == StReflectionFall; }));

    //Wind
    //Add(new WindMover(WindMove));

    //Add(wallSlideSfx = new SoundSource());
    //Add(swimSurfaceLoopSfx = new SoundSource());

    //Sprite.OnFrameChange = (anim) = >
    //{
    //	if (Scene != null && !Dead)
    //	{
    //		// footsteps
    //		var frame = Sprite.CurrentAnimationFrame;
    //		if ((anim.Equals(PlayerSprite.RunCarry) && (frame == 0 || frame == 6)) ||
    //			(anim.Equals(PlayerSprite.RunFast) && (frame == 0 || frame == 6)) ||
    //			(anim.Equals(PlayerSprite.RunSlow) && (frame == 0 || frame == 6)) ||
    //			(anim.Equals(PlayerSprite.Walk) && (frame == 0 || frame == 6)) ||
    //			(anim.Equals(PlayerSprite.RunStumble) && frame == 6) ||
    //			(anim.Equals(PlayerSprite.Flip) && frame == 4) ||
    //			(anim.Equals(PlayerSprite.RunWind) && (frame == 0 || frame == 6)) ||
    //			(anim.Equals("idleC") && Sprite.Mode == PlayerSpriteMode.MadelineNoBackpack && (frame == 3 || frame == 6 || frame == 8 || frame == 11)) ||
    //			(anim.Equals("carryTheoWalk") && (frame == 0 || frame == 6)))
    //		{
    //			var landed = SurfaceIndex.GetPlatformByPriority(CollideAll<Platform>(Position + Vector2.UnitY, temp));
    //			if (landed != null)
    //				Play(Sfxs.char_mad_footstep, SurfaceIndex.Param, landed.GetStepSoundIndex(this));
    //		}
    //		// climbing (holds)
    //		else if ((anim.Equals(PlayerSprite.ClimbUp) && (frame == 5)) ||
    //			(anim.Equals(PlayerSprite.ClimbDown) && (frame == 5)))
    //		{
    //			var holding = SurfaceIndex.GetPlatformByPriority(CollideAll<Solid>(Center + Vector2.UnitX * (int)Facing, temp));
    //			if (holding != null)
    //				Play(Sfxs.char_mad_handhold, SurfaceIndex.Param, holding.GetWallSoundIndex(this, (int)Facing));
    //		}
    //		else if (anim.Equals("wakeUp") && frame == 19)
    //			Play(Sfxs.char_mad_campfire_stand);
    //		else if (anim.Equals("sitDown") && frame == 12)
    //			Play(Sfxs.char_mad_summit_sit);
    //		else if (anim.Equals("push") && (frame == 8 || frame == 15))
    //			Dust.BurstFG(Position + new Vector2(-(int)Facing * 5, -1), new Vector2(-(int)Facing, -0.5f).Angle(), 1, 0);
    //	}
    //};

    //Sprite.OnLastFrame = (anim) = >
    //{
    //	if (Scene != null && !Dead && Sprite.CurrentAnimationID == "idle" && !level.InCutscene && idleTimer > 3f)
    //	{
    //		if (Calc.Random.Chance(0.2f))
    //		{
    //			var next = "";
    //			if (Sprite.Mode == PlayerSpriteMode.Madeline)
    //				next = (level.CoreMode == Session.CoreModes.Hot ? idleWarmOptions : idleColdOptions).Choose();
    //			else
    //				next = idleNoBackpackOptions.Choose();

    //			if (!string.IsNullOrEmpty(next))
    //			{
    //				Sprite.Play(next);

    //				if (Sprite.Mode == PlayerSpriteMode.Madeline)
    //				{
    //					if (next == "idleB")
    //						idleSfx = Play(Sfxs.char_mad_idle_scratch);
    //					else if (next == "idleC")
    //						idleSfx = Play(Sfxs.char_mad_idle_sneeze);
    //				}
    //				else if (next == "idleA")
    //					idleSfx = Play(Sfxs.char_mad_idle_crackknuckles);
    //			}
    //		}
    //	}
    //};

    // cancel special idle sounds if the anim changed
    //Sprite.OnChange = (last, next) = >
    //{
    //	if ((last == "idleB" || last == "idleC") && next != null && !next.StartsWith("idle") && idleSfx != null)
    //		Audio.Stop(idleSfx);
    //};

    //Add(reflection = new MirrorReflection());




}
CPlayerScript::~CPlayerScript()
{
    if (nullptr != StateMachine)
    {
        delete StateMachine;
    }
    if (nullptr != Sprite)
    {
        delete Sprite;
    }
    if (nullptr != m_pHairUpdate)
    {
        delete m_pHairUpdate;
    }
}
void CPlayerScript::begin()
{
    //Hair
    Sprite = new CPlayerSprite(this, PlayerSpriteMode::Madeline);
    startHairCount = Sprite->HairCount;

    m_pPlayerHairGO = new CGameObject;
    m_pPlayerHairGO->SetName(L"PlayerHair");
    //hair component
    m_pHairComp = new CHair;
    m_pPlayerHairGO->AddComponent(m_pHairComp);
    //Tranform
    m_pPlayerHairGO->AddComponent(new CTransform);
    m_pPlayerHairGO->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    m_pPlayerHairGO->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
    //레벨에넣고 Player Object에 자식으로 넣는다
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_pPlayerHairGO, LAYER_PLAYER_EFFECT, false);
    GetOwner()->AddChild(m_pPlayerHairGO);
    //Add(Hair = new PlayerHair(Sprite));
    //Add(Sprite);
    m_pHairUpdate = new CPlayerHairUpdate(Sprite, m_pHairComp);
    m_pHairUpdate->Color = NormalHairColor;

    auto m_pPlayerAfterImageGO = new CGameObject;
    m_pPlayerAfterImageGO->SetName(L"PlayerAfterImage");
    //After Image
    m_AfterImage = new CAfterImage;
    m_pPlayerAfterImageGO->AddComponent(m_AfterImage);
    //Tranform
    m_pPlayerAfterImageGO->AddComponent(new CTransform);
    m_pPlayerAfterImageGO->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
    m_pPlayerAfterImageGO->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
    //레벨에넣고 Player Object에 자식으로 넣는다
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_pPlayerAfterImageGO, LAYER_PLAYER_EFFECT, false);
    GetOwner()->AddChild(m_pPlayerAfterImageGO);


    // PostProcess 오브젝트 추가
    auto m_ShockObj = new CGameObject;
    m_ShockObj->SetName(L"PlayerShockWave");
    m_ShockObj->AddComponent(new CTransform);
    m_ShockWave = new CShockWave;
    m_ShockObj->AddComponent(m_ShockWave);
    m_ShockObj->Transform()->SetAbsolute(false);
    m_ShockObj->Transform()->SetRelativePos(Vec3(0.f, .375f, 0.f));
    m_ShockObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_ShockObj, LAYER_PLAYER_EFFECT, false);
    GetOwner()->AddChild(m_ShockObj);


    // Particle 오브젝트 추가
    auto m_ParticleObj = new CGameObject;
    m_ParticleObj->SetName(L"PlayerParticle");
    m_ParticleObj->AddComponent(new CTransform);
    m_ParticleObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));
    m_Particle = new CCustomParticleSystem;
    m_ParticleObj->AddComponent(m_Particle);
    CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(m_ParticleObj, LAYER_PLAYER_EFFECT, false);
    GetOwner()->AddChild(m_ParticleObj);



    //플레이어 애님들 생성(낱장,폴더)
    std::filesystem::path base_path = CPathMgr::GetContentPath();
    wstring OriginPath = base_path;
    OriginPath += STR_FOLDER_PATH_PLAYERANIMATION;
    auto folders = getFoldersFromDirectory(OriginPath);
    for (auto& f : folders)
    {
        auto rel = std::filesystem::relative(f, OriginPath);

        Animator2D()->CreateFromFolder(rel, f, 0.08f, Vec2(0.f, -0.375f));
    }
    GetRenderComponent()->GetDynamicMaterial();
    auto a = Animator2D();
    Animator2D()->Play(L"idle");

    //피직스
    //SetPhysics(new CPhysics(GetOwner()));

    //스머
    StateMachine->Begin();

    //Imgui 자식오브젝트 업데이트용
    CTaskMgr::GetInst()->TriggetObjectEvent();
}
void CPlayerScript::StartHair()
{
    if (startHairCalled)
    {
        return;
    }
    startHairCalled = true;
    m_pHairUpdate->facing = Facing;
    m_pHairUpdate->Start();
    UpdateHair(true);
}

#pragma endregion


#pragma region tick & Update

void CPlayerScript::tick()
{
    Vec3 vPos = Transform()->GetRelativePos();
    Vec3 vRot = Transform()->GetRelativeRotation();



    Transform()->SetRelativePos(vPos);
    Transform()->SetRelativeRotation(vRot);

    //if (KEY_TAP(KEY::SPACE))
    //{
    //    Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
    //    //GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
    //    GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);
    //}

    //if (KEY_PRESSED(KEY::SPACE))
    //{
    //    Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
    //    if (nullptr != pMtrl)
    //    {
    //        pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
    //    }
    //}
    //else if (KEY_RELEASED(KEY::SPACE))
    //{
    //    Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
    //    if (nullptr != pMtrl)
    //    {
    //        pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
    //    }
    //}
    //GetPhysics()->Update();

    StateMachine->Update();
    Update();

    UpdateHair(true);
    m_pHairUpdate->Update();
    m_pHairUpdate->AfterUpdate();
    PushAfterImageEvent();

    //DEBUG STATE
    static bool DEBUG_STATE = false;
    if (KEY_TAP(KEY::SPACE))
    {
        DEBUG_STATE = !DEBUG_STATE;
        if (DEBUG_STATE)
        {
            onGround = true;
            Transform()->SetRelativePos(Vec3(0, 0, Transform()->GetRelativePos().z));
            Speed = Vec2(0, 0);
        }
        else
        {
            onGround = false;
        }
    }

    //static float f = 0.f;
    //f += DT * 0.3f;
    //GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

    //GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
    //GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);
}
void CPlayerScript::Update()
{
    inputx = 0;
    inputy = 0;
    Vec3 V3Pos = Transform()->GetRelativePos();
    Position = Vec2(V3Pos.x, V3Pos.y);
    float X = Position.x;
    float Y = Position.y;

    //Infinite Stamina variant
    //if (SaveData.Instance.AssistMode && SaveData.Instance.Assists.InfiniteStamina)
    //    Stamina = ClimbMaxStamina;

    PreviousPosition = Position;

    //Vars       
    {
        // strawb reset timer
        StrawberryCollectResetTimer -= DT;
        if (StrawberryCollectResetTimer <= 0)
            StrawberryCollectIndex = 0;

        // idle timer
        //idleTimer += DT;
        //if (level != null && level.InCutscene)
        //    idleTimer = -5;
        //else if (Speed.X != 0 || Speed.Y != 0)
        //    idleTimer = 0;

        //Underwater music
        //if (!Dead)
        //    Audio.MusicUnderwater = UnderwaterMusicCheck();

        //Just respawned
        if (JustRespawned && Speed != Vec2(0, 0))
            JustRespawned = false;

        //Get ground
        //if (StateMachine.State == StDreamDash)
        //    onGround = OnSafeGround = false;
        //else if (Speed.Y >= 0)
        //{
        //    Platform first = CollideFirst<Solid>(Position + Vector2.UnitY);
        //    if (first == null)
        //        first = CollideFirstOutside<JumpThru>(Position + Vector2.UnitY);

        //    if (first != null)
        //    {
        //        onGround = true;
        //        OnSafeGround = first.Safe;
        //    }
        //    else
        //        onGround = OnSafeGround = false;
        //}
        //else
        //    onGround = OnSafeGround = false;

        //if (StateMachine.State == StSwim)
        //    OnSafeGround = true;

        //Safe Ground Blocked?
        //if (OnSafeGround)
        //{
        //    foreach(SafeGroundBlocker blocker in Scene.Tracker.GetComponents<SafeGroundBlocker>())
        //    {
        //        if (blocker.Check(this))
        //        {
        //            OnSafeGround = false;
        //            break;
        //        }
        //    }
        //}

        playFootstepOnLand -= DT;

        //Highest Air Y
        if (onGround)
            highestAirY = Y;
        else
            highestAirY = min(Y, highestAirY);

        //Flashing
        //if (Scene.OnInterval(.05f))
        //    flash = !flash;

        //Wall Slide
        if (wallSlideDir != 0)
        {
            wallSlideTimer = max(wallSlideTimer - DT, 0);
            wallSlideDir = 0;
        }

        //Wall Boost
        if (wallBoostTimer > 0)
        {
            wallBoostTimer -= DT;
            if (moveX == wallBoostDir)
            {
                Speed.x = WallJumpHSpeed * moveX;
                Stamina += ClimbJumpCost;
                wallBoostTimer = 0;
                //sweatSprite.Play("idle");
            }
        }

        //After Dash
        if (onGround && (int)StateMachine->GetCurState() != StClimb)
        {
            AutoJump = false;
            Stamina = ClimbMaxStamina;
            wallSlideTimer = WallSlideTime;
        }

        //Dash Attack
        if (dashAttackTimer > 0)
            dashAttackTimer -= DT;

        //Jump Grace
        if (onGround)
        {
            //dreamJump = false;
            jumpGraceTimer = JumpGraceTime;
        }
        else if (jumpGraceTimer > 0)
            jumpGraceTimer -= DT;

        //refill Dashes
        {
            if (dashCooldownTimer > 0)
                dashCooldownTimer -= DT;

            if (dashRefillCooldownTimer > 0)
                dashRefillCooldownTimer -= DT;
            else if (onGround) //if (SaveData.Instance.AssistMode && SaveData.Instance.Assists.DashMode == Assists.DashModes.Infinite && !level.InCutscene)
                RefillDash();
            //else if (!Inventory.NoRefills)
            //{
            //    if (StateMachine.State == StSwim)
            //        RefillDash();
            //    else if (onGround)
            //        if (CollideCheck<Solid, NegaBlock>(Position + Vector2.UnitY) || CollideCheckOutside<JumpThru>(Position + Vector2.UnitY))
            //            if (!CollideCheck<Spikes>(Position) || (SaveData.Instance.AssistMode && SaveData.Instance.Assists.Invincible))
            //                RefillDash();
            //}
        }

        //Var Jump
        if (varJumpTimer > 0)
            varJumpTimer -= DT;

        //Auto Jump
        if (AutoJumpTimer > 0)
        {
            if (AutoJump)
            {
                AutoJumpTimer -= DT;
                if (AutoJumpTimer <= 0)
                    AutoJump = false;
            }
            else
                AutoJumpTimer = 0;
        }

        //Force Move X
        if (forceMoveXTimer > 0)
        {
            forceMoveXTimer -= DT;
            moveX = forceMoveX;
        }
        else
        {

            if (LEFTKEY)
            {
                inputx = -1;
            }
            else if (RIGHTKEY)
            {
                inputx = 1;
            }

            if (UPKEY)
            {
                inputy = -1;
            }
            else if (DOWNKEY)
            {
                inputy = 1;
            }

            moveX = inputx;


            //climbHopSolid = null;
        }

        //Climb Hop Solid Movement
        //if (climbHopSolid != null && !climbHopSolid.Collidable)
        //    climbHopSolid = null;
        //else if (climbHopSolid != null && climbHopSolid.Position != climbHopSolidPosition)
        //{
        //    var move = climbHopSolid.Position - climbHopSolidPosition;
        //    climbHopSolidPosition = climbHopSolid.Position;
        //    MoveHExact((int)move.X);
        //    MoveVExact((int)move.Y);
        //}

        //Wind
        if (noWindTimer > 0)
            noWindTimer -= DT;

        //Facing
        //if (moveX != 0 && InControl()
        //    && StateMachine->GetCurState() != StClimb)// && StateMachine.State != StPickup && StateMachine.State != StRedDash && StateMachine.State != StHitSquash)
        //{
        //    auto to = (Facings)moveX;
        //    if (to != Facing && Ducking)
        //        CAnimator2D().GetCurAnim()-> // = new Vector2(0.8f, 1.2f);
        //    Facing = to;
        //}

        //Aiming
        if (0 == inputx && 0 == inputy)
        {
            lastAim = Vec2(Facing, 0);
        }
        else
        {
            lastAim = Vec2(inputx, inputy);
            lastAim.Normalize();
        }


        //Wall Speed Retention
        //if (wallSpeedRetentionTimer > 0)
        //{
        //    if (math.Sign(Speed.x) == -Math.Sign(wallSpeedRetained))
        //        wallSpeedRetentionTimer = 0;
        //    else if (!CollideCheck<Solid>(Position + Vector2.UnitX * Math.Sign(wallSpeedRetained)))
        //    {
        //        Speed.X = wallSpeedRetained;
        //        wallSpeedRetentionTimer = 0;
        //    }
        //    else
        //        wallSpeedRetentionTimer -= DT;
        //}

        //Hop Wait X
        //if (hopWaitX != 0)
        //{
        //    if (Math.Sign(Speed.X) == -hopWaitX || Speed.Y > 0)
        //        hopWaitX = 0;
        //    else if (!CollideCheck<Solid>(Position + Vector2.UnitX * hopWaitX))
        //    {
        //        Speed.X = hopWaitXSpeed;
        //        hopWaitX = 0;
        //    }
        //}

        // Wind Timeout
        //if (windTimeout > 0)
        //    windTimeout -= DT;

        //Hair
        {
            Vec2 windDir = windDirection;
            if (ForceStrongWindHair.Length() > 0)
                windDir = ForceStrongWindHair;

            if (windTimeout > 0 && windDir.x != 0)
            {
                windHairTimer += DT * 8.f;

                m_pHairUpdate->StepPerSegment = Vec2(windDir.x * 5.f, (float)sinf(windHairTimer));
                m_pHairUpdate->StepInFacingPerSegment = 0.f;
                m_pHairUpdate->StepApproach = 128.f;
                m_pHairUpdate->StepYSinePerSegment = 0;
            }
            else if (Dashes > 1)
            {
                m_pHairUpdate->StepPerSegment = Vec2((float)sinf(DT * 4 * 2) * 0.7f - (int)Facing * 3, (float)sinf(DT * 4 * 1.f));
                m_pHairUpdate->StepInFacingPerSegment = 0.f;
                m_pHairUpdate->StepApproach = 90.f;
                m_pHairUpdate->StepYSinePerSegment = 1.f;

                m_pHairUpdate->StepPerSegment.y += windDir.y * 2.f;
            }
            else
            {
                m_pHairUpdate->StepPerSegment = Vec2(0.f, 2.f);
                m_pHairUpdate->StepInFacingPerSegment = 0.5f;
                m_pHairUpdate->StepApproach = 64.f;
                m_pHairUpdate->StepYSinePerSegment = 0;

                m_pHairUpdate->StepPerSegment.y += windDir.y * 0.5f;
            }
        }

        //if (StateMachine->GetCurState() == StRedDash)
        //    Sprite.HairCount = 1;
        //else if (StateMachine.State != StStarFly)
        //    Sprite.HairCount = (Dashes > 1 ? 5 : startHairCount);

        //Min Hold Time
        if (minHoldTimer > 0)
            minHoldTimer -= DT;

        //Launch Particles
        //if (launched)
        //{
        //    var sq = Speed.LengthSquared();
        //    if (sq < LaunchedMinSpeedSq)
        //        launched = false;
        //    else
        //    {
        //        var was = launchedTimer;
        //        launchedTimer += DT;

        //        if (launchedTimer >= .5f)
        //        {
        //            launched = false;
        //            launchedTimer = 0;
        //        }
        //        else if (Calc.OnInterval(launchedTimer, was, .15f))
        //            level.Add(Engine.Pooler.Create<SpeedRing>().Init(Center, Speed.Angle(), Color.White));
        //    }
        //}
        //else
        //    launchedTimer = 0;
    }

    if (IsTired())
    {
        //Input.Rumble(RumbleStrength.Light, RumbleLength.Short);s
        if (!wasTired)
        {
            wasTired = true;
        }
    }
    else
        wasTired = false;

    //base.Update();

    //Light Offset
    //if (Ducking)
    //    Light.Position = duckingLightOffset;
    //else
    //    Light.Position = normalLightOffset;

    //Jump Thru Assist
    //if (!onGround && Speed.y <= 0 && (StateMachine->GetCurState() != StClimb || lastClimbMove == -1) && CollideCheck<JumpThru>() && !JumpThruBoostBlockedCheck())
    //    MoveV(JumpThruAssistSpeed * DT);

    //Dash Floor Snapping
    //if (!onGround && DashAttacking && DashDir.y == 0)
    //{
    //    if (CollideCheck<Solid>(Position + Vector2.UnitY * DashVFloorSnapDist) || CollideCheckOutside<JumpThru>(Position + Vector2.UnitY * DashVFloorSnapDist))
    //        MoveVExact(DashVFloorSnapDist);
    //}

    //Falling unducking
    if (Speed.y > 0 && CanUnDuck() && Collider != starFlyHitbox && !onGround)
        SetDucking(false);

    //Physics
    float newx = Position.x + Speed.x * DT;
    float newy = Position.y - Speed.y * DT;
    if (StateMachine->GetCurState() != StDreamDash && StateMachine->GetCurState() != StAttract)
        Transform()->SetRelativePos(Vec3(newx, newy, V3Pos.z));
    //    MoveH(Speed.x * DT, onCollideH);
    if (StateMachine->GetCurState() != StDreamDash && StateMachine->GetCurState() != StAttract)
        Transform()->SetRelativePos(Vec3(newx, newy, V3Pos.z));
    if (0 != inputx)
    {
        GetOwner()->m_facing = (Facings)inputx;
    }
    //    MoveV(Speed.y * DT, onCollideV);

    //Swimming
    //if (StateMachine->GetCurState() == StSwim)
    //{
    //    //Stay at water surface
    //    if (Speed.y < 0 && Speed.y >= SwimMaxRise)
    //    {
    //        while (!SwimCheck())
    //        {
    //            Speed.y = 0;
    //            if (MoveVExact(1))
    //                break;
    //        }
    //    }
    //}
    //else if (StateMachine->GetCurState() == StNormal && SwimCheck())
    //    StateMachine->GetCurState() = StSwim;
    //else if (StateMachine->GetCurState() == StClimb && SwimCheck())
    //{
    //    var water = CollideFirst<Water>(Position);
    //    if (water != null && Center.Y < water.Center.Y)
    //    {
    //        while (SwimCheck())
    //            if (MoveVExact(-1))
    //                break;
    //        if (SwimCheck())
    //            StateMachine->GetCurState() = StSwim;
    //    }
    //    else
    //        StateMachine->GetCurState() = StSwim;
    //}

    // wall slide SFX
    //{
    //    var isSliding = Sprite.CurrentAnimationID != null && Sprite.CurrentAnimationID.Equals(PlayerSprite.WallSlide) && Speed.y > 0;
    //    if (isSliding)
    //    {
    //        if (!wallSlideSfx.Playing)
    //            Loop(wallSlideSfx, Sfxs.char_mad_wallslide);

    //        var platform = SurfaceIndex.GetPlatformByPriority(CollideAll<Solid>(Center + Vector2.UnitX * (int)Facing, temp));
    //        if (platform != null)
    //            wallSlideSfx.Param(SurfaceIndex.Param, platform.GetWallSoundIndex(this, (int)Facing));
    //    }
    //    else
    //        Stop(wallSlideSfx);
    //}

    // update sprite
    UpdateSprite();

    //Carry held item
    //UpdateCarry();

    //Triggers
    //if (StateMachine->GetCurState() != StReflectionFall)
    //{
    //    foreach(Trigger trigger in Scene.Tracker.GetEntities<Trigger>())
    //    {
    //        if (CollideCheck(trigger))
    //        {
    //            if (!trigger.Triggered)
    //            {
    //                trigger.Triggered = true;
    //                triggersInside.Add(trigger);
    //                trigger.OnEnter(this);
    //            }
    //            trigger.OnStay(this);
    //        }
    //        else if (trigger.Triggered)
    //        {
    //            triggersInside.Remove(trigger);
    //            trigger.Triggered = false;
    //            trigger.OnLeave(this);
    //        }
    //    }
    //}

    //Strawberry Block
    //StrawberriesBlocked = CollideCheck<BlockField>();

    // Camera (lerp by distance using delta-time)
    //if (InControl || ForceCameraUpdate)
    //{
    //    if (StateMachine->GetCurState() == StReflectionFall)
    //    {
    //        level.Camera.Position = CameraTarget;
    //    }
    //    else
    //    {
    //        var from = level.Camera.Position;
    //        var target = CameraTarget;
    //        var multiplier = StateMachine->GetCurState() == StTempleFall ? 8 : 1f;

    //        level.Camera.Position = from + (target - from) * (1f - (float)Math.Pow(0.01f / multiplier, DT));
    //    }
    //}

    //Player Colliders
    //if (!Dead && StateMachine->GetCurState() != StCassetteFly)
    //{
    //    Collider was = Collider;
    //    Collider = hurtbox;

    //    foreach(PlayerCollider pc in Scene.Tracker.GetComponents<PlayerCollider>())
    //    {
    //        if (pc.Check(this) && Dead)
    //        {
    //            Collider = was;
    //            return;
    //        }
    //    }

    //    // If the current collider is not the hurtbox we set it to, that means a collision callback changed it. Keep the new one!
    //    bool keepNew = (Collider != hurtbox);

    //    if (!keepNew)
    //        Collider = was;
    //}

    //Bounds
    //if (InControl && !Dead && StateMachine->GetCurState() != StDreamDash)
    //    level.EnforceBounds(this);

    //UpdateChaserStates();
    //UpdateHair(true);

    //Sounds on ducking state change
    //if (wasDucking != Ducking)
    //{
    //    wasDucking = Ducking;
    //    if (wasDucking)
    //        Play(Sfxs.char_mad_duck);
    //    else if (onGround)
    //        Play(Sfxs.char_mad_stand);
    //}

    // shallow swim sfx
    //if (Speed.x != 0 && ((StateMachine->GetCurState() == StSwim && !SwimUnderwaterCheck()) || (StateMachine->GetCurState() == StNormal && CollideCheck<Water>(Position))))
    //{
    //    if (!swimSurfaceLoopSfx.Playing)
    //        swimSurfaceLoopSfx.Play(Sfxs.char_mad_water_move_shallow);
    //}
    //else
    //    swimSurfaceLoopSfx.Stop();

    wasOnGround = onGround;
}
void CPlayerScript::UpdateSprite()
{
    Facing = GetOwner()->m_facing;
    auto SpriteScale = Animator2D()->GetMulScale();

    //Tween
    SpriteScale.x = Approach(SpriteScale.x, 1.f, 1.75f * DT);
    SpriteScale.y = Approach(SpriteScale.y, 1.f, 1.75f * DT);

    Animator2D()->SetMulScale(SpriteScale);

    //Animation
    if (InControl() && Sprite->CurrentAnimationID != Sprite->Throw && StateMachine->GetCurState() != StTempleFall &&
        StateMachine->GetCurState() != StReflectionFall && StateMachine->GetCurState() != StStarFly && StateMachine->GetCurState() != StCassetteFly)
    {
        if (StateMachine->GetCurState() == StAttract)
        {
            Sprite->Play(Sprite->FallFast);
        }
        else if (StateMachine->GetCurState() == StSummitLaunch)
        {
            Sprite->Play(Sprite->Launch);
        }
        // picking up
        else if (StateMachine->GetCurState() == StPickup)
        {
            Sprite->Play(Sprite->PickUp);
        }
        // swiming
        else if (StateMachine->GetCurState() == StSwim)
        {
            if (inputy > 0)
                Sprite->Play(Sprite->SwimDown);
            else if (inputy < 0)
                Sprite->Play(Sprite->SwimUp);
            else
                Sprite->Play(Sprite->SwimIdle);
        }
        // dream dashing
        else if (StateMachine->GetCurState() == StDreamDash)
        {
            if (Sprite->CurrentAnimationID != Sprite->DreamDashIn && Sprite->CurrentAnimationID != Sprite->DreamDashLoop)
                Sprite->Play(Sprite->DreamDashIn);
        }
        else if (Sprite->DreamDashing() && Sprite->LastAnimationID != Sprite->DreamDashOut)
        {
            Sprite->Play(Sprite->DreamDashOut);
        }
        else if (Sprite->CurrentAnimationID != Sprite->DreamDashOut)
        {
            //// during dash
            //if (DashAttacking())
            //{
            //    if (onGround && DashDir.Y == 0 && !Ducking && Speed.X != 0 && moveX == -Math.Sign(Speed.X))
            //    {
            //        if (Scene.OnInterval(.02f))
            //            Dust.Burst(Position, Calc.Up, 1);
            //        Sprite->Play(Sprite->Skid);
            //    }
            //    else
            //        Sprite->Play(Sprite->Dash);
            //}
            // climbing
            if (StateMachine->GetCurState() == StClimb)
            {
                if (lastClimbMove < 0)
                    Sprite->Play(Sprite->ClimbUp);
                else if (lastClimbMove > 0)
                    Sprite->Play(Sprite->WallSlide);
                //else if (!CollideCheck<Solid>(Position + new Vector2((int)Facing, 6)))
                //    Sprite->Play(Sprite->Dangling);
                else if (inputx == -(int)Facing)
                {
                    if (Sprite->CurrentAnimationID != Sprite->ClimbLookBack)
                        Sprite->Play(Sprite->ClimbLookBackStart);
                }
                else
                    Sprite->Play(Sprite->WallSlide);
            }
            // ducking
            else if (Ducking() && StateMachine->GetCurState() == StNormal)
            {
                Sprite->Play(Sprite->Duck);
            }
            else if (onGround)
            {
                fastJump = false;
                //if (Holding == null && moveX != 0 && CollideCheck<Solid>(Position + Vector2.UnitX * moveX))
                //{
                //    Sprite->Play("push");
                //}
                if (abs(Speed.x) <= RunAccel / 40.f && moveX == 0)
                {
                    if (Holding != null)
                    {
                        Sprite->Play(Sprite->IdleCarry);
                    }
                    //else if (!Scene.CollideCheck<Solid>(Position + new Vector2((int)Facing * 1, 2)) && !Scene.CollideCheck<Solid>(Position + new Vector2((int)Facing * 4, 2)) && !CollideCheck<JumpThru>(Position + new Vector2((int)Facing * 4, 2)))
                    //{
                    //    Sprite->Play(Sprite->FrontEdge);
                    //}
                    //else if (!Scene.CollideCheck<Solid>(Position + new Vector2(-(int)Facing * 1, 2)) && !Scene.CollideCheck<Solid>(Position + new Vector2(-(int)Facing * 4, 2)) && !CollideCheck<JumpThru>(Position + new Vector2(-(int)Facing * 4, 2)))
                    //{
                    //    Sprite->Play("edgeBack");
                    //}
                    //else if (Input.MoveY.Value == -1)
                    //{
                    //    if (Sprite->LastAnimationID != Sprite->LookUp)
                    //        Sprite->Play(Sprite->LookUp);
                    //}
                    else
                    {
                        if (Sprite->CurrentAnimationID != "" && !(string::npos != Sprite->CurrentAnimationID.find("idle")))
                            Sprite->Play(Sprite->Idle);
                    }
                }
                else if (Holding != null)
                {
                    Sprite->Play(Sprite->RunCarry);
                }
                else if (Sign(Speed.x) == -moveX && moveX != 0)
                {
                    if (abs(Speed.x) > MaxRun)
                        Sprite->Play(Sprite->Skid);
                    else if (Sprite->CurrentAnimationID != Sprite->Skid)
                        Sprite->Play(Sprite->Flip);
                }
                else if (windDirection.x != 0 && windTimeout > 0.f && (int)Facing == -Sign(windDirection.x))
                {
                    Sprite->Play(Sprite->RunWind);
                }
                else if (!Sprite->Running())
                {
                    if (abs(Speed.x) < MaxRun * .5f)
                        Sprite->Play(Sprite->RunSlow);
                    else
                        Sprite->Play(Sprite->RunFast);
                }
            }
            // wall sliding
            else if (wallSlideDir != 0 && Holding == nullptr)
            {
                Sprite->Play(Sprite->WallSlide);
            }
            // jumping up
            else if (Speed.y < 0)
            {
                if (Holding != nullptr)
                {
                    Sprite->Play(Sprite->JumpCarry);
                }
                else if (fastJump || abs(Speed.x) > MaxRun)
                {
                    fastJump = true;
                    Sprite->Play(Sprite->JumpFast);
                }
                else
                    Sprite->Play(Sprite->JumpSlow);
            }
            // falling down
            else
            {
                if (Holding != nullptr)
                {
                    Sprite->Play(Sprite->FallCarry);
                }
                else if (fastJump || Speed.y >= MaxFall) //|| level.InSpace)
                {
                    fastJump = true;
                    if (Sprite->LastAnimationID != Sprite->FallFast)
                        Sprite->Play(Sprite->FallFast);
                }
                else
                    Sprite->Play(Sprite->FallSlow);
            }
        }
    }


    //if (StateMachine->GetCurState() != StDummy)
    //{
    //    if (level.InSpace)
    //        Sprite->Rate = .5f;
    //    else
    //        Sprite->Rate = 1.f;
    //}
}
void CPlayerScript::UpdateHair(bool applyGravity)
{
    if (StateMachine->GetCurState() == 19)
    {
        m_pHairUpdate->Color = Sprite->Color;
        applyGravity = false;
    }
    else if (Dashes == 0 && Dashes < MaxDashes)
    {
        if (Sprite->Mode == PlayerSpriteMode::MadelineAsBadeline)
        {
            m_pHairUpdate->Color = (Color)Lerp((Vec4)m_pHairUpdate->Color, (Vec4)UsedBadelineHairColor, 6.f * DT);
        }
        else
        {
            m_pHairUpdate->Color = (Color)Lerp((Vec4)m_pHairUpdate->Color, (Vec4)UsedHairColor, 6.f * DT);
        }
    }
    else
    {
        Color color;
        if (lastDashes != Dashes)
        {
            color = FlashHairColor;
            hairFlashTimer = 0.12f;
        }
        else if (hairFlashTimer > 0.f)
        {
            color = FlashHairColor;
            hairFlashTimer -= DT;
        }
        else if (Sprite->Mode == PlayerSpriteMode::MadelineAsBadeline)
        {
            if (Dashes == 2)
            {
                color = NormalBadelineHairColor;
                //color = TwoDashesBadelineHairColor;
            }
            else
            {
                color = TwoDashesBadelineHairColor;
                //color = NormalBadelineHairColor;
            }
        }
        else if (Dashes == 2)
        {
            //color = TwoDashesHairColor;
            color = NormalHairColor;
        }
        else
        {
            color = TwoDashesHairColor;
            //color = NormalHairColor;
        }
        m_pHairUpdate->Color = color;
    }
    //if (OverrideHairColor != null)
    //{
    //    m_pHairUpdate->Color = OverrideHairColor;
    //}
    m_pHairUpdate->facing = Facing;
    m_pHairUpdate->SimulateMotion = applyGravity;
    lastDashes = Dashes;
}
void CPlayerScript::UpdateRender()
{

}

#pragma endregion


#pragma region About Normal State

void CPlayerScript::SetGround()
{
    Speed.y = 0.f;
    onGround = true;
    Animator2D()->SetMulScale(Vec2(1.f, 1.f));
}
void CPlayerScript::UnSetGround()
{
    onGround = false;
}
bool CPlayerScript::InControl()
{
    switch ((PLAYER_STATE)StateMachine->GetCurState())
    {
    case StIntroJump:
    case StIntroWalk:
    case StIntroWakeUp:
    case StIntroRespawn:
    case StDummy:
    case StFrozen:
    case StBirdDashTutorial:
        return false;
    default:
        return true;
    }
}
void CPlayerScript::jump()
{
    //Input.Jump.ConsumeBuffer();
    jumpGraceTimer = 0.f;
    varJumpTimer = VarJumpTime;
    AutoJump = false;
    dashAttackTimer = 0;
    wallSlideTimer = WallSlideTime;
    wallBoostTimer = 0;

    Speed.x += JumpHBoost * moveX;
    Speed.y = JumpSpeed;
    //Speed += LiftBoost;
    varJumpSpeed = Speed.y;

    LaunchedBoostCheck();

    //if (playSfx)
    //{
    //    if (launched)
    //        Play(Sfxs.char_mad_jump_assisted);

    //    if (dreamJump)
    //        Play(Sfxs.char_mad_jump_dreamblock);
    //    else
    //        Play(Sfxs.char_mad_jump);
    //}
    Animator2D()->SetMulScale(Vec2(.6f, 1.4f));
    //if (particles)
    //    Dust.Burst(BottomCenter, Calc.Up, 4);

    //SaveData.Instance.TotalJumps++;
}
int CPlayerScript::NormalUpdate()
{
    //Use Lift Boost if walked off platform
    //if (wasOnGround && !onGround && Speed.y >= 0)
    //    Speed.y = LiftBoost.Y;

    //if (Holding == null)
    //{
        if (GRABKEY && !IsTired() && !Ducking())
        {
            //Grabbing Holdables
            //foreach(Holdable hold in Scene.Tracker.GetComponents<Holdable>())
            //    if (hold.Check(this) && Pickup(hold))
            //        return StPickup;

            //Climbing
            //if (Speed.y >= 0 && Math.Sign(Speed.y) != -(int)Facing)
            //{
            //    if (ClimbCheck((int)Facing))
            //    {
            //        Ducking = false;
            //        return StClimb;
            //    }

            //    if (Input.MoveY < 1 && level.Wind.Y <= 0)
            //    {
            //        for (int i = 1; i <= ClimbUpCheckDist; i++)
            //        {
            //            if (!CollideCheck<Solid>(Position + Vector2.UnitY * -i) && ClimbCheck((int)Facing, -i))
            //            {
            //                MoveVExact(-i);
            //                Ducking = false;
            //                return StClimb;
            //            }
            //        }
            //    }
            //}
        }

        //Dashing
        if (CanDash())
        {
            //Speed += LiftBoost;
            return StartDash();
        }

        //Ducking
        //if (Ducking)
        //{
        //    if (onGround && Input.MoveY != 1)
        //    {
        //        if (CanUnDuck)
        //        {
        //            Ducking = false;
        //            Sprite.Scale = new Vector2(.8f, 1.2f);
        //        }
        //        else if (Speed.x == 0)
        //        {
        //            for (int i = DuckCorrectCheck; i > 0; i--)
        //            {
        //                if (CanUnDuckAt(Position + Vector2.UnitX * i))
        //                {
        //                    MoveH(DuckCorrectSlide * Engine.DeltaTime);
        //                    break;
        //                }
        //                else if (CanUnDuckAt(Position - Vector2.UnitX * i))
        //                {
        //                    MoveH(-DuckCorrectSlide * Engine.DeltaTime);
        //                    break;
        //                }
        //            }
        //        }
        //    }
        //}
        //else if (onGround && Input.MoveY == 1 && Speed.y >= 0)
        //{
        //    Ducking = true;
        //    Sprite.Scale = new Vector2(1.4f, .6f);
        //}
    //}
    //else
    //{
    //    //Throw
    //    if (!Input.Grab.Check && minHoldTimer <= 0)
    //        Throw();

    //    //Ducking
    //    if (!Ducking && onGround && Input.MoveY == 1 && Speed.y >= 0)
    //    {
    //        Drop();
    //        Ducking = true;
    //        Sprite.Scale = new Vector2(1.4f, .6f);
    //    }
    //}

    //Running and Friction
        if (Ducking() && onGround)
            Speed.x = Approach(Speed.x, 0, DuckFriction * DT);
        else
        {
            float mult = onGround ? 1 : AirMult;
            //if (onGround && level.CoreMode == Session.CoreModes.Cold)
            //    mult *= .3f;
            float max = Holding == nullptr ? MaxRun : HoldingMaxRun;
            //if (level.InSpace)
            //    max *= SpacePhysicsMult;
            if (abs(Speed.x) > max && Sign(Speed.x) == moveX)
                Speed.x = Approach(Speed.x, max * moveX, RunReduce * mult * DT);  //Reduce back from beyond the max speed
            else
                Speed.x = Approach(Speed.x, max * moveX, RunAccel * mult * DT);   //Approach the max speed
        }

        //Vertical
        {
            //Calculate current max fall speed
            {
                float mf = MaxFall;
                float fmf = FastMaxFall;

                //if (level.InSpace)
                //{
                //    mf *= SpacePhysicsMult;
                //    fmf *= SpacePhysicsMult;
                //}

                //Fast Fall
                if (KEY_PRESSED(DOWN) && Speed.y >= mf)
                {
                    maxFall = Approach(maxFall, fmf, FastMaxAccel * DT);

                    float half = mf + (fmf - mf) * .5f;
                    if (Speed.y >= half)
                    {
                        float spriteLerp = min(1.f, (Speed.y - half) / (fmf - half));
                        float lx = Lerp(1.f, .5f, spriteLerp);
                        float ly = Lerp(1.f, 1.5f, spriteLerp);

                        Animator2D()->SetMulScale(Vec2(lx, ly));
                    }
                }
                else
                    maxFall = Approach(maxFall, mf, FastMaxAccel * DT);
            }

            //Gravity
            if (!onGround)
            {
                float max = maxFall;

                //Wall Slide
                //if ((moveX == (int)Facing || (moveX == 0 && GRABKEY)) && Input.MoveY.Value != 1)
                //{
                //    if (Speed.y >= 0 && wallSlideTimer > 0 && Holding == nullptr && ClimbBoundsCheck((int)Facing) && CollideCheck<Solid>(Position + Vector2.UnitX * (int)Facing) && CanUnDuck)
                //    {
                //        SetDucking(false);
                //        wallSlideDir = (int)Facing;
                //    }

                //    if (wallSlideDir != 0)
                //    {
                //        if (wallSlideTimer > WallSlideTime * .5f && true) //ClimbBlocker.Check(level, this, Position + Vector2.UnitX * wallSlideDir))
                //            wallSlideTimer = WallSlideTime * .5f;

                //        max = Lerp(MaxFall, WallSlideStartMax, wallSlideTimer / WallSlideTime);
                //        if (wallSlideTimer / WallSlideTime > .65f)
                //            CreateWallSlideParticles(wallSlideDir);
                //    }
                //}

                float mult = (abs(Speed.y) < HalfGravThreshold && (JUMPKEY || AutoJump)) ? .5f : 1.f;

                //if (level.InSpace)
                //    mult *= SpacePhysicsMult;

                Speed.y = Approach(Speed.y, max, Gravity * mult * DT);
            }

            //Variable Jumping
            if (varJumpTimer > 0)
            {
                if (AutoJump || JUMPKEY)
                    Speed.y = min(Speed.y, varJumpSpeed);
                else
                    varJumpTimer = 0;
            }

            //Jumping
            if (JUMPKEY)
            {
                //Water water = nullptr;
                if (jumpGraceTimer > 0)
                {
                    jump();
                }
                //else if (CanUnDuck())
                //{
                //    bool canUnduck = CanUnDuck();
                //    if (canUnduck && WallJumpCheck(1))
                //    {
                //        if (Facing == Facings.Right && Input.Grab.Check && Stamina > 0 && Holding == null && !ClimbBlocker.Check(Scene, this, Position + Vector2.UnitX * WallJumpCheckDist))
                //            ClimbJump();
                //        else if (DashAttacking && DashDir.X == 0 && DashDir.Y == -1)
                //            SuperWallJump(-1);
                //        else
                //            WallJump(-1);
                //    }
                //    else if (canUnduck && WallJumpCheck(-1))
                //    {
                //        if (Facing == Facings.Left && Input.Grab.Check && Stamina > 0 && Holding == null && !ClimbBlocker.Check(Scene, this, Position + Vector2.UnitX * -WallJumpCheckDist))
                //            ClimbJump();
                //        else if (DashAttacking && DashDir.X == 0 && DashDir.Y == -1)
                //            SuperWallJump(1);
                //        else
                //            WallJump(1);
                //    }
                //    //else if ((water = CollideFirst<Water>(Position + Vector2.UnitY * 2)) != null)
                //    //{
                //    //    Jump();
                //    //    water.TopSurface.DoRipple(Position, 1);
                //    //}
                //}
            }
        }
     //}
    return StNormal;
}
void CPlayerScript::NormalBegin()
{
    maxFall = MaxFall;
}
void CPlayerScript::NormalEnd()
{
    wallBoostTimer = 0;
    wallBoostTimer = 0;
    wallSpeedRetentionTimer = 0;
    hopWaitX = 0;
}

#pragma endregion


#pragma region About Dash State

void CPlayerScript::CallDashEvents()
{
    if (!calledDashEvents)
    {
        calledDashEvents = true;
        //if (CurrentBooster == null)
        //{
            // Increment Counter
            //SaveData.Instance.TotalDashes++;
            //level.Session.Dashes++;
            //Stats.Increment(Stat.DASHES);

            // Sfxs
            //{
            //    var rightDashSound = DashDir.Y < 0 || (DashDir.Y == 0 && DashDir.X > 0);
            //    if (DashDir == Vector2.Zero)
            //        rightDashSound = Facing == Facings.Right;

            //    if (rightDashSound)
            //    {
            //        if (wasDashB)
            //            Play(Sfxs.char_mad_dash_pink_right);
            //        else
            //            Play(Sfxs.char_mad_dash_red_right);
            //    }
            //    else
            //    {
            //        if (wasDashB)
            //            Play(Sfxs.char_mad_dash_pink_left);
            //        else
            //            Play(Sfxs.char_mad_dash_red_left);
            //    }

            //    if (SwimCheck())
            //        Play(Sfxs.char_mad_water_dash_gen);
            //}

            //Dash Listeners
            //foreach(DashListener dl in Scene.Tracker.GetComponents<DashListener>())
            //    if (dl.OnDash != null)
            //        dl.OnDash(DashDir);
        //}
        //else
        //{
            //Booster
            //CurrentBooster.PlayerBoosted(this, DashDir);
            //CurrentBooster = null;
        //}
    }
}
int CPlayerScript::DashUpdate()
{
    static Vector2 swapCancel = Vec2(0, 0);
    StartedDashing = false;
    DashCoroutineTimer -= DT;

    //Trail
    if (dashTrailTimer > 0)
    {
        dashTrailTimer -= DT;
        if (dashTrailTimer <= 0)
            CreateTrail();
    }

    //Grab Holdables
    if (Holding == null && GRABKEY && !IsTired() && CanUnDuck())
    {
        ////Grabbing Holdables
        //foreach(Holdable hold in Scene.Tracker.GetComponents<Holdable>())
        //    if (hold.Check(this) && Pickup(hold))
        //        return StPickup;
    }

    if (DashDir.y == 0)
    {
        ////JumpThru Correction
        //foreach(JumpThru jt in Scene.Tracker.GetEntities<JumpThru>())
        //    if (CollideCheck(jt) && Bottom - jt.Top <= DashHJumpThruNudge)
        //        MoveVExact((int)(jt.Top - Bottom));

        ////Super Jump
        //if (CanUnDuck && JUMPKEY && jumpGraceTimer > 0)
        //{
        //    SuperJump();
        //    return StNormal;
        //}
    }

    if (DashDir.x == 0 && DashDir.y == -1)
    {
        //if (JUMPKEY && CanUnDuck())
        //{
        //    if (WallJumpCheck(1))
        //    {
        //        SuperWallJump(-1);
        //        return StNormal;
        //    }
        //    else if (WallJumpCheck(-1))
        //    {
        //        SuperWallJump(1);
        //        return StNormal;
        //    }
        //}
    }
    else
    {
        if (JUMPKEY && CanUnDuck())
        {
            //if (WallJumpCheck(1))
            //{
            //    WallJump(-1);
            //    return StNormal;
            //}
            //else if (WallJumpCheck(-1))
            //{
            //    WallJump(1);
            //    return StNormal;
            //}
        }
    }

    //Dash Coroutine
    if (0 == DashCoroutineCnt)
    {
        //Do Nothing
        ++DashCoroutineCnt;
        return StDash;
    }
    if (1 == DashCoroutineCnt)
    {

        auto dir = lastAim;
        //if (OverrideDashDirection)
        //    dir = OverrideDashDirection.Value;

        auto newSpeed = dir * DashSpeed;
        if (Sign(beforeDashSpeed.x) == Sign(newSpeed.x) && abs(beforeDashSpeed.x) > abs(newSpeed.x))
            newSpeed.x = beforeDashSpeed.x;
        Speed = newSpeed;

        //if (CollideCheck<Water>())
        //    Speed *= SwimDashSpeedMult;

        DashDir = dir;
        CRenderMgr::GetInst()->DirectionalShake(DashDir, .2f);
        SetShockWaveEvent();

        if (DashDir.x != 0)
            Facing = (Facings)Sign(DashDir.x);

        CallDashEvents();

        //Feather particles
        //if (StateMachine->GetPrevState() == StStarFly)
        //    level.Particles.Emit(FlyFeather.P_Boost, 12, Center, Vector2.One * 4, (-dir).Angle());

        //Dash Slide
        if (onGround && DashDir.x != 0 && DashDir.y > 0 && Speed.y > 0)
            //&& (!Inventory.DreamDash || !CollideCheck<DreamBlock>(Position +Vec2(0,1))))
        {
            DashDir.x = Sign(DashDir.x);
            DashDir.y = 0;
            Speed.y = 0;
            Speed.x *= DodgeSlideSpeedMult;
            SetDucking(true);
        }

        //SlashFx.Burst(Center, DashDir.Angle());
        CreateTrail();
        dashTrailTimer = .08f;

        //Swap Block check
        //if (DashDir.X != 0 && Input.Grab.Check)
        //{
        //    var swapBlock = CollideFirst<SwapBlock>(Position + Vector2.UnitX * Math.Sign(DashDir.X));
        //    if (swapBlock != null && swapBlock.Direction.X == Math.Sign(DashDir.X))
        //    {
        //        StateMachine.State = StClimb;
        //        Speed = Vector2.Zero;
        //        yield break;
        //    }
        //}

        //Stick to Swap Blocks
        swapCancel = Vec2(1, 1);
        //foreach(SwapBlock swapBlock in Scene.Tracker.GetEntities<SwapBlock>())
        //{
        //    if (CollideCheck(swapBlock, Position + Vector2.UnitY))
        //    {
        //        if (swapBlock != null && swapBlock.Swapping)
        //        {
        //            if (DashDir.X != 0 && swapBlock.Direction.X == Math.Sign(DashDir.X))
        //                Speed.X = swapCancel.X = 0;
        //            if (DashDir.Y != 0 && swapBlock.Direction.Y == Math.Sign(DashDir.Y))
        //                Speed.Y = swapCancel.Y = 0;
        //        }
        //    }
        //}

        ++DashCoroutineCnt;
        return StDash;
    }
    if (2 == DashCoroutineCnt && DashCoroutineTimer < 0.f)
    {
        CreateTrail();

        AutoJump = true;
        AutoJumpTimer = 0;
        if (DashDir.y <= 0)
        {
            Speed = DashDir * EndDashSpeed;
            Speed.x *= swapCancel.x;
            Speed.y *= swapCancel.y;
        }
        if (Speed.y < 0)
            Speed.y *= EndDashUpMult;
        return StNormal;
    }



    if (Speed != Vec2(0.f, 0.f))//&& level.OnInterval(0.02f))
    {
        static const Color Normal = HexToColor("AC3232");
        static const Color Used = HexToColor("44B7FF");

        static const Color NormalVib = HexToColor("FFFFFF");
        static const Color UsedVib = HexToColor("FFFFFF");

        Color c = wasDashB ? Normal : Used;
        Color c2 = wasDashB ? NormalVib : UsedVib;
        m_Particle->GenerateParticle(c,c2);
        //level.ParticlesFG.Emit(wasDashB ? P_DashB : P_DashA, Center + Calc.Random.Range(Vector2.One * -2, Vector2.One * 2), DashDir.Angle());
    }
    return StDash;
}
void CPlayerScript::DashBegin()
{
    DashCoroutineTimer = DashTime;
    DashCoroutineCnt = 0;
    calledDashEvents = false;
    dashStartedOnGround = onGround;
    launched = false;

    //if (Engine.TimeRate > 0.25f)
    Celeste->Freeze(.05f);
    //level.Displacement.AddBurst(Center, .4f, 8, 64, .5f, Ease.QuadOut, Ease.QuadOut);
    //Input.Rumble(RumbleStrength.Strong, RumbleLength.Medium);

    dashCooldownTimer = DashCooldown;
    dashRefillCooldownTimer = DashRefillCooldown;
    StartedDashing = true;
    wallSlideTimer = WallSlideTime;
    dashTrailTimer = 0;

    dashAttackTimer = DashAttackTime;
    beforeDashSpeed = Speed;
    Speed = Vec2(0.f, 0.f);
    DashDir = Vec2(0.f, 0.f);

    if (!onGround && Ducking() && CanUnDuck())
        SetDucking(false);
}
void CPlayerScript::DashEnd()
{
    DashCoroutineTimer = 0.f;
    DashCoroutineCnt = 0;
    CallDashEvents();
    demoDashed = false;
}
void CPlayerScript::CreateTrail()
{
    m_bAfterImageRequest = true;
}
void CPlayerScript::PushAfterImageEvent()
{
    if (false == m_bAfterImageRequest)
        return;

    tAfterImageEvent Event= {};

    //공통
    Event.Duration = DashAfterImageTime;
    Event.AccTime = 0.0f;

    //플레이어
    Event.PlayerWorldMat = GetOwner()->Transform()->GetWorldMat();
    Event.HairWorldMat = m_pPlayerHairGO->Transform()->GetWorldMat();
    Event.facing = GetOwner()->m_facing;
    Event.vMult = GetOwner()->Animator2D()->GetMulScale();
    static const Color Normal = HexToColor("AC3232");
    //static const Color Normal = HexToColor("ff6def");
    //static const Color Normal = HexToColor("44B7FF");

    //static const Color Used = HexToColor("AC3232");
    //static const Color Used = HexToColor("ff6def");
    static const Color Used = HexToColor("44B7FF");

    Color c = wasDashB ? Normal : Used;


    Event.Color = c;
    //플레이어 텍스쳐
    auto anim = GetOwner()->Animator2D()->GetCurAnim();
    auto& vecfrm = anim->GetVecFrm();
    Event.PlayerAnimTex = vecfrm[anim->GetCurIdx()].pFrameTex;

    //헤어
    Event.HairInfo = m_pHairComp->GetRenderInfo();




    m_AfterImage->PushEvent(Event);
    m_bAfterImageRequest = false;
}
void CPlayerScript::SetShockWaveEvent()
{
    tShockEvent SE= {};
    
    SE.ShockMat = m_ShockWave->Transform()->GetWorldMat();
    SE.AccTime = 0.f;

    m_ShockWave->PushShockEvent(SE);
}

#pragma endregion 

#pragma region About Dream Dash State

void CPlayerScript::DreamDashBegin()
{
}
int CPlayerScript::DreamDashUpdate()
{
    return 0;
}
void CPlayerScript::DreamDashEnd()
{
}

#pragma endregion





#pragma region FrameWork Script Legacy

void CPlayerScript::BeginOverlap(CCollider2D* _Collider
    , CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
    Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
    //fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
    //fread(&m_Speed, sizeof(float), 1, _File);
}

#pragma endregion
