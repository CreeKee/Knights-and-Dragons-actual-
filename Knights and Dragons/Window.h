#pragma once
#include "Definitions.h"
#include "Exceptions.h"
#include "resource.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <memory>
#include <optional>

//error exception helper macros
#define CHWND_EXCEPT(hr) Window::WindException(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT(hr) Window::WindException(__LINE__, __FILE__, GetLastError())

class Window
{
private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;

	class WindowClass
	{
	public:
		static const char* getName() noexcept { return wndClassName; }
		static HINSTANCE getInstance() noexcept { return wndClass.hInst; }
	private:
		WindowClass() noexcept;
		~WindowClass() { UnregisterClass(wndClassName, getInstance()); }
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "directx engine window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

	static LRESULT CALLBACK handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK handleMsgLogic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	
public:
	Window(int width, int height, const char* name);
	~Window() { DestroyWindow(hWnd); };
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	static std::optional<int> processMessages();
	void setTitle(const std::string title) { if (SetWindowText(hWnd, title.c_str()) == 0) { throw CHWND_LAST_EXCEPT(); } }
	Graphics& getGfx() { return *pGfx; }

	class WindException : public Exceptions
	{
	private:
		HRESULT hr;
	public:
		WindException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		static std::string translateErrorCode(HRESULT hr) noexcept;

		//getters
		virtual const char* getType() const noexcept override { return "Window Class Exception"; }
		HRESULT getErrorCode() const noexcept { return hr; }
		std::string getErrorString() const noexcept { return translateErrorCode(hr); }
	};
	class NoGfxExcept:public Exceptions
	{
	public:
		using Exceptions::Exceptions;
		const char* getType() const noexcept override;
	};

	Keyboard keys;
	Mouse mouse;


};




 