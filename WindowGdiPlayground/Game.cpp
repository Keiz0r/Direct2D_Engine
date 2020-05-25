#include "Game.h"

#pragma comment(lib, "winmm.lib")

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_board(m_gfx),
    m_Sonic(m_gfx, {100.0f, 50.0f}),
    Homescreen(m_gfx)
{
    Homescreen.Initialize();

    mciSendString(L"open \"..\\Assets\\testmusic.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    //To play* .mp3:
 //   mciSendString(L"play mp3", NULL, 0, NULL);
    //To playand wait until the* .mp3 has finished playing :
//    mciSendString(L"play mp3 wait", NULL, 0, NULL);
    //To replay(play again from start) the* .mp3:
//    mciSendString(L"play mp3 from 0", NULL, 0, NULL);
    //To replayand wait until the* .mp3 has finished playing :
//    mciSendString(L"play mp3 from 0 wait", NULL, 0, NULL);
    //To play the* .mp3and replay it every time it ends like a loop :
    mciSendString(L"play mp3 repeat", NULL, 0, NULL);
    //To pause the* .mp3 in middle :
//    mciSendString(L"pause mp3", NULL, 0, NULL);
    //and to resume it :
//    mciSendString(L"resume mp3", NULL, 0, NULL);
    //To stop it in middle :
//    mciSendString(L"stop mp3", NULL, 0, NULL);
    //close file
//    mciSendString(L"close mp3", NULL, 0, NULL);
}

Game::~Game(){
    for (std::vector<Box*>::reverse_iterator i = boxes.rbegin(); i < boxes.rend(); i++) {
        delete (*i);
    }
}

void Game::gameLoop(){
	m_gfx.beginFrame();
    updateGameState();
	composeFrame();
	m_gfx.endFrame();
}

void Game::updateGameState() {
    const float dt = ft.Mark();
    if ((*m_kbd).keyIsPressed('D')) {
        m_Sonic.speedUp(true);
    }
    else if ((*m_kbd).keyIsPressed('A')) {
        m_Sonic.speedUp(false);
    }
    if ((*m_kbd).keyIsPressed('W')) {
        m_Sonic.move(0.0f, -5.0f);
    }
    if ((*m_kbd).keyIsPressed('S')) {
        m_Sonic.move(0.0f, 5.0f);
    }

    if ((*m_kbd).keyIsPressed('T')) {
        bool flag = false;
        D2D1_POINT_2F position{ 400.0f, 600.0f };
        //spawns shitload of boxes LOL
        if (!flag) {
                Box* box1 = new Box(m_gfx, position);
                boxes.push_back(box1);
        }
    }
    m_Sonic.update();
}

void Game::composeFrame() {
    Homescreen.draw();

    m_board.drawBoardCells();
    std::wstring displaycoordsSONIC = std::to_wstring(m_Sonic.getPosition().x) + L", " + std::to_wstring(m_Sonic.getPosition().y);                                                         //DEBUG
    m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, 2, { m_Sonic.getPosition().x, m_Sonic.getPosition().y - 30.0f,  m_Sonic.getPosition().x + 200.0f, m_Sonic.getPosition().y + 5.0f });  //DEBUG
    m_Sonic.draw();
    for (std::vector<Box*>::iterator i = boxes.begin(); i < boxes.end(); i++) {
        (*i)->draw();
    }
    m_gfx.drawTextBox(ft.getFPS().c_str(), 1, 2, { 10.0f, 680.0f, 200.0f, 720.0f});
}

void Game::LoadLevel(GameLevel& level) {
}
