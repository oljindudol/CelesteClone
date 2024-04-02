#pragma once

#include "CRenderComponent.h"
#include <unordered_map>

struct tPlayerHairInfo
{
	Vec2 HairOffset;
	//¾Õ¸Ó¸® Frame
	int  BangFrame;
	bool HasHair;
	tPlayerHairInfo() : HairOffset(Vec2()), BangFrame(0), HasHair(true) {}
};

struct tHairNode
{
	Vec2 vOffset;
	Vec2 vScale;
	tHairNode() :vOffset(Vec2(0, 0)), vScale(Vec2(0, 0)){}
};

struct tRenderInfo
{
	vector< tHairNode> vecHairNodes;
	bool	HasHair;
	int		HairCnt;
	int		ThisFrameBangIdx;
	Color   HairColor;
	Color   BorderColor;
	Facings facing;
	bool	BangOnePixelDown;
};

class CHair
	:public CRenderComponent
{
private:
	std::unordered_map<string, tPlayerHairInfo> m_umHairInfo;
	Ptr<CTexture> m_HairTex;
	vector<Ptr<CTexture>> m_vecBangTex;
	bool DrawPlayerSpriteOutline;


private:
	tRenderInfo m_RenderInfo;
	void RenderHair(Color Color, Vec3 offset = Vec3(0, 0, 0));

public:
	void SetMetaData();
	tPlayerHairInfo GettHairInfo();
	auto& GetRenderInfoRef() { return m_RenderInfo; }
	auto& GetRenderInfo() { return m_RenderInfo; }
	void SetRenderInfo(tRenderInfo _RenderInfo) { m_RenderInfo = _RenderInfo; }

	void SetHairColor(const Color& _HairColor) { m_RenderInfo.HairColor= _HairColor;}
	//void SetThisFrameBangIdx(int _ThisFrameBangIdx) { m_RenderInfo.ThisFrameBangIdx = _ThisFrameBangIdx; }
	//void SetBorderColor(const Color& _BorderColor) { m_RenderInfo.BorderColor = _BorderColor; }
	//void SetHairCnt(int _HairCnt) { m_RenderInfo.HairCnt = _HairCnt; }

	virtual void finaltick() override;
	virtual void UpdateData() override;
	virtual void render() override;
	CLONE(CHair);
	CHair();
	~CHair();


};

