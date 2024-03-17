#include "ui.h"

bool isRunning = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	LRESULT result = 0;

	switch (uMsg) {

		case WM_CREATE: {
			SetWindows(hwnd);
		} break;

		case WM_CLOSE:

		case WM_DESTROY: {

			isRunning = false;
		} break;

		case WM_COMMAND: {

			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
				TCHAR ListItem[256] = {};
				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
			}

			switch (LOWORD(wParam)) {

				case (int)URUN_TAMAM: 
					Urun::Tamam(urun, lira, kurus, gun, ay, yil, ozet, aylik);
					break;

				case (int)URUN_SIL:
					if (*GetContent(urun) == '\0') break;
					if (IDOK == MessageBox(hwnd, L"Ürünü silmek istediðinizden emin misiniz?", L"Ürün Sil", MB_OKCANCEL | MB_ICONEXCLAMATION))
						Urun::Remove(urun, ozet, aylik);
					break;

				case (int)URUN_EDIT:
					Log::Rename(urun, ozet);
					break;
			}
		}

		default:
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
			break;
	}

	return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = L"MAIN_WINDOW";
	windowClass.lpfnWndProc = WndProc;
	if (!RegisterClass(&windowClass)) {
		return 1;
	}
	HWND window = CreateWindow(windowClass.lpszClassName, L"Muherosebe", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 510, 345, 0, 0, hInstance, 0);

	while (isRunning) {
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		Sleep(10);
	}

	return 0;
}