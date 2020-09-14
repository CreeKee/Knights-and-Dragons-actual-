#include "Window.h"
#include <sstream>
#include <exception>
#include <iostream>

//WindowClass functions (subclass)
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept 
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON, 48, 48, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	RegisterClassEx(&wc);
}

Window::Window(int width, int height, const char* name) 
	:
	width(width),
	height(height)
{

	//calculate window size based on client region
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw CHWND_LAST_EXCEPT();
	}


	//create window
	hWnd = CreateWindow(
		WindowClass::getName(),
		name,
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right-wr.left, wr.bottom-wr.top,
		nullptr, nullptr, WindowClass::getInstance(),  this
		);
	//check for errors
	if (hWnd == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}

	//display window te currently hidden window
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	//create graphics object
	pGfx = std::make_unique<Graphics>(hWnd);
}

//WindException funtions (subclass)
Window::WindException::WindException(int line, const char* file, HRESULT hr) noexcept 
	:
	Exceptions(line, file),
	hr(hr)
{}

const char* Window::WindException::what() const noexcept {
	std::ostringstream output;
	output << getType() << std::endl
		<< "ERROR CODE: " << getErrorCode() << std::endl
		<< "DESCRIPTION: " << getErrorString() << std::endl
		<< getOriginString() << std::endl;
	whatBuffer = output.str();
	return whatBuffer.c_str();
}

std::string Window::WindException::translateErrorCode(HRESULT hr) noexcept {
	char* messageBuffer = nullptr;
	std::string errorResult = "Unidentified error code";

	DWORD messageLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

	if (messageLength != 0) {
		errorResult = messageBuffer;
	}
	LocalFree(messageBuffer);
	return errorResult;
		
}

//Window functions (main class)
LRESULT CALLBACK Window::handleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	
	//use creation parameter passed from CreateIndwow()
	if (msg == WM_NCCREATE) {

		//find and copy a pointer to the created window class
		CONST CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//set CALLBACK-managed user data to store in a window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		//set message proc to normal handler
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMsgLogic));
		
		return pWnd->handleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::handleMsgLogic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	
	//retrieve window class pointer
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//forward to msg handler
	return pWnd->handleMsg(hWnd, msg, wParam, lParam);

}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {

	POINTS pts;	
	
	switch (msg) {
		
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KILLFOCUS:
		keys.ClearState();
		break;

	//keyboard messages---------------------------
	case WM_SYSKEYUP:
	case WM_KEYUP:
		keys.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || keys.AutorepeatIsEnabled()) {
			keys.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_CHAR:
		keys.OnChar(static_cast<unsigned char>(wParam));
		break;
	
	//mouse messages-----------------------------
	case WM_MOUSEMOVE:
		pts = MAKEPOINTS(lParam);

		//check to see if the mouse is in the client window or not
		if (pts.x >= 0 && pts.x < width && pts.y >= 0 && pts.y < height) {
			mouse.OnMouseMove(pts.x, pts.y);
			if (!mouse.IsInWindow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}

		//if not then maintain the caputre of the mouse if the button is down
		else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pts.x, pts.y);
			}
			//once done holding button down release capture
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;

	case WM_LBUTTONDOWN:
		pts = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pts.x, pts.y);
		break;

	case WM_RBUTTONDOWN:
		pts = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pts.x, pts.y);
		break;

	case WM_LBUTTONUP:
		pts = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pts.x, pts.y);
		break;

	case WM_RBUTTONUP:
		pts = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pts.x, pts.y);
		break;
		
	case WM_MOUSEWHEEL:
		pts = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pts.x, pts.y, delta);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


std::optional<int> Window::processMessages() {

	MSG msg;
	while ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) != 0) {
		if (msg.message == WM_QUIT) {
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return{};
}