#include "stdafx.h"
#include "MainScene.h"

MainScene::MainScene() {
	socket = new Socket(this);
	socket->SetMsg("test");

	kInputter = new KoreanInputter(socket);

	me = new ZeroSprite("Texture/%s.png", "sample");
	PushScene(me);
}

void MainScene::Update(float eTime) {
	ZeroIScene::Update(eTime);
	this->eTime = eTime;

	socket->Update();
	me->Update(eTime);

	char dir = '\0';

	// moving myself
	if (ZeroInputMgr->GetKey('A') == INPUTMGR_KEYON) { // ��
		me->AddPosX(-moveSpd * eTime);
		dir = 'l';
	}
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON) { // ��
		me->AddPosX(moveSpd * eTime);
		dir = 'r';
	}
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON) { // ��
		me->AddPosY(-moveSpd * eTime);
		dir = 'u';
	}
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON) { // ��
		me->AddPosY(moveSpd * eTime);
		dir = 'd';
	}

	if (dir != '\0') {
		char msg[1+sizeof(int)+1+1];
		sprintf_s(msg, sizeof(msg), "u%d%c", socket->num, dir);
		cout << msg << endl;
		socket->SetMsg(msg);
		socket->SendMessage();
		dir = '\0';
	}
}

void MainScene::Render() {
	ZeroIScene::Render();
	me->Render();
	for (ZeroFont* chat : chats)
		chat->Render();
	for (auto user : users)
		user.second->Render();
}


/* Socket.cpp | �������� �޼����� �޾��� �� ȣ��� */
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
void MainScene::OnUserMove(int num, char dir) {
	auto useriter = users.find(num);
	if (useriter == users.end())
		return;

	ZeroSprite* user = useriter->second;

	switch (dir) {
		case 'l': user->AddPosX(-moveSpd * eTime); break;
		case 'r': user->AddPosX(+moveSpd * eTime); break;
		case 'u': user->AddPosY(-moveSpd * eTime); break;
		case 'd': user->AddPosY(+moveSpd * eTime); break;
	}
}

// ������ ������ �� �÷��̾ ����� ȭ�鿡 ����.
void MainScene::OnUserJoin(int num) {
	ZeroSprite* user = new ZeroSprite("Texture/%s.png", "sample");
	PushScene(user);

	users.insert({ num, user });
}

// ������ ������ �� �÷��̾ ���ش�.
void MainScene::OnUserLeave(int num) {
	users.erase(num);
}
