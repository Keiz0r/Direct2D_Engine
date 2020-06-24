#include "GameConsole.h"

GameConsole::GameConsole(Graphics& p_gfx, Log& p_log)
:
	m_pgfx(p_gfx),
	m_plog(p_log)
{
}

void GameConsole::draw() {
	if (active) {
		m_pgfx.DrawRect({ 0.0f, 0.0f, 1366.0f, 200.0f }, true, bckgndColor);
		std::deque<std::wstring> out = m_plog.commands;
		for (int i = 0; i < 7 && i < out.size(); i++) {
			float msgOffset = static_cast<float>(i) * 24.0f;
			m_pgfx.drawTextBox(out.at(i).c_str(), 2, textColor, { 5.0f, 146.0f - msgOffset, 1361.0f, 170.0f - msgOffset });

		}
		if (activeinput) {
			if (blinkCounter == 31) {
				if (blink == L"_") {
					blink.replace(0, 1, L" ");
				}
				else {
					blink.replace(0, 1, L"_");
				}
				blinkCounter = 0;
			}
			m_pgfx.drawTextBox((L"C:> " + input + blink).c_str(), 2, textColor, { 5.0f, 170.0f , 1361.0f, 200.0f });
			blinkCounter++;
		}
	}
}

void GameConsole::activate() {
	active = !active;
}

void GameConsole::activateInput() {
	activeinput = !activeinput;
}

bool GameConsole::isActive() const {
	return active;
}

bool GameConsole::isActiveInput() const {
	return activeinput;
}

void GameConsole::setInput(const char& c) {
	tempInput = c;
	std::wstring jo{ tempInput.begin(), tempInput.end() };
	input = (input + jo);
}

void GameConsole::removeInput() {
	if (input.size()) {
		input.erase(input.end() - 1);
	}
}

std::wstring GameConsole::getInput() {
	std::wstring temp{ input };
	input = {};
	return temp;
}
