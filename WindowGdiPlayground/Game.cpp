#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_console(m_gfx, m_log),
    m_Sonic(m_gfx, {0.0f, 0.0f}),
    m_Level(m_gfx, LEVEL_1_SIZE),
    m_obstacles(m_gfx, m_log)
{
    Sound::openMP3();
    Sound::playOnRepeatMP3();
    cmdln = std::thread([this](){this->commandInput(); });
    
}

Game::~Game(){
    Sound::closeMP3();
    cmdRun = false;
    cmdCV.notify_all();
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
    clampCoordinates(m_Sonic);

    m_Level.rotateBckgnd(rotor);
    rotor += 0.1f;
    m_obstacles.update(updObstacles);

}

void Game::composeFrame() {
    m_Level.draw(m_Sonic.getPosition());

    {
        std::wstring displaycoordsSONIC = std::to_wstring(static_cast<int>(m_Sonic.getPosition().x)) + L", " + std::to_wstring(static_cast<int>(m_Sonic.getPosition().y));  //DEBUG
        m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, Graphics::D2D_SOLID_COLORS::OrangeRed, { m_gfx.getScreenSize().width / 2.0f - 20.0f, m_gfx.getScreenSize().height / 2.0f - 60.0f,  m_gfx.getScreenSize().width / 2.0f + 180.0f, m_gfx.getScreenSize().height / 2.0f - 25.0f });  //DEBUG
        m_Sonic.draw();
    }

    m_obstacles.draw();

    m_gfx.drawTextBox(ft.getFPS().c_str(), 1, Graphics::D2D_SOLID_COLORS::OrangeRed, { 10.0f, 680.0f, 200.0f, 720.0f});
    m_console.draw();
}

void Game::LoadLevel(GameLevel& level) {
}

void Game::execCommand(std::wstring& command) {
    // Console input
    // TODO: network input
//    if (command == L"$MOVER") {
//        CameraCenter.x+= 1.0f;
//    }
//    if (command == L"$MOVEL") {
//        CameraCenter.x -= 1.0f;
//    }
//    if (command == L"$MOVEU") {
//        CameraCenter.y -= 1.0f;
//    }
//    if (command == L"$MOVED") {
//        CameraCenter.y += 1.0f;
//    }
    if (command == L"$COORDS") {
        std::wstring str = L"World: " + std::to_wstring(m_Sonic.getPosition().x) + L"; " + std::to_wstring(m_Sonic.getPosition().y) + L"|| Cell: " + std::to_wstring(static_cast<int>(m_Sonic.getPosition().x * 20)) + L"; " + std::to_wstring(static_cast<int>(m_Sonic.getPosition().y));
        m_log.putMessage(str.c_str());
    }
    m_log.putMessage(command.c_str());
    command = L"";
}

void Game::clampCoordinates(Sonic& sonic) {
    D2D1_POINT_2F borders_x = m_Level.h_Board()->getWorldBorders_x();
    D2D1_POINT_2F borders_y = m_Level.h_Board()->getWorldBorders_y();
    if (sonic.getPosition().x > borders_x.y) {
        sonic.setPosition(borders_x.y, sonic.getPosition().y);
    }
    else if (sonic.getPosition().x < borders_x.x) {
        sonic.setPosition(borders_x.x, sonic.getPosition().y);
    }
    if (sonic.getPosition().y > borders_y.y) {
        sonic.setPosition(sonic.getPosition().x, borders_y.y);
    }
    else if (sonic.getPosition().y < borders_y.x) {
        sonic.setPosition(sonic.getPosition().x, borders_y.x);
    }
    
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

            //  movements
            if ((*m_kbd).keyIsPressed('D') && (*m_kbd).keyIsPressed('W')) {
                m_Sonic.speedUp(Sonic::Direction::N);
            }
            else if ((*m_kbd).keyIsPressed('D') && (*m_kbd).keyIsPressed('S')) {
                m_Sonic.speedUp(Sonic::Direction::E);
            }
            else if((*m_kbd).keyIsPressed('D')) {
                m_Sonic.speedUp(Sonic::Direction::NE);
            }
            else if ((*m_kbd).keyIsPressed('A') && (*m_kbd).keyIsPressed('W')) {
                m_Sonic.speedUp(Sonic::Direction::W);
            }
            else if ((*m_kbd).keyIsPressed('A') && (*m_kbd).keyIsPressed('S')) {
                m_Sonic.speedUp(Sonic::Direction::S);
            }
            else if ((*m_kbd).keyIsPressed('A')) {
                m_Sonic.speedUp(Sonic::Direction::SW);
            }
            else if ((*m_kbd).keyIsPressed('W')) {
                m_Sonic.speedUp(Sonic::Direction::NW);
            }
            else if ((*m_kbd).keyIsPressed('S')) {
                m_Sonic.speedUp(Sonic::Direction::SE);
            }

            if ((*m_kbd).keyIsPressed('R')) {
                m_Sonic.blink(10);
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
            //case of active console input
            if (!keyboardBlock) {
                if ((*m_kbd).keyIsPressed(VK_RETURN)) {
                    m_console.activateInput();
                    str_GameCommand = m_console.getInput();
                    keyboardBlock = 8;
                }
                else if ((*m_kbd).keyIsPressed(VK_BACK)) {
                    m_console.removeInput();
                    keyboardBlock = 4;
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
