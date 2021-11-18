#pragma once
#include "ZeroIScene.h"
#include "ZeroFont.h"
#include "Zero.h"
#include "KoreanInputter.h"
#include "Socket.h"
class MainScene : public ZeroIScene {
public :
	MainScene();
	~MainScene();
private :
	void Update(float eTime);
	void Render();

	ZeroFont* testFont;
	KoreanInputter* kInputter;
	Socket* socket;
	ZeroSprite* player;

	int moveSpd = 200;
};

