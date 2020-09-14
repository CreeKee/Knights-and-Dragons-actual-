#pragma once
#include"Drawable.h"
#include"IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:
	static bool isStaticInitialized() { return !staticBinds.empty(); }
	static void addStaticBind(std::unique_ptr<Bindable> bind) {/* assert("*Must* use addIndexBuffer to bind index buffer" && typeid(*bind));*/ staticBinds.push_back(std::move(bind)); }
	void addStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) { assert(pIndexBuffer == nullptr); pIndexBuffer = ibuf.get(); staticBinds.push_back(std::move(ibuf)); }
	void setIndexFromStatic() { 
		for (const auto& b : staticBinds) { 
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get())) { 
				pIndexBuffer = p;
				return; 
			}
		} 
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const noexcept { return staticBinds; }
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;