#pragma once
#include "ZeroIScene.h"
#include "ZeroFont.h"
#include "Zero.h"
#include "KoreanInputter.h"
#include "Socket.h"
#include "KoreanInputter.h"
class MainScene : public ZeroIScene {
public :
	MainScene(HINSTANCE hInst);
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

