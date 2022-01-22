
#include "resource.h"
#include <iostream>
#include <Windows.h>
#include <string>
#include <objidl.h>
#include <gdiplus.h>
#include <thread>


#pragma comment(lib,"gdiplus.lib")

using namespace std;
using namespace Gdiplus;

Gdiplus::Image * image;
UINT dimensions_count;
GUID* pDimensionIDs;
UINT frameCount;
UINT frameIndex = 0;

thread * pthread = 0;

void thread_proc_draw(HWND hwnd);


void thread_proc_draw(HWND hwnd)
{
	for (;;)
	{
		Sleep(200);
		InvalidateRect(hwnd, 0, 0);
		frameIndex = frameIndex%frameCount;
		frameIndex++;
	}

}


INT_PTR CALLBACK Dlgproc(
	HWND hWnd,
	UINT message,
	WPARAM wParam, LPARAM lParam
)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			//GDI
			/*
			char a[100] = {0};
			string str = to_string((int)hWnd);
			TextOutA(hdc, 10, 10, str.c_str(), str.length());
			//RGB(100, 252, 253)
			HBRUSH hbrush = (HBRUSH)SelectObject(hdc, (HGDIOBJ)CreateSolidBrush(RGB(100, 252, 253)));
			Rectangle(hdc, 300, 300, 600, 600);
			DeleteObject(SelectObject(hdc, hbrush));
			*/
			
			Graphics graphics(hdc);
			Pen      pen(Color(255, 0, 0, 255));
			pen.SetWidth(10);
			graphics.DrawLine(&pen, 300, 300, 600, 600);
			image->SelectActiveFrame(pDimensionIDs, frameIndex);
			graphics.DrawImage(image, 0, 0);


			EndPaint(hWnd, &ps);
			return TRUE;
		}
		
		case WM_INITDIALOG:

			image = new Image{ L"C:\\Users\\luxiang\\Desktop\\li.gif" };
			dimensions_count = image->GetFrameDimensionsCount();
			pDimensionIDs = (GUID*)malloc(sizeof(GUID)*dimensions_count);
			image->GetFrameDimensionsList(pDimensionIDs, dimensions_count);
			frameCount = image->GetFrameCount(&pDimensionIDs[0]);

			

			if (frameCount > 1)
			{
				pthread = new thread{ thread_proc_draw ,hWnd };
			}

			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return FALSE;
}


int main()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), (HWND)0x0, Dlgproc);


	GdiplusShutdown(gdiplusToken);

	return 0;
}