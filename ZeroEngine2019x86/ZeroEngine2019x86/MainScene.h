#pragma once
#include "ZeroIScene.h"
#include "ZeroFont.h"
#include "Zero.h"
#include "KoreanInputter.h"
#include "Socket.h"
#include "KoreanInputter.h"
class MainScene : public ZeroIScene {
public:
	MainScene();
	~MainScene();

	void OnRecieveMessage(char* msg);
	void OnUserMove(int num, int dir);
	void OnUserJoin(int num);
	void OnUserLeave(int num);

private:
	void Update(float eTime);
	void Render();

	list<ZeroFont*> chats;
	KoreanInputter* kInputter;
	Socket* socket;
	ZeroSprite* me;
	list<ZeroSprite*> users;

	int moveSpd = 200;
};

