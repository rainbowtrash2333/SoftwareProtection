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


void doSth(TCHAR* szUsername, TCHAR* szPassword) {


	std::string username = szUsername;
	std::string string1 = md5(username);
	std::string string2 = szPassword;

	char char1[100];
	char char2[100];
	char char3[100];

	strcpy(char1, string1.c_str());

	strcpy(char2, string2.c_str());
	Encryption(char1, char2, char3);
	MessageBoxA(0, char3, "ÌבÊ¾", 0);
	
}