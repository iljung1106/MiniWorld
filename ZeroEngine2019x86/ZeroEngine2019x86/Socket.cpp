#include "stdafx.h"
#include "Socket.h"
#include "MainScene.h"

Socket::Socket(MainScene* mainScene, char* addrs) {
	this->mainScene = mainScene;
	strcpy(this->address, addrs);

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
	//addr.sin_addr.s_addr = inet_addr("211.204.30.40"); // �ּ�
	addr.sin_addr.s_addr = inet_addr(address); // �ּ�

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

			//cout << buffer << endl;

			switch (buffer[0]) {

				// message (m(1) + msg(1023))
				case 'm': {
					char recievedMsg[PACKET_SIZE-1];
					memcpy(recievedMsg, &buffer[1], PACKET_SIZE-1);

					mainScene->OnRecieveMessage(recievedMsg);
					break;
				}
				
				// user movement (u(1) + client num(2) + pos.x(5) + pos.y(5))
				case 'u': {
					char num[2];
					memcpy(&num, &buffer[1], 2);
					char x[5];
					memcpy(&x, &buffer[1+2], 5);
					char y[5];
					memcpy(&y, &buffer[1+2+5], 5);

					mainScene->OnUserMove(atoi(num), atoi(x), atoi(y));
					break;
				}
				
				// join (j(1) + client num(2))
				case 'j': {
					char num[2];
					memcpy(&num, &buffer[1], 2);

					mainScene->OnUserJoin(atoi(num));
					break;
				}
				
				// leave (l(1) + client num(2))
				case 'l': {
					char num[2];
					memcpy(&num, &buffer[1], 2);

					mainScene->OnUserLeave(atoi(num));
					break;
				}
				
				// init (i(1) + my num(2) + user count(2) + [(user num(2) + user.x(5) + user.y(5)), ... ]
				case 'i': {
					char num[2];
					memcpy(&num, &buffer[1], 2);
					this->num = atoi(num);

					char usercount[2];
					memcpy(&usercount, &buffer[1+2], 2);

					vector<tuple<int, int, int>> users;
					for (int i = 0; i < atoi(usercount); i++) {

						char usernum[2];
						memcpy(&usernum, &buffer[1+2+2 + i*12], 2);
						char x[5];
						memcpy(&x, &buffer[1+2+2+2 + i*12], 5);
						char y[5];
						memcpy(&y, &buffer[1+2+2+2+5 + i*12], 5);

						tuple<int, int, int> user = make_tuple<int, int, int>(atoi(usernum), atoi(x), atoi(y));
						users.push_back(user);
					}

					mainScene->OnInit(users);
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