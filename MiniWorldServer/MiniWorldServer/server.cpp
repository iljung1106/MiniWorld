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
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 주소

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
		ZeroMemory(join_msg, sizeof(join_msg));
		sprintf_s(join_msg, "j%2d", num); // 입장(j) 프로토콜
		char send_msg[PACKET_SIZE];
		ZeroMemory(send_msg, sizeof(send_msg));
		sprintf_s(send_msg, "m[#] Client #%d joined", num); // 메세지(m) 프로토콜
		
		cout << join_msg << endl;

		for (ClientSocket client : c) {

			if (client.number == num) { // 지금 들어온 클라이언트한테는 초기화 메세지를 보냄
				char init_msg[PACKET_SIZE];
				ZeroMemory(init_msg, sizeof(init_msg));
				int csize = 0;
				for (ClientSocket cl : c) {
					cout << cl.number << endl;
					if (cl.connected) { csize++; cout << "++ " << endl; }
				}
				sprintf_s(init_msg, "i%2d%2d", num, (int)csize-1); // 초기화(i) 프로토콜

				for (int i = 0; i < c.size(); i++) {
					if (c[i].number == num) continue; // 자기 자신 제외
					if (!c[i].connected) continue; // 연결이 끊긴 소켓 제외

					char clientPos[2+5+5+1];
					sprintf_s(clientPos, "%2d%5d%5d", c[i].number, c[i].x, c[i].y);
					memcpy(&init_msg[strlen(init_msg)], clientPos, sizeof(clientPos));
				}

				cout << init_msg << endl;

				send(client.client, init_msg, (int)strlen(init_msg), 0);
			}

			else send(client.client, join_msg, (int)strlen(join_msg), 0);
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
			ZeroMemory(leave_msg, sizeof(leave_msg));
			sprintf_s(leave_msg, "l%2d", num); // 퇴장(l) 프로토콜
			char send_msg[PACKET_SIZE];
			ZeroMemory(send_msg, sizeof(send_msg));
			sprintf_s(send_msg, "m[#] Client #%d left", num); // 메세지(m) 프로토콜

			for (auto client = c.begin(); client != c.end(); client++) {
				if (client->number == num) {
					client->connected = false;
					cout << num << " disconnected" << endl;
					continue;
				}
				send(client->client, leave_msg, (int)strlen(leave_msg), 0);
				send(client->client, send_msg, (int)strlen(send_msg), 0);
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
				char num[2+1];
				memcpy(&num, &buffer[1], 2); num[2] = '\0';
				char x[5+1];
				memcpy(&x, &buffer[1+2], 5); x[5] = '\0';
				char y[5+1];
				memcpy(&y, &buffer[1+2+5], 5); y[5] = '\0';

				for (int i = 0; i < c.size(); i++) {
					if (c[i].number == atoi(num)) {
						c[i].x = atoi(x);
						c[i].y = atoi(y);
					}
				}

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