#include "ConstantBuffer.h"

VertexConstantBuffer::VertexConstantBuffer(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer) {
	D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
	constantSubresourceData.pSysMem = constBuffer.data();

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.ByteWidth = sizeof(constBuffer);
	constantBufferDesc.StructureByteStride = sizeof(DirectX::XMMATRIX);

	graphics.getDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, &pConstantBuffer);
}

void VertexConstantBuffer::Bind(Graphics& graphics) {
	graphics.getDeviceContext()->VSSetConstantBuffers(0u, 1u, &pConstantBuffer);
}

void VertexConstantBuffer::Update(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer) {
	D3D11_MAPPED_SUBRESOURCE mappedSubSource;
	graphics.getDeviceContext()->Map(
		pConstantBuffer, 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&mappedSubSource
	);
	memcpy(mappedSubSource.pData, &constBuffer, sizeof(constBuffer));
	graphics.getDeviceContext()->Unmap(pConstantBuffer, 0u);
}

PixelConstantBuffer::PixelConstantBuffer(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer) {
	D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
	constantSubresourceData.pSysMem = constBuffer.data();

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0u;
	constantBufferDesc.ByteWidth = sizeof(constBuffer);
	constantBufferDesc.StructureByteStride = sizeof(DirectX::XMMATRIX);

	graphics.getDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, &pConstantBuffer);
}

void PixelConstantBuffer::Bind(Graphics& graphics) {
	graphics.getDeviceContext()->PSSetConstantBuffers(0u, 1u, &pConstantBuffer);
}

void PixelConstantBuffer::Update(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer) {
	D3D11_MAPPED_SUBRESOURCE mappedSubSource;
	graphics.getDeviceContext()->Map(
		pConstantBuffer, 0u,
		D3D11_MAP_WRITE_DISCARD, 0u,
		&mappedSubSource
	);
	memcpy(mappedSubSource.pData, &constBuffer, sizeof(constBuffer));
	graphics.getDeviceContext()->Unmap(pConstantBuffer, 0u);
}
