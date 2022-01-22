// Win32Project1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <thread>
#include <shellapi.h>

#pragma comment(lib,"gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#define MAX_LOADSTRING 100

// 全局变量: 

Gdiplus::Image * image;
UINT dimensions_count;
GUID* pDimensionIDs;
UINT frameCount;
UINT frameIndex = 0;
UINT image_w;
UINT image_h;



thread * pthread = 0;

void thread_proc_draw(HWND hwnd);

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{





    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	image = new Gdiplus::Image{ L"C:\\Users\\luxiang\\Desktop\\3.gif" };
	//image = new Gdiplus::Image{ L"C:\\Users\\luxiang\\Desktop\\alpha.gif" };
	dimensions_count = image->GetFrameDimensionsCount();
	pDimensionIDs = (GUID*)malloc(sizeof(GUID)*dimensions_count);
	image->GetFrameDimensionsList(pDimensionIDs, dimensions_count);
	frameCount = image->GetFrameCount(&pDimensionIDs[0]);
	image_w = image->GetWidth();
	image_h = image->GetHeight();




    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

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
	GdiplusShutdown(gdiplusToken);
    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW| CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(100,252,253));//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = 0;// MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
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

   
   //去边框
   LONG_PTR Style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
   Style = Style &~WS_CAPTION &~WS_SYSMENU &~WS_SIZEBOX;
   ::SetWindowLongPtr(hWnd, GWL_STYLE, Style);
   
   LONG ret = ::GetWindowLong(hWnd, GWL_EXSTYLE);
   ret = ret | WS_EX_LAYERED;
   ::SetWindowLong(hWnd, GWL_EXSTYLE, ret);



   
   if (frameCount > 1)
   {
	   pthread = new thread{ thread_proc_draw ,hWnd };
   }

   /*
   CreateWindowA(
	   "BUTTON",                                                 //类型为BUTTON，若是文本框为EDIT
	   "hahaha",
	   WS_CHILD | WS_VISIBLE,
	   50,
	   50,
	   100,
	   100,
	   hWnd,
	   (HMENU)123,//child-window identifier
	   GetModuleHandle(0),
	   NULL);
	   */
   

   //SetLayeredWindowAttributes(hWnd, RGB(100, 252, 253), 255, LWA_COLORKEY| LWA_ALPHA);



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
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	

    switch (message)
    {

	case WM_LBUTTONDOWN:
		{
			Gdiplus::Point point{ LOWORD(lParam),HIWORD(lParam) };
			Gdiplus::Rect rect{ (int)(image_w-20),0,20,20 };
			if (rect.Contains(point))
			{
				PostQuitMessage(0);
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			PostMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.X, point.Y));
			
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			ShellExecute(NULL, L"open", L"http://www.baidu.com/", NULL, NULL, SW_SHOWNORMAL);
			
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {


            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

			
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			//GDI
			
			
			



			/*
			Graphics graphics(hdc);
			Pen      pen(Color(50, 0, 0, 255));
			pen.SetWidth(10);
			graphics.DrawLine(&pen, 300, 300, 600, 600);

			image->SelectActiveFrame(pDimensionIDs, frameIndex);
			graphics.DrawImage(image, 0, 0);
			*/




			
			
				
			/*
			char a[100] = { 0 };
			string str = to_string((int)hWnd);
			TextOutA(hdc, 10, 10, str.c_str(), str.length());
			//RGB(100, 252, 253)
			HBRUSH hbrush = (HBRUSH)SelectObject(hdc, (HGDIOBJ)CreateSolidBrush(RGB(100, 252, 253)));
			Rectangle(hdc, 300, 300, 600, 600);
			DeleteObject(SelectObject(hdc, hbrush));
			*/



			//GDI+


            EndPaint(hWnd, &ps);

			

        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void thread_proc_draw(HWND hwnd)
{
	Gdiplus::Bitmap * bitmap;
	HDC wdc;//窗口DC
	HDC cdc;//compatible DC

	bitmap = new Gdiplus::Bitmap(1000, 800, PixelFormat32bppARGB);
	wdc = GetDC(hwnd);
	cdc = CreateCompatibleDC(wdc);
	HBITMAP hb;
	
	
	for (;;)
	{
		Sleep(100);

		bitmap->GetHBITMAP(Color{ 0,0,0,0 }, &hb);
		DeleteObject(SelectObject(cdc, hb));

		Graphics graphics(cdc);
		image->SelectActiveFrame(pDimensionIDs, frameIndex);
		graphics.DrawImage(image, 0, 0);
		Pen      pen(Color(255, 255, 100, 100));
		pen.SetWidth(3);
		graphics.DrawLine(&pen, image_w-20, 20, image_w, 0);
		graphics.DrawLine(&pen, image_w - 20, 0, image_w, 20);






		BLENDFUNCTION blendfunction;
		blendfunction.BlendOp = AC_SRC_OVER;
		blendfunction.BlendFlags = 0;
		blendfunction.AlphaFormat = AC_SRC_ALPHA;
		blendfunction.SourceConstantAlpha = 255;


		POINT win_pos{ 100,100 };
		POINT bitmap_pos{ 0,0 };
		SIZE win_size{ image_w,image_h };
		SIZE bitmap_size{ 300,300 };
		int i = UpdateLayeredWindow(hwnd,
			0,
			0,
			&win_size,
			cdc,
			&bitmap_pos,
			0, &blendfunction, ULW_ALPHA);


		frameIndex = frameIndex%frameCount;
		frameIndex++;
	}
	

}

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
