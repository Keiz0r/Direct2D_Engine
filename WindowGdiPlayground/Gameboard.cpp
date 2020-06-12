#include "Gameboard.h"

GameBoard::GameBoard(Graphics& p_gfx)
    :
    m_pgfx(p_gfx),
    m_pTilesSprite(NULL)
{
    for (float j = 0.0f; j < float(boardrows); j += 1.0f) {
        float y = 700.0f - (j * 50.0f );
        for (float i = 0.0f; i < float(boardcolumns); i += 1.0f) {
            float x = 200.0f + (j * 15 ) + (i * 36.0f );
            boardcells[int(i + (j * float(boardcolumns)))].InitCell({ x, y });
            boardcells[int(i + (j * float(boardcolumns)))].assignCellNum(int(i + (j * float(boardcolumns))));

            auto search = bordermap.find(y);
            if (search != bordermap.end()) {
                float lborder = bordermap[y].left;
                bordermap[y] = {lborder, x + 36.0f};
            }
            else {
                bordermap.insert({ static_cast<int>(y), {x,x + 36.0f} });
            }
        }
        float lbrd = bordermap[y].left;
        float rbrd = bordermap[y].right;
        int counter = 1;
        for (int newy = static_cast<int>(y) - 1; newy > 700.0f - ((j+1) * 50.0f); newy--) {
            bordermap.insert({ static_cast<int>(newy), {lbrd, rbrd } });
            counter++;
        }
    }
    boardcells[48].SetThickness(3.0f);
    Initialize();
}

GameBoard::~GameBoard() {
    SafeRelease(&m_pTilesSprite);
}

void GameBoard::Initialize() {
    if (!initialised) {
        loadSprite(GAMESPRITE(GroundSprites.png), m_pTilesSprite);
        initialised = true;
    }
}

void GameBoard::drawBoardCells() {
    for (int i = 0; i < boardrows * boardcolumns; i++) {
        boardcells[i].draw(m_pgfx, m_pTilesSprite);
        boardcells[i].ShowCellNum(m_pgfx);
    }
}

bool GameBoard::isInside(const D2D1_POINT_2F& coord) const {
    auto search = bordermap.find(coord.y);
    if (search != bordermap.end()) {
        if (coord.x >= search->second.left && coord.x <= search->second.right) {
            return true;
        }
    }
    return false;
}

void GameBoard::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
    m_pgfx.loadD2DBitmap(name, 0, sprite);
}

void GameBoard::BoardCell::InitCell(const D2D1_POINT_2F& coordinates) {
    coords = coordinates;
    borderThickness = defaultThickness;
}

void GameBoard::BoardCell::draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite) const {
    //p_gfx.DrawRect({coords.x, coords.y, coords.x + cellwidth, coords.y - cellheight }, true);
    p_gfx.drawBitmap(pTilesSprite, { coords.x, coords.y, coords.x + cellwidth, coords.y - cellheight }, 1.0f, { 8.0f, 28.0f, 51.0f, 59.0f });
    p_gfx.DrawLine(coords.x, coords.y, coords.x + cellwidth, coords.y, borderThickness);
    p_gfx.DrawLine(coords.x, coords.y, coords.x, coords.y - cellheight, borderThickness);
    p_gfx.DrawLine(coords.x, coords.y - cellheight, coords.x + cellwidth, coords.y - cellheight, borderThickness);
    p_gfx.DrawLine(coords.x + cellwidth, coords.y, coords.x + cellwidth, coords.y - cellheight, borderThickness);
}

void GameBoard::BoardCell::SetThickness(const float& thickness) {
    borderThickness = thickness;
}

void GameBoard::BoardCell::ResetThickness() {
    borderThickness = defaultThickness;
}

void GameBoard::BoardCell::ShowCellNum(const Graphics& p_gfx) const {
    wchar_t name[4];
    swprintf_s(name, L"%d", cellnum);
    p_gfx.drawTextBox(name, 0, 2, { coords.x + 25.0f ,coords.y - 45.0f, coords.x + 50.0f ,coords.y });
}

void GameBoard::BoardCell::assignCellNum(const int& num) {
    cellnum = num;
}

D2D1_POINT_2F GameBoard::BoardCell::getCoords() const {
    return coords;
}
