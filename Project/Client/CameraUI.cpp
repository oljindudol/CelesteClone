#include "pch.h"
#include "CameraUI.h"


#include <Engine/CAssetMgr.h>
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>


#include "CImGuiMgr.h"
#include "ListUI.h"
#include "Inspector.h"

CameraUI::CameraUI()
	: ComponentUI("Camera", "##Camera", COMPONENT_TYPE::CAMERA)
{
	SetSize(ImVec2(0.f, 600.f));
	SetComopnentTitle("Camera");

}

CameraUI::~CameraUI()
{
}



void CameraUI::render_update()
{

    ComponentUI::render_update();

    CGameObject* pTarget = GetTargetObject();

    CCamera* pCamera = pTarget->Camera();

    // 투영 방식
    PROJ_TYPE ProjType = pCamera->GetProjType();

    PROJ_TYPE::ORTHOGRAPHIC;
    PROJ_TYPE::PERSPECTIVE;

    PROJ_TYPE ProjTypeTo = ProjType;

    ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.0f);
    ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.0f);
    ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.0f);
    ImVec4 Gray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    //직교
    if (PROJ_TYPE::ORTHOGRAPHIC == ProjType)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Red); // 기본 배경색
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("ORTHOGRAPHIC"))
        {
            ProjTypeTo = PROJ_TYPE::ORTHOGRAPHIC;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Gray); // 기본 배경색
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("PERSPECTIVE"))
        {
            ProjTypeTo = PROJ_TYPE::PERSPECTIVE;
        }
        ImGui::PopStyleColor(3);


        const static float min = 0.00001f;

        //width 조절
        //float width = pCamera->GetWidth();
        //ImGui::Text("Width:");
        //ImGui::SameLine();
        //ImGui::DragFloat("##Widthinput", &width, 0.01f, 0.01f);
        //if (min > width)
        //{
        //    width = min;
        //}
        //pCamera->SetWidth(width);

        //scale 조절
        float scale = pCamera->GetScale();
        ImGui::Text("Scale:");
        ImGui::SameLine();
        ImGui::DragFloat("##Scaleinput", &scale, 0.01f, 0.01f);

        if (min > scale)
        {
            scale = min;
        }
        pCamera->SetScale(scale);

    }
    //원근
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Gray); // 기본 배경색
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("ORTHOGRAPHIC"))
        {
            ProjTypeTo = PROJ_TYPE::ORTHOGRAPHIC;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        
        ImGui::PushStyleColor(ImGuiCol_Button, Red); // 기본 배경색
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // 호버 배경색도 기본 배경색과 동일하게 설정
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("PERSPECTIVE"))
        {
            ProjTypeTo = PROJ_TYPE::PERSPECTIVE;
        }
        ImGui::PopStyleColor(3);

        //FOV조절
        float FOV = pCamera->GetFOV();
        ImGui::Text("FOV:");
        ImGui::SameLine();
        ImGui::DragFloat("##FOVinput", &FOV, 0.01f, 0.01f);

        float Fovmin = 0.0001f;

        if (Fovmin > FOV)
        {
            FOV = Fovmin;
        }
        pCamera->SetFOV(FOV);

    }
    pCamera->SetProjType(ProjTypeTo);
    

    int  m_CameraPriority = pCamera->GetCameraPriority();

    ImGui::Text(("CameraPriority:" + std::to_string(m_CameraPriority)).c_str());

    const static auto winsize = ImVec2(0, 0);

    ImGui::NewLine();

    //========== 카메라 visibility설정 =============
    ImGui::Text("<Layer Visibility>");
    ImGui::BeginChild("Layer Veiw", winsize, true);
    auto CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    auto LayerCheckOrigin = pCamera->GetLayerCheck();
    UINT LayerCheckNext = LayerCheckOrigin;

    //int = layer idx
    vector<std::pair<int, string>> ButtonName;

    //이중 for문이 들어간 button 을 우회처리
    //(조건에 맞는 원소를 vector에 한번더 담아준다)
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        auto Layer = CurLevel->GetLayer(i);
        auto& LayerName = Layer->GetName();
        auto layersize = Layer->GetLayerObjects().size();

        string LayerNameStr;

        if ("" != LayerName)
        {
            LayerNameStr.assign(LayerName.begin(), LayerName.end());
        }
        else
        {
            if (0 == layersize)
            {
                continue;
            }
            LayerNameStr = "Layer" + std::to_string(i);
        }
        string ButtonNameStr = std::to_string(i) + ":" + LayerNameStr + "(" + std::to_string(layersize) + ")";
        ButtonName.push_back({ i,ButtonNameStr });
    }

    for (auto& b : ButtonName)
    {
        int i = b.first;

        auto color = Blue;
        bool LCK = LayerCheckOrigin & (1 << i);


       if (false == LCK)
        {
           color = Gray;
        }

       ImGui::PushStyleColor(ImGuiCol_Button, color);
       if (ImGui::Button(b.second.c_str()))
       {
           LayerCheckNext = LayerCheckNext ^ (1 << i);
       }
       ImGui::PopStyleColor();

    }
    pCamera->SetLayerCheck(LayerCheckNext);
    ImGui::EndChild();
}
