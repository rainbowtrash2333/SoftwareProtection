// key.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<Windows.h>
# include<Windows.h>
# include<ostream>
#include"md5.h"


UINT64 get_cpuid(char* username)
{

    DWORD dwStandard = 0;
    DWORD dwFeature = 0;

    _asm {
        mov eax, 1
        cpuid
        mov dwStandard, eax
        mov dwFeature, edx
    }
    DWORD d1 = dwStandard ^ dwFeature;

    bool b = true;
    char* a = username;

    int index = 0;
    int i = 0;
    // 48 75
    while (a[i] != '\0') {
        index = (i % 4) * 8;
        a[i] = (char)(((((int)a[i] ^ (int)(d1 >> index))) % 5) + 48 + 5);
        ++i;
    }
   
}


int main(int argc, char* argv[]) {
  
    char* name;
    if (argc == 2)
    {
        name = argv[1];
    }
    else if (argc > 2)
    {
        printf("参数过多\n");
       // return 0;
    }
    else
    {
        printf("请输入密钥\n");
       // return 0;
    }
    char char1[100];
    //std::string username = name;
    std::string username = "2222222";
    std::string string1 = md5(username);
    std::cout << "size: " << string1.length() << std::endl;
    strcpy(char1, string1.c_str());
    get_cpuid(char1);
    std::cout << "激活码: " << char1 << std::endl;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
