#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include<iostream>



VertexShader::VertexShader(Graphics& gfx, const std::wstring& path)
	:
	path(path)
{
	INFOMAN(gfx);


	GFX_THROW_INFO_ONLY(D3DReadFileToBlob(path.c_str(), &pBytecodeBlob));
	GFX_THROW_INFO_ONLY(getDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));
}
