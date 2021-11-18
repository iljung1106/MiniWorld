#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene(HINSTANCE hInst) {

	kInputter = new KoreanInputter(hInst);

	socket = new Socket(this);
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
	for (ZeroFont* chat : chats)
		chat->Render();
}


/* Socket.cpp */
// 서버에서 메세지를 받으면 새로운 채팅 텍스트를 띄운다.
void MainScene::OnRecieveMessage(char* msg) {
	ZeroFont* testFont = new ZeroFont(30, msg, (char*)"나눔고딕");
	testFont->SetColor(0xffff0000);
	testFont->SetPos(300, 400);
	PushScene(testFont);

	for (ZeroFont* chat : chats)
		chat->AddPosY(-30);
	chats.push_back(testFont);
}

// 서버에서 플레이어 움직임을 받으면 그 플레이어를 움직인다.
void MainScene::OnRecieveMovement(int num, int dir) {

}

// 서버에서 플레이어가 들어오면 새 플레이어를 만들고 화면에 띄운다.
void MainScene::OnPlayerJoin(int num) {

}

// 서버에서 플레이어가 들어오면 그 플레이어를 없앤다.
void MainScene::OnPlayerLeave(int num) {

}
