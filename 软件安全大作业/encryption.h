#pragma once
# include<Windows.h>
# include<ostream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>

#include"md5.h"
#pragma code_seg(".SMC")
#include "head.h"
#pragma code_seg()
#pragma comment(linker, "/SECTION:.SMC,ERW")

using namespace std;

#define MAX_LENGTH 100

void xorPlus(char* soure, int dLen, const char* Key, int Klen)
{
	for (int i = 0; i < dLen;)
	{
		for (int j = 0; (j < Klen) && (i < dLen); j++, i++)
		{
			soure[i] = soure[i] ^ Key[j];
			soure[i] = ~soure[i];
		}
	}
}

void SMC(LPVOID pImageBase, const char* key) {
	// SMC 加密XX区段
	const char* szSecName = ".SMC";
	short nSec;
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	IMAGE_FILE_HEADER FileHeader;
	PIMAGE_SECTION_HEADER pSectionHeader;
	pDOSHeader = (PIMAGE_DOS_HEADER)pImageBase;
	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)(pDOSHeader->e_lfanew) + (DWORD)pImageBase);
	nSec = pNTHeader->FileHeader.NumberOfSections;
	FileHeader = pNTHeader->FileHeader;
	pSectionHeader = (IMAGE_SECTION_HEADER*)((ULONG_PTR)pNTHeader + FIELD_OFFSET(IMAGE_NT_HEADERS, OptionalHeader) + FileHeader.SizeOfOptionalHeader);
	for (int i = 0; i < nSec; i++)
	{


		if (strcmp((char*)&pSectionHeader->Name, szSecName) == 0)
		{
			int pack_size = pSectionHeader->SizeOfRawData;;
			char* packStart = 0;
			char* pBuf;
			pBuf = (char*)pImageBase;
			packStart = &pBuf[pSectionHeader->VirtualAddress];

			DWORD dwOldProtect = 0;
			bool b = VirtualProtect(packStart, pack_size, PAGE_EXECUTE_READWRITE, &dwOldProtect);

			xorPlus(packStart, pack_size, key, strlen(key));
			return;
		}
		pSectionHeader++;
	}
	//printf("no such section!");
	MessageBoxA(0, "no such section!", "error", 0);
}

bool IsDebuggerPresentFlag() {
	__asm {
		mov eax, fs: [0x30]
		movzx eax, byte ptr [eax + 2]
		test eax,eax
		jne rt_label
		jmp rf_label
	}
	rt_label:
		return true;
	rf_label:
		return false;
}

// IsDebuggerPresent() 反调试
bool CheckDebuggerPresent(){
	BOOL(*IsDebuggerPresent)();
	HMODULE hModule = GetModuleHandle("KernelBase.dll");  // 加载模块 Kernel32
	if (hModule == NULL)
	{
		return true;
	}
	IsDebuggerPresent = (BOOL(*)())GetProcAddress(hModule, "IsDebuggerPresent");  // 获取下地址
	if (IsDebuggerPresent == NULL)
	{
		return true;
	}
	if (*(BYTE*)IsDebuggerPresent == 0xcc || *(BYTE*)IsDebuggerPresent != 0x64 || IsDebuggerPresent())  // 调用
	{
		return true;
	}
	return false;
}

bool NtGlobalFlags() {
	__asm {
		mov eax, fs:[30h]	//PEB地址
		mov eax, [eax+68h]	// NtGlobalFlags标志
		and eax, 0x70
		test eax, eax
		jne rt_label
		jmp rf_label
	}
	rt_label:
		return true;
	rf_label:
		return false;
}

bool HeapFlags() {
	__asm {
		mov eax, fs: [30h]		// 进程的PEB 
		mov eax, [eax + 18h]; 	// 进程的堆，访问默认的堆 
		mov eax, [eax + 10h]; 	// 检查Heap_Flags标志位 
		test eax, eax
		jne rt_label
		jmp rf_label
	}
rt_label:
	return true;
rf_label:
	return false;
}

// Heap_ForceFlags标志 
bool HeapForceFlags() {
	__asm {
		mov eax, fs: [30h]
		mov eax, [eax + 18h]	//PEB.ProcessHeap
		mov eax, [eax + 0ch]	//PEB.ProcessHeap.ForceFlags
		test eax, eax
		cmp eax, 2
		jne rt_label
		jmp rf_label
	}
rt_label:
	return true;
rf_label:
	return false;
}

// CheckRemoteDebuggerPresent
bool CheckRemoteDebuggerPresent() {
	FARPROC Func_addr;
	HMODULE hModule = GetModuleHandle("kernel32.dll");
	if (hModule == INVALID_HANDLE_VALUE)	
		return false;
	(FARPROC&)Func_addr = GetProcAddress(hModule, "CheckRemoteDebuggerPresent");
	if (Func_addr != NULL)
	{
		_asm
		{
			push eax;	//保护EAX
			push esp;	// CheckRemoteDebuggerPresent 返回值保存位置	
			push 0xffffffff;	//当前进程
			call Func_addr;
			test eax, eax;
			je rf_label;
			pop eax;
			test eax, eax
			je rf_label;
			jmp	rt_label;
		}
	rt_label:
		return true;
	rf_label:
		return false;
	}
}

#include<Tlhelp32.h>
#include<Psapi.h>
// 检查父进程。
BOOL IsInDebugger()
{
	HANDLE hProcessSnap = NULL;
	char Expchar[] = "\\EXPLORER.EXE";
	char szBuffer[MAX_PATH] = { 0 };
	char FileName[MAX_PATH] = { 0 };
	PROCESSENTRY32 pe32 = { 0 };
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	//得到所有进程的列表快照
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hProcessSnap, &pe32))                    //查找进程
	{
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//遍历所有进程
	do {
		//判断是否是自己的进程
		if (pe32.th32ProcessID == GetCurrentProcessId()){
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ParentProcessID);
			//打开父进程
			if (hProcess) {
				// 得到父进程名
				if (GetModuleFileNameEx(hProcess, NULL, FileName, MAX_PATH)){ 				
					GetWindowsDirectory(szBuffer,MAX_PATH);  //得到系统所在目录
					strcat(szBuffer, Expchar);
					//比较当前是否为Explorer.exe进程
					if (_strcmpi(FileName, szBuffer))
						return TRUE;
				}
				else {
					return FALSE; // 无法获得进程名  
				} 
					CloseHandle(hProcess);
				}
			else {
				return FALSE;                   //无权访问该进程 
			}
		}
	}while (Process32Next(hProcessSnap, &pe32));
		CloseHandle(hProcessSnap);
		return FALSE;
}
// 断点检测
BOOL IsBPX(void* address)
{
	_asm {
		mov esi, address    // 被监视函数的地址
		mov al, [esi]       // 取该函数首字节的机器码 
		cmp al, 0xCC      // 检测机器码是否为0xCC？
		je BPXed         // 如是，则断定该函数被设断跟踪	
	}
	return FALSE;
BPXed:
	return TRUE;
}


bool CheckWindowName() {
	bool b = false;
	// check OllyDbg
	TCHAR AppName[] = TEXT("OllyDbg - [CPU]");
	HANDLE hWnd = FindWindow(NULL, AppName);
	b = (hWnd != 0);
	
}


// 句柄检测
bool isOllyDBG(const PROCESSENTRY32W& entry) {
	return std::wstring(entry.szExeFile) == L"OllyDBG.EXE";
}
bool isIDA(const PROCESSENTRY32W& entry) {
	return std::wstring(entry.szExeFile) == L"ida.exe";
}
BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
	const auto& pids = *reinterpret_cast<std::vector<DWORD>*>(lParam);

	DWORD winId;
	GetWindowThreadProcessId(hwnd, &winId);

	for (DWORD pid : pids) {
		if (winId == pid) {
			std::wstring title(GetWindowTextLength(hwnd) + 1, L'\0');
			GetWindowTextW(hwnd, &title[0], title.size()); 
			ExitProcess(1);
		}
	}

	return TRUE;
}
int CheckHandle() {
	std::vector<DWORD> pids;

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32W entry;
	entry.dwSize = sizeof entry;

	if (!Process32FirstW(snap, &entry)) {
		return 0;
	}

	do {
		if (isOllyDBG(entry)) {
			pids.emplace_back(entry.th32ProcessID);
		}
		else if (isIDA(entry)) {
			pids.emplace_back(entry.th32ProcessID);
		}
	} while (Process32NextW(snap, &entry));

	EnumWindows(enumWindowsProc, reinterpret_cast<LPARAM>(&pids));
}

typedef UINT(CALLBACK* LPFNDLLFUNC1)(HWND, LPCTSTR, LPCTSTR, UINT);

void doSth(TCHAR* szUsername, TCHAR* szPassword) {

	if (IsDebuggerPresentFlag() || CheckDebuggerPresent() || NtGlobalFlags() || HeapFlags() || HeapForceFlags() || CheckRemoteDebuggerPresent() || IsInDebugger()) {
		ExitProcess(1);
	}
	CheckHandle();

	std::string username = szUsername;
    std::string string1 = md5(username);
    std::string string2 = szPassword;

    char char1[100];
    char char2[100];
    char char3[100];
    
	strcpy(char1, string1.c_str());

    strcpy(char2, string2.c_str());
    Encryption(char1, char2,  char3);
	HINSTANCE hDLL;               // Handle to DLL
	LPFNDLLFUNC1 lpfnDllFunc1;    // Function pointer	
	hDLL = LoadLibrary("user32.dll");
	if (hDLL != NULL)
	{
		lpfnDllFunc1 = (LPFNDLLFUNC1)GetProcAddress(hDLL, "MessageBoxA");
		if (!lpfnDllFunc1)
		{
			FreeLibrary(hDLL);
		}
		else
		{
			lpfnDllFunc1(0, char3, "提示", 0);
		}
	}
}