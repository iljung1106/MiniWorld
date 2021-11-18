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
	addr.sin_port = htons(4444); // ��Ʈ
	addr.sin_addr.s_addr = inet_addr("211.204.30.40"); // �ּ�

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


/* �������׼� �޼����� �޴� ������ */
void Socket::proc_recv() {
	static char buffer[PACKET_SIZE] = {};
	
	while (true) {
		if (isConnected) {
			ZeroMemory(&buffer, PACKET_SIZE);
			recv(server, buffer, PACKET_SIZE, 0);

			switch (buffer[0]) {

				// message (m(1) + msg(1023))
				case 'm': {
					char recievedMsg[PACKET_SIZE - 1];
					memcpy(recievedMsg, &buffer[1], PACKET_SIZE - 1);
					//cout << recievedMsg << endl;
					mainScene->OnRecieveMessage(recievedMsg);
					break;
				}
				
				// user movement (u(1) + client num(int) + direction(int))
				case 'u': {
					int num;
					memcpy(&num, &buffer[1], sizeof(int));
					int dir;
					memcpy(&dir, &buffer[1 + sizeof(int)], sizeof(int));
					mainScene->OnUserMove(num, dir);
					break;
				}
				
				// join (j(1) + client num(int))
				case 'j': {
					int num;
					memcpy(&num, &buffer[1], sizeof(int));
					mainScene->OnUserJoin(num);
					break;
				}
				
				// leave (l(1) + client num(int))
				case 'l': {
					int num;
					memcpy(&num, &buffer[1], sizeof(int));
					mainScene->OnUserLeave(num);
					break;
				}
			}

		}
	}
}

void Socket::SetMsg(const char* t) {
	msg = t;
}

/* ������ �޼��� ������ */
void Socket::SendMessage() {
	send(server, msg, strlen(msg), 0);
}