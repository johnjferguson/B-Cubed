#pragma once
#include <Windows.h>
#include <optional>
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Controller.h"

class Window
{
private:
	// singleton class i.e. can only create one
	class WindowClass
	{
	public:
		const char* GetName() const;
		HINSTANCE GetInstance() const;
	public:
		static WindowClass wc;
	private:
		WindowClass(const char* name);
		~WindowClass();
		// don't want copy operator or constructor for singleton
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		const char* name;
		HINSTANCE hInstance;
	};
public:
	Window(int width, int height, const char* name);
	~Window();
	HWND GetHandle() const;
	std::optional<int> ProcessMessages();
	int GetWidth() const;
	int GetHeight() const;
private:
	HWND hWnd;
	int width;
	int height;
public:
	Graphics gfx;
	Mouse mouse;
	Keyboard kbd;
	Controller clr;
private:
	static LRESULT CALLBACK ProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK ProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
