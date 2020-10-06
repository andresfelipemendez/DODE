#pragma once
#include <d3d11.h>

struct MatrixBufferType;

class Renderer
{
public:
	static void Initialize(HWND WindowHandle, int SCREEN_WIDTH, int SCREEN_HEIGHT);

	static ID3D11Device* d3ddev;
	static ID3D11DeviceContext* d3dctx;
	static IDXGISwapChain* sc;
	 
	static ID3D11Texture2D* d3dbb;
	static ID3D11RenderTargetView* view;
	 
	static ID3D11InputLayout* m_layout;
	static ID3D11VertexShader* pVS;
	static ID3D11PixelShader* pPS;
	 
	static ID3D11ShaderResourceView* m_texture;
	static ID3D11SamplerState* m_sampleState;
	 
	static ID3D11Buffer* g_pIndexBuffer;
	static ID3D11Buffer* g_pVertexBuffe;
	 
	static ID3D11Buffer* m_matrixBuffer;
	 
	static D3D11_MAPPED_SUBRESOURCE mappedResource;
	static MatrixBufferType* dataPtr;
};

