#include "PixelShader.h"
#include "GraphicsThrowMacros.h"



PixelShader::PixelShader(Graphics& gfx, const std::wstring& path)
	:
	path( path )
{
	INFOMAN(gfx);
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	
	GFX_THROW_INFO_ONLY(D3DReadFileToBlob(path.c_str(), &pBlob));
	GFX_THROW_INFO_ONLY(getDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
}
