#pragma once
#define UNICODE
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <windows.h>
#include "Keyboard.h"

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

class mainWindow {
public:
	mainWindow(HINSTANCE hInst, wchar_t* pArgs);
	~mainWindow();
	void initialize();
    HWND getHandle();
	bool runMessageLoop();
private:
//    void OnResize(UINT width, UINT height);
    //  Setup creaets a pointer to itself, stores it in userdata and makes thunk the default MSGhandler
    static LRESULT CALLBACK _HandleMsgSetup(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    //  Thunk is a static function (can't use kbd, mouse class). It forwards to a non-static handleMSG func
    static LRESULT CALLBACK _HandleMsgThunk(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT handleMSG(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
    Keyboard kbd;
private:
    HWND hwnd = NULL;
    const wchar_t mainWindowClassName[18] = L"Main Window Class";
    const wchar_t mainWindowName[18] = L"Чародей Василий";
    HINSTANCE hInstance = NULL;
    wchar_t* Args;
};