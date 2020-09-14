#pragma once
#include "DrawableBase.h"
#include <random>

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void update(float dt) noexcept;
	DirectX::XMMATRIX getTransformXM() const noexcept override {
		return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
			DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
	}

private:
	//positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;

	//speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};
