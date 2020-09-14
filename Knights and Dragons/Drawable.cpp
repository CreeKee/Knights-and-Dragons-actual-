#include "Drawable.h"
#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"
#include <cassert>
#include <typeinfo>

void Drawable::draw(Graphics& gfx) const {
	for (auto& b : binds) {
		b->bind(gfx);
	}
	for (auto& b : getStaticBinds()) {
		b->bind(gfx);
	}
	gfx.drawIndexed(pIndexBuffer->getCount());

}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noexcept {
	assert("*Must* use addIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::addIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept {
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}