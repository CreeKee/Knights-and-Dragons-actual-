#include "VertexBuffer.h"
#include"GraphicsThrowMacros.h"

void VertexBuffer::bind(Graphics& gfx) {
	const UINT offset = 0u;
	INFOMAN(gfx);
	GFX_THROW_INFO_ONLY(getContext(gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset));
}