#pragma once

#pragma once
# include<Windows.h>
# include<ostream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <tlhelp32.h>
#include<Tlhelp32.h>
#include<Psapi.h>
#include"md5.h"
#include "head.h"

using namespace std;

#define MAX_LENGTH 100

void debug(LPVOID pv, const char* msg) {
	
	std::ostringstream stringStream;
	stringStream << (LPVOID)pv;
	std::string copyOfStr = stringStream.str();
	char char1[100];
	strcpy(char1, copyOfStr.c_str());
	MessageBoxA(0, char1, msg, 0);
}
void doSth(TCHAR* szUsername, TCHAR* szPassword) {
	LPVOID  start = GetModuleHandle(NULL);

	debug(start, "Handle");
	std::string username = szUsername;
	std::string string1 = md5(username);
	std::string string2 = szPassword;

	debug(Encryption, "MD5");

	char char1[100];
	char char2[100];
	char char3[100];

	strcpy(char1, string1.c_str());

	strcpy(char2, string2.c_str());
	Encryption(char1, char2, char3);
	MessageBoxA(0, char3, "��ʾ", 0);
	
}