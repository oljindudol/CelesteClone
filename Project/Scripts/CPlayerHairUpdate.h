#pragma once

class CPlayerSprite;

class CPlayerHairUpdate
{
public:
	void Start();
	void Update();
	void AfterUpdate();
	Vec2 GetHairScale(int index);
	bool SimulateMotion = true;
	float StepInFacingPerSegment = 0.5f;
	float StepApproach = 64.f;
	float StepYSinePerSegment;
	float wave;
	Vec2 StepPerSegment = Vec2(0.f, 2.f);
	CPlayerSprite* Sprite;
	class CHair* Hair;
	Facings facing;
	Color Color;

public:
	CPlayerHairUpdate() = delete;
	CPlayerHairUpdate(CPlayerSprite* _Sprite , CHair* _Hair);
	~CPlayerHairUpdate();
};

