#pragma once
class CHair;
class CPlayerHairUpdate;



class CPlayerSprite
{
#pragma region consts
public:
	const string Idle = "idle";
	const string Shaking = "shaking";
	const string FrontEdge = "edge";
	const string LookUp = "lookUp";
	const string Walk = "walk";
	const string RunSlow = "runSlow";
	const string RunFast = "runFast";
	const string RunWind = "runWind";
	const string RunStumble = "runStumble";
	const string JumpSlow = "jumpSlow";
	const string FallSlow = "fallSlow";
	const string Fall = "fall";
	const string JumpFast = "jumpFast";
	const string FallFast = "fallFast";
	const string FallBig = "bigFall";
	const string LandInPose = "fallPose";
	const string Tired = "tired";
	const string TiredStill = "tiredStill";
	const string WallSlide = "wallslide";
	const string ClimbUp = "climbUp";
	const string ClimbDown = "climbDown";
	const string ClimbLookBackStart = "climbLookBackStart";
	const string ClimbLookBack = "climbLookBack";
	const string Dangling = "dangling";
	const string Duck = "duck";
	const string Dash = "dash";
	const string Sleep = "sleep";
	const string Sleeping = "asleep";
	const string Flip = "flip";
	const string Skid = "skid";
	const string DreamDashIn = "dreamDashIn";
	const string DreamDashLoop = "dreamDashLoop";
	const string DreamDashOut = "dreamDashOut";
	const string SwimIdle = "swimIdle";
	const string SwimUp = "swimUp";
	const string SwimDown = "swimDown";
	const string StartStarFly = "startStarFly";
	const string StarFly = "starFly";
	const string StarMorph = "starMorph";
	const string IdleCarry = "idle_carry";
	const string RunCarry = "runSlow_carry";
	const string JumpCarry = "jumpSlow_carry";
	const string FallCarry = "fallSlow_carry";
	const string PickUp = "pickup";
	const string Throw = "throw";
	const string Launch = "launch";
	const string TentacleGrab = "tentacle_grab";
	const string TentacleGrabbed = "tentacle_grabbed";
	const string TentaclePull = "tentacle_pull";
	const string TentacleDangling = "tentacle_dangling";
	const string SitDown = "sitDown";
#pragma endregion
public:
	int HairCount = 4;
	PlayerSpriteMode Mode;
private:
	string spriteName;
	class CPlayerScript* m_Owner;

public:
	string CurrentAnimationID;
	string LastAnimationID;
	CPlayerHairUpdate* Hair;
	PlayerSpriteMode GetMode() { return Mode; }
	Color Color;
	//Vec2 HairOffset();
	//bool HasHair();
	//int HairFrame();

	bool Running();
	bool DreamDashing();
	void Play(string _animid);

public:
	CPlayerSprite(CPlayerScript* _Owner, PlayerSpriteMode _mode);
	CPlayerSprite();
	~CPlayerSprite();
	
	friend CPlayerHairUpdate;
};

