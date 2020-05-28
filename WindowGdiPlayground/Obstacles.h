#pragma once
#include <memory>
#include "Box.h"
#include "Log.h"

class Obstacles {
public:
	enum Type {
		MakeBox = 1, Something, Yikes
	};
	Obstacles(Graphics& p_gfx, Log& p_log);
	void draw() const;
	void addBox(D2D1_POINT_2F& position);
	void update(unsigned int& obstacleType);
private:
	Graphics& m_pgfx;
	Log& m_plog;
	std::vector<std::shared_ptr<Box>> boxes{};
};