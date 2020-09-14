#pragma once
#include "Bindable.h"
#include<d3dcompiler.h>


class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path);
	void bind(Graphics& gfx) noexcept override { getContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u); }
	ID3DBlob* getBytecode() const noexcept { return pBytecodeBlob.Get(); }
protected:
	std::wstring path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};