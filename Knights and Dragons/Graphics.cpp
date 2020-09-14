#include"Graphics.h"
#include<d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

Graphics::Graphics(HWND hwnd) {
	

	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferDesc.Width = 0u;
	scd.BufferDesc.Height = 0u;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0u;
	scd.BufferDesc.RefreshRate.Denominator = 0u;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1u;
	scd.SampleDesc.Quality = 0u;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1u;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0u;

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&ptrSwapChain,
		&ptrDevice,
		nullptr,
		&ptrContext
	);

	//add error check

	wrl::ComPtr<ID3D11Resource> ptrBackBuffer;
	GFX_THROW_INFO_ONLY(ptrSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &ptrBackBuffer));
	GFX_THROW_INFO_ONLY(ptrDevice->CreateRenderTargetView(
		ptrBackBuffer.Get(),
		nullptr,
		&ptrTarget
	));
	D3D11_VIEWPORT vp = {};
	vp.Width = 1280u;
	vp.Height = 720u;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	ptrContext->RSSetViewports(1u, &vp);


	//depth stenciling
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_INFO_ONLY(ptrDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	ptrContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth= {};
	descDepth.Width = 1280u;
	descDepth.Height = 720u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO_ONLY(ptrDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_INFO_ONLY(ptrDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &ptrDSV));

	ptrContext->OMSetRenderTargets(1u, ptrTarget.GetAddressOf(), ptrDSV.Get());
}

void Graphics::endFrame() {

	HRESULT hr;
	if (FAILED(hr = ptrSwapChain->Present(1u, 0u))) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			//add error check
		}
	}
}

void Graphics::clearBuffer(float red, float green, float blue) noexcept {

	const float color[] = { red, green, blue, 1.0f };
	ptrContext->ClearRenderTargetView(ptrTarget.Get(), color);
	ptrContext->ClearDepthStencilView(ptrDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
	
//graphics error sub-class
Graphics::GraphicsException::GraphicsException(int line, const char* file, HRESULT hr) noexcept
	:
	Exceptions(line, file),
	hr(hr)
{}

const char* Graphics::GraphicsException::what() const noexcept {
	std::ostringstream output;
	output << getType() << std::endl
		<< "ERROR CODE: " << getErrorCode() << std::endl
		<< "DESCRIPTION: " << getErrorString() << std::endl
		<< getOriginString() << std::endl;
	whatBuffer = output.str();
	return whatBuffer.c_str();
}

std::string Graphics::GraphicsException::translateErrorCode(HRESULT hr) noexcept {
	char* messageBuffer = nullptr;
	std::string errorResult = "Unidentified error code";

	DWORD messageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

	if (messageLength != 0) {
		errorResult = messageBuffer;
	}
	LocalFree(messageBuffer);
	return errorResult;

}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file),
	hr(hr)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty())
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::getType() const noexcept
{
	return "Chili Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return "fix me";//DXGetErrorString(hr);
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	//char buf[512];
	//DXGetErrorDescription(hr, buf, sizeof(buf));
	return "fix me too";//buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept
	:
	Exception(line, file)
{
	// join all info messages with newlines into single string
	for (const auto& m : infoMsgs)
	{
		info += m;
		info.push_back('\n');
	}
	// remove final newline if exists
	if (!info.empty())
	{
		info.pop_back();
	}
}


const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::getType() const noexcept
{
	return "Chili Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}


//draw triang
void Graphics::drawTestTriangle() {
	//make a vertex struct that holds coordinets
	struct Vertex {
		float x;
		float y;
		float r;
		float g;
		float b;
	};
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA sd = {};

	

	
	wrl::ComPtr<ID3D11PixelShader> ptrPixelShader;
	
	

	const Vertex verticies[] = { 
		{0.0f, 0.5f, 1.0f, 0.0f, 0.0f}, 
		{0.5f,-0.5f, 0.0f, 1.0f, 0.0f},
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f} };



	//fill the structs
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(verticies);
	bd.StructureByteStride = sizeof(Vertex);

	sd.pSysMem = verticies;


	Microsoft::WRL::ComPtr <ID3D11Buffer> ptrVertexBuffer;

	//create the buffer
	ptrDevice->CreateBuffer(&bd, &sd, &ptrVertexBuffer);

	//set primitive tech
	ptrContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	ptrContext->IASetVertexBuffers(0u, 1u, ptrVertexBuffer.GetAddressOf(), &stride, &offset);

	//bind pixel shader
	wrl::ComPtr<ID3DBlob> ptrBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &ptrBlob);
	ptrDevice->CreatePixelShader(ptrBlob->GetBufferPointer(), ptrBlob->GetBufferSize(), nullptr, &ptrPixelShader);
	ptrContext->PSSetShader(ptrPixelShader.Get(), nullptr, 0u);

	//bind vertex shader
	wrl::ComPtr<ID3D11VertexShader> ptrVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &ptrBlob);
	ptrDevice->CreateVertexShader(ptrBlob->GetBufferPointer(), ptrBlob->GetBufferSize(), nullptr, &ptrVertexShader);
	ptrContext->VSSetShader(ptrVertexShader.Get(), 0, 0);
	ptrContext->VSSetShader(ptrVertexShader.Get(), nullptr, 0u);
	//create input vertex layout
	wrl::ComPtr<ID3D11InputLayout> ptrInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	ptrDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		ptrBlob->GetBufferPointer(),
		ptrBlob->GetBufferSize(),
		&ptrInputLayout
	);

	//bind vertex layout
	ptrContext->IASetInputLayout(ptrInputLayout.Get());

	//bind render target
	ptrContext->OMSetRenderTargets(1u, ptrTarget.GetAddressOf(), nullptr);

	//configure viewport
	D3D11_VIEWPORT vp = {};
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 1;
	vp.TopLeftY = 1;
	ptrContext->RSSetViewports(1u, &vp);




	ptrContext->Draw((UINT)std::size(verticies), 0u);
}

