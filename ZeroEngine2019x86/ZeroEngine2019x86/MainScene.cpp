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
		me->AddPosX(-moveSpd * eTime); // ←
		moved = true;
	}
	if (ZeroInputMgr->GetKey('D') == INPUTMGR_KEYON) {
		me->AddPosX(moveSpd * eTime); // →
		moved = true;
	}
	if (ZeroInputMgr->GetKey('W') == INPUTMGR_KEYON) {
		me->AddPosY(-moveSpd * eTime); // ↑
		moved = true;
	}
	if (ZeroInputMgr->GetKey('S') == INPUTMGR_KEYON) {
		me->AddPosY(moveSpd * eTime); // ↓
		moved = true;
	}

	if (moved) {
		// 이동 메세지 보내기
		char msg[PACKET_SIZE];
		sprintf_s(msg, "u%2d%5d%5d", socket->num, (int)me->Pos().x, (int)me->Pos().y); // 유저 이동(u) 프로토콜
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
void MainScene::OnUserMove(int num, int x, int y) {
	auto useriter = users.find(num);
	if (useriter == users.end())
		return;

	ZeroSprite* user = useriter->second;
	ZeroVec* vec = new ZeroVec(x, y);
	user->SetPos(*vec);
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

// 클라이언트에 처음 접속하면 현재 서버에 있는 유저들의 위치를 받는다.
void MainScene::OnInit(vector<tuple<int, int, int>> currentUsers) {
	for (tuple<int, int, int> user : currentUsers) {
		OnUserJoin(get<0>(user));
		OnUserMove(get<0>(user), get<1>(user), get<2>(user));
	}
}
