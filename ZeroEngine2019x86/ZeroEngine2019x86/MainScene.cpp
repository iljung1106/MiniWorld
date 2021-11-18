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
	if (ZeroInputMgr->GetKey('A') == INPUTMGR_KEYON) { // ←
		me->AddPosX(-moveSpd * eTime);
		dir = 'l';
	}
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON) { // →
		me->AddPosX(moveSpd * eTime);
		dir = 'r';
	}
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON) { // ↑
		me->AddPosY(-moveSpd * eTime);
		dir = 'u';
	}
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON) { // ↓
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


/* Socket.cpp | 서버에서 메세지를 받았을 때 호출됨 */
// 새로운 채팅 텍스트를 띄운다.
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

// 유저가 들어오면 새 플레이어를 만들고 화면에 띄운다.
void MainScene::OnUserJoin(int num) {
	ZeroSprite* user = new ZeroSprite("Texture/%s.png", "sample");
	PushScene(user);

	users.insert({ num, user });
}

// 유저가 들어오면 그 플레이어를 없앤다.
void MainScene::OnUserLeave(int num) {
	users.erase(num);
}
