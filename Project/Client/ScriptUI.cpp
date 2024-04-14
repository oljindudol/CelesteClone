#include "pch.h"
#include "ScriptUI.h"

#include <Scripts/CScriptMgr.h>

#include "ParamUI.h"

ScriptUI::ScriptUI()
	: ComponentUI("Script", "##Script", COMPONENT_TYPE::LIGHT2D)
	, m_TargetScript(nullptr)
{
	SetSize(ImVec2(0,300));
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	wstring strScriptName = CScriptMgr::GetScriptName(_Script);
	SetName(ToString(strScriptName));
	SetComopnentTitle(ToString(strScriptName));
	Activate();
}

void ScriptUI::render_update()
{
	ComponentUI::render_update();

	const auto& umParam = m_TargetScript->GetScriptParam();

	
	//for (size_t i = 0; i < vecParam.size(); ++i)
	for (auto& p : umParam)
	{
		auto t = p.second;
		switch (t.Type)
		{
		case SCRIPT_PARAM::INT:
			ParamUI::Param_INT((int*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::FLOAT:
			ParamUI::Param_FLOAT((float*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::VEC2:
			ParamUI::Param_VEC2((Vec2*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::VEC3:
			break;
		case SCRIPT_PARAM::VEC4:
			ParamUI::Param_VEC4((Vec4*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::OBJECT:
			break;		
		case SCRIPT_PARAM::STRING:
			ParamUI::Param_STRING((string*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::INT_READONLY:
			ParamUI::Param_INT_READONLY((int*)t.pData, p.first);
			break;
		case SCRIPT_PARAM::BOOL:
			ParamUI::Param_BOOL((bool*)t.pData, p.first);
			break;
		}		
	}
}