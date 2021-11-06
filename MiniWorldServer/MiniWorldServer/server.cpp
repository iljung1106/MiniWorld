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
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // �ּ�

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

		char send_msg[PACKET_SIZE];
		sprintf_s(send_msg, "[#] Client #%d(��)�� �����߽��ϴ�.", num);
		for (ClientSocket client : c)
			send(client.client, send_msg, strlen(send_msg), 0);

		thread(recvData, c[num].client, num).detach();
	}
}

/* Ŭ���̾�Ʈ���� ������ �޼����� �޴� ������ */
void recvData(SOCKET s, int num) {
	char msg[PACKET_SIZE];
	char send_msg[PACKET_SIZE];

	while (1) {
		ZeroMemory(&msg, PACKET_SIZE);
		recv(s, msg, PACKET_SIZE, 0);

		// ����
		if (WSAGetLastError()) {
			cout << "> Client #" << num << " left" << endl;
			sprintf_s(send_msg, "[#] Client #%d(��)�� �����߽��ϴ�.", num);
			for (ClientSocket client : c)
				send(client.client, send_msg, strlen(send_msg), 0);
			return;
		}

		// �޼���
		cout << "> [Client #" << num << "] " << msg << endl;
		sprintf_s(send_msg, "<Client #%d> %s", num, msg);
		for (ClientSocket client : c)
			send(client.client, send_msg, strlen(send_msg), 0);
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