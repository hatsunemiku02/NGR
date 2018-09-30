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

const char* textureshadercode = "\
cbuffer objConstantBuffer : register(b0)\
{\
	float4 offset;\
};\
cbuffer matConstantBuffer : register(b1)\
{\
	float4 color;\
};\
Texture2D g_texture : register(t0);\
SamplerState g_sampler : register(s0);\
struct PSInput\
{\
	float4 position : SV_POSITION;\
	float2 uv : TEXCOORD;\
};\
\
PSInput VSMain(float4 position : POSITION, float4 uv : TEXCOORD)\
{\
	PSInput result;\
	result.position = position + offset;\
	result.uv = uv;\
	return result;\
}\
\
float4 PSMain(PSInput input) : SV_TARGET\
{\
	return color*g_texture.Sample(g_sampler, input.uv);\
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

	ID3DBlob* mtexVertexShader;
	ID3DBlob* mtexPixelShader;
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
	hr = D3DCompile(shadercode, strlen(shadercode), "testshader", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &mPixelShader, nullptr);

	hr = D3DCompile(textureshadercode, strlen(textureshadercode), "texturetestshader", nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &mtexVertexShader, nullptr);
	hr = D3DCompile(textureshadercode, strlen(textureshadercode), "texturetestshader", nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &mtexPixelShader, nullptr);

}

