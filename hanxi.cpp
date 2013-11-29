// hanxi.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "hanxi.h"
#include "Graph.h"

#include<vector>
#include<Commdlg.h>
#include<fstream>
using std::vector;
using std::fstream;
using std::ofstream;
using std::ifstream;
using std::ios_base;
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
vector<HWND> g_vecHWND;
HWND g_HWNDParent;
// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	WndProcRecieve(HWND, UINT, WPARAM, LPARAM);
#define ABS(x) ( (x)>0?(x):(-(x) ) )


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HANXI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HANXI));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{

	WNDCLASSEX wcex;
	if(   (g_HWNDParent = ::FindWindow(szWindowClass,szTitle) ) == 0 )  //not find
		wcex.lpfnWndProc = WndProc;
	else
	{
		wcex.lpfnWndProc = WndProcRecieve;
	}
	
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
//	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HANXI));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HANXI);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle,   WS_SYSMENU    |WS_CAPTION    |    WS_MINIMIZEBOX   |   WS_MAXIMIZEBOX, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR bufferSave[] = TEXT("文件没有保存，是否要保存?");
	static vector<CInterface*> vecGraph;
	static vector<CInterface*> vecGraphBac;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdcBit;
	static HBITMAP hbitbit;
	static HDC hdcMem;
	static HBITMAP hbit;
	static RECT rect;
	HDC hdc;

	static DWORD penStyle = PS_SOLID;
	static DWORD penWidth = 1;
	static DWORD penRGB = RGB(0,0,0);
	static DWORD BrushStyle = 0;
	static DWORD BrushRGB = RGB(0,0,0);

	static bool bUndo = false;
	static bool bRedo = false;

	static DWORD dwToolNow = 0;

	static bool bIsDrawing;

	static POINT pointStart;

	static vector<POINT> vecPol;

	static bool bNeedSave = false;
	
	POINT pointNow;
	RECT  rectNow;
	ifstream ifstreamFile;
	ofstream ofstreamFile;

	static 
		
	
	CHOOSECOLOR cc;
	COLORREF acrCustClr[16] = {0}; // array of custom colors 
	TCHAR szFile[260];       // buffer for file name
	OPENFILENAME ofn;       // common dialog box structure
	DWORD dwGraphStyle;

	COPYDATASTRUCT   cpd;   /*给COPYDATASTRUCT结构赋值*/

	switch (message)
	{
	case WM_CREATE:
		GetClientRect(hWnd,&rect);
		hdc = GetDC(hWnd);
		hdcBit = CreateCompatibleDC(hdc);
		hbitbit = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		SelectObject(hdcBit,hbitbit);
		hdcMem = CreateCompatibleDC(hdc);
		hbit = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		SelectObject(hdcMem,hbit);
		SelectObject(hdcMem,GetStockObject(NULL_BRUSH));
		SelectObject(hdcMem,GetStockObject(BLACK_PEN));
		SelectObject(hdcBit,GetStockObject(NULL_PEN) );
		SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
		Rectangle(hdcBit,rect.left,rect.top,rect.right,rect.bottom);
		BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
		SelectObject(hdcBit,GetStockObject(BLACK_PEN) );
		SelectObject(hdcBit,GetStockObject(NULL_BRUSH));
		ReleaseDC(hWnd,hdc);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch(wmId)
		{
		case IDM_FILE_EXIT:
			if(!bNeedSave)
			{
				PostQuitMessage(0);
				break;
			}			
			else
			{
				if(MessageBox(hWnd,bufferSave,NULL,MB_OKCANCEL) == IDCANCEL)
				{
					PostQuitMessage(0);
					break;
				}
			}
			
		case IDM_FILE_SAVE:
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			//
			// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
			// use the contents of szFile to initialize itself.
			//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("我的文件(*.hx)\0 *.hx\0");
			ofn.nFilterIndex = 0;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
		
			ofn.Flags   =   OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT; 
			if(GetSaveFileName(&ofn) )
			{
				ofstreamFile.open(ofn.lpstrFile,ofstream::out | ios_base::binary);
				for(int i = 0;i<vecGraph.size();i++)
				{
					(*vecGraph[i])>>ofstreamFile;
				}
				ofstreamFile.close();
				ofstreamFile.clear();
								
			}
			bNeedSave = false;
			break;
		case IDM_FILE_OPEN:
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = TEXT("我的文件(*.hx)\0 *.hx\0");
			ofn.nFilterIndex = 0;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			if(GetOpenFileName(&ofn) )
			{
				vecGraph.clear();
				ifstreamFile.open(ofn.lpstrFile,ofstream::in | ios_base::binary);
				while(ifstreamFile)
				{
					ifstreamFile.read( ( char*)&dwGraphStyle,sizeof(DWORD) );
					switch(dwGraphStyle)
					{
					case 0: //Rect
						vecGraph.push_back(new CRect() );
						(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
						break;
					case 1://ellipse
						vecGraph.push_back(new CEllipse() );
						(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
						break;
					case 2://RoundRect
						vecGraph.push_back(new CRoundRect() );
						(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
						break;
					case 3://Line
						vecGraph.push_back(new CLine() );
						(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
						break;
					case 4://Pol
						vecGraph.push_back(new CPol() );
						(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
						break;


					}
					
				}
				ifstreamFile.close();
				ifstreamFile.clear();
				SelectObject(hdcBit,GetStockObject(NULL_PEN) );
				SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
				Rectangle(hdcBit,0,0,rect.right,rect.bottom);

				for(int i = 0;i<vecGraph.size();i++)
				{
					vecGraph[i]->Draw(hdcBit);
				}
				hdc = GetDC(hWnd);
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				ReleaseDC(hWnd,hdc);
			}
			
			break;
	
			
		case IDM_EDIT_UNDO:
			bNeedSave = true;
			vecGraphBac.push_back ( vecGraph[vecGraph.size() -1] );
			vecGraph.pop_back();
			SelectObject(hdcBit,GetStockObject(NULL_PEN) );
			SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
			Rectangle(hdcBit,0,0,rect.right,rect.bottom);

			for(int i = 0;i<vecGraph.size();i++)
			{
				vecGraph[i]->Draw(hdcBit);
			}
			hdc = GetDC(hWnd);
			BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
			BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);
			bRedo = true;
			break;

		case IDM_EDIT_REDO:
			bNeedSave = true;
			vecGraph.push_back ( vecGraphBac[vecGraphBac.size() -1] );
			vecGraphBac.pop_back();
			SelectObject(hdcBit,GetStockObject(NULL_PEN) );
			SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
			Rectangle(hdcBit,0,0,rect.right,rect.bottom);

			for(int i = 0;i<vecGraph.size();i++)
			{
				vecGraph[i]->Draw(hdcBit);
			}
			hdc = GetDC(hWnd);
			BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
			BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);

			break;

		case IDM_PS_SOLID:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_SOLID,penWidth,penRGB) ) );
			penStyle = PS_SOLID;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			


			break;
		case IDM_PS_DASH:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_DASH,penWidth,penRGB) ) );
			penStyle = PS_DASH;

			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			
			break;
		case IDM_PS_DOT:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_DOT,penWidth,penRGB) ) );
			penStyle = PS_DOT;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
		
			break;
		case IDM_PS_DASHDOT:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_DASHDOT,penWidth,penRGB) ) );
			penStyle = PS_DASHDOT;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			penWidth = 1;
			break;
		case IDM_PS_DASHDOTDOT:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_DASHDOTDOT,penWidth,penRGB) ) );
			penStyle = PS_DASHDOTDOT;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			penWidth = 1;
			break;
		case IDM_PS_NULL:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_NULL,penWidth,penRGB) ) );
			penStyle = PS_NULL;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
	
			break;
		case IDM_PS_INSIDEFRAME:
			penWidth = 1;
			DeleteObject( SelectObject(hdcMem, CreatePen(PS_INSIDEFRAME,penWidth,penRGB) ) );
			penStyle = PS_INSIDEFRAME;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_SOLID,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASH,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_DASHDOTDOT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_NULL,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_INSIDEFRAME,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			penWidth = 1;
			break;
		case IDM_PS_WIDTH1:
			DeleteObject( SelectObject(hdcMem, CreatePen(penStyle,1,penRGB) ) );
			penWidth = 1;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			break;
		case IDM_PS_WIDTH2:
			DeleteObject( SelectObject(hdcMem, CreatePen(penStyle,2,penRGB) ) );
			penWidth = 2;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			break;
		case IDM_PS_WIDTH3:
			DeleteObject( SelectObject(hdcMem, CreatePen(penStyle,3,penRGB) ) );
			penWidth = 3;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			break;
		case IDM_PS_WIDTH4:
			DeleteObject( SelectObject(hdcMem, CreatePen(penStyle,4,penRGB) ) );
			penWidth = 4;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_UNCHECKED);
			break;
		case IDM_PS_WIDTH5:
			DeleteObject( SelectObject(hdcMem, CreatePen(penStyle,5,penRGB) ) );
			penWidth = 5;
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH1,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_CHECKED);
			break;
		case IDM_PEN_COLOR:
			ZeroMemory(acrCustClr,sizeof(COLORREF)*16);
			ZeroMemory(&cc,sizeof(cc) );
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD) acrCustClr;
			cc.rgbResult = penRGB;
			cc.Flags =   CC_RGBINIT | CC_FULLOPEN;
			if(::ChooseColor(&cc))
			{
				DeleteObject( SelectObject(hdcMem,CreatePen(penStyle,penWidth,cc.rgbResult) ) );
				penRGB = cc.rgbResult;
			}
			break;
		case IDM_BRUSH_NULL:
			DeleteObject( SelectObject(hdcMem,GetStockObject(NULL_BRUSH) ) );
			CheckMenuItem(GetMenu(hWnd),IDM_BRUSH_NULL,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_BRUSH_COLOR,MF_UNCHECKED);
			BrushStyle= 0;
			break;			

		case IDM_BRUSH_COLOR:
			ZeroMemory(&cc,sizeof(cc) );
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD) acrCustClr;
			cc.rgbResult = penRGB;
			cc.Flags =   CC_RGBINIT | CC_FULLOPEN;
			if(::ChooseColor(&cc))
			{
				DeleteObject( SelectObject(hdcMem,CreateSolidBrush(cc.rgbResult) ));
				BrushRGB = cc.rgbResult;
				CheckMenuItem(GetMenu(hWnd),IDM_BRUSH_COLOR,MF_CHECKED);
				CheckMenuItem(GetMenu(hWnd),IDM_BRUSH_NULL,MF_UNCHECKED);
				BrushStyle = 1;
			}
			
			break;
		case IDM_RECT:
			dwToolNow = 0;
			CheckMenuItem(GetMenu(hWnd),IDM_RECT,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ROUNDRECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_LINE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_POL,MF_UNCHECKED);
			break;

		case IDM_ELLIPSE:
			dwToolNow = 1;
			CheckMenuItem(GetMenu(hWnd),IDM_RECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ELLIPSE,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ROUNDRECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_LINE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_POL,MF_UNCHECKED);
			break;
		case IDM_ROUNDRECT:
			dwToolNow = 2;
			CheckMenuItem(GetMenu(hWnd),IDM_RECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ROUNDRECT,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_LINE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_POL,MF_UNCHECKED);
			break;
		case IDM_LINE:
			dwToolNow = 3;
			CheckMenuItem(GetMenu(hWnd),IDM_RECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ROUNDRECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_LINE,MF_CHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_POL,MF_UNCHECKED);
			break;
		case IDM_POL:
			dwToolNow = 4;
			CheckMenuItem(GetMenu(hWnd),IDM_RECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_ROUNDRECT,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_LINE,MF_UNCHECKED);
			CheckMenuItem(GetMenu(hWnd),IDM_POL,MF_CHECKED);
			break;

		}

		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_INITMENUPOPUP:
		if (LOWORD(lParam) == 2 ) //pen
		{
			if(penStyle == PS_SOLID)
			{
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_ENABLED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_ENABLED);
			}
			else
			{				
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH2,MF_GRAYED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH3,MF_GRAYED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH4,MF_GRAYED);
				EnableMenuItem(GetMenu(hWnd),IDM_PS_WIDTH5,MF_GRAYED);

			}
		}
		else if(LOWORD(lParam) == 1) //Edit
		{
			EnableMenuItem(GetMenu(hWnd),IDM_EDIT_UNDO,bUndo?MF_ENABLED:MF_GRAYED);
			EnableMenuItem(GetMenu(hWnd),IDM_EDIT_REDO,bRedo?MF_ENABLED:MF_GRAYED);
			if(vecGraph.size() == 0)
				EnableMenuItem(GetMenu(hWnd),IDM_EDIT_UNDO,MF_GRAYED);
			if(vecGraphBac.size() == 0)
				EnableMenuItem(GetMenu(hWnd),IDM_EDIT_REDO,MF_GRAYED);

		}

		break;
	case WM_LBUTTONDOWN:
		pointStart.x = LOWORD(lParam);
		pointStart.y = HIWORD(lParam);
		if(dwToolNow ==  4 && vecPol.size() == 0)   //pol do 
			vecPol.push_back(pointStart);
		SetCapture(hWnd);
		break;
	case WM_MOUSEMOVE:
		pointNow.x = LOWORD(lParam);
		pointNow.y = HIWORD(lParam);

		if(pointNow.x > rect.right && pointNow.x < 10000)
			pointNow.x = rect.right;
		else if (pointNow.x > 10000)
			pointNow.x = 0;
		if(pointNow.y > rect.bottom&& pointNow.y < 10000)
			pointNow.y = rect.bottom;
		else if(pointNow.y > 10000 )
			pointNow.y = 0;
		if(wParam & MK_LBUTTON)
		{
			bIsDrawing = true;
			bNeedSave = true;
			if(bRedo)
			{
				for(int i = 0;i<vecGraphBac.size();i++)
				{
					delete vecGraphBac[i];
				}
				vecGraphBac.clear();
				bRedo = false;
			}
			SelectObject(hdcBit,GetStockObject(NULL_PEN) );
			SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
			Rectangle(hdcBit,0,0,rect.right,rect.bottom);

			for(int i = 0;i<vecGraph.size();i++)
			{
				vecGraph[i]->Draw(hdcBit);
			}
			hdc = GetDC(hWnd);
			switch(dwToolNow)
			{
			case 0:  //RECT
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				Rectangle(hdcMem,pointStart.x,pointStart.y,pointNow.x,pointNow.y);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				break;
			case 1://ellipse
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				Ellipse(hdcMem,pointStart.x,pointStart.y,pointNow.x,pointNow.y);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				break;
			case 2://roundrect
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				RoundRect(hdcMem,pointStart.x,pointStart.y,pointNow.x,pointNow.y, ABS(pointNow.x - pointStart.x)/4 , ABS(pointNow.y - pointStart.y)/4);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				break;
			case 3: //Line
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				MoveToEx(hdcMem,pointStart.x,pointStart.y,NULL);
				LineTo(hdcMem,pointNow.x,pointNow.y);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				break;
			case 4: //Pol
							
				BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
				for(int i =0;i< vecPol.size() -1 ;i++)
				{
					MoveToEx(hdcMem,vecPol[i].x,vecPol[i].y,NULL);
					LineTo(hdcMem,vecPol[i + 1].x,vecPol[i+1].y);
				}
				if( vecPol.size()-1 >=0)
					MoveToEx(hdcMem,vecPol[vecPol.size() -1].x,vecPol[vecPol.size() - 1].y,NULL);
				else
					MoveToEx(hdcMem,pointStart.x,pointStart.y,NULL);
				
				LineTo(hdcMem,pointNow.x,pointNow.y);
				BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				break;
					
			}
			ReleaseDC(hWnd,hdc);
		}
		break;

	case WM_LBUTTONUP:
		ReleaseCapture();
		if(bIsDrawing){
		bIsDrawing = false;
		pointNow.x = LOWORD(lParam);
		pointNow.y = HIWORD(lParam);
		rectNow.left = pointStart.x;
		rectNow.top = pointStart.y;
		rectNow.right = pointNow.x;
		rectNow.bottom = pointNow.y;
		if( dwToolNow !=4)
			bUndo = true;
		switch(dwToolNow)
		{
		case 0://rect
			vecGraph.push_back( new CRect(rectNow,penStyle,penWidth,penRGB,BrushStyle,BrushRGB));
			break;
		case 1: //ellipse
			vecGraph.push_back( new CEllipse(rectNow,penStyle,penWidth,penRGB,BrushStyle,BrushRGB));
			break;
		case 2://round rect
			vecGraph.push_back( new CRoundRect(rectNow,penStyle,penWidth,penRGB,BrushStyle,BrushRGB));
			break;
		case 3://Line
			vecGraph.push_back( new CLine(rectNow,penStyle,penWidth,penRGB) );
			break;
		case 4:   //pol do 
			vecPol.push_back(pointNow);
			break;	
		
		}
		cpd.cbData = 0;
		cpd.dwData = sizeof( vector<CInterface*> ) * vecGraph.size();
		cpd.lpData = &vecGraph;

		ofstreamFile.open(TEXT("c:\\tempmy.hx"),ofstream::out | ios_base::binary);
		for(int i = 0;i<vecGraph.size();i++)
		{
			(*vecGraph[i])>>ofstreamFile;
		}
		ofstreamFile.close();
		ofstreamFile.clear();
		for(int i = 0;i<g_vecHWND.size();i++)
		{
			SendMessage(g_vecHWND[i],WM_COPYDATA,(WPARAM)hWnd,(LPARAM)&vecGraph);
		}
		}
		break;
	case WM_LBUTTONDBLCLK:
		if(dwToolNow == 4)
		{
			vecGraph.push_back( new CPol(vecPol,penStyle,penWidth,penRGB,BrushStyle,BrushRGB) );
			bUndo = true;
			bIsDrawing = false;
			vecPol.clear();
			SelectObject(hdcBit,GetStockObject(NULL_PEN) );
			SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
			Rectangle(hdcBit,0,0,rect.right,rect.bottom);
			
			for(int i = 0;i<vecGraph.size();i++)
			{
				vecGraph[i]->Draw(hdcBit);
			}
			hdc = GetDC(hWnd);
			BitBlt(hdc,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);
			for(int i = 0;i<g_vecHWND.size();i++)
			SendMessage(g_vecHWND[i],WM_COPYDATA,(WPARAM)hWnd,(LPARAM)&vecGraph);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COPYDATA:
		g_vecHWND.push_back( (HWND)wParam );
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


LRESULT CALLBACK	WndProcRecieve(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	static RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC hdcBit;
	static HDC hdcMem;
	static HBITMAP hbitbit;
	static HBITMAP hbit;
	 COPYDATASTRUCT   cpd;   /*给COPYDATASTRUCT结构赋值*/   
	 COPYDATASTRUCT*  pcrecieve;
	static vector<CInterface*> vecGraph;
	ifstream ifstreamFile;
	DWORD dwGraphStyle;

         

	switch(message)
	{
	case WM_CREATE:
		GetClientRect(hWnd,&rect);
		hdc = GetDC(hWnd);
		hdcBit = CreateCompatibleDC(hdc);
		hbitbit = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		SelectObject(hdcBit,hbitbit);
		hdcMem = CreateCompatibleDC(hdc);
		hbit = CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		SelectObject(hdcMem,hbit);
		SelectObject(hdcMem,GetStockObject(NULL_BRUSH));
		SelectObject(hdcMem,GetStockObject(BLACK_PEN));
		SelectObject(hdcBit,GetStockObject(NULL_PEN) );
		SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
		Rectangle(hdcBit,rect.left,rect.top,rect.right,rect.bottom);
		BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
		SelectObject(hdcBit,GetStockObject(BLACK_PEN) );
		ReleaseDC(hWnd,hdc);
        cpd.dwData   =   0;   
	    cpd.cbData   =  3;
        cpd.lpData   =   NULL;   
		::SendMessage(g_HWNDParent,WM_COPYDATA,(WPARAM)hWnd,(LPARAM)&cpd);//发送！  
		break;
	case WM_COPYDATA:
		//open the file
		vecGraph.clear();

		ifstreamFile.open(TEXT("c:\\tempmy.hx"),ofstream::in | ios_base::binary);
		while(ifstreamFile)
		{
				ifstreamFile.read( ( char*)&dwGraphStyle,sizeof(DWORD) );
				switch(dwGraphStyle)
				{
				case 0: //Rect
					vecGraph.push_back(new CRect() );
					(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
					break;
				case 1://ellipse
					vecGraph.push_back(new CEllipse() );
					(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
					break;
				case 2://RoundRect
					vecGraph.push_back(new CRoundRect() );
					(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
					break;
				case 3://Line
					vecGraph.push_back(new CLine() );
					(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
					break;
				case 4://Pol
					vecGraph.push_back(new CPol() );
					(*vecGraph[vecGraph.size() -1 ])<<ifstreamFile;
					break;

				}
				
			}
			ifstreamFile.close();
			ifstreamFile.clear();
			SelectObject(hdcBit,GetStockObject(NULL_PEN) );
			SelectObject(hdcBit,GetStockObject(WHITE_BRUSH) );
			Rectangle(hdcBit,0,0,rect.right,rect.bottom);
				for(int i = 0;i<vecGraph.size();i++)
			{
				vecGraph[i]->Draw(hdcBit);
			}
			hdc = GetDC(hWnd);
			BitBlt(hdcMem,0,0,rect.right,rect.bottom,hdcBit,0,0,SRCCOPY);
			BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
			ReleaseDC(hWnd,hdc);
	break;
case WM_PAINT:
	hdc = BeginPaint(hWnd,&ps);
	BitBlt(hdc,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
	EndPaint(hWnd,&ps);
	break;
case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);

}