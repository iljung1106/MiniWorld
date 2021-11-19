#include "stdafx.h"
#include "Socket.h"
#include "MainScene.h"

Socket::Socket(MainScene* mainScene) {
	this->mainScene = mainScene;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		cout << "WSA error";
		WSACleanup();
		return;
	}

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET) {
		cout << "socket error";
		closesocket(server);
		WSACleanup();
		return;
	}

	SOCKADDR_IN addr = {0,};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4444); // 포트
	addr.sin_addr.s_addr = inet_addr("211.204.30.40"); // 주소

	while (connect(server, (SOCKADDR*)&addr, sizeof(addr)));
	isConnected = true;
	thread(&Socket::proc_recv, this).detach();
	//tr = thread([&]() {proc_recv(); });
	//thread proc1(&Socket::proc_recv, this);
	//cout << "created Thread" << endl;
}

Socket::~Socket() {
	closesocket(server);
	WSACleanup();
	cout << "Socket closed" << endl;
}

void Socket::Update() {

	//cout << ((isConnected) ? "connected\n" : "notConnected\n");
	if (isConnected) {
		
	}
}


/* 서버한테서 메세지를 받는 스레드 */
void Socket::proc_recv() {
	static char buffer[PACKET_SIZE] = {};
	
	while (true) {
		if (isConnected) {
			ZeroMemory(&buffer, PACKET_SIZE);
			recv(server, buffer, PACKET_SIZE, 0);

			//cout << buffer << endl;

			switch (buffer[0]) {

				// message (m(1) + msg(1023))
				case 'm': {
					char recievedMsg[PACKET_SIZE-1];
					memcpy(recievedMsg, &buffer[1], PACKET_SIZE-1);

					mainScene->OnRecieveMessage(recievedMsg);
					break;
				}
				
				// user movement (u(1) + client num(2) + pos.x(5) + pos.y(5))
				case 'u': {
					char num[2];
					memcpy(&num, &buffer[1], 2);
					char x[5];
					memcpy(&x, &buffer[1+2], 5);
					char y[5];
					memcpy(&y, &buffer[1+2+5], 5);

					mainScene->OnUserMove(atoi(num), atoi(x), atoi(y));
					break;
				}
				
				// join (j(1) + client num(2))
				case 'j': {
					char num[2];
					memcpy(&num, &buffer[1], 2);

					if (Socket::num == -1)
						Socket::num = atoi(num);
					else
						mainScene->OnUserJoin(atoi(num));
					break;
				}
				
				// leave (l(1) + client num(2))
				case 'l': {
					char num[2];
					memcpy(&num, &buffer[1], 2);

					mainScene->OnUserLeave(atoi(num));
					break;
				}
			}

		}
	}
}

void Socket::SetMsg(const char* t) {
	msg = t;
}

/* 서버로 메세지 보내기 */
void Socket::SendMessage() {
	send(server, msg, strlen(msg), 0);
}