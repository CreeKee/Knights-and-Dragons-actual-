#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		:
		type(type)
	{}
	void bind(Graphics& gfx) noexcept override {getContext(gfx)->IASetPrimitiveTopology(type); }

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};
