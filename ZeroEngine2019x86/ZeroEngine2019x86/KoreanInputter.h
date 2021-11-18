#pragma once
#include <iostream>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class KoreanInputter {
private:
	char* text = "";
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
public:
	void SetText(char* t);
	char* GetText();
	void Update(float eTime);
	KoreanInputter(HINSTANCE hInst);
};