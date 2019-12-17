// 软件安全大作业.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "软件安全大作业.h"
#include"encryption.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    LPVOID  start = GetModuleHandle(NULL);
    
    SMC(start, "123456789");

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;
    static HFONT hFont;  //逻辑字体
    static HWND hLabUsername;  //静态文本框--用户名
    static HWND hLabPassword;  //静态文本框--密码
    static HWND hEditUsername;  //单行文本输入框
    static HWND hEditPassword;  //密码输入框
    static HWND hBtnLogin;  //登录按钮
    //定义缓冲区
    TCHAR szUsername[100];
    TCHAR szPassword[100];
    switch (message) {
    case  WM_CREATE:
        //创建逻辑字体
        hFont = CreateFont(-14/*高*/, -7/*宽*/, 0, 0, 400 /*一般这个值设为400*/,
            FALSE/*斜体?*/, FALSE/*下划线?*/, FALSE/*删除线?*/, DEFAULT_CHARSET,
            OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
            FF_DONTCARE, TEXT("微软雅黑")
        );
        //创建静态文本框控件--用户名
        hLabUsername = CreateWindow(TEXT("static"), TEXT("用户名："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ | SS_RIGHT /*水平居右*/,
            0 /*x坐标*/, 20 /*y坐标*/, 70 /*宽度*/, 26 /*高度*/,
            hWnd /*父窗口句柄*/, (HMENU)1 /*控件ID*/, hInst /*当前程序实例句柄*/, NULL
        );
        //创建静态文本框控件--激活码
        hLabPassword = CreateWindow(TEXT("static"), TEXT("激活码："),
            WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE /*垂直居中*/ | SS_RIGHT /*水平居右*/,
            0, 56, 70, 26,
            hWnd, (HMENU)2, hInst, NULL
        );
        //创建单行文本框控件
        hEditUsername = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER /*边框*/ | ES_AUTOHSCROLL /*水平滚动*/,
            80, 20, 200, 26,
            hWnd, (HMENU)3, hInst, NULL
        );
        //创建密码输入框
        hEditPassword = CreateWindow(TEXT("edit"), TEXT(""),
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL /*水平滚动*/,
            80, 56, 200, 26,
            hWnd, (HMENU)4, hInst, NULL
        );
        //创建按钮控件
        hBtnLogin = CreateWindow(TEXT("button"), TEXT("激活"),
            WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
            80, 92, 200, 30,
            hWnd, (HMENU)5, hInst, NULL
        );
        //依次设置控件的字体
        SendMessage(hLabUsername, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hLabPassword, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hEditUsername, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hEditPassword, WM_SETFONT, (WPARAM)hFont, NULL);
        SendMessage(hBtnLogin, WM_SETFONT, (WPARAM)hFont, NULL);
        break;
    case WM_COMMAND:
        wmId = LOWORD(wParam);
        wmEvent = HIWORD(wParam);
        switch (wmId) {
        case 5:  //按钮的HMENU参数
            //获取输入框的数据
            GetWindowText(hEditUsername, szUsername, 100);
            GetWindowText(hEditPassword, szPassword, 100);
            /*wsprintf(szUserInfo, TEXT("C语言中文网提示：\r\n您的用户账号：%s\r\n您的用户密码：%s"), szUsername, szPassword);
            MessageBox(hWnd, szUserInfo, TEXT("信息提示"), MB_ICONINFORMATION);*/
            
            doSth(szUsername, szPassword);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        // TODO:  在此添加任意绘图代码...
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}