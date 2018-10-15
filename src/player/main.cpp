
#include "config/RenderDeviceConfig.h"
#include "d3d12/VertexBufferD3D12.h"
#include "d3d12/DX12Pso.h"
#include "d3d12/DX12RootSignature.h"
#include "math/float4.h"
#include "Test_Vert.h"
#include "RenderObj.h"
#include "d3d12/ViewPort.h"
#include "d3d12/Texture.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <thread>

#include "pipeline.h"
#include "d3d12/RenderTarget.h"
#include "TestShader.h"
#include <wrl.h>

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
TestVertTex testVertTEX;

using Microsoft::WRL::ComPtr;

void RunPipline( Pipeline* pipline)
{
	pipline->Render();
	pipline->Wait();
}


int APIENTRY  wWinMain(_In_ HINSTANCE hInstance,
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

	HACCEL hAccelTable = LoadAccelerators(hInstance, L"gogo");

	MSG msg;

	OutputDebugString(L"before init device\n");
	D3D12::RenderDeviceD3D12::Instance()->InitDevice();
	OutputDebugString(L"after init device\n");
	g_pResourceCmdList = std::make_shared<D3D12::GraphicCommandList>();
	g_pResourceCmdList->ResetState();
	testVert.SetupVertData();
	testVertTEX.SetupVertData();

	 std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPG12 = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	 pPG12->LoadBuffers(g_pResourceCmdList, testVert.m_pData, nullptr);
	std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPGPRE = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	pPGPRE->LoadBuffers(g_pResourceCmdList, testVertTEX.m_pData, nullptr);

	TestShader testShader;
	OutputDebugString(L"compile shader\n");
	testShader.Init();
	CD3DX12_SHADER_BYTECODE vs = CD3DX12_SHADER_BYTECODE(testShader.mVertexShader);
	CD3DX12_SHADER_BYTECODE ps = CD3DX12_SHADER_BYTECODE(testShader.mPixelShader);
	OutputDebugString(L"AFTER compile shader\n");
	TestShader texTestShader;
	texTestShader.Init();
	CD3DX12_SHADER_BYTECODE vsTex = CD3DX12_SHADER_BYTECODE(texTestShader.mtexVertexShader);
	CD3DX12_SHADER_BYTECODE psTex = CD3DX12_SHADER_BYTECODE(texTestShader.mtexPixelShader);

 	std::shared_ptr<Material> mat = std::make_shared<Material>();
	std::shared_ptr<Material> texmat = std::make_shared<Material>();
	texmat->SetShaderCode(const_cast<void*>(vsTex.pShaderBytecode), vsTex.BytecodeLength, const_cast<void*>(psTex.pShaderBytecode), psTex.BytecodeLength);
	mat->SetShaderCode(const_cast<void*>(vs.pShaderBytecode), vs.BytecodeLength, const_cast<void*>(ps.pShaderBytecode), ps.BytecodeLength);

	
	Math::float4 color = Math::float4(1, 0, 0, 0);
	RenderBase::DataStream color_datastream;
	color_datastream.data = &color;
	color_datastream.sizeInByte = sizeof(color);
	mat->InitMat({ (uint)color_datastream.sizeInByte },0,0);
	mat->UpdateConstantBuffer(0, color_datastream);


	Math::float4 color2 = Math::float4(1, 1, 0, 1);
	RenderBase::DataStream color_datastream2;
	color_datastream2.data = &color2;
	color_datastream2.sizeInByte = sizeof(color2);
	texmat->InitMat({ (uint)color_datastream2.sizeInByte }, 2,1);
	texmat->UpdateConstantBuffer(0, color_datastream2);

	g_pResourceCmdList->ExecuteCommandList();
	g_pResourceCmdList->WaitForExecution();

	

	std::shared_ptr<ViewPortD3D12> pViewPort = std::make_shared<ViewPortD3D12>();
	pViewPort->Init( 800, 600);

	std::shared_ptr<RenderObj> pObj = std::make_shared<RenderObj>();
	pObj->Init( pPGPRE);
	pObj->m_pMaterial = texmat;


	std::shared_ptr<RenderObj> pObjPreChain = std::make_shared<RenderObj>();
	pObjPreChain->Init(pPG12);
	pObjPreChain->m_pMaterial = mat;

	std::shared_ptr<RenderObj> pObjPreChain1 = std::make_shared<RenderObj>();
	pObjPreChain1->Init(pPG12);
	pObjPreChain1->m_pMaterial = mat;

	Math::float4 offset = Math::float4(0.0, .0, .0, 0);
	RenderBase::DataStream datastream;
	datastream.data = &offset;
	datastream.sizeInByte = sizeof(offset);
	pObj->UpdatePosBuffer(datastream);

	Math::float4 offset2 = Math::float4(0.5, 0.5, 0.0, 0);
	RenderBase::DataStream datastream2;
	datastream2.data = &offset2;
	datastream2.sizeInByte = sizeof(offset2);
	pObjPreChain->UpdatePosBuffer(datastream2);

	Math::float4 offset3 = Math::float4(0.2, 0.2, 0.0, 0);
	RenderBase::DataStream datastream3;
	datastream3.data = &offset3;
	datastream3.sizeInByte = sizeof(offset3);
	pObjPreChain1->UpdatePosBuffer(datastream3);

	
	//pre pipeline
	std::shared_ptr<Texture> m_pP1RTTex = std::make_shared<Texture>();
	m_pP1RTTex->Init(true, 800, 600, RenderBase::PixelFormat::A4R4G4B4);
	std::shared_ptr<RenderTarget> m_P1RTTar = std::make_shared<RenderTarget>();
	m_P1RTTar->Init(m_pP1RTTex);

	Pipeline* rptt1 = new Pipeline();
	rptt1->SetViewPort(pViewPort);
	rptt1->SetRenderTarget(m_P1RTTar);
	rptt1->AddRenderObj(pObjPreChain);
	pObjPreChain->GenerateInternal(rptt1->GenerateMatExternalInfo());

	//pre pipeline2
	std::shared_ptr<Texture> m_pP1RTTex2 = std::make_shared<Texture>();
	m_pP1RTTex2->Init(true, 800, 600, RenderBase::PixelFormat::A4R4G4B4);
	std::shared_ptr<RenderTarget> m_P1RTTar2 = std::make_shared<RenderTarget>();
	m_P1RTTar2->Init(m_pP1RTTex2);

	Pipeline* rptt2 = new Pipeline();
	rptt2->SetViewPort(pViewPort);
	rptt2->SetRenderTarget(m_P1RTTar2);
	rptt2->AddRenderObj(pObjPreChain1);
	pObjPreChain1->GenerateInternal(rptt2->GenerateMatExternalInfo());


	//main pipeline
 	Pipeline* renderPipeline = new Pipeline();
 	renderPipeline->SetViewPort(pViewPort);
 	renderPipeline->SetRenderToScreen(RenderBase::PixelFormat::A4R4G4B4, hwnd);


	renderPipeline->AddRenderObj(pObj);
 	texmat->SetTexture(0, m_pP1RTTex);
	texmat->SetTexture(1, m_pP1RTTex2);
 	pObj->GenerateInternal(renderPipeline->GenerateMatExternalInfo());



	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		rptt1->Reset();
		rptt2->Reset();
		std::thread   t1(RunPipline, rptt1);
		std::thread   t2(RunPipline, rptt2);
//   		rptt1->Reset();
//   		rptt1->Render();
//   		rptt1->Wait();
// 
// 		rptt2->Reset();
// 		rptt2->Render();
// 		rptt2->Wait();
		t1.join();
		t2.join();
 		renderPipeline->Reset();
 		renderPipeline->Render();

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
	wcex.hIcon = LoadIcon(hInstance, L"gogo");
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