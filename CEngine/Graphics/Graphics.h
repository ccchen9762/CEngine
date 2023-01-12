#pragma once

#include "CEngine/Window/InitWindows.h"
#include <D3D11.h>

#include "CEngine/Math/Vector3.h"

struct Vertex {
	Vertex();
	Vertex(Vector3 position, Vector3 color);
	~Vertex() = default;

	Vector3 m_position;
	Vector3 m_color;
};

class Graphics {
public:
	Graphics(HWND hWnd, int width, int height);
	~Graphics();
	// not copyable
	Graphics(const Graphics&) = delete;
	Graphics& operator= (const Graphics&) = delete;

	int ClearBuffer(float red, float green, float blue);

	void drawTriangle();

	void GraphicsEnd();

private:
	int m_width;
	int m_height;

	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pRenderTargetView = nullptr;
};