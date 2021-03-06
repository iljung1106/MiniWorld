#pragma once

#include "ZeroIScene.h"
#include "ZeroFont.h"
#include "Zero.h"
#include "Socket.h"
#include "KoreanInputter.h"

class MainScene : public ZeroIScene {
private:
	Socket* socket;
	KoreanInputter* kInputter;
	ZeroSprite* me;
	ZeroSprite* backGround;
	map<int, ZeroSprite*> users;
	list<ZeroFont*> chats;

	int moveSpd = 200;

	void Update(float eTime);
	void Render();

public:
	MainScene(char* addr);

	void OnRecieveMessage(char* msg);
	void OnUserMove(int num, int x, int y);
	void OnUserJoin(int num);
	void OnUserLeave(int num);
	void OnInit(vector<tuple<int, int, int>> currentUsers);
};
