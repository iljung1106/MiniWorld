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
	map<int, ZeroSprite*> users;
	list<ZeroFont*> chats;

	float eTime;

	int moveSpd = 200;

	void Update(float eTime);
	void Render();

public:
	MainScene();

	void OnRecieveMessage(char* msg);
	void OnUserMove(int num, char dir);
	void OnUserJoin(int num);
	void OnUserLeave(int num);

};
