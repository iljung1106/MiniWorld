#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() {
	char* text;
	text = (char*)"test";
	testFont = new ZeroFont(30, text, (char*)"³ª´®°íµñ");
	testFont->SetColor(0xffff0000);
	testFont->SetPos(0, 100);
	PushScene(testFont);
	kInputter = new KoreanInputter();
	socket = new Socket();
	socket->SetMsg("test");
}

MainScene::~MainScene() {
	//
}

void MainScene::Update(float eTime) {
	ZeroIScene::Update(eTime);
	socket->Update();
	if (ZeroInputMgr->GetKey(VK_SPACE) == INPUTMGR_KEYDOWN) {
		socket->SendMessage();
	}
}

void MainScene::Render() {
	ZeroIScene::Render();
	testFont->Render();
}