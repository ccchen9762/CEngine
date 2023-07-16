#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(Graphics& graphics, const std::vector<Vertex>& vertices) :
	m_stride(sizeof(Vertex)) {
	D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
	vertexSubresourceData.pSysMem = vertices.data();

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	graphics.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &pVertexBuffer);
}

void VertexBuffer::Bind(Graphics& graphics) {
	const unsigned int offset = 0;

	graphics.getDeviceContext()->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &m_stride, &offset);
}
