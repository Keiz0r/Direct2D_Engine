#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_board(m_gfx),
    m_Sonic(m_gfx, {100.0f, 50.0f}),
    Homescreen(m_gfx),
    level(m_gfx)
{
    Homescreen.Initialize();
}

Game::~Game(){
}

void Game::gameLoop(){
	m_gfx.beginFrame();
    updateModel();
	composeFrame();
	m_gfx.endFrame();
}

void Game::updateModel() {
    const float dt = ft.Mark();
    if ((*m_kbd).keyIsPressed('D')) {
        m_Sonic.speedUp(true);
    }
    else if ((*m_kbd).keyIsPressed('A')) {
        m_Sonic.speedUp(false);
    }
    if ((*m_kbd).keyIsPressed('W'))
    {
        m_Sonic.move(0.0f, -5.0f);
    }
    if ((*m_kbd).keyIsPressed('S'))
    {
        m_Sonic.move(0.0f, 5.0f);
    }
    m_Sonic.update();
}

void Game::composeFrame() {
    Homescreen.draw();

    m_board.drawBoardCells();
    std::wstring displaycoordsSONIC = std::to_wstring(m_Sonic.getPosition().x) + L", " + std::to_wstring(m_Sonic.getPosition().y);                                                         //DEBUG
    m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, 2, { m_Sonic.getPosition().x, m_Sonic.getPosition().y - 30.0f,  m_Sonic.getPosition().x + 200.0f, m_Sonic.getPosition().y + 5.0f });  //DEBUG
    m_Sonic.draw();
    m_gfx.drawTextBox(ft.getFPS().c_str(), 1, 2, { 10.0f, 680.0f, 200.0f, 720.0f});
}

void Game::LoadLevel(GameLevel& level) {
}
