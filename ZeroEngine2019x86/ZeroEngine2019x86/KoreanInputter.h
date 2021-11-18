#pragma once
#include <iostream>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class KoreanInputter {
private:
	char* text = "";
public:
	char* GetText();
	void Update(float eTime);
	KoreanInputter();
};