#include "Renderer.h"

#include "Types.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>


#include "imgui.h"

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

void Renderer::Initialize(HWND WindowHandle,int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	HRESULT hr;

	look_at_.x = 0;
	look_at_.y = 0;

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
			OutputDebugStringA(static_cast<char*>(msg->GetBufferPointer()));
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

	float fieldOfView = DirectX::XM_PI / 4.0f;
	float screenAspect = (float)800 / (float)600;

	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.03f, 10000.0f);	
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	HRESULT result = d3ddev->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
}

void Renderer::Clear() {
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	d3dctx->ClearRenderTargetView(view, color);

	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(camera_rot_.x,camera_rot_.y,camera_rot_.z);
	
	auto translation = DirectX::XMMatrixTranslation(camera_pos_.x,camera_pos_.y,camera_pos_.z);

	viewMatrix = DirectX::XMMatrixMultiply(rotm,translation);
	viewMatrix = DirectX::XMMatrixInverse(nullptr,viewMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
}

void Renderer::Present() {
	HRESULT res = sc->Present(0, 0);
}

void Renderer::CameraRotation(const vec2& dir, double deltaTime)
{
	camera_rot_.x += -dir.y * deltaTime;
	camera_rot_.y += dir.x * deltaTime;
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

void* Renderer::create_texture_buffer(const unsigned char* data,int x,int y,int n) const
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = x;
	desc.Height = y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality= 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE |D3D11_BIND_RENDER_TARGET ;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA d[1];
	d[0].pSysMem = data;
	d[0].SysMemPitch = (x * n) * sizeof(unsigned char);
;
	ID3D11Texture2D *pTexture = NULL;
	HRESULT  hr = d3ddev->CreateTexture2D( &desc, NULL, &pTexture );

	if (FAILED(hr))
	{
		return pTexture;
	}

	d3dctx->UpdateSubresource(
		pTexture,
		0,
		NULL,
		data,
		(x * n) * sizeof(unsigned char),
		0
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	ID3D11ShaderResourceView* textureView;
	hr = d3ddev->CreateShaderResourceView(
	  pTexture,
	  &srvDesc,
	  &textureView
	);

	if (FAILED(hr))
	{
		return pTexture;
	}

	d3dctx->GenerateMips(textureView);
	
	return textureView;
}

void Renderer::SetBuffers(Transform t, unsigned int numIndices, void* indexBuffer, void* vertexBuffer, void* texture)
{
	D3D11_MAPPED_SUBRESOURCE resource;

	CalculateMatrix(t);

	HRESULT result = d3dctx->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	d3dctx->Unmap( m_matrixBuffer, 0);

	bufferNumber = 0;
	d3dctx->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	auto tx = static_cast<ID3D11ShaderResourceView*>(texture);
	d3dctx->PSSetShaderResources(0, 1, &tx);

	unsigned int off = 0;
	unsigned int str = sizeof(Vertex);
	d3dctx->IASetIndexBuffer((ID3D11Buffer*)indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	d3dctx->IASetVertexBuffers(0, 1, (ID3D11Buffer*const*)&vertexBuffer, &str, &off);

	d3dctx->DrawIndexed(numIndices, 0, 0);
}

void Renderer::CameraPosition(const vec2& lt, double get_delta_time)
{
	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(camera_rot_.x,camera_rot_.y,camera_rot_.z);
	
	DirectX::XMVECTOR cpos = DirectX::XMVectorSet(lt.x,0,lt.y,0);
	
	cpos = DirectX::XMVector3TransformNormal(cpos,rotm);
	
	camera_pos_.x += DirectX::XMVectorGetX(cpos) * get_delta_time * 4;
	camera_pos_.y += DirectX::XMVectorGetY(cpos) * get_delta_time * 4;
	camera_pos_.z += DirectX::XMVectorGetZ(cpos) * get_delta_time * 4;
}

void Renderer::CalculateMatrix(Transform t)
{
	//DirectX::XMMATRIX rotm = 
	
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(t.scale.x,t.scale.y,t.scale.z);
	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(t.rotate.x,t.rotate.y,t.rotate.z);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(t.translate.x,t.translate.y,t.translate.z);
	
	worldMatrix = scale;
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotm);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translation);
	//worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, scale);
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
}
