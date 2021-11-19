#include "server.h"

int main() {
	openSocket();
	thread(acceptingClients).detach();

	while (1);

	closeSocket();
}


/* 소켓 열기 */
void openSocket() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		cout << "WSA error";
		return;
	}

	server = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == INVALID_SOCKET) {
		cout << "socket error";
		closesocket(server);
		WSACleanup();
		return;
	}

	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4444); // 포트
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 주소

	if (bind(server, (SOCKADDR*)&addr, sizeof(addr))) {
		cout << "bind error";
		closesocket(server);
		WSACleanup();
		return;
	}
	if (listen(server, SOMAXCONN)) {
		cout << "listen error";
		closesocket(server);
		WSACleanup();
		return;
	}

	cout << "> Socket opened" << endl;
}

/* 클라이언트 접속을 받는 스레드 */
void acceptingClients() {
	for (int num = 0;; num++) {

		// 입장
		c.push_back(ClientSocket());
		c[num].client = accept(server, (SOCKADDR*)&c[num].client_info, &c[num].client_size);
		if (c[num].client == INVALID_SOCKET) {
			cout << "accept error";
			closesocket(c[num].client);
			closesocket(server);
			WSACleanup();
			return;
		}
		c[num].number = num;
		
		cout << "> Client #" << num << " joined" << endl;
		char join_msg[PACKET_SIZE];
		sprintf_s(join_msg, "j%d", num); // 입장(j) 프로토콜
		char send_msg[PACKET_SIZE];
		sprintf_s(send_msg, "m[#] Client #%d joined", num); // 메세지(m) 프로토콜

		for (ClientSocket client : c) {
			send(client.client, join_msg, (int)strlen(join_msg), 0);
			send(client.client, send_msg, (int)strlen(send_msg), 0);
		}

		thread(recvData, c[num].client, num).detach();
	}
}

/* 클라이언트에서 보내는 메세지를 받는 스레드 */
void recvData(SOCKET s, int num) {
	char buffer[PACKET_SIZE];
	char send_msg[PACKET_SIZE];

	while (1) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(s, buffer, PACKET_SIZE, 0);

		// 퇴장
		if (WSAGetLastError()) {

			cout << "> Client #" << num << " left" << endl;
			char leave_msg[PACKET_SIZE];
			sprintf_s(leave_msg, "l%d", num); // 퇴장(l) 프로토콜
			char send_msg[PACKET_SIZE];
			sprintf_s(send_msg, "m[#] Client #%d left", num); // 메세지(m) 프로토콜

			for (ClientSocket client : c) {
				send(client.client, leave_msg, (int)strlen(leave_msg), 0);
				send(client.client, send_msg, (int)strlen(send_msg), 0);
			}
			return;
		}


		switch (buffer[0]) {

			// message (m(1) + msg(1023))
			case 'm':
				// 메세지
				char msg[PACKET_SIZE];
				memcpy(msg, &buffer[1], sizeof(buffer)-1);

				cout << "> [Client #" << num << "] " << msg << endl;
				sprintf_s(send_msg, "m<Client #%d> %s", num, msg); // 메세지(m) 프로토콜
				for (ClientSocket client : c)
					send(client.client, send_msg, (int)strlen(send_msg), 0);
				break;
			
			// user movement (u(1) + client num(2) + pos.x(5) + pos.y(5))
			case 'u':
				// 유저 이동
				char num[2];
				memcpy(&num, &buffer[1], 2);
				char x[5];
				memcpy(&x, &buffer[1+2], 5);
				char y[5];
				memcpy(&y, &buffer[1+2+5], 5);

				for (ClientSocket client : c)
					send(client.client, buffer, (int)strlen(buffer), 0);
				break;
		}
	}
}

/* 소켓 닫기 */
void closeSocket() {
	for (ClientSocket client : c)
		closesocket(client.client);
	closesocket(server);
	WSACleanup();
	cout << "> Socket closed" << endl;
}