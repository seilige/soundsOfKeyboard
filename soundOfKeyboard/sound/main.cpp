#pragma comment(lib,"winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <string>

NOTIFYICONDATA Icon = { 0 };
HWND window;
bool keyDownValue = true;
HHOOK keyboardHook;

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdsh)
{
	MSG SoftwareMainMessage = { 0 };

	WNDCLASSEX main = { 0 };
	main.cbSize = sizeof(WNDCLASSEX);
	main.hInstance = hInst;
	main.lpszClassName = TEXT("Main");
	main.lpfnWndProc = SoftwareMainProcedure;
	RegisterClassEx(&main);

	window = CreateWindowEx(0, TEXT("Main"), NULL, 0, 0, 0, 0, 0, NULL, NULL, hInst, NULL);

	Icon.cbSize = sizeof(NOTIFYICONDATA);
	Icon.hWnd = window;
	Icon.uVersion = NOTIFYICON_VERSION;
	Icon.uCallbackMessage = WM_USER;
	Icon.hIcon = (HICON)LoadImage(NULL, TEXT("golds.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	Icon.uFlags = NIF_MESSAGE | NIF_ICON;
	Shell_NotifyIcon(NIM_ADD, &Icon);

	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	while (GetMessage(&SoftwareMainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&SoftwareMainMessage);
		DispatchMessage(&SoftwareMainMessage);
	}
	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (keyDownValue == true && nCode >= 0 && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN))
	{
		keyDownValue = false;
		KBDLLHOOKSTRUCT* pKeyStruct = (KBDLLHOOKSTRUCT*)lParam;

		if (pKeyStruct->vkCode == VK_BACK)
			PlaySound(L"1.wav", 0, SND_ASYNC);
		else
			PlaySound(L"2.wav", 0, SND_ASYNC);
	}
	if (keyDownValue == false && (wParam == WM_KEYUP || wParam == WM_SYSKEYUP))
		keyDownValue = true;

	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) 
	{
	case WM_CREATE:
		break;

	case WM_USER:
		if (lp == WM_RBUTTONDOWN)
			if (MessageBox(NULL, TEXT("Delete process?"), TEXT("Tray"), MB_YESNO) == IDYES)
				DestroyWindow(window);
		break;

	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &Icon);
		PostQuitMessage(0);
		break;
	
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
}