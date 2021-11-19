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

	socket->Update();
	me->Update(eTime);

	bool moved = false;

	// moving myself
	if (ZeroInputMgr->GetKey('A') == INPUTMGR_KEYON) { 
		me->AddPosX(-moveSpd * eTime); // ��
		moved = true;
	}
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON) {
		me->AddPosX(moveSpd * eTime); // ��
		moved = true;
	}
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON) {
		me->AddPosY(-moveSpd * eTime); // ��
		moved = true;
	}
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON) {
		me->AddPosY(moveSpd * eTime); // ��
		moved = true;
	}

	if (moved) {
		// �̵� �޼��� ������
		char msg[PACKET_SIZE];
		sprintf_s(msg, "u%2d%5d%5d", socket->num, (int)me->Pos().x, (int)me->Pos().y); // ���� �̵�(u) ��������
		socket->SetMsg(msg);
		socket->SendMessage();
	}
}

void MainScene::Render() {
	ZeroIScene::Render();
	me->Render();
	for (ZeroFont* chat : chats)
		chat->Render();
	for (pair<int, ZeroSprite*> user : users)
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
void MainScene::OnUserMove(int num, int x, int y) {
	auto useriter = users.find(num);
	if (useriter == users.end())
		return;

	ZeroSprite* user = useriter->second;
	ZeroVec* vec = new ZeroVec(x, y);
	user->SetPos(*vec);
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

// Ŭ���̾�Ʈ�� ó�� �����ϸ� ���� ������ �ִ� �������� ��ġ�� �޴´�.
void MainScene::OnInit(vector<tuple<int, int, int>> currentUsers) {
	for (tuple<int, int, int> user : currentUsers) {
		OnUserJoin(get<0>(user));
		OnUserMove(get<0>(user), get<1>(user), get<2>(user));
	}
}
