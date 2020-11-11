#include "Renderer.h"

#include "Types.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
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

	if (FAILED(hr))
	{
		if (msg)
		{
			OutputDebugStringA((char*)msg->GetBufferPointer());
			msg->Release();
		}
	}

	hr = D3DCompileFromFile(L"Shader.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &msg);

	if (FAILED(hr))
	{
		if (msg)
		{
			OutputDebugStringA((char*)msg->GetBufferPointer());
			msg->Release();
		}
	}

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
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA, 0},
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
	
	float fieldOfView = DirectX::XM_PI / 4.0f;
	float screenAspect = (float)800 / (float)600;
	//D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, 0.03f, 100.0f);
	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.03f, 100.0f);
	
	//D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	HRESULT result = d3ddev->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	//D3DXMatrixIdentity(&worldMatrix);
	worldMatrix = DirectX::XMMatrixIdentity();
	
	//D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
}

void Renderer::Clear() {
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	d3dctx->ClearRenderTargetView(view, color);
}

void Renderer::Render() {
	HRESULT res = sc->Present(0, 0);
}

void* Renderer::CreateVertexBuffer(Vertex* vertices, size_t size)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertices[0]) * size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	ID3D11Buffer* pVertexBuffer = 0;
	d3ddev->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);

	unsigned int sovc = sizeof(vertices[0]) * size;
	D3D11_MAPPED_SUBRESOURCE ms;

	d3dctx->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, sizeof(vertices[0]) * size);
	d3dctx->Unmap(pVertexBuffer, NULL);
		
	return pVertexBuffer;
}

void* Renderer::CreateIndexBuffer(unsigned int* indices, unsigned int size)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	ID3D11Buffer* pIndexBuffer = 0;
	d3ddev->CreateBuffer(&bufferDesc, &InitData, &pIndexBuffer);

	return pIndexBuffer;
}

void Renderer::SetBuffers(vec3 pos, unsigned int numIndices, void* indexBuffer, void* vertexBuffer)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	CalculateMatrix(pos);

	HRESULT result = d3dctx->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	d3dctx->Unmap( m_matrixBuffer, 0);

	bufferNumber = 0;
	d3dctx->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	d3dctx->PSSetShaderResources(0, 1, &m_texture);

	unsigned int off = 0;
	unsigned int str = sizeof(Vertex);
	d3dctx->IASetIndexBuffer((ID3D11Buffer*)indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dctx->IASetVertexBuffers(0, 1, (ID3D11Buffer*const*)&vertexBuffer, &str, &off);

	d3dctx->DrawIndexed(numIndices, 0, 0);
}

void Renderer::CalculateMatrix(vec3 p) {
	vec3 rot;

	rot.x = 0; //yaw
	rot.y = 0; //pitch
	rot.z = 0; //roll

	// D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rot.x, rot.y, rot.z);
	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(rot.z, rot.y, rot.x);
	
	// D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	DirectX::XMVECTOR lookAt = DirectX::XMVectorSet(0,0,1.0f,0);
	lookAt = DirectX::XMVector3TransformCoord(lookAt, rotm);

	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1.0f, 0, 0);
	up = DirectX::XMVector3TransformCoord(up, rotm);

	// lookAt = add(position, lookAt);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(p.x, p.y, p.z, 0);
	lookAt = DirectX::XMVectorAdd(pos, lookAt);

	// D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);
	// D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	viewMatrix = DirectX::XMMatrixLookAtLH(pos, lookAt, up);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
}