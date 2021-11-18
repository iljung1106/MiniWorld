#include "stdafx.h"
#include "Socket.h"

Socket::Socket() {
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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 주소

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
			recievedMsg = buffer;
			if (recievedMsg == "/exit") break;
			cout << recievedMsg << endl;
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