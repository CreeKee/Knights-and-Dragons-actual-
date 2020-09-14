#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void bind(Graphics& gfx) = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* getContext(Graphics& gfx) noexcept { return gfx.ptrContext.Get(); }
	static ID3D11Device* getDevice(Graphics& gfx) noexcept { return gfx.ptrDevice.Get(); }
	static DxgiInfoManager& getInfoManager(Graphics& gfx) noexcept { return gfx.infoManager; }
};
