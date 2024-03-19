#include "pch.h"
#include "AnimEditor.h"



namespace fs = std::filesystem;

std::vector<fs::path> getFilesInDirectory(const std::string& path) {
    std::vector<fs::path> fileList;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (entry.is_directory() || entry.path().extension() == ".jpg" || entry.path().extension() == ".png") {
            fileList.push_back(entry.path());
        }
    }
    return fileList;
}


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


    //==================== 1. Anim 선택기=====================

    ImVec2 pos(100, 100); // 적절한 위치에 콤보박스를 배치하세요.

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

            // 항목 선택 시 자동으로 스크롤
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

    //============애님위치============
    ImGui::SameLine();
    ImGui::Text((std::to_string(m_targetidx + 1) + "/" + std::to_string(items.size())).c_str());

    //============프레임위치============
    wstring tws;
    tws.assign(items[m_targetidx].begin(), items[m_targetidx].end());
    std::pair<wstring, CAnim*> targetpair = { tws , MapAnim.find(tws)->second };
    vector<tAnimFrm>& vecFrm = targetpair.second->GetVecFrm();
    ImGui::Text((std::to_string(m_curframe + 1) + "/" + std::to_string(vecFrm.size())).c_str());


    //============이전프레임============
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
    //============다음프레임============
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

    //============재생control============
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
        const float minValue = 0.01f; // 최소값
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


    //============프레임삭제버튼============
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

    //==================== 2. 재생기=====================
    ImGui::BeginChild("Animation Veiw", ImVec2(500, 500), true);

    //이부분 구조개선필요
    auto srv = targetpair.second->m_AtlasTex->GetSRV();
    auto imimid = (ImTextureID)srv.Get();
    auto aw = (float)targetpair.second->m_AtlasTex->GetWidth();
    auto ah = (float)targetpair.second->m_AtlasTex->GetHeight();

    if (0 != vecFrm.size())
    {


        auto slice = ImVec2(vecFrm[m_curframe].vSlice.x, vecFrm[m_curframe].vSlice.y);
        auto slicesize = ImVec2(300, int(slice.y * (float(300) / float(slice.x))));
        auto lt = vecFrm[m_curframe].vLeftTop;
        auto uv = ImVec2(lt.x / aw, lt.y / ah);

        // 가운데 정렬을 위한 여백 계산
        //float window_centerx = ImGui::GetWindowSize().x / 3.0f;
        //float window_centery = ImGui::GetWindowSize().y / 3.0f;
        //float image_halfx = targetpair.second->m_AtlasTex.Get()->GetWidth()* slice.x / 2.0f;
        //float image_halfy = targetpair.second->m_AtlasTex.Get()->GetHeight() * slice.y / 2.0f;
        //ImGui::SetCursorPosX(window_centerx - image_halfx);
        //ImGui::SetCursorPosY(window_centery - image_halfy);

        ImGui::SetCursorPosX(80);
        ImGui::SetCursorPosY(60);


        ImGui::Image(imimid, slicesize, ImVec2(lt.x, lt.y), ImVec2(lt.x, lt.y) + slice);
    }
    ImGui::EndChild();

    //==================== 3. sprite view=====================
    ImGui::SameLine();
    ImGui::BeginChild("Sprite View", ImVec2(ImGui::GetWindowSize().x - 550, 500), true);

    if (0 == m_selectedfrm.size())
    {
        m_selectedfrm.resize(vecFrm.size(), false);
    }

    float windowWidth = ImGui::GetContentRegionAvail().x - 50; // 현재 창의 가용 너비
    float imageWidth = 100; // 예시 이미지 너비
    float spaceBetweenImages = 10; // 이미지 사이의 간격
    float currentPosition = 0; // 현재 이미지를 그리기 시작할 x 위치

    for (int i = 0; i < vecFrm.size(); ++i) {

        auto slice = ImVec2(vecFrm[i].vSlice.x, vecFrm[i].vSlice.y);
        int w = 100;
        auto slicesize = ImVec2(w, int(slice.y * (float(w) / float(slice.x))));
        auto lt = vecFrm[i].vLeftTop;
        auto uv = ImVec2(lt.x / aw, lt.y / ah);

        if (currentPosition + imageWidth > windowWidth) {
            // 가용 너비를 초과할 경우, 새로운 라인에서 시작
            ImGui::NewLine();
            currentPosition = 0;
        }

        ImGui::PushID(i);
        if (ImGui::ImageButton(imimid, slicesize, ImVec2(lt.x, lt.y), ImVec2(lt.x, lt.y) + slice))
        {
            m_selectedfrm[i] = !m_selectedfrm[i];
        }
        ImGui::PopID();

        if (m_selectedfrm[i]) {
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
        }

        ImGui::SameLine();


        targetpair.second->SetVecFrm(vecFrm);

        currentPosition += imageWidth + spaceBetweenImages; // 다음 이미지 위치 업데이트

    }
    ImGui::EndChild();

    //==================== 4. 탐색기=====================
    //ImGui::BeginChild("Explorer", ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y- 500), true);

    ////경로

    //CPathMgr::GetContentPath();
    //char buffer[256] = "C:\\DirectXJ\\OutputFile\\content\\texture\\Atlases\\Gameplay\\characters\\player";
    //ImGui::InputText("##path", buffer, IM_ARRAYSIZE(buffer));
    //ImGui::NewLine();
    //
    //auto fileList = getFilesInDirectory(buffer);

    //static fs::path base_path = "C:\\DirectXJ\\OutputFile\\content";

    //windowWidth = ImGui::GetContentRegionAvail().x; // 현재 창의 가용 너비
    //imageWidth = 100; // 예시 이미지 너비
    //spaceBetweenImages = 10; // 이미지 사이의 간격
    //currentPosition = 0; // 현재 이미지를 그리기 시작할 x 위치


    //// 파일 목록을 바탕으로 이미지 버튼 출력
    //for (const auto& filePath : fileList) {
    //    if (filePath.extension() == ".jpg" || filePath.extension() == ".png") {
    //        fs::path relative_path = fs::relative(filePath, base_path);
    //        Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(relative_path, relative_path);

    //        if (currentPosition + imageWidth > windowWidth) {
    //            // 가용 너비를 초과할 경우, 새로운 라인에서 시작
    //            ImGui::NewLine();
    //            currentPosition = 0;
    //        }

    //        auto srv2 = pAltasTex->GetSRV();
    //        auto imimid2 = (ImTextureID)srv2.Get();
    //        ImGui::SameLine();
    //        if (ImGui::ImageButton(imimid2, ImVec2(100, 100)))
    //        {
    //            // 버튼 클릭 시 이벤트 처리...
    //        }
    //    }
    //    else { // 폴더인 경우
    //        if (ImGui::Button(filePath.filename().string().c_str(), ImVec2(100, 100))) {
    //            // 폴더 클릭 시 이벤트 처리...
    //        }
    //        ImGui::SameLine();
    //    }

    //    currentPosition += imageWidth + spaceBetweenImages; // 다음 이미지 위치 업데이트
    //}


    //ImGui::EndChild();

    //============curframe순회,반복============
    if (m_isPlaying && !vecFrm.empty())
    {
        m_acctime += ImGui::GetIO().DeltaTime;

        // 현재 프레임의 Duration 이상 경과하면 다음 프레임으로 이동
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
