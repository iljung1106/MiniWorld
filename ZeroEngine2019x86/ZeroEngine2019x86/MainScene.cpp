#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() {
	socket = new Socket(this);
	socket->SetMsg("test");

	kInputter = new KoreanInputter(socket);

	me = new ZeroSprite("Texture/%s.png", "sample");
	PushScene(me);
}

MainScene::~MainScene() {
	
}

void MainScene::Update(float eTime) {
	ZeroIScene::Update(eTime);

	socket->Update();
	me->Update(eTime);



	if (ZeroInputMgr->GetKey('A') == INPUTMGR_KEYON)
		me->AddPosX(-moveSpd * eTime);
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON)
		me->AddPosX(moveSpd * eTime);
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON)
		me->AddPosY(-moveSpd * eTime);
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON)
		me->AddPosY(moveSpd * eTime);
}

void MainScene::Render() {
	ZeroIScene::Render();
	me->Render();
	for (ZeroFont* chat : chats)
		chat->Render();
}

// Socket.cpp | �������� �޼����� �޾��� �� ȣ���
// ���ο� ä�� �ؽ�Ʈ�� ����.
void MainScene::OnRecieveMessage(char* msg) {
	ZeroFont* testFont = new ZeroFont(30, msg, (char*)"�������");
	testFont->SetColor(0xffff0000);
	testFont->SetPos(300, 400);
	PushScene(testFont);

	for (ZeroFont* chat : chats)
		chat->AddPosY(-30);
	chats.push_back(testFont);

}

// �������� �÷��̾� �������� ������ �� �÷��̾ �����δ�.
void MainScene::OnUserMove(int num, int dir) {

}

// ������ ������ �� �÷��̾ ����� ȭ�鿡 ����.
void MainScene::OnUserJoin(int num) {
	
}

// ������ ������ �� �÷��̾ ���ش�.
void MainScene::OnUserLeave(int num) {
	
}
