#pragma once
const char* shadercode = "\
cbuffer objConstantBuffer : register(b0)\
{\
	float4 offset;\
};\
cbuffer matConstantBuffer : register(b1)\
{\
	float4 color;\
};\
struct PSInput\
{\
	float4 position : SV_POSITION;\
};\
\
PSInput VSMain(float4 position : POSITION)\
{\
	PSInput result;\
\
	result.position = position + offset;\
\
	return result;\
}\
\
float4 PSMain(PSInput input) : SV_TARGET\
{\
	return color;\
}\
";
#include "d3d12/RenderDeviceD3D12.h"
#include <d3dcompiler.h>
class TestShader
{
public:


	void Init();

	ID3DBlob* mVertexShader;
	ID3DBlob* mPixelShader;
};

void TestShader::Init()
{
#if defined(_DEBUG)
	// Enable better shader debugging with the graphics debugging tools.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	HRESULT hr;
	hr = D3DCompile(shadercode, strlen(shadercode) , "testshader", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &mVertexShader, nullptr);
	if (FAILED(hr))
	{
		throw std::exception();
	}
	hr = D3DCompile(shadercode, strlen(shadercode), "testshader", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &mPixelShader, nullptr);
}

