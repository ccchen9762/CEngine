#include "Graphics.h"

#include <d3dcompiler.h>

#include <CEngine/Imgui/imgui_impl_dx11.h>

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
	if (pBackBuffer) {
		pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget);
		pBackBuffer->Release();
	}

	// Initialize viewport
	viewport.Width = kRenderWidth;
	viewport.Height = kRenderHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &viewport);

	ImGui_ImplDX11_Init(pDevice, pContext);

	ID3DBlob* pVertexShaderBlob;
	D3DReadFileToBlob(L"VertexShader.cso", &pVertexShaderBlob);
	pDevice->CreateVertexShader(pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), nullptr, &pVertexShader);

	ID3DBlob* pPixelShaderBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pPixelShaderBlob);
	pDevice->CreatePixelShader(pPixelShaderBlob->GetBufferPointer(), pPixelShaderBlob->GetBufferSize(), nullptr, &pPixelShader);

	D3D11_INPUT_ELEMENT_DESC s_inputElementDesc[2] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Initialize input layout
	pDevice->CreateInputLayout(s_inputElementDesc, 2,
		pVertexShaderBlob->GetBufferPointer(), pVertexShaderBlob->GetBufferSize(), &pInputLayout);

	pVertexShaderBlob->Release();
	pPixelShaderBlob->Release();
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
	if (pInputLayout) {
		pInputLayout->Release();
	}
	if (pVertexShader) {
		pVertexShader->Release();
	}
	if (pPixelShader) {
		pPixelShader->Release();
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) {
	const float color[4] = { red, green, blue, 1.0f };

	pContext->ClearRenderTargetView(pRenderTarget, color);
	pContext->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	pContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencil);
	pContext->RSSetViewports(1, &viewport);
}

void Graphics::DrawTriangle() {
	Vertex vertices[3] = {
		{ DirectX::XMFLOAT4(0.0f,   0.5f,  -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(0.5f,  -0.5f,  -3.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f, -0.5f,  0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertices;

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);
	
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &pVertexBuffer);

	pContext->OMSetRenderTargets(1u, &pRenderTarget, nullptr);

	pContext->IASetInputLayout(pInputLayout);

	const unsigned int stride = sizeof(Vertex);
	const unsigned int offset = 0;

	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->VSSetShader(pVertexShader, nullptr, 0u);
	pContext->PSSetShader(pPixelShader, nullptr, 0u);

	pContext->Draw(3u, 0u);

	pVertexBuffer->Release();
}

void Graphics::GraphicsEnd() {
	pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}
