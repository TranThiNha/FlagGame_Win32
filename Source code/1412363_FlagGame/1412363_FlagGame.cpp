// 1412363_FlagGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412363_FlagGame.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<iostream>
#include <fstream>
#include<string>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
vector<wstring>LinkName;                        //array of name of country
vector<wstring>Name;
HWND btnImage1, btnImage2, staticCountry, btnStart, score, StaticTime;
HBITMAP hImage1, hImage2;
int IdRightAns, IdWrongAns, IdAns;
int countQuesion = 0;
int countRightAns = 0;
int remain_Time = 30;
HFONT hfontTime;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	wfstream wif_Link;
	wif_Link.open("flag.txt", ios::in);

	wfstream wif_Name;
	wif_Name.open("Name.txt", ios::in);
	
	if (wif_Link.is_open())
	{
		wstring wline;
		while (getline(wif_Link, wline))
		{
			LinkName.push_back(wline);
		}

		wif_Link.close();
	}

	if (wif_Name.is_open())
	{
		wstring wline;
		while (getline(wif_Name, wline))
		{
			Name.push_back(wline);
		}
		wif_Name.close();
	}


	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MY1412363_FLAGGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412363_FLAGGAME));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412363_FLAGGAME));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNSHADOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MY1412363_FLAGGAME);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 700, 500, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//


void CALLBACK f_run(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	remain_Time--;
	if (remain_Time == -1)
	{
		MessageBox(hwnd, L"Time is over! :(((", L"Look! :o", MB_ICONSTOP | MB_OK);
		KillTimer(hwnd, IDT_TIME_RUN);
		EnableWindow(btnImage1, false);
		EnableWindow(btnImage2, false);
	}

	return;
}

void CALLBACK f_out(HWND hwnd, UINT uMsg, UINT timerId, DWORD dwTime)
{
	WCHAR * buffer = new WCHAR[255];

	if (remain_Time >= 10)
	{
		swprintf(buffer, 100, L"00:%d", remain_Time);
	}
	else if (remain_Time == 0)
	{
		swprintf(buffer, 100, L"00:0%d", remain_Time);
		KillTimer(hwnd, IDT_TIME_OUT);
	}
	else
	{
		swprintf(buffer, 100, L"00:0%d", remain_Time);
	}
	SetWindowText(StaticTime, buffer);
	return;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	switch (message)
	{
	case WM_CREATE:

		StaticTime = CreateWindowEx(0, L"STATIC", L"00:00", WS_CHILD | WS_VISIBLE | ACS_CENTER, 100, 50, 450, 50, hWnd, NULL, NULL, NULL);
	    hfontTime = CreateFont(40, 30, 10, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Fixedsys");
		SendMessage(StaticTime, WM_SETFONT, WPARAM(hfontTime), TRUE);
		btnStart = CreateWindowEx(0, L"BUTTON", L"START", WS_CHILD | WS_VISIBLE | WS_BORDER | BS_PUSHBUTTON, 100, 130, 450, 50, hWnd, (HMENU)IDC_BUTTONSTART, hInst, NULL);
		SendMessage(btnStart, 0, WPARAM(hFont), TRUE);
		btnImage1 = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP | ACS_CENTER, 100, 200, 200, 100, hWnd, (HMENU)IDC_BUTTON1, hInst, NULL);
		btnImage2 = CreateWindowEx(0, L"BUTTON", L"", WS_CHILD | WS_BORDER | WS_VISIBLE | BS_PUSHBUTTON | ACS_CENTER  | BS_BITMAP, 350, 200, 200, 100, hWnd, (HMENU)IDC_BUTTON2, hInst, NULL);
		staticCountry = CreateWindowEx(0, L"STATIC", L"WELCOM TO FLAG GAME! \nTry to choose right flag for each country", WS_CHILD | WS_BORDER | WS_VISIBLE | ACS_CENTER, 220, 380, 200, 50, hWnd, NULL, hInst, NULL);
		hfontTime = CreateFont(20, 10, 10, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"italic");
		SendMessage(score, WM_SETFONT, WPARAM(hfontTime), TRUE);
		score = CreateWindowEx(0, L"STATIC", L"0 / 0", WS_CHILD | WS_BORDER | WS_VISIBLE | ACS_CENTER, 270, 310, 100, 50, hWnd, NULL, hInst, NULL);
		hfontTime = CreateFont(20, 10, 10, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"italic");
		SendMessage(score, WM_SETFONT, WPARAM(hfontTime), TRUE);
		EnableWindow(btnImage1, false);
		EnableWindow(btnImage2, false);

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		// Parse the menu selections:
		switch (wmId)
		{

		case IDC_BUTTONSTART:

			countQuesion++;
			srand(time(0));
			IdAns = rand() % 2;
			IdRightAns = rand() % 223;
			IdWrongAns = rand() % 223;
			SetTimer(hWnd, IDT_TIME_RUN, 1000, (TIMERPROC)f_run);
			SetTimer(hWnd, IDT_TIME_OUT, 1000, (TIMERPROC)f_out);
			while (IdWrongAns == IdRightAns)
			{
				IdWrongAns = rand() % 223;
			}

			WCHAR bufferName[50];
			wsprintf(bufferName, (LPCWSTR)Name[IdRightAns].c_str(),1000);
			SetWindowText(staticCountry, bufferName);
			if (IdAns == 0)
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			else
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			EnableWindow(btnStart, false);
			EnableWindow(btnImage1, true);
			EnableWindow(btnImage2, true);

			break;

		case IDC_BUTTON1:
			if (IdAns == 0)
			{
				countRightAns++;
			}
			srand(time(0));
			IdAns = rand() % 2;
			IdRightAns = rand() % 223;
			IdWrongAns = rand() % 223;
			while (IdWrongAns == IdRightAns)
			{
				IdWrongAns = rand() % 223;
			}

			WCHAR bufferName1[50];
			WCHAR bufferScore1[10];
			wsprintf(bufferName1, (LPCWSTR)Name[IdRightAns].c_str(), 1000);
			SetWindowText(staticCountry, bufferName1);

			wsprintf(bufferScore1, L"%d / %d", countRightAns, countQuesion);
			SetWindowText(score, bufferScore1);
			if (IdAns == 0)
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			else
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			countQuesion++;

			break;
		case IDC_BUTTON2:
			if (IdAns == 1)
			{
				countRightAns++;
			}
			srand(time(0));
			IdAns = rand() % 2;
			IdRightAns = rand() % 223;
			IdWrongAns = rand() % 223;
			while (IdWrongAns == IdRightAns)
			{
				IdWrongAns = rand() % 223;
			}

			WCHAR bufferName2[50];
			WCHAR bufferScore2[10];
			wsprintf(bufferName2, (LPCWSTR)Name[IdRightAns].c_str(), 1000);
			SetWindowText(staticCountry, bufferName2);                    

			wsprintf(bufferScore2, L"%d / %d", countRightAns,countQuesion);
			SetWindowText(score, bufferScore2);
			if (IdAns == 0)
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			else
			{
				hImage1 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdWrongAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage1);
				hImage2 = (HBITMAP)LoadImage(NULL, (LPCWSTR)LinkName[IdRightAns].c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				SendMessage(btnImage2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage2);
			}
			countQuesion++;

			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
