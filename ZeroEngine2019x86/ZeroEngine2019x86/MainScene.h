#pragma once
#include "ZeroIScene.h"
#include "ZeroFont.h"
#include "Zero.h"
#include "KoreanInputter.h"
#include "Socket.h"
#include "KoreanInputter.h"
class MainScene : public ZeroIScene {
public:
	MainScene(HINSTANCE hInst);
	~MainScene();

	void OnRecieveMessage(char* msg);
	void OnRecieveMovement(int num, int dir);
	void OnPlayerJoin(int num);
	void OnPlayerLeave(int num);

private:
	void Update(float eTime);
	void Render();

	list<ZeroFont*> chats;
	KoreanInputter* kInputter;
	Socket* socket;
	ZeroSprite* player;

	int moveSpd = 200;
};

