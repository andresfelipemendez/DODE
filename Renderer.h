#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Types.h"

struct MatrixBufferType;

class Renderer
{
public:
	static void CheckError(HRESULT hr, ID3D10Blob* msg);
	void Initialize(HWND window_handle, float screen_width, float screen_height);
	void Clear();
	void Present() const;

	void Viewport() const;
	
	ID3D11Buffer* CreateVertexBuffer(Vertex* vertices, size_t size) const;
	ID3D11Buffer* CreateIndexBuffer(const unsigned int* indices, unsigned int size) const;
	ID3D11ShaderResourceView* CreateTextureBuffer(const unsigned char *data,int x,int y,int n) const;
	void Draw(Transform t, unsigned int num_indices, ID3D11Buffer* index_buffer, ID3D11Buffer* vertex_buffer, void* texture);
	void DrawLine(Vec3 p1, Vec3 p2);
	void DrawCircle(Vec3 center, float radius);

	void CameraRotation(const Vec2& dir, double delta_time);
	void CameraPosition(const Vec2& lt, float get_delta_time);

	ID3D11Device* d3ddev;
	ID3D11DeviceContext* d3dctx;

	Vec3 camera_pos{0, 0, -8};
	Vec3 camera_rot{0, 0, 0};
	Vec3 look_at{0, 0, 0};
	
private:
	
	
	IDXGISwapChain* m_SwapChain = nullptr;

	ID3D11Texture2D* m_D3dbb = nullptr;
	ID3D11RenderTargetView* m_View = nullptr;

	ID3D11InputLayout* m_Layout = nullptr;
	ID3D11VertexShader* m_PVs = nullptr;
	ID3D11PixelShader* m_PPs = nullptr;

	ID3D11SamplerState* m_SampleState = nullptr;

	ID3D11Buffer* m_MatrixBuffer = nullptr;
	ID3D11Buffer* m_VertexBuffer = nullptr;
	

	D3D11_MAPPED_SUBRESOURCE m_MappedResource = {};
	MatrixBufferType* m_DataPtr = nullptr;

	DirectX::XMMATRIX m_WorldMatrix = {}, m_ViewMatrix = {}, m_ProjectionMatrix = {};

	unsigned int m_BufferNumber = 0;

	void CalculateMatrix(Transform t);
};

