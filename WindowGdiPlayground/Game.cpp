#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_console(m_gfx, m_log),
    m_Sonic(m_gfx, {1000.0f, 600.0f}),
    m_Level(m_gfx, 20, 20),
    CellSpaceDrawCenter({ 30.0f,60.0f }),
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
    //clip world coords
    if (CellSpaceDrawCenter.x < 0.0f) {
        CellSpaceDrawCenter.x = 0.0f;
    }
    else if (CellSpaceDrawCenter.x > 200.0f) {
        CellSpaceDrawCenter.x = 200.0f;
    }
    if (CellSpaceDrawCenter.y < 0.0f) {
        CellSpaceDrawCenter.y = 0.0f;
    }
    else if (CellSpaceDrawCenter.y > 200.0f) {
        CellSpaceDrawCenter.y = 200.0f;
    }

    m_Sonic.update();
    m_Level.rotateBckgnd(rotor);
    rotor += 0.1f;
    m_obstacles.update(updObstacles);

 //   if (m_Level.h_Board().isInside(m_Sonic.getPosition())) {
 //       m_log.putMessage(L"YES");
 //   }
 //   else
 //   m_log.putMessage(L"NO");
}

void Game::composeFrame() {
    m_Level.draw(CellSpaceDrawCenter);

    

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

void Game::execCommand(std::wstring& command) {
    if (command == L"$MOVER") {
        CellSpaceDrawCenter.x+= 10.0f;
    }
    if (command == L"$MOVEL") {
        CellSpaceDrawCenter.x -= 10.0f;
    }
    if (command == L"$MOVEU") {
        CellSpaceDrawCenter.y -= 10.0f;
    }
    if (command == L"$MOVED") {
        CellSpaceDrawCenter.y += 10.0f;
    }
    if (command == L"$COORDS") {
        std::wstring str = L"World: " + std::to_wstring(CellSpaceDrawCenter.x) + L"; " + std::to_wstring(CellSpaceDrawCenter.y) + L"|| Cell: " + std::to_wstring(CellSpaceDrawCenter.x / 10.0f * 20) + L"; " + std::to_wstring(CellSpaceDrawCenter.y / 10.0f);
        m_log.putMessage(str.c_str());
    }
    m_log.putMessage(command.c_str());
    command = L"";
}

void Game::commandInput() {
    //cmd should only send commands, no work here pls
    std::unique_lock<std::mutex> lk(cmdMutex);
    unsigned short consoleBlock = 0;
    unsigned short keyboardBlock = 0;
    while (cmdRun) {
        if (!m_console.isActiveInput()) {

            if (str_GameCommand.size()) {
                //decypher
                execCommand(str_GameCommand);
            }

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

            if ((*m_kbd).keyIsPressed(VK_OEM_3) && !consoleBlock) {   // for Tilde
                m_console.activate();
                consoleBlock += 20; 
                keyboardBlock = 8;
            }

            if (m_console.isActive() && (*m_kbd).keyIsPressed(VK_RETURN) && !keyboardBlock) {
                m_console.activateInput();
                (*m_kbd).flushCharBuffer();
                keyboardBlock = 8;
            }

            if ((*m_kbd).keyIsPressed('T')) {
                updObstacles = Obstacles::MakeBox;  // mek it part  of obstacles after implemented only 1 cmd per frame
            }

        }
        else {
            //case of console input
            if (!keyboardBlock) {
                if ((*m_kbd).keyIsPressed(VK_RETURN)) {
                    m_console.activateInput();
                    str_GameCommand = m_console.getInput();
                    keyboardBlock = 8;
                }
                else if (!(*m_kbd).charIsEmpty()) {
                    m_console.setInput((*m_kbd).readChar());
                }
            }
        }
        

        auto timeout = std::chrono::system_clock::now() + std::chrono::milliseconds(100);
        cmdCV.wait_until(lk, timeout);

        if (consoleBlock) {
            consoleBlock -= 1;
        }
        if (keyboardBlock) {
            keyboardBlock -= 1;
        }
    }
}
