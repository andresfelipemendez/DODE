#include "Renderer.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

struct MatrixBufferType
{
	float world[4][4];
	float view[4][4];
	float projection[4][4];
};

void Renderer::Initialize(HWND WindowHandle,int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	HRESULT hr;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;
	DXGI_OUTPUT_DESC od;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &output);
	output->GetDesc(&od);
	output->Release();

	if (FAILED(hr)) {
		if (hr == E_INVALIDARG) {
			int i = 0;
		}
		int i = 0;
	}
	
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = WindowHandle;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	D3D_FEATURE_LEVEL FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT numLevelsRequested = 1;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,						//	pAdapter
		D3D_DRIVER_TYPE_HARDWARE,	//	DriverType
		NULL,						//	Software
		NULL,						//	Flags
		&FeatureLevelsRequested,	//	pFeatureLevels
		1,							//  FeatureLevels
		D3D11_SDK_VERSION,			//	SDKVersion
		&scd,						//	pSwapChainDesc
		&sc,						//	ppSwapChain
		&d3ddev,					//	ppDevice
		NULL,						//	pFeatureLevel
		&d3dctx);					//	ppImmediateContext

	if (FAILED(hr)) {
		if (hr == E_INVALIDARG) {
			int i = 0;
		}
		else if (hr == DXGI_ERROR_INVALID_CALL)
		{
			int i = 0;
		}
		int i = 0;
	}

	hr = sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dbb);

	D3D11_RENDER_TARGET_VIEW_DESC vd;
	D3D11_VIEWPORT vp;
	vd.Format = DXGI_FORMAT_UNKNOWN;
	vd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipSlice = 0;
	hr = d3ddev->CreateRenderTargetView(d3dbb, NULL, &view);

	d3dctx->OMSetRenderTargets(1, &view, NULL);
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dctx->RSSetViewports(1, &vp);

	
	ID3D10Blob* VS, * PS;
	ID3D10Blob* msg = NULL;
	hr = D3DCompileFromFile(L"Shader.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &msg);
	hr = D3DCompileFromFile(L"Shader.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &msg);

	hr = d3ddev->CreateVertexShader( VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	d3ddev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
	
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = d3ddev->CreateSamplerState(&samplerDesc, &m_sampleState);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	d3ddev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &m_layout);

	d3dctx->IASetInputLayout( m_layout);
	d3dctx->VSSetShader( pVS, 0, 0);
	d3dctx->PSSetShader( pPS, 0, 0);
	d3dctx->PSSetSamplers( 0, 1, &m_sampleState);

	//D3DX11_IMAGE_LOAD_INFO dili = { 0 };

	//dili.Width = 341;
	//dili.Height = 125;
	//dili.Depth = D3DX11_DEFAULT;
	//dili.FirstMipLevel = D3DX11_DEFAULT;
	//dili.MipLevels = D3DX11_DEFAULT;
	//dili.Usage = (D3D11_USAGE)D3DX11_DEFAULT;
	//dili.BindFlags = D3DX11_DEFAULT;
	//dili.CpuAccessFlags = D3DX11_DEFAULT;
	//dili.MiscFlags = D3DX11_DEFAULT;
	//dili.Format = DXGI_FORMAT_FROM_FILE;
	//dili.Filter = D3DX11_DEFAULT;
	//dili.MipFilter = D3DX11_DEFAULT;
	//dili.pSrcInfo = NULL;

	//hr = D3DX11CreateShaderResourceViewFromFile(d3ddev, L"awdli-om21s.dds", &dili, NULL, &m_texture, NULL);
	
}

void Renderer::Render() {
	float color[4] = { 0.0f, 2.0f, 1.0f, 255 };
	d3dctx->ClearRenderTargetView(view, color);
	HRESULT res = sc->Present(0, 0);
}