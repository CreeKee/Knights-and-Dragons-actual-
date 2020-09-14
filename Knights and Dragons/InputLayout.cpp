#include "InputLayout.h"
#include "GraphicsThrowMacros.h"


InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* pVertexShaderByteCode){
	INFOMAN( gfx );

	GFX_THROW_INFO_ONLY(getDevice(gfx)->CreateInputLayout(
		layout.data(), (UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&pInputLayout
	));
}
