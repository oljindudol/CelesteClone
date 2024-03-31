#pragma once

#include "CRenderComponent.h"
#include <unordered_map>

struct tPlayerHairInfo
{
	Vec2 HairOffset;
	//¾Õ¸Ó¸® Frame
	int  BangFrame;
	bool HasHair;
	tPlayerHairInfo() : HairOffset(Vec2()), BangFrame(-1), HasHair(true) {}
};

struct tHairNode
{
	Vec2 vOffset;
	Vec2 vScale;
	tHairNode() :vOffset(Vec2(0, 0)), vScale(Vec2(0, 0)) {}
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
	int m_ThisFrameBangIdx;
	vector<tHairNode> m_HairNodes;
	Color m_HairColor;
	Color m_BorderColor;
	int m_HairCnt;

public:
	void SetThisFrameBangIdx(int _ThisFrameBangIdx) { m_ThisFrameBangIdx = _ThisFrameBangIdx; }
	void SetMetaData();
	tPlayerHairInfo GettHairInfo(string& _Key) { return m_umHairInfo[_Key]; }
	vector<tHairNode>& GetHairNodesRef() { return m_HairNodes; }

	void SetHairColor(const Color& _HairColor) {m_HairColor= _HairColor;}
	void SetBorderColor(const Color& _BorderColor) { m_BorderColor = _BorderColor; }
	void SetHairCnt(int _HairCnt) { m_HairCnt = _HairCnt; }
	int GetHairCnt() { return m_HairCnt; }

	virtual void finaltick() override;
	virtual void UpdateData() override;
	virtual void render() override;
	CLONE(CHair);
	CHair();
	~CHair();


};

