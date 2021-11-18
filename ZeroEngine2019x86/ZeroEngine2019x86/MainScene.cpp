#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() {
	char* text = "test";
	testFont = new ZeroFont(30, text, (char*)"³ª´®°íµñ");
	testFont->SetColor(0xffff0000);
	testFont->SetPos(0, 100);
	PushScene(testFont);

	kInputter = new KoreanInputter();

	socket = new Socket();
	socket->SetMsg("test");

	player = new ZeroSprite("Texture/%s.png", "sample");
	PushScene(player);
}

MainScene::~MainScene() {
	
}

void MainScene::Update(float eTime) {
	ZeroIScene::Update(eTime);

	socket->Update();
	player->Update(eTime);

	testFont->SetString(socket->recievedMsg);

	if (ZeroInputMgr->GetKey(VK_SPACE) == INPUTMGR_KEYDOWN) {
		socket->SendMessage();
	}

	if (ZeroInputMgr->GetKey('A') == INPUTMGR_KEYON)
		player->AddPosX(-moveSpd * eTime);
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON)
		player->AddPosX(moveSpd * eTime);
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON)
		player->AddPosY(-moveSpd * eTime);
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON)
		player->AddPosY(moveSpd * eTime);
}

void MainScene::Render() {
	ZeroIScene::Render();
	player->Render();
	testFont->Render();
}