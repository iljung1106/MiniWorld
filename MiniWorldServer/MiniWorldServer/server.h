/* server.h */

#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>
#include <string>

using namespace std;

#define PACKET_SIZE 1024

/* �ϳ��� Ŭ���̾�Ʈ ������ ���� ������ Ŭ���� */
class ClientSocket {
public:
	SOCKET client;
	SOCKADDR_IN client_info = { 0, };
	int client_size = sizeof(client_info);
	int number;
	int x = 0, y = 0;
	bool connected = true;

	ClientSocket() {
		client = { 0, };
		client_info = { 0, };
		client_size = sizeof(client_info);
	}
	~ClientSocket() {
		client = { 0, };
		client_info = { 0, };
		client_size = -1;
		number = -1;
	}
};

SOCKET server; //���� ����
vector<ClientSocket> c; // ����� Ŭ���̾�Ʈ ����Ʈ

void openSocket();
void acceptingClients();
void recvData(SOCKET, int);
void closeSocket();