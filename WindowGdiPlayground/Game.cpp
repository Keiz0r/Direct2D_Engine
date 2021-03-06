﻿#include "Game.h"

Game::Game(const HWND &hwnd, Keyboard& kbd)
	:
    m_kbd(&kbd),
	m_hwnd(hwnd),
	m_gfx(hwnd),
    m_console(m_gfx),
    m_Level(m_gfx, LEVEL_1_SIZE),
    m_obstacles(m_gfx)
{
    GameObject::initialize(&m_gfx, &screenCenterCoordinates, isometricCoefficients); //gfx in objects doesnt exist before this call
    Sound::openMP3();
    Sound::playOnRepeatMP3();
    ObjectManager objm(10);
    objm.addObj<Tree, const D2D1_POINT_2F&>({ 0.0f, 0.0f }, 1.0f, 1.0f, 0.0f, false);
    m_pSonic = std::make_unique<Sonic, const D2D1_POINT_2F&>({ 0.0f, 0.0f }, 1.0f, 1.0f, 0.0f);
    //launch cmd thread
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
#ifdef KEIZOR_DEBUG
    //crosshair
    m_gfx.DrawLine(0.0f, m_gfx.getScreenSize().height /2.0f, m_gfx.getScreenSize().width, m_gfx.getScreenSize().height / 2.0f, 1.0f);
    m_gfx.DrawLine(m_gfx.getScreenSize().width /2.0f, 0.0f, m_gfx.getScreenSize().width /2.0f, m_gfx.getScreenSize().height, 1.0f);
    static float_t blow = 0.0f;
    static bool flag = false;
    static Barrel* brl = new Barrel{ {0.0f, 0.0f}, 3.0f, 0.90f, blow * 1, false };
    static Barrel* brl2 = new Barrel{ {384.0f, 225.0f}, 3.0f, 0.90f, blow * 1, false };

    static Tree* tree = new Tree{ {0.0f, 0.0f}, 1.0f, 1.0f, 0.0f, false };
    static Tree* tree1 = new Tree{ {150.0f, 100.0f}, 0.7f, 1.0f, 0.0f, false };
    static Tree* tree2 = new Tree{ {-100.0f, -150.0f}, 0.5f, 1.0f, 0.0f, false }; 

    static bool test = false;
    static bool tst2 = true;
    if (!test) {
        brl->attachScript(Scripts::patrol(*brl, 2.0f));
        brl2->attachScript(Scripts::patrol(*brl2, 5.0f));
        tree1->attachScript(Scripts::patrol(*tree1, 1.0f));
        test = true;
    }
    brl->runScript();
    brl2->runScript();
    tree1->runScript();
    if (tst2 && blow > 210) {
        brl->detachScript();
        tree1->attachScript(Scripts::patrol(*tree1, 1.0f));
        tst2 = false;
    }
    if (!tst2 && blow < -100) {
        brl->attachScript(Scripts::patrol(*brl, 5.0f));
        tree1->attachScript(Scripts::patrol(*tree1, -2.0f));
        tst2 = true;
    }
    tree->draw();
    tree1->draw();
    tree2->draw();
    brl2->draw();
    brl->draw();
    brl->setRotationAngle(blow);
    brl2->setRotationAngle(blow);
    if (blow > 1274) {
        flag = true;
    }
    if (blow < -1225)
        flag = false;
    if (flag) {
        blow -= 100.0f;
    }
    else {
        blow += 100.0f;
    }
    clampCoordinates(*brl);
#endif
	m_gfx.endFrame();
    cmdCV.notify_all(); //update cmd
}

void Game::updateGameState() {
    const float_t dt = ft.Mark();
    m_pSonic->update();
    clampCoordinates(*m_pSonic);    //TODO: for_each of objectManager<>
    m_Level.rotateBckgnd(rotor);
    rotor += 0.1f;
    m_obstacles.update(updObstacles);
    screenCenterCoordinates = m_pSonic->getPosition();
}

void Game::composeFrame() {
    m_Level.draw(m_pSonic->getPosition());

    {
        std::wstring displaycoordsSONIC = std::to_wstring(static_cast<int>(m_pSonic->getPosition().x)) + L", " + std::to_wstring(static_cast<int>(m_pSonic->getPosition().y));  //DEBUG
        m_gfx.drawTextBox(displaycoordsSONIC.c_str(), 0, Graphics::D2D_SOLID_COLORS::OrangeRed, { m_gfx.getScreenSize().width / 2.0f - 20.0f, m_gfx.getScreenSize().height / 2.0f - 80.0f,  m_gfx.getScreenSize().width / 2.0f + 180.0f, m_gfx.getScreenSize().height / 2.0f - 45.0f });  //DEBUG
        m_pSonic->draw();
    }

    m_obstacles.draw();

    //  FPS
    m_gfx.drawTextBox((L"Fps: " + std::to_wstring(ft.getFPS())).c_str(), 1, Graphics::D2D_SOLID_COLORS::OrangeRed, { 10.0f, 680.0f, 250.0f, 720.0f});
    m_console.draw();
}

void Game::LoadLevel(GameLevel& level) {
}

void Game::execCommand(std::wstring& command_str) {
    // TODO:World of Regex
    // Console input
    static std::basic_regex re_isCmd(L"^\\$");
    static std::basic_regex re_Teleport(L"^\\$TELEPORT");
    static std::basic_regex re_Coords(L"^\\$COORDS");

    if (!std::regex_search(command_str, re_isCmd)) {
        //output entered string if it is not a command
        Log::putMessage(command_str.c_str());
    }
    else {
        command_str += L" ";    //TODO : further search for now needs ending space
        // TODO: network input
        if (std::regex_search(command_str, re_Teleport)) {
            std::vector<int32_t> newPos = getArgs<int32_t>(command_str);
            if (newPos.size() == 2) {
                m_pSonic->setPosition(static_cast<float_t>(newPos[0]), static_cast<float_t>(newPos[1]));
            }
            else {
                Log::putMessage(L"Wrong arguments for command <TELEPORT>");
            }
        }
        else if (std::regex_search(command_str, re_Coords)) {
            std::wstring str = L"World: " + std::to_wstring(m_pSonic->getPosition().x) + L"; " + std::to_wstring(m_pSonic->getPosition().y) + L"|| Cell: " + std::to_wstring(static_cast<int>(m_pSonic->getPosition().x * 20)) + L"; " + std::to_wstring(static_cast<int>(m_pSonic->getPosition().y));
            Log::putMessage(str.c_str());
        }
    }
    
    command_str = L"";
}

void Game::clampCoordinates(GameObject& entity) {
    D2D1_POINT_2F borders_x = m_Level.h_Board()->getWorldBorders_x();
    D2D1_POINT_2F borders_y = m_Level.h_Board()->getWorldBorders_y();
    D2D1_POINT_2F current_pos = entity.getPosition();
    if (current_pos.x > borders_x.y) {
        entity.setPosition(borders_x.y, current_pos.y);
        current_pos = {borders_x.y, current_pos.y};
    }
    else if (current_pos.x < borders_x.x) {
        entity.setPosition(borders_x.x, current_pos.y);
        current_pos = {borders_x.x, current_pos.y};
    }
    if (current_pos.y > borders_y.y) {
        entity.setPosition(current_pos.x, borders_y.y);
        current_pos = {current_pos.x, borders_y.y};
    }
    else if (current_pos.y < borders_y.x) {
        entity.setPosition(current_pos.x, borders_y.x);
        current_pos = {current_pos.x, borders_y.x};
    }
}

void Game::commandInput() {
    //cmd should only send commands, no work here pls
    std::unique_lock<std::mutex> lk(cmdMutex);
    uint8_t consoleBlock = 0;
    uint8_t keyboardBlock = 0;
    while (cmdRun) {
        if (!m_console.isActiveInput()) {

            if (str_GameCommand.size()) {
                //decypher
                execCommand(str_GameCommand);
            }

            //  movements
            if ((*m_kbd).keyIsPressed('D') && (*m_kbd).keyIsPressed('W')) {
                m_pSonic->speedUp(Sonic::Direction::N);
            }
            else if ((*m_kbd).keyIsPressed('D') && (*m_kbd).keyIsPressed('S')) {
                m_pSonic->speedUp(Sonic::Direction::E);
            }
            else if((*m_kbd).keyIsPressed('D')) {
                m_pSonic->speedUp(Sonic::Direction::NE);
            }
            else if ((*m_kbd).keyIsPressed('A') && (*m_kbd).keyIsPressed('W')) {
                m_pSonic->speedUp(Sonic::Direction::W);
            }
            else if ((*m_kbd).keyIsPressed('A') && (*m_kbd).keyIsPressed('S')) {
                m_pSonic->speedUp(Sonic::Direction::S);
            }
            else if ((*m_kbd).keyIsPressed('A')) {
                m_pSonic->speedUp(Sonic::Direction::SW);
            }
            else if ((*m_kbd).keyIsPressed('W')) {
                m_pSonic->speedUp(Sonic::Direction::NW);
            }
            else if ((*m_kbd).keyIsPressed('S')) {
                m_pSonic->speedUp(Sonic::Direction::SE);
            }

            if ((*m_kbd).keyIsPressed('R')) {
                m_pSonic->blink(10);
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

template<typename T>
std::vector<T> Game::getArgs(const std::wstring cmd) const
{
    std::basic_regex re(L"^[[:digit:]]+$|^[[:digit:]]+[\.,][[:digit:]]+$");   //more than 1 numeric OR more than 1 numeric;optional dot\comma;0 or more numerics
    std::vector<T> args;
    //TODO: put into recursive function getArgs
    size_t commandEnd = cmd.find(L" ");
    int8_t flip = 1;
    //parse 1st arg
    size_t posOfSpace = 0;
    std::wstring arg;
    auto getArg = [&]() {
        flip = 1;
        posOfSpace = cmd.find_first_of(L" ", commandEnd + 1) - 1;
        arg = cmd.substr(commandEnd + 1, posOfSpace - commandEnd);
        if (arg.length() > 9) {
            arg = L"";
            return;
        }
        if (arg.substr(0, 1) == L"-") {
            flip = -1;
            arg.erase(0, 1);
        }
        return;
    };


    getArg();
    if (std::regex_match(arg, re)) {
        args.push_back(std::stoi(arg) * flip);
        //parse 2nd arg
        commandEnd = posOfSpace + 1;
        getArg();
        if (std::regex_match(arg, re)) {
            args.push_back(std::stoi(arg) * flip);
        }
    }
    return args;
}