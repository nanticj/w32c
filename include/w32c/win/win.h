#ifndef W32C_WIN_WIN_H
#define W32C_WIN_WIN_H

#include <w32c/win/err.h>

LRESULT CALLBACK defWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HRESULT WINAPI winInitialize(WNDPROC lpfnWndProc, HWND* oHWND);
BOOL    WINAPI dispatchGetMessage();
BOOL    WINAPI dispatchPeekMessage();

#endif
