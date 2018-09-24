
#include "config/RenderDeviceConfig.h"
#include "d3d12/VertexBufferD3D12.h"
#include "Test_Vert.h"
#include "RenderObj.h"
#include "ViewPort.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "pipeline.h"
#include "RenderTarget.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
const WCHAR* szTitle = L"testplayer";               // 标题栏文本
const WCHAR* szWindowClass = L"testplayer1";      // 主窗口类名

												// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

D3D12::RenderDeviceD3D12* g_pDevice = new D3D12::RenderDeviceD3D12();
std::shared_ptr<D3D12::GraphicCommandList> g_pResourceCmdList;
TestVert testVert;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。

	// 初始化全局字符串
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_DX12RENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	HWND hwnd = InitInstance(hInstance, nCmdShow);
	if (hwnd==nullptr)
	{
		return FALSE;
	}
	

	HACCEL hAccelTable = LoadAccelerators(hInstance, "gogo");

	MSG msg;

	D3D12::RenderDeviceD3D12::Instance()->InitDevice();
	


	g_pResourceCmdList = std::make_shared<D3D12::GraphicCommandList>();
	g_pResourceCmdList->ResetState();
	testVert.SetupVertData();

	std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPG12 = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	g_pResourceCmdList->ExecuteCommandList();
	g_pResourceCmdList->WaitForExecution();

	pipeline* renderPipeline = new pipeline();
	std::shared_ptr<ViewPortD3D12> pViewPort = std::make_shared<ViewPortD3D12>();
	pViewPort->Init( 800, 600, hwnd);
	std::shared_ptr<RenderTarget> pRenderTarget = std::make_shared<RenderTarget>();
	pRenderTarget->Init(pViewPort);
	renderPipeline->SetViewPort(pViewPort);

	std::shared_ptr<RenderObj> pObj = std::make_shared<RenderObj>();
	pObj->Init(pPG12);
	renderPipeline->AddRenderObj(pObj);

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		renderPipeline->Reset();
		renderPipeline->Render();
		// 		gDevice.BeforeRender();
		// 		gDevice.DrawInstance(3, 1, 0, 0);
		// 		gDevice.AfterRender();
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, "gogo");
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = L"testplayer";
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = nullptr;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}




	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	return hWnd;
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
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
// 		switch (wmId)
// 		{
// 		case IDM_ABOUT:
// 			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
// 			break;
// 		case IDM_EXIT:
// 			DestroyWindow(hWnd);
// 			break;
// 		default:
// 			return DefWindowProc(hWnd, message, wParam, lParam);
// 		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
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