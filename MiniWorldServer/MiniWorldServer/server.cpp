#include "server.h"

int main() {
	openSocket();
	thread(acceptingClients).detach();

	while (1);

	closeSocket();
}


/* ���� ���� */
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
	addr.sin_port = htons(4444); // ��Ʈ
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // �ּ�

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

/* Ŭ���̾�Ʈ ������ �޴� ������ */
void acceptingClients() {
	for (int num = 0;; num++) {

		// ����
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
		sprintf_s(join_msg, "j%2d", num); // ����(j) ��������
		char send_msg[PACKET_SIZE];
		ZeroMemory(send_msg, sizeof(send_msg));
		sprintf_s(send_msg, "m[#] Client #%d joined", num); // �޼���(m) ��������
		
		cout << join_msg << endl;

		for (ClientSocket client : c) {

			if (client.number == num) { // ���� ���� Ŭ���̾�Ʈ���״� �ʱ�ȭ �޼����� ����
				char init_msg[PACKET_SIZE];
				ZeroMemory(init_msg, sizeof(init_msg));
				int csize = 0;
				for (ClientSocket cl : c) {
					cout << cl.number << endl;
					if (cl.connected) { csize++; cout << "++ " << endl; }
				}
				sprintf_s(init_msg, "i%2d%2d", num, (int)csize-1); // �ʱ�ȭ(i) ��������

				for (int i = 0; i < c.size(); i++) {
					if (c[i].number == num) continue; // �ڱ� �ڽ� ����
					if (!c[i].connected) continue; // ������ ���� ���� ����

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

/* Ŭ���̾�Ʈ���� ������ �޼����� �޴� ������ */
void recvData(SOCKET s, int num) {
	char buffer[PACKET_SIZE];
	char send_msg[PACKET_SIZE];

	while (1) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(s, buffer, PACKET_SIZE, 0);

		// ����
		if (WSAGetLastError()) {

			cout << "> Client #" << num << " left" << endl;
			char leave_msg[PACKET_SIZE];
			ZeroMemory(leave_msg, sizeof(leave_msg));
			sprintf_s(leave_msg, "l%2d", num); // ����(l) ��������
			char send_msg[PACKET_SIZE];
			ZeroMemory(send_msg, sizeof(send_msg));
			sprintf_s(send_msg, "m[#] Client #%d left", num); // �޼���(m) ��������

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
				// �޼���
				char msg[PACKET_SIZE];
				memcpy(msg, &buffer[1], sizeof(buffer)-1);

				cout << "> [Client #" << num << "] " << msg << endl;
				sprintf_s(send_msg, "m<Client #%d> %s", num, msg); // �޼���(m) ��������
				for (ClientSocket client : c)
					send(client.client, send_msg, (int)strlen(send_msg), 0);
				break;
			
			// user movement (u(1) + client num(2) + pos.x(5) + pos.y(5))
			case 'u':
				// ���� �̵�
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

/* ���� �ݱ� */
void closeSocket() {
	for (ClientSocket client : c)
		closesocket(client.client);
	closesocket(server);
	WSACleanup();
	cout << "> Socket closed" << endl;
}