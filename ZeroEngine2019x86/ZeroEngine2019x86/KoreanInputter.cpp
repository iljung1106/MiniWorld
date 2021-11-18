#include "stdafx.h"
#include "KoreanInputter.h"


char* KoreanInputter::GetText() {

	return text;
}

void KoreanInputter::SetText(char* t) {
	text = t;
}

KoreanInputter::KoreanInputter(HINSTANCE hInst) {
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInst;
	//
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	//
	WndClass.lpszClassName = L"chat";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(L"Engine2D", L"프로그램명",
		WS_OVERLAPPEDWINDOW, 100, 0, 300, 200,
		GetDesktopWindow(), NULL, hInst, NULL);
	ShowWindow(hWnd, 10);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[256];
	int len;
	switch (iMessage) {
	case WM_CHAR:
		len = strlen(str);
		str[len] = (TCHAR)wParam;
		str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wchar_t wtext[20];
		mbstowcs(wtext, str, strlen(str) + 1);//Plus null
		LPWSTR ptr = wtext;
		TextOut(hdc, 100, 100, ptr, strlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
