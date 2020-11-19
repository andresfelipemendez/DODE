#pragma once
#include <d3d11.h>
#include "Types.h"
#include <DirectXMath.h>

const int GL_ARRAY_BUFFER = 34962;
const int GL_ELEMENT_ARRAY_BUFFER = 34963;

struct MatrixBufferType;

class Renderer
{
public:
	void Initialize(HWND WindowHandle, int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void Clear();
	void Present();

	void CameraRotation(const vec2& dir, double deltaTime);
	void* CreateVertexBuffer(Vertex* vertices, size_t size);
	void* CreateIndexBuffer(unsigned int* indices, unsigned int size);
	void* create_texture_buffer(const unsigned char *data,int x,int y,int n) const;
	void SetBuffers(vec3 pos, unsigned int numIndices, void* indexBuffer, void* vertexBuffer, void* texture);
	void CameraPosition(const vec2& lt, double get_delta_time);

	ID3D11Device* d3ddev;
	ID3D11DeviceContext* d3dctx;

	vec3 camera_pos_ {0,0,-20};
private:
	
	vec3 camera_rot_ {0,0,0};
	vec3 look_at_ {0,0,0};
	
	
	IDXGISwapChain* sc;

	ID3D11Texture2D* d3dbb;
	ID3D11RenderTargetView* view;

	ID3D11InputLayout* m_layout;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;

	ID3D11ShaderResourceView* m_texture;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* g_pIndexBuffer;
	ID3D11Buffer* g_pVertexBuffe;

	ID3D11Buffer* m_matrixBuffer;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	DirectX::XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	unsigned int bufferNumber;

	void CalculateMatrix(vec3 position);
};

