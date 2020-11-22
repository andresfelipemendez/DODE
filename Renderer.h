#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Types.h"

struct MatrixBufferType;

class Renderer
{
public:
	static void CheckError(HRESULT hr, ID3D10Blob* msg);
	void Initialize(HWND WindowHandle, int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void Clear();
	void Present() const;

	void viewport() const;
	
	ID3D11Buffer* CreateVertexBuffer(Vertex* vertices, size_t size) const;
	ID3D11Buffer* CreateIndexBuffer(const unsigned int* indices, unsigned int size) const;
	ID3D11ShaderResourceView* CreateTextureBuffer(const unsigned char *data,int x,int y,int n) const;
	void SetBuffers(Transform t, unsigned int num_indices, ID3D11Buffer* index_buffer, ID3D11Buffer* vertex_buffer, void* texture);
	
	void CameraRotation(const Vec2& dir, double deltaTime);
	void CameraPosition(const Vec2& lt, double get_delta_time);

	ID3D11Device* d3ddev;
	ID3D11DeviceContext* d3dctx;

	Vec3 camera_pos{0,0,-8};
	Vec3 camera_rot{0,0,0};
	Vec3 look_at{0,0,0};
	
private:
	
	
	IDXGISwapChain* m_SwapChain = nullptr;

	ID3D11Texture2D* d3dbb = nullptr;
	ID3D11RenderTargetView* view = nullptr;

	ID3D11InputLayout* m_layout = nullptr;
	ID3D11VertexShader* pVS = nullptr;
	ID3D11PixelShader* pPS = nullptr;

	ID3D11SamplerState* m_sampleState= nullptr;

	ID3D11Buffer* m_matrixBuffer= nullptr;

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	MatrixBufferType* dataPtr= nullptr;

	DirectX::XMMATRIX worldMatrix = {}, viewMatrix = {}, projectionMatrix = {};

	unsigned int bufferNumber = 0;

	void CalculateMatrix(Transform t);
};

