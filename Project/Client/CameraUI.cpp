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

    // ���� ���
    PROJ_TYPE ProjType = pCamera->GetProjType();

    PROJ_TYPE::ORTHOGRAPHIC;
    PROJ_TYPE::PERSPECTIVE;

    PROJ_TYPE ProjTypeTo = ProjType;

    ImVec4 Red = ImVec4(1.f, 0.f, 0.f, 1.0f);
    ImVec4 Green = ImVec4(0.f, 1.f, 0.f, 1.0f);
    ImVec4 Blue = ImVec4(0.f, 0.f, 1.f, 1.0f);
    ImVec4 Gray = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

    //����
    if (PROJ_TYPE::ORTHOGRAPHIC == ProjType)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Red); // �⺻ ����
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // ȣ�� ������ �⺻ ������ �����ϰ� ����
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("ORTHOGRAPHIC"))
        {
            ProjTypeTo = PROJ_TYPE::ORTHOGRAPHIC;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, Gray); // �⺻ ����
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // ȣ�� ������ �⺻ ������ �����ϰ� ����
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("PERSPECTIVE"))
        {
            ProjTypeTo = PROJ_TYPE::PERSPECTIVE;
        }
        ImGui::PopStyleColor(3);


        const static float min = 0.00001f;

        //width ����
        //float width = pCamera->GetWidth();
        //ImGui::Text("Width:");
        //ImGui::SameLine();
        //ImGui::DragFloat("##Widthinput", &width, 0.01f, 0.01f);
        //if (min > width)
        //{
        //    width = min;
        //}
        //pCamera->SetWidth(width);

        //scale ����
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
    //����
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, Gray); // �⺻ ����
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // ȣ�� ������ �⺻ ������ �����ϰ� ����
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("ORTHOGRAPHIC"))
        {
            ProjTypeTo = PROJ_TYPE::ORTHOGRAPHIC;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        
        ImGui::PushStyleColor(ImGuiCol_Button, Red); // �⺻ ����
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Red); // ȣ�� ������ �⺻ ������ �����ϰ� ����
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, Red);
        if (ImGui::Button("PERSPECTIVE"))
        {
            ProjTypeTo = PROJ_TYPE::PERSPECTIVE;
        }
        ImGui::PopStyleColor(3);

        //FOV����
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

    //========== ī�޶� visibility���� =============
    ImGui::Text("<Layer Visibility>");
    ImGui::BeginChild("Layer Veiw", winsize, true);
    auto CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    auto LayerCheckOrigin = pCamera->GetLayerCheck();
    UINT LayerCheckNext = LayerCheckOrigin;

    //int = layer idx
    vector<std::pair<int, string>> ButtonName;

    //���� for���� �� button �� ��ȸó��
    //(���ǿ� �´� ���Ҹ� vector�� �ѹ��� ����ش�)
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
