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
	static char recievedMsg[PACKET_SIZE] = {};
	
	while (true) {
		if (isConnected) {
			ZeroMemory(&recievedMsg, PACKET_SIZE);
			recv(server, recievedMsg, PACKET_SIZE, 0);
			if (recievedMsg == "/exit") break;
			cout << recievedMsg << endl;

			mainScene->OnRecieveMessage(recievedMsg);
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