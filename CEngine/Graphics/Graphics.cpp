#include "Graphics.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>

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

	// create dpeth stencil state
	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	ID3D11DepthStencilState* pDepthState;
	pDevice->CreateDepthStencilState(&depthDesc, &pDepthState);

	// bind depth stencil state
	pContext->OMSetDepthStencilState(pDepthState, 1u);

	ID3D11Texture2D* pDepthTexture;
	D3D11_TEXTURE2D_DESC depthTextureDesc = {};
	depthTextureDesc.Width = kRenderWidth;
	depthTextureDesc.Height = kRenderHeight;
	depthTextureDesc.MipLevels = 1u;
	depthTextureDesc.ArraySize = 1u;
	depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthTextureDesc.SampleDesc.Count = 1u;
	depthTextureDesc.SampleDesc.Quality = 0u;
	depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&depthTextureDesc, nullptr, &pDepthTexture);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
	depthViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(pDepthTexture, &depthViewDesc, &pDepthStencil);

	pContext->OMSetRenderTargets(1u, &pRenderTarget, pDepthStencil);

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
	//pContext->OMSetRenderTargets(1, &pRenderTarget, pDepthStencil); // shouldn't be update every frame
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

void Graphics::DrawIndex(float angle, float zTrans) {
	// vertex buffer
	Vertex vertices[] = {
		{ DirectX::XMFLOAT4(-0.5f, -0.5f,  -0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4( 0.5f, -0.5f,  -0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4( 0.5f, -0.5f,   0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f, -0.5f,   0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f,  0.5f,  -0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4( 0.5f,  0.5f,  -0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ DirectX::XMFLOAT4( 0.5f,  0.5f,   0.5f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ DirectX::XMFLOAT4(-0.5f,  0.5f,   0.5f, 1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
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

	// index buffer
	// its front clockwise omg
	const unsigned short indices[] = {
		1,3,0,
		1,2,3,
		5,0,4,
		5,1,0,
		6,1,5,
		6,2,1,
		7,2,6,
		7,3,2,
		4,3,7,
		4,0,3,
		6,4,7,
		6,5,4,
	};

	D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
	indexSubresourceData.pSysMem = indices;

	ID3D11Buffer* pIndexBuffer = nullptr;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indices);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	pDevice->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &pIndexBuffer);

	// constant buffer (transform matrix)
	const DirectX::XMMATRIX constantBuffer[] = { 
		DirectX::XMMatrixTranspose(
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationX(angle) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 2.0f + zTrans) *
		DirectX::XMMatrixPerspectiveLH(1.0f, kScreenRatio, 0.1f, 10.0f)) };

	D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
	constantSubresourceData.pSysMem = constantBuffer;

	ID3D11Buffer* pConstantBuffer = nullptr;
	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.ByteWidth = sizeof(constantBuffer);
	constantBufferDesc.StructureByteStride = sizeof(DirectX::XMMATRIX);

	pDevice->CreateBuffer(&constantBufferDesc, &constantSubresourceData, &pConstantBuffer);

	pContext->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);

	// render
	pContext->IASetInputLayout(pInputLayout);

	const unsigned int stride = sizeof(Vertex);
	const unsigned int offset = 0;

	pContext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);
	pContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->VSSetShader(pVertexShader, nullptr, 0u);
	pContext->PSSetShader(pPixelShader, nullptr, 0u);

	pContext->DrawIndexed(sizeof(indices) / sizeof(short), 0u, 0u);

	pVertexBuffer->Release();
}

void Graphics::GraphicsEnd() {
	pSwapChain->Present(1u, 0u); // swapchain present need to be call at the end of frame
}
