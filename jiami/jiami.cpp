#include <iostream>
#include <iostream>
#include <windows.h>
#include <winnt.h>
#include <dbghelp.h>
#pragma comment (lib, "Dbghelp.lib")  
using namespace std;
// 全局变量
LPVOID pImageBase;

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

void getIamgeBase() {
	OPENFILENAME ofn;      // 公共对话框结构。   
	TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。             
	// 初始化选择文件对话框。   
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All(*.*)\0*.*\0Text(*.txt)\0*.TXT\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
	// 显示打开选择文件对话框。   

	if (GetOpenFileName(&ofn))
	{
		//显示选择的文件。   
		OutputDebugString(szFile);
		OutputDebugString(L"\r\n");
	}

	TCHAR* lpImageName = ofn.lpstrFile;
	HANDLE hImgFD = CreateFile(lpImageName, (GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE), FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	HANDLE hImgMapping = CreateFileMapping(hImgFD, NULL, PAGE_EXECUTE_READWRITE, 0, 0, NULL);
	pImageBase = MapViewOfFile(hImgMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (pImageBase == NULL) printf("no ImageBase");
}
void hexDump(char* desc, void* addr, int len)
{
	int i;
	unsigned char buff[17];
	unsigned char* pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		printf("%s:\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
				printf("  %s\n", buff);

			// Output the offset.
			printf("  %04x ", i);
		}

		// Now the hex code for the specific character.
		printf(" %02x", pc[i]);

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
			buff[i % 16] = '.';
		}
		else {
			buff[i % 16] = pc[i];
		}

		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		printf("   ");
		i++;
	}

	// And print the final ASCII bit.
	printf("  %s\n", buff);
}
void XORMemory(LPVOID StartAddr, DWORD dwSize) {
	__asm {
		push eax // 起始位置
		push ecx //终止位置
		mov ecx, StartAddr
		add ecx, dwSize
		mov eax, StartAddr

		crypt_loop :
		xor byte ptr ds : [eax] , 0x40
			inc eax
			cmp eax, ecx
			jl crypt_loop;

		pop ecx
			pop eax
	}
}
void startup() {
	getIamgeBase();

}

void SMC(LPVOID pImageBase, const char* key)
{
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
		printf("Name: %s\n", pSectionHeader->Name);
		if (strcmp((char*)&pSectionHeader->Name, szSecName) == 0)
		{
			int pack_size = pSectionHeader->SizeOfRawData;;
			//byte* packStart= (byte*)pImageBase+ pSectionHeader->PointerToRawData;
			char* packStart=0;
			char* pBuf;
			pBuf = (char*)pImageBase;
			packStart = &pBuf[pSectionHeader->PointerToRawData];

			cout << "packStart: " << (LPVOID)packStart << endl;
			cout << "pImageBase: " << (LPVOID)pImageBase << endl;
			cout << "pack_size: " << pack_size << endl;

			DWORD dwOldProtect = 0;
			bool b = VirtualProtect(packStart, pack_size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
			
			//XORMemory(packStart, pack_size);
			hexDump(NULL, packStart, pack_size);
			cout << endl;
			xorPlus(packStart, pack_size, key, strlen(key));
			hexDump(NULL, packStart, pack_size);
			printf("加密中！\n");
			return;
		}
		pSectionHeader++;
	}
	printf("no such section!");
}

int main(int argc, char* argv[]) {
	char* key;
	if (argc == 2)
	{
		key = argv[1];
	}
	else if (argc > 2)
	{
		printf("参数过多\n");
		return 0;
	}
	else
	{
		printf("请输入密钥\n");
		return 0;
	}
	startup();

	SMC(pImageBase,key);
	system("pause");
	return 0;
}