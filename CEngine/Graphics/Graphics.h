#pragma once

#include "CEngine/Window/InitWindows.h"
#include <D3D11.h>
#include <DirectXMath.h>

#include "CEngine/Main/Constants.h"
#include "CEngine/Math/Vector3.h"

struct Vertex {
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 color;
};

class Graphics {
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics();
	// cannot be copied
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	void ClearBuffer(float red, float green, float blue);

	void drawTriangle();

	void GraphicsEnd();

private:
	int m_width;
	int m_height;

	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTarget = nullptr;
	ID3D11DepthStencilView* pDepthStencil = nullptr;
	ID3D11InputLayout* pInputLayout = nullptr;
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11VertexShader* pVertexShader = nullptr;
	ID3D11PixelShader* pPixelShader = nullptr;
	D3D11_VIEWPORT viewport;
};