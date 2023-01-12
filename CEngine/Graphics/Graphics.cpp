#include "Graphics.h"

#include <d3dcompiler.h>
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND hWnd, int width, int height) :
	m_width(width), m_height(height) {

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext);

	ID3D11Texture2D* pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();
	pContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// Initialize viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &viewport);
}

Graphics::~Graphics() {
	if (pContext) {
		pContext->Release();
	}
	if (pSwapChain) {
		pSwapChain->Release();
	}
	if (pDevice) {
		pDevice->Release();
	}
}

int Graphics::ClearBuffer(float red, float green, float blue) {
	
	const float color[4] = { red, green, blue, 1.0f };
	pContext->ClearRenderTargetView(pRenderTargetView, color);

	static int cnt=0;
	++cnt;
	return cnt;
}

void Graphics::drawTriangle() {
	Vertex vertices[3] = {
		Vertex(Vector3(0.5f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)),
		Vertex(Vector3(0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f)),
		Vertex(Vector3(0.0f, 0.5f), Vector3(1.0f, 1.0f, 1.0f))
	};

	ID3D11Buffer* pVertexBuffer = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertices;
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &pVertexBuffer);
	const unsigned int stride = sizeof(Vector3);
	const unsigned int offset = 0;

	// Initialize input layout
	ID3D11InputLayout* inputLayout = nullptr;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	pDevice->CreateInputLayout(layout, 1, NULL, 0, &inputLayout);

	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);

	ID3DBlob* pVertexShaderBlob;
	ID3D11VertexShader* pVertexShader = nullptr;
	D3DCompileFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", 0, 0, &pVertexShaderBlob, nullptr);
	pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &pVertexShader);
	pContext->VSSetShader(pVertexShader, nullptr, 0u);

	ID3DBlob* pPixelShaderBlob;
	ID3D11PixelShader* pPixelShader = nullptr;
	D3DCompileFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", 0, 0, &pPixelShaderBlob, nullptr);
	pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), NULL, &pPixelShader);
	pContext->PSSetShader(pPixelShader, nullptr, 0u);

	pContext->Draw(3u, 0u);
	pSwapChain->Present(0, 0);

	delete pVertexShader;
	delete pVertexBuffer;
}

void Graphics::GraphicsEnd() {
	pSwapChain->Present(1u, 0u);
}

Vertex::Vertex() : m_position(Vector3()), m_color(Vector3()) {
}

Vertex::Vertex(Vector3 position, Vector3 color) : m_position(position), m_color(color) {
}
