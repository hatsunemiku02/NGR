
#include "config/RenderDeviceConfig.h"
#include "d3d12/VertexBufferD3D12.h"
#include "d3d12/DX12Pso.h"
#include "d3d12/DX12RootSignature.h"
#include "Test_Vert.h"
#include "RenderObj.h"
#include "ViewPort.h"
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include "pipeline.h"
#include "RenderTarget.h"
#include "TestShader.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
const WCHAR* szTitle = L"testplayer";               // �������ı�
const WCHAR* szWindowClass = L"testplayer1";      // ����������

												// �˴���ģ���а����ĺ�����ǰ������: 
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

	// TODO: �ڴ˷��ô��롣

	// ��ʼ��ȫ���ַ���
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_DX12RENDER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	HWND hwnd = InitInstance(hInstance, nCmdShow);
	if (hwnd==nullptr)
	{
		return FALSE;
	}
	

	HACCEL hAccelTable = LoadAccelerators(hInstance, L"gogo");

	MSG msg;

	D3D12::RenderDeviceD3D12::Instance()->InitDevice();
	


	g_pResourceCmdList = std::make_shared<D3D12::GraphicCommandList>();
	g_pResourceCmdList->ResetState();
	testVert.SetupVertData();

	std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPG12 = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	pPG12->LoadBuffers(g_pResourceCmdList, testVert.m_pData, nullptr);

	TestShader testShader;
	testShader.Init();

	RenderBase::PsoData psoData;

	RenderBase::VertexLayoutDesc vertLayoutDesc;
	vertLayoutDesc.name = "POSITION";
	vertLayoutDesc.offset = 0;
	vertLayoutDesc.si = 0;
	psoData.vertexLayout.push_back(vertLayoutDesc);
	RenderBase::DeviceBlendState blendState;
	psoData.renderBlendState = blendState;
	RenderBase::DeviceRasterizerState rasterState;
	psoData.renderRasterizerState = rasterState;
	RenderBase::DeviceDepthAndStencilState dsState;
	psoData.renderDepthStencilState = dsState;
	psoData.rtvFormat = RenderBase::PixelFormat::A8R8G8B8;

	CD3DX12_SHADER_BYTECODE vs = CD3DX12_SHADER_BYTECODE(testShader.mVertexShader);
	psoData.vsByteCode = const_cast<void*>(vs.pShaderBytecode);
	psoData.vsLength = vs.BytecodeLength;

	CD3DX12_SHADER_BYTECODE ps = CD3DX12_SHADER_BYTECODE(testShader.mPixelShader);
	psoData.psByteCode = const_cast<void*>(ps.pShaderBytecode);
	psoData.psLength = ps.BytecodeLength;


	RenderBase::SignatureInfo sigInfo;
	std::shared_ptr<RenderBase::RootSignature> pRS = std::make_shared<D3D12::DX12RootSignature>();
	pRS->Init(sigInfo);

	std::shared_ptr<D3D12::DX12Pso> dx12pso = std::make_shared<D3D12::DX12Pso>();

	dx12pso->Init(psoData, pRS);

	g_pResourceCmdList->ExecuteCommandList();
	g_pResourceCmdList->WaitForExecution();

	Pipeline* renderPipeline = new Pipeline();
	std::shared_ptr<ViewPortD3D12> pViewPort = std::make_shared<ViewPortD3D12>();
	pViewPort->Init( 800, 600, hwnd);
	std::shared_ptr<RenderTarget> pRenderTarget = std::make_shared<RenderTarget>();
	pRenderTarget->Init(pViewPort);
	renderPipeline->SetViewPort(pViewPort);

	std::shared_ptr<RenderObj> pObj = std::make_shared<RenderObj>();
	pObj->Init(pPG12);
	pObj->m_pPipeStateObj = dx12pso;
	pObj->m_pRootSig = std::static_pointer_cast<D3D12::DX12RootSignature>(pRS);
	renderPipeline->AddRenderObj(pObj);

	// ����Ϣѭ��: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND  - ����Ӧ�ó���˵�
//  WM_PAINT    - ����������
//  WM_DESTROY  - �����˳���Ϣ������
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
		// TODO: �ڴ˴����ʹ�� hdc ���κλ�ͼ����...
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

// �����ڡ������Ϣ�������
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