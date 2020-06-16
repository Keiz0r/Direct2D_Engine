#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_console(m_gfx, m_log),
    m_Sonic(m_gfx, {1000.0f, 600.0f}),
    m_Level(m_gfx, 20, 20),
    m_obstacles(m_gfx, m_log)
{
    m_Level.Initialize();
    Sound::openMP3();
    Sound::playOnRepeatMP3();
    cmdln = std::thread([this](){this->commandInput(); });
    
}

Game::~Game(){
    Sound::closeMP3();
    cmdRun = false;
    cmdln.join();
}

void Game::gameLoop(){
	m_gfx.beginFrame();
    updateGameState();
	composeFrame();
	m_gfx.endFrame();
    cmdCV.notify_all();
}

void Game::updateGameState() {
    const float dt = ft.Mark();
    
    m_Sonic.update();
    m_Level.rotateBckgnd(rotor);
    rotor += 0.1f;
    m_obstacles.update(updObstacles);

    if (m_Level.h_Board().isInside(m_Sonic.getPosition())) {
        m_log.putMessage(L"YES");
    }
    else
    m_log.putMessage(L"NO");
}

void Game::composeFrame() {
    m_Level.draw({0.0f, 0.0f});

    

    {
        std::wstring displaycoordsSONIC = std::to_wstring(static_cast<int>(m_Sonic.getPosition().x)) + L", " + std::to_wstring(static_cast<int>(m_Sonic.getPosition().y));  //DEBUG
        m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, 2, { m_Sonic.getPosition().x, m_Sonic.getPosition().y - 30.0f,  m_Sonic.getPosition().x + 200.0f, m_Sonic.getPosition().y + 5.0f });  //DEBUG
        m_Sonic.draw();
    }

    m_obstacles.draw();

    m_gfx.drawTextBox(ft.getFPS().c_str(), 1, 2, { 10.0f, 680.0f, 200.0f, 720.0f});
    m_console.draw();
}

void Game::LoadLevel(GameLevel& level) {
}

void Game::commandInput() {
    //cmd should only send commands, no work here pls
    std::unique_lock<std::mutex> lk(cmdMutex);
    while (cmdRun) {
        if ((*m_kbd).keyIsPressed('D')) {
            m_Sonic.speedUp(true);
        }
        if ((*m_kbd).keyIsPressed('A')) {
            m_Sonic.speedUp(false);
        }
        if ((*m_kbd).keyIsPressed('W')) {
            m_Sonic.move(0.0f, -5.0f);
        }
        if ((*m_kbd).keyIsPressed('S')) {
            m_Sonic.move(0.0f, 5.0f);
        }

        if ((*m_kbd).keyIsPressed(VK_OEM_3)) {   // for Tilde
            m_console.activate();
        }

        if ((*m_kbd).keyIsPressed('T')) {
            updObstacles = Obstacles::MakeBox;  // mek it part  of obstacles after implemented only 1 cmd per frame
        }
        auto timeout = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
        cmdCV.wait_until(lk, timeout);
    }
}
