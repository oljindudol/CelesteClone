#include "pch.h"
#include "PathMgr.h"

#include <fstream>
#include <iostream>
using std::ofstream;
vector<wstring> g_vecName;
vector<wstring> g_vecStateNames;

void MakeStateMgrHeader(wstring strHeaderPath);
void MakeStateMgrCPP();
void NameInput() {
	wstring solPath = CPathMgr::GetProjectPath();
	wstring filterPath = solPath + L"..\\Project\\Scripts\\Scripts.vcxproj.filters";

	wifstream fin;
	fin.open(filterPath);
	wstring line;
	wstring header;
	bool isheader = false;
	wcout << L"Script!!=======================" << endl;
	while (getline(fin, line))
	{

		if (line.find(L".cpp") != string::npos) {
			isheader = false;
			continue;
		}

		if (line.find(L".h") != string::npos) {
			int start = line.find(L"\"");
			int end = line.find(L".");

			header = line.substr(start + 1, end - start - 1);
			isheader = true;
		}

		if (line.find(L"<Filter>02. Scripts") != string::npos) {
			if (isheader) {
				wcout << header << endl;
				g_vecName.push_back(header);
			}
		}

	}
}

void StateNameInput() {
	wstring solPath = CPathMgr::GetProjectPath();
	wstring filterPath = solPath + L"..\\Project\\Scripts\\Scripts.vcxproj.filters";

	wifstream fin;
	fin.open(filterPath);
	wstring line;
	wstring header;
	bool isheader = false;
	wcout <<endl <<  L"States!!=======================" << endl;
	while (getline(fin, line))
	{

		if (line.find(L".cpp") != string::npos) {
			isheader = false;
			continue;
		}

		if (line.find(L".h") != string::npos) {
			int start = line.find(L"\"");
			int end = line.find(L".");

			header = line.substr(start + 1, end - start - 1);
			isheader = true;
		}

		if (line.find(L"<Filter>08. States") != string::npos) {
			if (isheader) {
				wcout << header << endl;
				g_vecStateNames.push_back(header);
			}
		}

	}
	getchar();
}

int main()
{
	CPathMgr::init();
	wstring strProjPath = CPathMgr::GetProjectPath();
	wstring strCppPath = strProjPath + L"\\Scripts\\CScriptMgr.cpp";
	wstring strHeaderPath = strProjPath + L"\\Scripts\\CScriptMgr.h";
	wstring strStateCppPath = strProjPath + L"\\Scripts\\CStateMgr.cpp";
	wstring strStateHeaderPath = strProjPath + L"\\Scripts\\CStateMgr.h";

	// 1. 현재 존재하는 모든 스크립트를 알아내야함.
	wstring strScriptIncludePath = CPathMgr::GetIncludePath();
	wstring strScriptCode = strScriptIncludePath + L"Scripts\\";

	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);


	NameInput();
	StateNameInput();

	MakeStateMgrHeader(strStateHeaderPath);
	MakeStateMgrCPP();
	if (INVALID_HANDLE_VALUE == handle)
		return 0;

	// 예외 리스트 목록을 알아낸다.
	FILE* pExeptList = nullptr;
	_wfopen_s(&pExeptList, L"exeptlist.txt", L"r");

	vector<wstring> strExept;

	if (nullptr != pExeptList)
	{
		wchar_t szName[255] = L"";

		while (true)
		{
			int iLen = fwscanf_s(pExeptList, L"%s", szName, 255);
			if (iLen == -1)
				break;

			strExept.push_back(szName);
		}
		fclose(pExeptList);
	}


	FindClose(handle);


	FILE* pFile = NULL;

	//=================
	// ScriptMgr h 작성
	//=================
	_wfopen_s(&pFile, strHeaderPath.c_str(), L"w");
	fwprintf_s(pFile, L"#pragma once\n\n");
	fwprintf_s(pFile, L"#include <vector>\n");
	fwprintf_s(pFile, L"#include <string>\n\n");



	fwprintf_s(pFile, L"enum class SCRIPT_TYPE\n{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, L"\t");
		fwprintf_s(pFile, strScriptUpperName.c_str());
		fwprintf_s(pFile, L",\n");
	}
	fwprintf_s(pFile, L"\tEND,\n");
	fwprintf_s(pFile, L"};\n\n");


	fwprintf_s(pFile, L"using namespace std;\n\n");
	fwprintf_s(pFile, L"class CScript;\n\n");

	fwprintf_s(pFile, L"class CScriptMgr\n{\n");
	fwprintf_s(pFile, L"public:\n\tstatic void GetScriptInfo(vector<wstring>& _vec);\n");
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(const wstring& _strScriptName);\n");
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(UINT _iScriptType);\n");
	fwprintf_s(pFile, L"\tstatic const wchar_t * GetScriptName(CScript * _pScript);\n};\n");


	fclose(pFile);


	//====================
	// ScriptMgr cpp 작성
	//====================
	_wfopen_s(&pFile, strCppPath.c_str(), L"w");

	// 헤더 입력
	fwprintf_s(pFile, L"#include \"pch.h\"\n");
	fwprintf_s(pFile, L"#include \"CScriptMgr.h\"\n\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"#include \"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L".h\"\n");
	}

	// 첫 번째 함수 작성
	fwprintf_s(pFile, L"\nvoid CScriptMgr::GetScriptInfo(vector<wstring>& _vec)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\t_vec.push_back(L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\");\n");
	}
	fwprintf_s(pFile, L"}\n\n");


	// 두번째 함수 작성
	fwprintf_s(pFile, L"CScript * CScriptMgr::GetScript(const wstring& _strScriptName)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tif (L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\" == _strScriptName)\n");
		fwprintf_s(pFile, L"\t\treturn new ");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L";\n");
	}
	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");


	// 세번째 함수
	fwprintf_s(pFile, L"CScript * CScriptMgr::GetScript(UINT _iScriptType)\n{\n");

	fwprintf_s(pFile, L"\tswitch (_iScriptType)\n\t{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, L"\tcase (UINT)SCRIPT_TYPE::");
		fwprintf_s(pFile, strScriptUpperName.c_str());
		fwprintf_s(pFile, L":\n");

		fwprintf_s(pFile, L"\t\treturn new ");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L";\n");
		
		fwprintf_s(pFile, L"\t\tbreak;\n");
	}

	fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}\n\n");

	// 네번째 함수
	fwprintf_s(pFile, L"const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)\n{\n");
	fwprintf_s(pFile, L"\tswitch ((SCRIPT_TYPE)_pScript->GetScriptType())\n\t{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tcase SCRIPT_TYPE::");

		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, strScriptUpperName.c_str());

		fwprintf_s(pFile, L":\n\t\treturn ");
		fwprintf_s(pFile, L"L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\";\n\t\tbreak;\n\n");
	}

	fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}");

	fclose(pFile);


	// 다음번 실행시 비교하기위한 정보 저장
	_wfopen_s(&pFile, L"ScriptList.txt", L"w");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\n");
	}

	fclose(pFile);

	return 0;
}

void MakeStateMgrHeader(wstring strHeaderPath)
{
	wstring solPath = CPathMgr::GetProjectPath();
	wstring Path = solPath + L"..\\Project\\Scripts\\CStateMgr.h";
	wfstream fout;
	fout.open(Path);

	if (!fout.is_open()) return;


	fout << L"#pragma once" << endl << endl;
	fout << L"#include <vector>" << endl;
	fout << L"#include <string>" << endl << endl;

	fout << L"enum class STATE_TYPE" << endl << L"{" << endl;

	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 1; j < g_vecStateNames[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecStateNames[i][j]);
		}

		fout << L"\t" << strScriptUpperName << L"," << endl;
	}
	fout << L"\tEND," << endl;
	fout << L"};" << endl << endl;

	fout << L"using namespace std;" << endl << endl;
	fout << L"class CState;" << endl << endl;
	fout << L"class CStateMgr" << endl << L"{" << endl;
	fout << L"public: " << endl;
	fout << L"\tstatic void GetStateInfo(vector<wstring>& _vec);" << endl;
	fout << L"\tstatic CState* GetState(const wstring& _strStateName);" << endl;
	fout << L"\tstatic CState* GetState(UINT _iStateType);" << endl;
	fout << L"\tstatic const wchar_t* GetStateName(CState* _pState);" << endl << L"};" << endl;

}

void MakeStateMgrCPP()
{

	wstring solPath = CPathMgr::GetProjectPath();
	wstring Path = solPath + L"..\\Project\\Scripts\\CStateMgr.cpp";
	wfstream fout(Path);

	// 헤더 입력
	fout << L"#include \"pch.h\"" << endl;
	fout << L"#include \"CStateMgr.h\"" << endl << endl;

	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		fout << L"#include \"" << g_vecStateNames[i] << L".h\"" << endl;
	}

	// 첫 번째 함수 작성
	fout << L"\nvoid CStateMgr::GetStateInfo(vector<wstring>& _vec)" << endl << "{" << endl;

	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		fout << L"\t_vec.push_back(L\"" << g_vecStateNames[i] << L"\");" << endl;
	}
	fout << L"}" << endl << endl;

	// 두번째 함수 작성
	fout << L"CState * CStateMgr::GetState(const wstring& _strStateName)" << endl << "{" << endl;

	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		fout << L"\tif (L\"" << g_vecStateNames[i] << L"\" == _strStateName)" << endl;
		fout << L"\t\treturn new " << g_vecStateNames[i] << L";" << endl;
	}
	fout << L"\treturn nullptr;" << endl << "}" << endl << endl;

	// 세번째 함수
	fout << L"CState * CStateMgr::GetState(UINT _iStateType)" << endl << "{" << endl;

	fout << L"\tswitch (_iStateType)" << endl << "\t{" << endl;
	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		wstring strStateUpperName = L"";
		for (UINT j = 1; j < g_vecStateNames[i].size(); ++j)
		{
			strStateUpperName += toupper(g_vecStateNames[i][j]);
		}
		fout << L"\tcase (UINT)STATE_TYPE::" << strStateUpperName << L":" << endl;
		fout << L"\t\treturn new " << g_vecStateNames[i] << L";" << endl;
		fout << L"\t\tbreak;" << endl;
	}
	fout << L"\t}\n\treturn nullptr;" << endl << "}" << endl << endl;

	// 네번째 함수
	fout << L"const wchar_t * CStateMgr::GetStateName(CState * _pState)" << endl << "{" << endl;
	fout << L"\tswitch ((STATE_TYPE)_pState->GetStateType())" << endl << "\t{" << endl;
	for (UINT i = 0; i < g_vecStateNames.size(); ++i)
	{
		fout << L"\tcase STATE_TYPE::";

		wstring strStateUpperName = L"";
		for (UINT j = 1; j < g_vecStateNames[i].size(); ++j)
		{
			strStateUpperName += toupper(g_vecStateNames[i][j]);
		}
		fout << strStateUpperName;
		fout << L":" << endl << "\t\treturn " << L"L\"" << g_vecStateNames[i];
		fout << L"\";" << endl << "\t\tbreak;" << endl << endl;
	}
	fout << L"\t}" << endl << "\treturn nullptr;" << endl << "}";
}
