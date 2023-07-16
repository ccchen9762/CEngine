#pragma once

#include "CEngine/GFXResource/IndexBuffer.h"
#include "CEngine/GFXResource/VertexBuffer.h"

class Drawable {
public:
	Drawable(Graphics& graphics, float scale = 1.0f);
	virtual ~Drawable() = default;

	void BindIndexBuffer(Graphics& gfx) const;
	void BindResources(Graphics& gfx) const;
	void Draw(Graphics& graphics);
	virtual DirectX::XMMATRIX GetTransformXM() = 0;

protected:
	IndexBuffer* pIndices;
 	std::vector<GFXResource> m_resources;
};