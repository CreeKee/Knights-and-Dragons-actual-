#pragma once
#include "Bindable.h"
#include<d3dcompiler.h>


class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, const std::wstring& path);
	void bind(Graphics& gfx) noexcept override { getContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u); }
protected:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};