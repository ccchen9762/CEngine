#pragma once

#include "GFXResource.h"

#include <vector>

class VertexBuffer : GFXResource {
public:
	VertexBuffer(Graphics& graphics, const std::vector<Vertex>& vertices);

	void Bind(Graphics& graphics) override;

protected:
	ID3D11Buffer* pVertexBuffer;
	UINT m_stride;
};