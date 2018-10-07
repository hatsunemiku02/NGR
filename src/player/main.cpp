
#include "config/RenderDeviceConfig.h"
#include "d3d12/VertexBufferD3D12.h"
#include "d3d12/DX12Pso.h"
#include "d3d12/DX12RootSignature.h"
#include "math/float4.h"
#include "Test_Vert.h"
#include "RenderObj.h"
#include "ViewPort.h"
#include "Texture.h"
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
TestVertTex testVertTEX;
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
	testVertTEX.SetupVertData();

	std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPG12 = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	pPG12->LoadBuffers(g_pResourceCmdList, testVert.m_pData, nullptr);

	std::shared_ptr<D3D12::PrimitiveGroupD3D12> pPGPRE = std::make_shared<D3D12::PrimitiveGroupD3D12>();
	pPGPRE->LoadBuffers(g_pResourceCmdList, testVertTEX.m_pData, nullptr);

	TestShader testShader;
	testShader.Init();
	CD3DX12_SHADER_BYTECODE vs = CD3DX12_SHADER_BYTECODE(testShader.mVertexShader);
	CD3DX12_SHADER_BYTECODE ps = CD3DX12_SHADER_BYTECODE(testShader.mPixelShader);

	TestShader texTestShader;
	texTestShader.Init();
	CD3DX12_SHADER_BYTECODE vsTex = CD3DX12_SHADER_BYTECODE(texTestShader.mtexVertexShader);
	CD3DX12_SHADER_BYTECODE psTex = CD3DX12_SHADER_BYTECODE(texTestShader.mtexPixelShader);

 	std::shared_ptr<Material> mat = std::make_shared<Material>();
	std::shared_ptr<Material> texmat = std::make_shared<Material>();
	texmat->SetShaderCode(const_cast<void*>(vsTex.pShaderBytecode), vsTex.BytecodeLength, const_cast<void*>(psTex.pShaderBytecode), psTex.BytecodeLength);
	mat->SetShaderCode(const_cast<void*>(vs.pShaderBytecode), vs.BytecodeLength, const_cast<void*>(ps.pShaderBytecode), ps.BytecodeLength);

	
	Math::float4 color = Math::float4(1, 0, 1, 0);
	RenderBase::DataStream color_datastream;
	color_datastream.data = &color;
	color_datastream.sizeInByte = sizeof(color);
	mat->InitMat({ (uint)color_datastream.sizeInByte },0);
	mat->UpdateConstantBuffer(0, color_datastream);

	texmat->InitMat({ (uint)color_datastream.sizeInByte }, 1);
	texmat->UpdateConstantBuffer(0, color_datastream);

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

	Math::float4 offset = Math::float4(0.5, .5, .5, 0);
	RenderBase::DataStream datastream;
	datastream.data = &offset;
	datastream.sizeInByte = sizeof(offset);
	pObj->UpdatePosBuffer(datastream);

	Math::float4 offset2 = Math::float4(0.0, 0.0, 0.0, 0);
	RenderBase::DataStream datastream2;
	datastream2.data = &offset2;
	datastream2.sizeInByte = sizeof(offset2);
	pObjPreChain->UpdatePosBuffer(datastream2);
	


	//main pipeline
	Pipeline* renderPipeline = new Pipeline();
	renderPipeline->SetViewPort(pViewPort);
	renderPipeline->SetRenderToScreen(RenderBase::PixelFormat::A4R4G4B4, hwnd);
	renderPipeline->AddRenderObj(pObj);
	pObj->GenerateInternal(renderPipeline->GenerateMatExternalInfo());


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




	// ����Ϣѭ��: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{

		rptt1->Reset();
		rptt1->Render();

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