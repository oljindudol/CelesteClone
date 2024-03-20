#include "pch.h"
#include "CameraUI.h"


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

    PROJ_TYPE               m_ProjType;     // ���� ���

    // ��������(Perspective)
    float                   m_FOV;          // �þ� ��(Filed Of View)

    // ��������(Orthographic)
    float                   m_Width;        // �������� ���� ����
    float                   m_Scale;        // �������� ����

    // Both
    float                   m_AspectRatio;  // ��Ⱦ��, ���� ���μ��� ����
    float                   m_Far;          // ���� �ִ� �Ÿ�

    // ��ȯ ���
    Matrix                  m_matView;
    Matrix                  m_matProj;

    UINT                    m_LayerCheck;

    int                     m_CameraPriority;


    // ��ü �з�
    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMaked;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;
}
