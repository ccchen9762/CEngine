#pragma once

#include "GFXResource.h"

#include <vector>

class IndexBuffer : GFXResource {
public:
	IndexBuffer(Graphics& graphics, const std::vector<unsigned short>& indices);

	void Bind(Graphics& graphics) override;
	UINT GetCount() const { return m_count; }

protected:
	ID3D11Buffer* pIndexBuffer;

	UINT m_count;
};