#include "GameConsole.h"

GameConsole::GameConsole(Graphics& p_gfx, Log& p_log)
:
	m_pgfx(p_gfx),
	m_plog(p_log)
{
}

void GameConsole::draw() const {
	if (active) {
		m_pgfx.DrawRect({ 200.0f, 0.0f, 1166.0f, 200.0f }, true);
		std::deque<std::wstring> out = m_plog.commands;
		for (int i = 0; i < 7 && i < out.size(); i++) {
			float msgOffset = static_cast<float>(i) * 24.0f;
			m_pgfx.drawTextBox(out.at(i).c_str(), 2, 2, { 200.0f, 152.0f - msgOffset, 1166.0f, 176.0f - msgOffset });

		}
	}
}

void GameConsole::activate() {
	active = !active;
}
