#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_console(m_gfx, m_log),
    m_board(m_gfx),
    m_Sonic(m_gfx, {100.0f, 50.0f}),
    Homescreen(m_gfx)
{
    Homescreen.Initialize();
    Sound::openMP3();
    Sound::playOnRepeatMP3();
}

Game::~Game(){
    for (std::vector<Box*>::reverse_iterator i = boxes.rbegin(); i < boxes.rend(); i++) {
        delete (*i);
    }
    Sound::closeMP3();
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

    if ((*m_kbd).keyIsPressed('C')) {
        m_console.activate();
        m_log.putMessage(L"console show");
    }

    if ((*m_kbd).keyIsPressed('T')) {
        bool flag = false;
        D2D1_POINT_2F position{ 400.0f, 600.0f };
        //spawns shitload of boxes LOL
        if (!flag) {
                Box* box1 = new Box(m_gfx, position);
                boxes.push_back(box1);
                m_log.putMessage(L"box emplaced");
        }
    }
    m_Sonic.update();
}

void Game::composeFrame() {
    Homescreen.draw();

    m_board.drawBoardCells();
    std::wstring displaycoordsSONIC = std::to_wstring(static_cast<int>(m_Sonic.getPosition().x)) + L", " + std::to_wstring(static_cast<int>(m_Sonic.getPosition().y));                                                         //DEBUG
    m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, 2, { m_Sonic.getPosition().x, m_Sonic.getPosition().y - 30.0f,  m_Sonic.getPosition().x + 200.0f, m_Sonic.getPosition().y + 5.0f });  //DEBUG
    m_Sonic.draw();
    for (std::vector<Box*>::iterator i = boxes.begin(); i < boxes.end(); i++) {
        (*i)->draw();
    }
    m_gfx.drawTextBox(ft.getFPS().c_str(), 1, 2, { 10.0f, 680.0f, 200.0f, 720.0f});
    m_console.draw();
}

void Game::LoadLevel(GameLevel& level) {
}
