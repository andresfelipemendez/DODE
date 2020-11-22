#include "Renderer.h"

#include "Types.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Lib/imgui/imgui.h"

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

void Renderer::Initialize(HWND window_handle, float screen_width, float screen_height)
{
	HRESULT hr;

	look_at.x = 0;
	look_at.y = 0;

	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* output = nullptr;
	DXGI_OUTPUT_DESC od;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&factory));
	factory->EnumAdapters(0, &adapter);
	adapter->EnumOutputs(0, &output);
	output->GetDesc(&od);
	output->Release();
	
	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window_handle;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	auto feature_levels_requested = D3D_FEATURE_LEVEL_11_0;
	
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		NULL,
		&feature_levels_requested,
		1,
		D3D11_SDK_VERSION,
		&scd,
		&m_SwapChain,
		&d3ddev,
		nullptr,
		&d3dctx);

	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_D3dbb));

	D3D11_RENDER_TARGET_VIEW_DESC vd{};
	D3D11_VIEWPORT vp{};
	vd.Format = DXGI_FORMAT_UNKNOWN;
	vd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	vd.Texture2D.MipSlice = 0;
	if (m_D3dbb) 
	{
		hr = d3ddev->CreateRenderTargetView(m_D3dbb, nullptr, &m_View);
	}

	d3dctx->OMSetRenderTargets(1, &m_View, nullptr);
	vp.Width = screen_width;
	vp.Height = screen_height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dctx->RSSetViewports(1, &vp);

	
	ID3D10Blob* vs;
	ID3D10Blob* ps;
	ID3D10Blob* msg = nullptr;
	hr = D3DCompileFromFile(L"Shader.shader", nullptr, nullptr, "VShader", "vs_4_0", 0, 0, &vs, &msg);
	CheckError(hr, msg);

	hr = D3DCompileFromFile(L"Shader.shader", nullptr, nullptr, "PShader", "ps_4_0", 0, 0, &ps, &msg);
	CheckError(hr, msg);

	hr = d3ddev->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), nullptr, &m_PVs);
	d3ddev->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), nullptr, &m_PPs);
	
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
	hr = d3ddev->CreateSamplerState(&samplerDesc, &m_SampleState);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	d3ddev->CreateInputLayout(ied, 3, vs->GetBufferPointer(), vs->GetBufferSize(), &m_Layout);

	d3dctx->IASetInputLayout(m_Layout);
	d3dctx->VSSetShader(m_PVs, nullptr, 0);
	d3dctx->PSSetShader(m_PPs, nullptr, 0);
	d3dctx->PSSetSamplers(0, 1, &m_SampleState);

	auto field_of_view = DirectX::XM_PI / 4.0f;
	auto screen_aspect = screen_width / screen_height;

	m_ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(field_of_view, screen_aspect, 0.03f, 10000.0f);	
	m_ProjectionMatrix = DirectX::XMMatrixTranspose(m_ProjectionMatrix);

	D3D11_BUFFER_DESC matrix_buffer_desc{};
	matrix_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	matrix_buffer_desc.ByteWidth = sizeof(MatrixBufferType);
	matrix_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrix_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrix_buffer_desc.MiscFlags = 0;
	matrix_buffer_desc.StructureByteStride = 0;
	d3ddev->CreateBuffer(&matrix_buffer_desc, nullptr, &m_MatrixBuffer);


	D3D11_BUFFER_DESC vertex_buffer_desc;
 
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	vertex_buffer_desc.ByteWidth = 2 * sizeof( Vertex );
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertex_buffer_desc.MiscFlags = 0;
 
	if(FAILED(d3ddev->CreateBuffer( &vertex_buffer_desc, NULL, &m_VertexBuffer ) ) )
		return;
 
	/*this->bIsInitialized = true;
 
	return this->bIsInitialized;*/
}

void Renderer::Clear()
{
	float color[4] = {0.0f, 0.5f, 0.5f, 1.0f};
	d3dctx->ClearRenderTargetView(m_View, color);

	const auto rotation = DirectX::XMMatrixRotationRollPitchYaw(camera_rot.x, camera_rot.y, camera_rot.z);
	const auto translation = DirectX::XMMatrixTranslation(camera_pos.x, camera_pos.y, camera_pos.z);

	m_ViewMatrix = DirectX::XMMatrixMultiply(rotation, translation);
	m_ViewMatrix = DirectX::XMMatrixInverse(nullptr, m_ViewMatrix);
	m_ViewMatrix = DirectX::XMMatrixTranspose(m_ViewMatrix);
}

void Renderer::Present() const
{
	m_SwapChain->Present(0, 0);
}

void Renderer::Viewport() const
{
	float color[4] = {0.0f, 0.5f, 0.5f, 1.0f};
	d3dctx->OMSetRenderTargets(1, &m_View, nullptr);
	
	d3dctx->ClearRenderTargetView(m_View, color);
}

void Renderer::CameraRotation(const Vec2& dir, double delta_time)
{
	camera_rot.x += static_cast<float>(delta_time * static_cast<double>(-dir.y));
	camera_rot.y += static_cast<float>(static_cast<double>(dir.x) * delta_time);
}

ID3D11Buffer* Renderer::CreateVertexBuffer(Vertex* vertices, size_t size) const
{
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertices[0])) * static_cast<UINT>(size);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	ID3D11Buffer* p_vertex_buffer = nullptr;
	d3ddev->CreateBuffer(&buffer_desc, nullptr, &p_vertex_buffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	if (p_vertex_buffer) 
	{
		d3dctx->Map(p_vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vertices, sizeof(vertices[0]) * size);
		d3dctx->Unmap(p_vertex_buffer, NULL);		
	}
	else
	{
		return nullptr;
	}
	return p_vertex_buffer;
}

ID3D11Buffer* Renderer::CreateIndexBuffer(const unsigned int* indices, unsigned int size) const
{
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = sizeof(unsigned int) * size;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = indices;
	init_data.SysMemPitch = 0;
	init_data.SysMemSlicePitch = 0;

	ID3D11Buffer* p_index_buffer = nullptr;
	d3ddev->CreateBuffer(&buffer_desc, &init_data, &p_index_buffer);

	return p_index_buffer;
}

ID3D11ShaderResourceView* Renderer::CreateTextureBuffer(const unsigned char* data, int x, int y, int n) const
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = x;
	desc.Height = y;
	desc.MipLevels = 0;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA d[1];
	d[0].pSysMem = data;
	d[0].SysMemPitch = (x * n) * sizeof(unsigned char);

	ID3D11Texture2D* p_texture = nullptr;
	auto hr = d3ddev->CreateTexture2D(&desc, nullptr, &p_texture);

	if (FAILED(hr))
	{
		return nullptr;
	}

	d3dctx->UpdateSubresource(
		p_texture,
		0,
		nullptr,
		data,
		(static_cast<UINT>(x) * n) * sizeof(unsigned char),
		0
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
	srv_desc.Format = desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MostDetailedMip = 0;
	srv_desc.Texture2D.MipLevels = -1;

	ID3D11ShaderResourceView* texture_view;
	hr = d3ddev->CreateShaderResourceView(
		p_texture,
		&srv_desc,
		&texture_view
	);

	if (FAILED(hr))
	{
		return nullptr;
	}

	d3dctx->GenerateMips(texture_view);
	
	return texture_view;
}

void Renderer::Draw(Transform t, unsigned int num_indices, ID3D11Buffer* index_buffer, ID3D11Buffer* vertex_buffer, void* texture)
{
	CalculateMatrix(t);

	d3dctx->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedResource);
	m_DataPtr = static_cast<MatrixBufferType*>(m_MappedResource.pData);

	m_DataPtr->world = m_WorldMatrix;
	m_DataPtr->view = m_ViewMatrix;
	m_DataPtr->projection = m_ProjectionMatrix;

	d3dctx->Unmap(m_MatrixBuffer, 0);

	m_BufferNumber = 0;
	d3dctx->VSSetConstantBuffers(m_BufferNumber, 1, &m_MatrixBuffer);
	auto* tx = static_cast<ID3D11ShaderResourceView*>(texture);
	d3dctx->PSSetShaderResources(0, 1, &tx);

	unsigned int off = 0;
	unsigned int str = sizeof(Vertex);
	d3dctx->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	d3dctx->IASetVertexBuffers(0, 1, &vertex_buffer, &str, &off);

	d3dctx->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	
	d3dctx->DrawIndexed(num_indices, 0, 0);
}

void Renderer::DrawLine(Vec3 p1, Vec3 p2)
{
	Transform t;
	t.translate = {0, 0, 0};
	t.rotate = {0, 0, 0};
	t.scale = {1.0f, 1.0f, 1.0f};
	CalculateMatrix(t);

	d3dctx->Map(m_MatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &m_MappedResource);
	m_DataPtr = static_cast<MatrixBufferType*>(m_MappedResource.pData);

	m_DataPtr->world = m_WorldMatrix;
	m_DataPtr->view = m_ViewMatrix;
	m_DataPtr->projection = m_ProjectionMatrix;

	d3dctx->Unmap(m_MatrixBuffer, 0);
	m_BufferNumber = 0;
	d3dctx->VSSetConstantBuffers(m_BufferNumber, 1, &m_MatrixBuffer);
	
	UINT viewport_number = 1;
 
	D3D11_VIEWPORT vp;

	d3dctx->RSGetViewports(&viewport_number, &vp);

	D3D11_MAPPED_SUBRESOURCE map_data;
 
	if (FAILED(d3dctx->Map( m_VertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &map_data)))
		return;

	auto* const v = static_cast<Vertex*>(map_data.pData);

	
	v[0].position[0] = p1.x;
	v[0].position[1] = p1.y;
	v[0].position[2] = p1.z;
 
	v[1].position[0] = p2.x;
	v[1].position[1] = p2.y;
	v[1].position[2] = p2.z;
	
	d3dctx->Unmap(m_VertexBuffer, NULL);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
 
	d3dctx->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
 
	d3dctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	
	d3dctx->Draw(2, 0);
}

void Renderer::DrawCircle(Vec3 center, float radius)
{
	for (auto i = 0; i < 24; ++i)
	{
		auto right = DirectX::XMVectorSet(1, 0, 0, 0);
		auto yaw = DirectX::XMConvertToRadians((15.0f) * static_cast<float>(i));
		auto rotation = DirectX::XMMatrixRotationRollPitchYaw(0, yaw, 0);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x1 = DirectX::XMVectorGetX(right);
		auto y1 = DirectX::XMVectorGetY(right);
		auto z1 = DirectX::XMVectorGetZ(right);
		
		right = DirectX::XMVectorSet(1, 0, 0, 0);
		yaw = DirectX::XMConvertToRadians((15.0f) * (static_cast<float>(i) + 1));
		rotation = DirectX::XMMatrixRotationRollPitchYaw(0, yaw, 0);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x2 = DirectX::XMVectorGetX(right);
		auto y2 = DirectX::XMVectorGetY(right);
		auto z2 = DirectX::XMVectorGetZ(right);
		
		DrawLine({x1,y1,z1},{x2,y2,z2});
	}

	for (auto i = 0; i < 24; ++i)
	{
		auto right = DirectX::XMVectorSet(0, 1, 0, 0);
		auto roll = DirectX::XMConvertToRadians((15.0f) * static_cast<float>(i));
		auto rotation = DirectX::XMMatrixRotationRollPitchYaw(0, 0, roll);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x1 = DirectX::XMVectorGetX(right);
		auto y1 = DirectX::XMVectorGetY(right);
		auto z1 = DirectX::XMVectorGetZ(right);
		
		right = DirectX::XMVectorSet(0, 1, 0, 0);
		roll = DirectX::XMConvertToRadians((15.0f) * (static_cast<float>(i) + 1));
		rotation = DirectX::XMMatrixRotationRollPitchYaw(0, 0, roll);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x2 = DirectX::XMVectorGetX(right);
		auto y2 = DirectX::XMVectorGetY(right);
		auto z2 = DirectX::XMVectorGetZ(right);
		
		DrawLine({x1,y1,z1},{x2,y2,z2});
	}

	for (auto i = 0; i < 24; ++i)
	{
		auto right = DirectX::XMVectorSet(0, 0, 1, 0);
		auto pitch = DirectX::XMConvertToRadians((15.0f) * static_cast<float>(i));
		auto rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, 0, 0);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x1 = DirectX::XMVectorGetX(right);
		auto y1 = DirectX::XMVectorGetY(right);
		auto z1 = DirectX::XMVectorGetZ(right);
		
		right = DirectX::XMVectorSet(0, 0, 1, 0);
		pitch = DirectX::XMConvertToRadians((15.0f) * (static_cast<float>(i) + 1));
		rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, 0, 0);
		right = DirectX::XMVector3TransformNormal(right, rotation);
		auto x2 = DirectX::XMVectorGetX(right);
		auto y2 = DirectX::XMVectorGetY(right);
		auto z2 = DirectX::XMVectorGetZ(right);
		
		DrawLine({x1,y1,z1},{x2,y2,z2});
	}
	
	return;
}


void Renderer::CameraPosition(const Vec2& lt, float get_delta_time)
{
	const auto rotation = DirectX::XMMatrixRotationRollPitchYaw(camera_rot.x, camera_rot.y, camera_rot.z);

	auto cpos = DirectX::XMVectorSet(lt.x, 0, lt.y, 0);
	cpos = DirectX::XMVector3TransformNormal(cpos, rotation);
	
	camera_pos.x += DirectX::XMVectorGetX(cpos) * get_delta_time * 4;
	camera_pos.y += DirectX::XMVectorGetY(cpos) * get_delta_time * 4;
	camera_pos.z += DirectX::XMVectorGetZ(cpos) * get_delta_time * 4;
}

void Renderer::CalculateMatrix(Transform t)
{
	const auto scale = DirectX::XMMatrixScaling(t.scale.x, t.scale.y, t.scale.z);
	const auto rotation = DirectX::XMMatrixRotationRollPitchYaw(t.rotate.x, t.rotate.y, t.rotate.z);
	const auto translation = DirectX::XMMatrixTranslation(t.translate.x, t.translate.y, t.translate.z);
	
	m_WorldMatrix = scale;
	m_WorldMatrix = DirectX::XMMatrixMultiply(m_WorldMatrix, rotation);
	m_WorldMatrix = DirectX::XMMatrixMultiply(m_WorldMatrix, translation);
	m_WorldMatrix = DirectX::XMMatrixTranspose(m_WorldMatrix);
}
