#include "Renderer.h"

#include "Types.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


#include "imgui.h"

struct MatrixBufferType
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
};

void Renderer::CheckError(HRESULT hr, ID3D10Blob* msg)
{
	if (FAILED(hr))
	{
		if (msg)
		{
			OutputDebugStringA(static_cast<char*>(msg->GetBufferPointer()));
			msg->Release();
		}
	}
}

void Renderer::Initialize(HWND WindowHandle,int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
	HRESULT hr;

	look_at.x = 0;
	look_at.y = 0;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;
	DXGI_OUTPUT_DESC od;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &output);
	output->GetDesc(&od);
	output->Release();
	
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = WindowHandle;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
	UINT numLevelsRequested = 1;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						//	pAdapter
		D3D_DRIVER_TYPE_HARDWARE,	//	DriverType
		nullptr,						//	Software
		NULL,						//	Flags
		&FeatureLevelsRequested,	//	pFeatureLevels
		1,							//  FeatureLevels
		D3D11_SDK_VERSION,			//	SDKVersion
		&scd,						//	pSwapChainDesc
		&m_SwapChain,						//	ppSwapChain
		&d3ddev,					//	ppDevice
		nullptr,						//	pFeatureLevel
		&d3dctx);					//	ppImmediateContext

	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&d3dbb);

	D3D11_RENDER_TARGET_VIEW_DESC vd;
	D3D11_VIEWPORT vp;
	vd.Format = DXGI_FORMAT_UNKNOWN;
	vd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipSlice = 0;
	if(d3dbb){
		hr = d3ddev->CreateRenderTargetView(d3dbb, NULL, &view);
	}
	else
	{
		
	}

	d3dctx->OMSetRenderTargets(1, &view, NULL);
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dctx->RSSetViewports(1, &vp);

	
	ID3D10Blob* VS, * PS;
	ID3D10Blob* msg = nullptr;
	hr = D3DCompileFromFile(L"Shader.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, &msg);
	CheckError(hr, msg);

	hr = D3DCompileFromFile(L"Shader.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, &msg);
	CheckError(hr, msg);

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

	auto fieldOfView = DirectX::XM_PI / 4.0f;
	auto screenAspect = 800.0f / 600.0f;

	projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.03f, 10000.0f);	
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	auto result = d3ddev->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
}

void Renderer::Clear() {
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	d3dctx->ClearRenderTargetView(view, color);

	const auto rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(camera_rot.x,camera_rot.y,camera_rot.z);

	const auto translation = DirectX::XMMatrixTranslation(camera_pos.x,camera_pos.y,camera_pos.z);

	viewMatrix = DirectX::XMMatrixMultiply(rotMatrix,translation);
	viewMatrix = DirectX::XMMatrixInverse(nullptr,viewMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
}

void Renderer::Present() const
{
	m_SwapChain->Present(0, 0);
}

void Renderer::viewport() const
{
	float color[4] = { 0.0f, 0.5f, 0.5f, 1.0f };
	d3dctx->OMSetRenderTargets(1, &view, nullptr);
	d3dctx->ClearRenderTargetView(view, color);
}

void Renderer::CameraRotation(const Vec2& dir, double deltaTime)
{
	camera_rot.x += static_cast<float>(deltaTime * static_cast<double>(-dir.y));
	camera_rot.y += static_cast<float>(static_cast<double>(dir.x) * deltaTime);
}

ID3D11Buffer* Renderer::CreateVertexBuffer(Vertex* vertices, size_t size) const
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

	ID3D11Buffer* pVertexBuffer = nullptr;
	d3ddev->CreateBuffer(&bufferDesc, NULL, &pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	if(pVertexBuffer) {
		d3dctx->Map(pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices[0]) * size);
		d3dctx->Unmap(pVertexBuffer, NULL);		
	}
	else
	{
		return nullptr;
	}
	return pVertexBuffer;
}

ID3D11Buffer* Renderer::CreateIndexBuffer(const unsigned int* indices, unsigned int size) const
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

ID3D11ShaderResourceView* Renderer::CreateTextureBuffer(const unsigned char* data,int x,int y,int n) const
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
	ID3D11Texture2D* p_texture = nullptr;
	auto hr = d3ddev->CreateTexture2D( &desc, nullptr, &p_texture );

	if (FAILED(hr))
	{
		return nullptr;
	}

	d3dctx->UpdateSubresource(
		p_texture,
		0,
		nullptr,
		data,
		(static_cast<unsigned long long>(x) * n) * sizeof(unsigned char),
		0
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	ID3D11ShaderResourceView* texture_view;
	hr = d3ddev->CreateShaderResourceView(
		p_texture,
		&srvDesc,
		&texture_view
	);

	if (FAILED(hr))
	{
		return nullptr;
	}

	d3dctx->GenerateMips(texture_view);
	
	return texture_view;
}

void Renderer::SetBuffers(Transform t, unsigned int num_indices, ID3D11Buffer* index_buffer, ID3D11Buffer* vertex_buffer, void* texture)
{
	CalculateMatrix(t);

	d3dctx->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = static_cast<MatrixBufferType*>(mappedResource.pData);

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	d3dctx->Unmap( m_matrixBuffer, 0);

	bufferNumber = 0;
	d3dctx->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	auto* tx = static_cast<ID3D11ShaderResourceView*>(texture);
	d3dctx->PSSetShaderResources(0, 1, &tx);

	unsigned int off = 0;
	unsigned int str = sizeof(Vertex);
	d3dctx->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	d3dctx->IASetVertexBuffers(0, 1, &vertex_buffer, &str, &off);

	d3dctx->DrawIndexed(num_indices, 0, 0);
}

void Renderer::CameraPosition(const Vec2& lt, double get_delta_time)
{
	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(camera_rot.x,camera_rot.y,camera_rot.z);
	
	DirectX::XMVECTOR cpos = DirectX::XMVectorSet(lt.x,0,lt.y,0);
	
	cpos = DirectX::XMVector3TransformNormal(cpos,rotm);
	
	camera_pos.x += DirectX::XMVectorGetX(cpos) * get_delta_time * 4;
	camera_pos.y += DirectX::XMVectorGetY(cpos) * get_delta_time * 4;
	camera_pos.z += DirectX::XMVectorGetZ(cpos) * get_delta_time * 4;
}

void Renderer::CalculateMatrix(Transform t)
{
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(t.scale.x,t.scale.y,t.scale.z);
	DirectX::XMMATRIX rotm = DirectX::XMMatrixRotationRollPitchYaw(t.rotate.x,t.rotate.y,t.rotate.z);
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(t.translate.x,t.translate.y,t.translate.z);
	
	worldMatrix = scale;
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, rotm);
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, translation);
	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
}
