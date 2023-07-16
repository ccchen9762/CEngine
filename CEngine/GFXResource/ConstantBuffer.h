#pragma once

#include "GFXResource.h"

#include <vector>

//template<typename bufferType>
class VertexConstantBuffer : GFXResource {
	public:
		VertexConstantBuffer(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer);

		void Bind(Graphics& graphics) override;
		void Update(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer);

	protected:
		ID3D11Buffer* pConstantBuffer;
};

class PixelConstantBuffer : GFXResource {
public:
	PixelConstantBuffer(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer);

	void Bind(Graphics& graphics) override;
	void Update(Graphics& graphics, const std::vector<DirectX::XMMATRIX>& constBuffer);

protected:
	ID3D11Buffer* pConstantBuffer;
};