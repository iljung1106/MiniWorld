#pragma once
#include <iostream>
#include <windows.h>
#include <thread>
#include "Socket.h"
#include <conio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


class KoreanInputter {
private:
	char* text = "";
	Socket* socket; 
	std::string str = "";
public:
	char* GetText();
	void Update(float eTime);
	void GetInput();
	KoreanInputter(Socket* s);
};