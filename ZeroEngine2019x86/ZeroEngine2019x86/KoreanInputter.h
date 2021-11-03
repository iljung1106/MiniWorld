#pragma once
#include <iostream>



class KoreanInputter {
private:
	char* text;
public:
	void SetText(char* t);
	char* GetText();
};