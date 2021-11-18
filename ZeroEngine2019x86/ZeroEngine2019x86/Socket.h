#pragma comment(lib, "ws2_32.lib")
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>
#include <thread>
using namespace std;

#define PACKET_SIZE 1024

class MainScene;

class Socket {
private:
	void proc_recv();
	SOCKET server;
	WSADATA wsa;
	SOCKADDR_IN addr = {};
	bool isConnected = false;
	thread* p;
	thread tr;
public:
	Socket(MainScene* mainScene);
	~Socket();
	const char* msg;
	MainScene* mainScene;

	int num = -1;

	void Update();
	void SendMessage();
	void SetMsg(const char* t);
};
