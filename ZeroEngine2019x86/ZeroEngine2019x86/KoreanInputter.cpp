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
	char str2[10000] = {};
	int count = 0;
	while (true) {
		ch = getch();
		cout << ch;
		str2[count++] = ch;
		if (ch == 13) {
			socket->SetMsg(str2);
			socket->SendMessageW();
			count = 0;
			for (int i = 0; i < 10000; i++) {
				str2[i] = '\0';
			}
		}
	}
}