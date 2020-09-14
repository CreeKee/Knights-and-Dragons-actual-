#pragma once
#include "Bindable.h"


class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices);
	void bind(Graphics& gfx) override { INFOMAN(gfx); GFX_THROW_INFO_ONLY(getContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u)); }
	UINT getCount() const noexcept { return count; }
		
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};