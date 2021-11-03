#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

#define PACKET_SIZE 1024

#pragma once

class Socket {
private:
	void proc_recv();
	SOCKET skt;
	WSADATA wsa;
	SOCKADDR_IN addr = {};
	bool isConnected = false;
	thread* p;
	thread tr;
public:
	char* recievedMsg ="";
	Socket();
	const char* msg;
	void Update();
	void SendMessage();
	void SetMsg(const char* t);
};