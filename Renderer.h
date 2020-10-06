#pragma once
#include <d3d11.h>

struct MatrixBufferType;

class Renderer
{
public:
	void Initialize(HWND WindowHandle, int SCREEN_WIDTH, int SCREEN_HEIGHT);
	void Render();
	ID3D11Device* d3ddev;
	ID3D11DeviceContext* d3dctx;
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
};

