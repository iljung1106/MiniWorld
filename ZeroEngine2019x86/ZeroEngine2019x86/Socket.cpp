#include "stdafx.h"
#include "stdafx.h"
#include "Socket.h"

Socket::Socket() {
	WSAStartup(MAKEWORD(2, 2), &wsa);

	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4444);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

}


void Socket::proc_recv() {
	static char buffer[PACKET_SIZE] = {};
	static string cmd;
	while (true) {
		if (isConnected) {
			ZeroMemory(&buffer, PACKET_SIZE);
			recv(skt, buffer, PACKET_SIZE, 0);
			cmd = buffer;
			if (cmd == "/exit") break;
			//strcat(recievedMsg, buffer);
			//cout << recievedMsg;
			cout << buffer;
		}
	}
	tr.join();
}

void Socket::Update() {

	//cout << ((isConnected) ? "connected\n" : "notConnected\n");
	if (isConnected) {
	}
	else {
		if (connect(skt, (SOCKADDR*)&addr, sizeof(addr))) {
			isConnected = true;
			tr = thread([&]() {proc_recv(); });
			//thread proc1(&Socket::proc_recv, this);
			cout << "created Thread";
		}
	}

}

void Socket::SetMsg(const char* t) {
	msg = t;
}

void Socket::SendMessage() {
	send(skt, msg, strlen(msg), 0);
}
