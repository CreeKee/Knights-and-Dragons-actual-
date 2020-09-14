#include "Box.h"
#include "GraphicsThrowMacros.h"
#include "ConstantBuffers.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Topology.h"
#include "InputLayout.h"
#include "TransformCbuf.h"
#include "IndexBuffer.h"
#include "Sphere.h"

Box::Box(Graphics& gfx, std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist) 
	:
	
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng))
	

{
	if (!isStaticInitialized()) {
		struct Vertex {

				DirectX::XMFLOAT3 pos;
		};

		auto model = Sphere::make<Vertex>();
		model.transform(DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f));

		addStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->getBytecode();
		addStaticBind(std::move(pvs));

		addStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

		const std::vector<unsigned short> indicies = {
			0,2,1, 2,3,1,
			1,3,5, 3,7,5,
			2,6,3, 3,6,7,
			4,5,7, 4,7,6,
			0,4,2, 2,4,6,
			0,1,4, 1,5,4
		};
		addStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct ConstantBuffer2 {
			struct {
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};

		const ConstantBuffer2 cb2 = {
			{
				{1.0f, 0.0f, 0.0f},
				{0.0f, 1.0f, 0.0f},
				{0.0f, 0.0f, 1.0f},
				{1.0f, 1.0f, 0.0f},
				{0.0f, 1.0f, 1.0f}
			}
		};
		addStaticBind(std::make_unique <PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		addStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		addStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else {
		setIndexFromStatic();
	}
	addBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void Box::update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}