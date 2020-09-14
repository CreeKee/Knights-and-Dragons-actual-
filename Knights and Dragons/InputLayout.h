#pragma once
#include "Bindable.h"


class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode);
	void bind(Graphics& gfx) noexcept override { getContext(gfx)->IASetInputLayout(pInputLayout.Get()); }

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

};
