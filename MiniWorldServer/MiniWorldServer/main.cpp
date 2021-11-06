#pragma comment(lib,"ws2_32.lib")

#include <iostream>
#include <winsock2.h>
#include <thread>

#define PACKET_SIZE 1024

using namespace std;
SOCKET skt, client_sock;

void proc_recvs() {
	char buffer[PACKET_SIZE] = { 0 };

	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);
		recv(client_sock, buffer, PACKET_SIZE, 0);
		cout << "받은 메세지: " << buffer << endl;
	}
}

int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4444);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(skt, (SOCKADDR*)&addr, sizeof(addr));
	listen(skt, SOMAXCONN);

	SOCKADDR_IN client = {};
	int client_size = sizeof(client);
	ZeroMemory(&client, client_size);
	client_sock = accept(skt, (SOCKADDR*)&client, &client_size);

	char buffer[PACKET_SIZE] = { 0 };
	thread proc2(proc_recvs);

	while (!WSAGetLastError()) {
		cin >> buffer;
		send(client_sock, buffer, strlen(buffer), 0);
	}
	proc2.join();
	closesocket(client_sock);
	closesocket(skt);
	WSACleanup();
}
