#pragma once
#include<d3d11.h>
#include<memory>
#include<sstream>
#include<wrl.h>
#include<directxmath.h>
#include "Definitions.h"
#include "Exceptions.h"
#include "DxgiInfoManager.h"
#include "GraphicsThrowMacros.h"



class Graphics
{
	friend class Bindable;
private:
	DirectX::XMMATRIX projection;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif

	Microsoft::WRL::ComPtr <ID3D11Device> ptrDevice;
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> ptrContext;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> ptrTarget;
	Microsoft::WRL::ComPtr <IDXGISwapChain> ptrSwapChain;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> ptrDSV;

public:
	class Exception : public Exceptions
	{
		using Exceptions::Exceptions;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* getType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class GraphicsException : public Exceptions
	{
	private:
		HRESULT hr;
	public:
		GraphicsException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		static std::string translateErrorCode(HRESULT hr) noexcept;

		//getters
		virtual const char* getType() const noexcept override { return "Graphics Class Exception"; }
		HRESULT getErrorCode() const noexcept { return hr; }
		std::string getErrorString() const noexcept { return translateErrorCode(hr); }
	};

	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	~Graphics() = default;

	Graphics& operator=(const Graphics&) = delete;
	void endFrame();
	void clearBuffer(float red, float green, float blue) noexcept;
	void drawTestTriangle();
	void drawIndexed(UINT count) { GFX_THROW_INFO_ONLY(ptrContext->DrawIndexed(count, 0u, 0u)); }
	DirectX::XMMATRIX getProjection() const noexcept { return projection; }
	void setProjection(DirectX::XMMATRIX proj) noexcept { projection = proj; }
};