#include "Obstacles.h"

Obstacles::Obstacles(Graphics& p_gfx)
    :
    m_pgfx(p_gfx)
{
}

void Obstacles::draw() const {
    std::vector<std::shared_ptr<Box>>::const_iterator endBoxesLoop = boxes.end();
    for (std::vector<std::shared_ptr<Box>>::const_iterator i = boxes.begin(); i < endBoxesLoop; i++) {
        (*i)->draw();
    }
}

void Obstacles::addBox(D2D1_POINT_2F& position) {
    std::shared_ptr<Box> box = std::make_shared<Box>(m_pgfx, position);
    boxes.push_back(box);
    Log::putMessage(L"Писос");
}

void Obstacles::update(unsigned int& obstacleType) {
    if (obstacleType == MakeBox) {
        D2D1_POINT_2F position{ 400.0f, 600.0f };
        addBox(position);
    }
    obstacleType = 0;
}
