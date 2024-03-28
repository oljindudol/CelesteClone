#include "pch.h"
#include "CPlayerScript.h"
#include <Engine/CAssetMgr.h>

#include <Engine/CGameObject.h>
#include "CMissileScript.h"

#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

//#include "CPhysics.h"
#include "CCustomStateMachine.h"



CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
{
	//Depth = Depths.Player;
	//Tag = Tags.Persistent;

	//hair sprite
	//Sprite = new PlayerSprite(spriteMode);
	//Add(Hair = new PlayerHair(Sprite));
	//Add(Sprite);
	//Hair.Color = NormalHairColor;
	//startHairCount = Sprite.HairCount;

	// sweat sprite
	//sweatSprite = GFX.SpriteBank.Create("player_sweat");
	//Add(sweatSprite);

	// physics
	Collider = normalHitbox;
	hurtbox = normalHurtbox;
	// physics델리게이트 함수
	//onCollideH = OnCollideH;
	//onCollideV = OnCollideV;

	StateMachine = new CCustomStateMachine<CPlayerScript>(this , (int)PLAYER_STATE::END);
	StateMachine->SetCallbacks( 0,"StNormal",&CPlayerScript::NormalUpdate, &CPlayerScript::NormalBegin, &CPlayerScript::NormalEnd,nullptr);
	StateMachine->SetCallbacks(1, "StUpdate", &CPlayerScript::DashUpdate, &CPlayerScript::DashBegin, &CPlayerScript::DashEnd, nullptr);

	// other stuff
	// Leader = 아이템따라오는것 , wind,light 
	//Add(Leader = new Leader(new Vector2(0, -8)));
	lastAim = Vector2(1,0);
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
	delete StateMachine;
}


void CPlayerScript::begin()
{
	string OriginPath = "C:\\CelesteClone\\OutputFile\\content\\texture\\Gameplay\\madeline";
	auto folders = getFoldersFromDirectory(OriginPath);
	
	//auto ty = Transform()->GetRelativeScale().y;
	for (auto& f : folders)
	{
		auto rel = std::filesystem::relative(f, OriginPath);

		Animator2D()->CreateFromFolder(rel,f, 12.5 , Vec2(0.f, -0.375f) );
	}

	GetRenderComponent()->GetDynamicMaterial();

	auto a = Animator2D();
	Animator2D()->Play(L"idle");

	//SetPhysics(new CPhysics(GetOwner()));

	StateMachine->Begin();
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();


	auto a = Animator2D();
	if (KEY_PRESSED(KEY::UP))
		//vPos.y += DT * m_Speed;	
	if (KEY_TAP(KEY::UP))
		Animator2D()->Play(L"MOVE_UP");
	if (KEY_RELEASED(UP))
		Animator2D()->Play(L"IDLE_UP");
	
	if (KEY_PRESSED(KEY::DOWN))
		//vPos.y -= DT * m_Speed;
	if (KEY_TAP(KEY::DOWN))
		Animator2D()->Play(L"MOVE_DOWN");
	if (KEY_RELEASED(DOWN))
		Animator2D()->Play(L"IDLE_DOWN");

	if (KEY_PRESSED(KEY::LEFT))
		//vPos.x -= DT * m_Speed;
	if (KEY_TAP(KEY::LEFT))
		Animator2D()->Play(L"MOVE_LEFT");
	if (KEY_RELEASED(LEFT))
		Animator2D()->Play(L"IDLE_LEFT");

	if (KEY_PRESSED(KEY::RIGHT))
		//vPos.x += DT * m_Speed;
	if (KEY_TAP(KEY::RIGHT))
		Animator2D()->Play(L"MOVE_RIGHT");
	if (KEY_RELEASED(RIGHT))
		Animator2D()->Play(L"IDLE_RIGHT");

	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}
	
	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	if (KEY_TAP(KEY::SPACE))
	{
		Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
		//GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
		GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 1);
		}
	}
	else if (KEY_RELEASED(KEY::SPACE))
	{
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		if (nullptr != pMtrl)
		{
			pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, 0);
		}
	}

	StateMachine->Update();
	Update();
	//GetPhysics()->Update();

	//static float f = 0.f;
	//f += DT * 0.3f;
	//GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

	//GamePlayStatic::DrawDebugRect(Vec3(0.f, 0.f, 0.f), Vec3(200.f, 200.f, 1.f), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), true, 20);
	//GamePlayStatic::DrawDebugCircle(Vec3(0.f, 0.f, 0.f), 200.f, Vec3(0.f, 1.f, 1.f), true);
}

void CPlayerScript::Update()
{
    {
        Vec3 V3Pos = Transform()->GetWorldPos();
        Vec2 Position = Vec2(V3Pos.x, V3Pos.y);
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
            if (JustRespawned && Speed != Vec2(0,0))
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
            if (onGround && StateMachine.State != StClimb)
            {
                AutoJump = false;
                Stamina = ClimbMaxStamina;
                wallSlideTimer = WallSlideTime;
            }

            //Dash Attack
            if (dashAttackTimer > 0)
                dashAttackTimer -= DT;

            //Jump Grace
            //if (onGround)
            //{
            //    dreamJump = false;
            //    jumpGraceTimer = JumpGraceTime;
            //}
            //else if (jumpGraceTimer > 0)
            //    jumpGraceTimer -= DT;

            //refill Dashes
            {
                if (dashCooldownTimer > 0)
                    dashCooldownTimer -= DT;

                if (dashRefillCooldownTimer > 0)
                    dashRefillCooldownTimer -= DT;
                else //if (SaveData.Instance.AssistMode && SaveData.Instance.Assists.DashMode == Assists.DashModes.Infinite && !level.InCutscene)
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
                float inputx =0.f;
                if (KEY_PRESSED(LEFT))
                {
                    inputx = -1.f;
                }
                else if (KEY_PRESSED(RIGHT))
                {
                    inputx = 1.f;
                }
                
                moveX = Inputx;
                climbHopSolid = null;
            }

            //Climb Hop Solid Movement
            if (climbHopSolid != null && !climbHopSolid.Collidable)
                climbHopSolid = null;
            else if (climbHopSolid != null && climbHopSolid.Position != climbHopSolidPosition)
            {
                var move = climbHopSolid.Position - climbHopSolidPosition;
                climbHopSolidPosition = climbHopSolid.Position;
                MoveHExact((int)move.X);
                MoveVExact((int)move.Y);
            }

            //Wind
            if (noWindTimer > 0)
                noWindTimer -= DT;

            //Facing
            if (moveX != 0 && InControl
                && StateMachine.State != StClimb && StateMachine.State != StPickup && StateMachine.State != StRedDash && StateMachine.State != StHitSquash)
            {
                var to = (Facings)moveX;
                if (to != Facing && Ducking)
                    Sprite.Scale = new Vector2(0.8f, 1.2f);
                Facing = to;
            }

            //Aiming
            lastAim = Input.GetAimVector(Facing);

            //Wall Speed Retention
            if (wallSpeedRetentionTimer > 0)
            {
                if (Math.Sign(Speed.X) == -Math.Sign(wallSpeedRetained))
                    wallSpeedRetentionTimer = 0;
                else if (!CollideCheck<Solid>(Position + Vector2.UnitX * Math.Sign(wallSpeedRetained)))
                {
                    Speed.X = wallSpeedRetained;
                    wallSpeedRetentionTimer = 0;
                }
                else
                    wallSpeedRetentionTimer -= DT;
            }

            //Hop Wait X
            if (hopWaitX != 0)
            {
                if (Math.Sign(Speed.X) == -hopWaitX || Speed.Y > 0)
                    hopWaitX = 0;
                else if (!CollideCheck<Solid>(Position + Vector2.UnitX * hopWaitX))
                {
                    Speed.X = hopWaitXSpeed;
                    hopWaitX = 0;
                }
            }

            // Wind Timeout
            if (windTimeout > 0)
                windTimeout -= DT;

            // Hair
            {
                var windDir = windDirection;
                if (ForceStrongWindHair.Length() > 0)
                    windDir = ForceStrongWindHair;

                if (windTimeout > 0 && windDir.X != 0)
                {
                    windHairTimer += DT * 8f;

                    Hair.StepPerSegment = new Vector2(windDir.X * 5f, (float)Math.Sin(windHairTimer));
                    Hair.StepInFacingPerSegment = 0f;
                    Hair.StepApproach = 128f;
                    Hair.StepYSinePerSegment = 0;
                }
                else if (Dashes > 1)
                {
                    Hair.StepPerSegment = new Vector2((float)Math.Sin(Scene.TimeActive * 2) * 0.7f - (int)Facing * 3, (float)Math.Sin(Scene.TimeActive * 1f));
                    Hair.StepInFacingPerSegment = 0f;
                    Hair.StepApproach = 90f;
                    Hair.StepYSinePerSegment = 1f;

                    Hair.StepPerSegment.Y += windDir.Y * 2f;
                }
                else
                {
                    Hair.StepPerSegment = new Vector2(0, 2f);
                    Hair.StepInFacingPerSegment = 0.5f;
                    Hair.StepApproach = 64f;
                    Hair.StepYSinePerSegment = 0;

                    Hair.StepPerSegment.Y += windDir.Y * 0.5f;
                }
            }

            if (StateMachine.State == StRedDash)
                Sprite.HairCount = 1;
            else if (StateMachine.State != StStarFly)
                Sprite.HairCount = (Dashes > 1 ? 5 : startHairCount);

            //Min Hold Time
            if (minHoldTimer > 0)
                minHoldTimer -= DT;

            //Launch Particles
            if (launched)
            {
                var sq = Speed.LengthSquared();
                if (sq < LaunchedMinSpeedSq)
                    launched = false;
                else
                {
                    var was = launchedTimer;
                    launchedTimer += DT;

                    if (launchedTimer >= .5f)
                    {
                        launched = false;
                        launchedTimer = 0;
                    }
                    else if (Calc.OnInterval(launchedTimer, was, .15f))
                        level.Add(Engine.Pooler.Create<SpeedRing>().Init(Center, Speed.Angle(), Color.White));
                }
            }
            else
                launchedTimer = 0;
        }

        if (IsTired)
        {
            Input.Rumble(RumbleStrength.Light, RumbleLength.Short);
            if (!wasTired)
            {
                wasTired = true;
            }
        }
        else
            wasTired = false;

        base.Update();

        //Light Offset
        if (Ducking)
            Light.Position = duckingLightOffset;
        else
            Light.Position = normalLightOffset;

        //Jump Thru Assist
        if (!onGround && Speed.Y <= 0 && (StateMachine.State != StClimb || lastClimbMove == -1) && CollideCheck<JumpThru>() && !JumpThruBoostBlockedCheck())
            MoveV(JumpThruAssistSpeed * DT);

        //Dash Floor Snapping
        if (!onGround && DashAttacking && DashDir.Y == 0)
        {
            if (CollideCheck<Solid>(Position + Vector2.UnitY * DashVFloorSnapDist) || CollideCheckOutside<JumpThru>(Position + Vector2.UnitY * DashVFloorSnapDist))
                MoveVExact(DashVFloorSnapDist);
        }

        //Falling unducking
        if (Speed.Y > 0 && CanUnDuck && Collider != starFlyHitbox && !onGround)
            Ducking = false;

        //Physics
        if (StateMachine.State != StDreamDash && StateMachine.State != StAttract)
            MoveH(Speed.X * DT, onCollideH);
        if (StateMachine.State != StDreamDash && StateMachine.State != StAttract)
            MoveV(Speed.Y * DT, onCollideV);

        //Swimming
        if (StateMachine.State == StSwim)
        {
            //Stay at water surface
            if (Speed.Y < 0 && Speed.Y >= SwimMaxRise)
            {
                while (!SwimCheck())
                {
                    Speed.Y = 0;
                    if (MoveVExact(1))
                        break;
                }
            }
        }
        else if (StateMachine.State == StNormal && SwimCheck())
            StateMachine.State = StSwim;
        else if (StateMachine.State == StClimb && SwimCheck())
        {
            var water = CollideFirst<Water>(Position);
            if (water != null && Center.Y < water.Center.Y)
            {
                while (SwimCheck())
                    if (MoveVExact(-1))
                        break;
                if (SwimCheck())
                    StateMachine.State = StSwim;
            }
            else
                StateMachine.State = StSwim;
        }

        // wall slide SFX
        {
            var isSliding = Sprite.CurrentAnimationID != null && Sprite.CurrentAnimationID.Equals(PlayerSprite.WallSlide) && Speed.Y > 0;
            if (isSliding)
            {
                if (!wallSlideSfx.Playing)
                    Loop(wallSlideSfx, Sfxs.char_mad_wallslide);

                var platform = SurfaceIndex.GetPlatformByPriority(CollideAll<Solid>(Center + Vector2.UnitX * (int)Facing, temp));
                if (platform != null)
                    wallSlideSfx.Param(SurfaceIndex.Param, platform.GetWallSoundIndex(this, (int)Facing));
            }
            else
                Stop(wallSlideSfx);
        }

        // update sprite
        UpdateSprite();

        //Carry held item
        UpdateCarry();

        //Triggers
        if (StateMachine.State != StReflectionFall)
        {
            foreach(Trigger trigger in Scene.Tracker.GetEntities<Trigger>())
            {
                if (CollideCheck(trigger))
                {
                    if (!trigger.Triggered)
                    {
                        trigger.Triggered = true;
                        triggersInside.Add(trigger);
                        trigger.OnEnter(this);
                    }
                    trigger.OnStay(this);
                }
                else if (trigger.Triggered)
                {
                    triggersInside.Remove(trigger);
                    trigger.Triggered = false;
                    trigger.OnLeave(this);
                }
            }
        }

        //Strawberry Block
        StrawberriesBlocked = CollideCheck<BlockField>();

        // Camera (lerp by distance using delta-time)
        if (InControl || ForceCameraUpdate)
        {
            if (StateMachine.State == StReflectionFall)
            {
                level.Camera.Position = CameraTarget;
            }
            else
            {
                var from = level.Camera.Position;
                var target = CameraTarget;
                var multiplier = StateMachine.State == StTempleFall ? 8 : 1f;

                level.Camera.Position = from + (target - from) * (1f - (float)Math.Pow(0.01f / multiplier, DT));
            }
        }

        //Player Colliders
        if (!Dead && StateMachine.State != StCassetteFly)
        {
            Collider was = Collider;
            Collider = hurtbox;

            foreach(PlayerCollider pc in Scene.Tracker.GetComponents<PlayerCollider>())
            {
                if (pc.Check(this) && Dead)
                {
                    Collider = was;
                    return;
                }
            }

            // If the current collider is not the hurtbox we set it to, that means a collision callback changed it. Keep the new one!
            bool keepNew = (Collider != hurtbox);

            if (!keepNew)
                Collider = was;
        }

        //Bounds
        if (InControl && !Dead && StateMachine.State != StDreamDash)
            level.EnforceBounds(this);

        UpdateChaserStates();
        UpdateHair(true);

        //Sounds on ducking state change
        if (wasDucking != Ducking)
        {
            wasDucking = Ducking;
            if (wasDucking)
                Play(Sfxs.char_mad_duck);
            else if (onGround)
                Play(Sfxs.char_mad_stand);
        }

        // shallow swim sfx
        if (Speed.X != 0 && ((StateMachine.State == StSwim && !SwimUnderwaterCheck()) || (StateMachine.State == StNormal && CollideCheck<Water>(Position))))
        {
            if (!swimSurfaceLoopSfx.Playing)
                swimSurfaceLoopSfx.Play(Sfxs.char_mad_water_move_shallow);
        }
        else
            swimSurfaceLoopSfx.Stop();

        wasOnGround = onGround;
    }
}

void CPlayerScript::UpdateHair()
{
}

void CPlayerScript::UpdateRender()
{
}

void CPlayerScript::UpdateSprite()
{
}

void CPlayerScript::StartHair()
{
}

void CPlayerScript::CreateSplitParticles()
{
}

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

void CPlayerScript::NormalBegin()
{
	printf("start Normal");
}

int CPlayerScript::NormalUpdate()
{
	if (KEY_PRESSED(RIGHT))
		return 1;
	if (KEY_PRESSED(LEFT))
		return 1;

	return 0;
}
void CPlayerScript::NormalEnd()
{
	printf("End Normal");
}
void CPlayerScript::DashBegin()
{
	printf("start Dash");
}
int CPlayerScript::DashUpdate()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();
	if (KEY_PRESSED(RIGHT))
	{
		vPos.x += DT * 200.f;
		Transform()->SetRelativePos(vPos);
		return 1;
	}
		
	if (KEY_PRESSED(LEFT))
	{
		vPos.x -= DT * 200.f;
		Transform()->SetRelativePos(vPos);
		return 1;
	}

	return 0;
}

void CPlayerScript::DashEnd()
{
	printf("End Dash");
}
