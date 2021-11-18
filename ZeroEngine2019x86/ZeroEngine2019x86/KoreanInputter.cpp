#include "stdafx.h"
#include "KoreanInputter.h"


char* KoreanInputter::GetText() {

	return text;
}


KoreanInputter::KoreanInputter(Socket* s) {
	std::thread::thread(&KoreanInputter::GetInput, this).detach();
	socket = s;
}

void KoreanInputter::GetInput() {
	char ch;
	char str2[PACKET_SIZE-1] = {};
	int count = 0;
	while (true) {
		ch = _getch();
		if (ch == '\b' && count > 0) {
			str2[count--] = '\0';
			Gotoxy(count, 0);
			cout << " ";
			Gotoxy(count, 0);
		}
		else {
			cout << ch;
			str2[count++] = ch;
			if (ch == 13) {
				char msg[PACKET_SIZE] = "m";
				memcpy(msg+1, str2, sizeof(str2));
				socket->SetMsg(msg);
				socket->SendMessageW();
				count = 0;
				ZeroMemory(str2, sizeof(str2));
				system("cls");
			}
		}
	}
}

void KoreanInputter::Gotoxy(int x, int y) {
	COORD pos = { x,y }; //x, y 좌표 설정
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //커서 설정
}
