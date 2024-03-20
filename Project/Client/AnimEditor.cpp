#include "pch.h"
#include "AnimEditor.h"



AnimEditor::AnimEditor()
    : UI("Edit Anim", "##AnimEditor")
    , m_TargetGO(nullptr)
    , m_targetidx(0)
{
    Deactivate();
    SetModal(true);
}


void AnimEditor::render_update()
{


    //==================== 1. Anim ���ñ�=====================

    ImVec2 pos(100, 100); // ������ ��ġ�� �޺��ڽ��� ��ġ�ϼ���.

    auto MapAnim = ((CAnimator2D*)m_TargetGO->GetComponent(COMPONENT_TYPE::ANIMATOR2D))->GetMapAnim();

    vector<string> items;


    for (auto& anim : MapAnim)
    {
        string s;
        s.assign(anim.first.begin(), anim.first.end());
        items.push_back(s);
    }


    ImGui::SetNextWindowPos(pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextItemWidth(150);
    if (ImGui::BeginCombo("##combo", items[m_targetidx].c_str())) {
        for (int i = 0; i < items.size(); i++) {
            const bool isSelected = (m_targetidx == i);
            if (ImGui::Selectable(items[i].c_str(), isSelected)) {
                m_targetidx = i;
            }

            // �׸� ���� �� �ڵ����� ��ũ��
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
                initialize();
            }
        }
        ImGui::EndCombo();
    }


    ImGui::SameLine();
    if (ImGui::ArrowButton("##left", ImGuiDir_Left)) {
        m_isPlaying = true;
        if (0 < m_targetidx)
        {
            --m_targetidx;
        }
        else
        {
            m_targetidx = items.size() - 1;
        }

        initialize();
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##right", ImGuiDir_Right)) {
        m_isPlaying = true;
        if ((items.size() - 1) > m_targetidx)
        {
            ++m_targetidx;
        }
        else
        {
            m_targetidx = 0;
        }

        initialize();
    }

    //============�ִ���ġ============
    ImGui::SameLine();
    ImGui::Text((std::to_string(m_targetidx + 1) + "/" + std::to_string(items.size())).c_str());

    //============��������ġ============
    wstring tws;
    tws.assign(items[m_targetidx].begin(), items[m_targetidx].end());
    std::pair<wstring, CAnim*> targetpair = { tws , MapAnim.find(tws)->second };
    vector<tAnimFrm>& vecFrm = targetpair.second->GetVecFrm();
    ImGui::Text((std::to_string(m_curframe + 1) + "/" + std::to_string(vecFrm.size())).c_str());


    //============����������============
    ImGui::SameLine();
    if (ImGui::Button("Prev##framePrev"))
    {
        m_isPlaying = false;
        if (0 < m_curframe)
        {
            --m_curframe;
        }
        else
        {
            m_curframe = targetpair.second->GetVecFrm().size() - 1;
        }
    }
    //============����������============
    ImGui::SameLine();
    if (ImGui::Button("Next##frameNext"))
    {
        m_isPlaying = false;
        if (vecFrm.size() - 1 > m_curframe)
        {
            ++m_curframe;
        }
        else
        {
            m_curframe = 0;
        }
    }

    //============���control============
    string tstr;
    if (m_isPlaying)
    {
        tstr = "Stop";
    }
    else
    {
        tstr = "Play";
    }
    ImGui::SameLine();
    if (ImGui::Button(tstr.c_str()))
    {
        m_isPlaying = !m_isPlaying;
    }
    //============duration============
    if (0 != vecFrm.size())
    {
        float dur = vecFrm[m_curframe].Duration;
        ImGui::Text("Duration");
        ImGui::SameLine();
        const float minValue = 0.01f; // �ּҰ�
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("##Duration", &dur, 0.1f, 1.f, "%.2f");
        if (dur < minValue) dur = minValue;
        vecFrm[m_curframe].Duration = dur;
    }



    //============offset============
    if (0 < vecFrm.size())
    {
        Vec2 ofs = vecFrm[m_curframe].vOffset;
        float ofx = ofs.x;
        float ofy = ofs.y;
        ImGui::SameLine();
        ImGui::Text("Offsetx");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("##Offx", &ofx, 0.1f, 1.f, "%.2f");
        ImGui::SameLine();
        ImGui::Text("Offsety");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputFloat("##Offy", &ofy, 0.1f, 1.f, "%.2f");
        vecFrm[m_curframe].vOffset = Vec2(ofx, ofy);
    }


    //============�����ӻ�����ư============
    ImGui::SameLine();
    if (ImGui::Button("Delete Selected Frame"))
    {
        for (int i = (m_selectedfrm.size() - 1); i >= 0; --i)
        {
            if (m_selectedfrm[i])
            {
                vecFrm.erase(vecFrm.begin() + i);
            }
        }
        initialize();
    }

    //==================== 2. �����=====================
    auto winsize = ImVec2(500, 500);
    ImGui::BeginChild("Animation Veiw", winsize, true);

    if (0 != vecFrm.size())
    {
        //���Ϲ������ ����
        auto srv = vecFrm[m_curframe].pFrameTex->GetSRV();
        auto imimid = (ImTextureID)srv.Get();
        auto aw = (float)vecFrm[m_curframe].pFrameTex->GetWidth();
        auto ah = (float)vecFrm[m_curframe].pFrameTex->GetHeight();

        auto imgsize = ImVec2(vecFrm[m_curframe].pFrameTex.Get()->GetWidth(), vecFrm[m_curframe].pFrameTex.Get()->GetHeight());
        ImVec2 slicesize;
        if (imgsize.x > imgsize.y)
        {
            slicesize = ImVec2(300, int(imgsize.y * (300.f / float(imgsize.x))));
        }
        else
        {
            slicesize = ImVec2(int(imgsize.x * (300.f / float(imgsize.y))), 300);
        }
        
        //auto lt = vecFrm[m_curframe].vLeftTop;
        //auto uv = ImVec2(lt.x / aw, lt.y / ah);

        // ��� ������ ���� ���� ���
        float window_centerx = ImGui::GetContentRegionAvail().x / 2.0f;
        float window_centery = ImGui::GetContentRegionAvail().y / 2.0f;
        float image_halfx = slicesize.x / 2.0f;
        float image_halfy = slicesize.y / 2.0f;
        ImGui::SetCursorPosX(window_centerx - image_halfx);
        ImGui::SetCursorPosY(window_centery - image_halfy);

        //ImGui::SetCursorPosX(100);
        //ImGui::SetCursorPosY(100);

        ImGui::Image(imimid, slicesize,  ImVec2(0.f, 0.02f), ImVec2(1.f, 1.f));
    }
    ImGui::EndChild();

    //==================== 3. sprite view=====================
    ImGui::SameLine();
    ImGui::BeginChild("Sprite View", ImVec2(ImGui::GetWindowSize().x - 550, 500), true);

    if (0 == m_selectedfrm.size())
    {
        m_selectedfrm.resize(vecFrm.size(), false);
    }

    float windowWidth = ImGui::GetContentRegionAvail().x - 50; // ���� â�� ���� �ʺ�
    float imageWidth = 100; // ���� �̹��� �ʺ�
    float spaceBetweenImages = 10; // �̹��� ������ ����
    float currentPosition = 0; // ���� �̹����� �׸��� ������ x ��ġ

    for (int i = 0; i < vecFrm.size(); ++i) {
        auto srv = vecFrm[i].pFrameTex->GetSRV();
        auto imimid = (ImTextureID)srv.Get();
        auto aw = (float)vecFrm[i].pFrameTex->GetWidth();
        auto ah = (float)vecFrm[i].pFrameTex->GetHeight();


        auto slice = ImVec2(vecFrm[i].vSlice.x, vecFrm[i].vSlice.y);
        int w = 100;
        auto slicesize = ImVec2(w, int(slice.y * (float(w) / float(slice.x))));
        auto lt = vecFrm[i].vLeftTop;
        auto uv = ImVec2(lt.x / aw, lt.y / ah);

        if (currentPosition + imageWidth > windowWidth) {
            // ���� �ʺ� �ʰ��� ���, ���ο� ���ο��� ����
            ImGui::NewLine();
            currentPosition = 0;
        }
        
        ImGui::PushID(i);
        if (ImGui::ImageButton(imimid, slicesize, ImVec2(0.f, 0.02f), ImVec2(1.f, 1.f)))
        {
            m_selectedfrm[i] = !m_selectedfrm[i];
        }
        ImGui::PopID();

        if (m_selectedfrm[i]) {
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
        }

        ImGui::SameLine();


        targetpair.second->SetVecFrm(vecFrm);

        currentPosition += imageWidth + spaceBetweenImages; // ���� �̹��� ��ġ ������Ʈ

    }
    ImGui::EndChild();

    //==================== 4. Ž����=====================
    //ImGui::BeginChild("Explorer", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y- 500), true);

    ////���

    //CPathMgr::GetContentPath();
    //char buffer[256] = "C:\\DirectXJ\\OutputFile\\content\\texture\\Atlases\\Gameplay\\characters\\player";
    //ImGui::InputText("##path", buffer, IM_ARRAYSIZE(buffer));
    //ImGui::NewLine();
    //
    //auto fileList = getFilesInDirectory(buffer);

    //static fs::path base_path = "C:\\DirectXJ\\OutputFile\\content";

    //windowWidth = ImGui::GetContentRegionAvail().x; // ���� â�� ���� �ʺ�
    //imageWidth = 100; // ���� �̹��� �ʺ�
    //spaceBetweenImages = 10; // �̹��� ������ ����
    //currentPosition = 0; // ���� �̹����� �׸��� ������ x ��ġ


    //// ���� ����� �������� �̹��� ��ư ���
    //for (const auto& filePath : fileList) {
    //    if (filePath.extension() == ".jpg" || filePath.extension() == ".png") {
    //        fs::path relative_path = fs::relative(filePath, base_path);
    //        Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);

    //        if (currentPosition + imageWidth > windowWidth) {
    //            // ���� �ʺ� �ʰ��� ���, ���ο� ���ο��� ����
    //            ImGui::NewLine();
    //            currentPosition = 0;
    //        }

    //        auto srv2 = pAltasTex->GetSRV();
    //        auto imimid2 = (ImTextureID)srv2.Get();
    //        ImGui::SameLine();
    //        if (ImGui::ImageButton(imimid2, ImVec2(100, 100)))
    //        {
    //            // ��ư Ŭ�� �� �̺�Ʈ ó��...
    //        }
    //    }
    //    else { // ������ ���
    //        if (ImGui::Button(filePath.filename().string().c_str(), ImVec2(100, 100))) {
    //            // ���� Ŭ�� �� �̺�Ʈ ó��...
    //        }
    //        ImGui::SameLine();
    //    }

    //    currentPosition += imageWidth + spaceBetweenImages; // ���� �̹��� ��ġ ������Ʈ
    //}


    //ImGui::EndChild();

    //============curframe��ȸ,�ݺ�============
    if (m_isPlaying && !vecFrm.empty())
    {
        m_acctime += ImGui::GetIO().DeltaTime;

        // ���� �������� Duration �̻� ����ϸ� ���� ���������� �̵�
        if (m_acctime >= vecFrm[m_curframe].Duration)
        {
            m_acctime = 0.0f;

            if ((int)vecFrm.size() - 1 > m_curframe)
            {
                ++m_curframe;
            }
            else
            {
                m_curframe = 0;
            }
        }
    }

}



AnimEditor::~AnimEditor()
{


}
