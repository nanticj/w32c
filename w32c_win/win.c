#include <w32c/win/win.h>

BOOL CALLBACK __ctrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		PostMessageW(HWND_BROADCAST, WM_CLOSE, 0, 0);
		return TRUE;
	default:
		return FALSE;
	}
}

LRESULT CALLBACK defWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(hWnd, message, wParam, lParam);
	}
	return 0;
}

HRESULT WINAPI winInitialize(WNDPROC lpfnWndProc, HWND* oHWND)
{
	if (oHWND == NULL) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_ARGS);
	}
	*oHWND = NULL;

	if (lpfnWndProc == NULL) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_ARGS);
	}

	if (SetConsoleCtrlHandler(__ctrlHandler, TRUE) == FALSE) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL1);
	}

	HMODULE hInstance = GetModuleHandleW(NULL);
	if (hInstance == NULL) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL2);
	}

	LPCWSTR lpszClassName = L"W32C_WIN_CLASS";
	LPCWSTR lpszWindowName = L"W32C_WIN_WINDOW";
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = 0;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = lpszClassName;
	wcex.hIconSm = NULL;

	ATOM ret = RegisterClassExW(&wcex);
	if (ret == 0) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL3);
	}

	HWND hWnd = CreateWindowExW(
		0,
		lpszClassName,
		lpszWindowName,
		0,
		0,
		0,
		0,
		0,
		HWND_MESSAGE,
		NULL,
		hInstance,
		NULL
	);
	if (hWnd == NULL) {
		return ERR_MAKE(ERR_FACILITY_WIN, ERR_CODE_INTERNAL4);
	}

	*oHWND = hWnd;
	return S_OK;
}

BOOL WINAPI dispatchGetMessage()
{
	MSG msg;
	if (GetMessageW(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		return TRUE;
	}
	return FALSE;
}

BOOL WINAPI dispatchPeekMessage()
{
	MSG msg;
	if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return FALSE;
		}
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return TRUE;
}
