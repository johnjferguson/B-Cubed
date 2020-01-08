#include "Window.h"

// initialize static variable for singleton
Window::WindowClass  Window::WindowClass::wc("B-Cubed");


const char * Window::WindowClass::GetName() const
{
	return name;
}

HINSTANCE Window::WindowClass::GetInstance() const
{
	return hInstance;
}

Window::WindowClass::WindowClass(const char * name)
	:
	name(name),
	hInstance(GetModuleHandle(NULL))
{
	// register window class
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = ProcSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	// could add logo for window here if we want
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;

	// could add custom cursor
	wc.hCursor = nullptr;

	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

Window::Window(int width, int height, const char * name)
	:
	width(width),
	height(height)
{
	// using the adjustwindowrect function allows the client region to be width * height
	// rather then including title bar and padding around window
	RECT rect;
	rect.left = 100;
	rect.right = rect.left + width;
	rect.top = 100;
	rect.bottom = rect.top + height;

	AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// create window instance
	HWND hWnd = CreateWindowEx(
		0,
		WindowClass::wc.GetName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		100, 100,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		WindowClass::wc.GetInstance(),
		this
	);
	// newly created window start off as hidden
	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

HWND Window::GetHandle() const
{
	return hWnd;
}

std::optional<int> Window::ProcessMessages()
{
	// windows message pump
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			// std::optional will evaluate to true since it is being assigned a value
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	// std::optional will evaluate to false for empty optional
	return {};
}

LRESULT Window::ProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		// getting the pointer to window we passed in CreateWindow function
		CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWnd = static_cast<Window*>(pcs->lpCreateParams);
		// setting some user data on windows api side to the window pointer
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// setting windows procedure to ProcThunk
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::ProcThunk));
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::ProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// since windows procedures have to be static we call the Window class function
	// using information we passed in using ProcSetup
	return pWnd->Proc(hWnd, msg, wParam, lParam); return LRESULT();
}

LRESULT Window::Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	// shuts down program when exit button is pressed
	case WM_CLOSE:
		PostQuitMessage(0); 
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
