#include "Gameboard.h"

GameBoard::GameBoard(Graphics& p_gfx, const int& width, const int& height)
    :
    m_pgfx(p_gfx),
    m_pTilesSprite(NULL),
    boardWidth(width),
    boardHeight(height)
{
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
    fillBoard();
}

void GameBoard::drawBoardCells() {
    for (int i = 0; i < boardWidth * boardHeight; i++) {
        boardcells[i].draw(m_pgfx, m_pTilesSprite);
    //    boardcells[i].ShowCellNum(m_pgfx);
    }
}

void GameBoard::fillBoard() {
    boardcells = std::make_unique<BoardCell[]>(boardWidth * boardHeight);
    float offsetx = 700.0f;
    float offsety = 100.0f;
    float shiftx = boardcells[0].getSize().x / 2.0f;
    float shifty = boardcells[0].getSize().y / 2.0f;
    for (float j = 0.0f; j < static_cast<float>(boardHeight); j += 1.0f) {
        offsetx += shiftx;
        offsety += shifty;
        for (float i = 0.0f; i < static_cast<float>(boardWidth); i += 1.0f) {
            float x = offsetx - (i * shiftx + 1);
            float y = offsety + (i * shifty - 1);
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].InitCell({ x, y });
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].assignCellNum(static_cast<int>(i + (j * static_cast<float>(boardWidth))));
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].setTileType(GameBoard::BoardCell::tiletype::Tree1_DoubleH);
            //    auto search = bordermap.find(y);
            //    if (search != bordermap.end()) {
            //        float lborder = bordermap[y].left;
            //        bordermap[y] = {lborder, x + 36.0f};
            //    }
            //    else {
            //        bordermap.insert({ static_cast<int>(y), {x,x + 36.0f} });
            //    }
            //}
            //float lbrd = bordermap[y].left;
            //float rbrd = bordermap[y].right;
            //int counter = 1;
            //for (int newy = static_cast<int>(y) - 1; newy > 700.0f - ((j+1) * 50.0f); newy--) {
            //    bordermap.insert({ static_cast<int>(newy), {lbrd, rbrd } });
            //    counter++;
        }
    }
    boardcells[48].SetThickness(3.0f);
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
    D2D1_RECT_F tileCoords;
    float doubleH = 1.0f;
    switch (tileType) {
    case White:
        tileCoords = { 0.0f, 1.0f, 39.0f, 24.0f };
        break;
    case Contour_Black:
        tileCoords = { 40.0f, 1.0f, 79.0f, 24.0f };
        break;
    case ContourI_Black:
        tileCoords = { 120.0f, 1.0f, 159.0f, 24.0f };
        break;
    case Black:
        tileCoords = { 80.0f, 1.0f, 119.0f, 24.0f };
        break;
    case Grass:
        tileCoords = { 0.0f, 26.0f, 39.0f, 48.0f };
        break;
    case Water1:
        tileCoords = { 40.0f, 26.0f, 79.0f, 48.0f };
        break;
    case Water2:
        tileCoords = { 80.0f, 26.0f, 119.0f, 48.0f };
        break;
    case Tree1_DoubleH:
        tileCoords = { 00.0f, 59.0f, 39.0f, 98.0f };
        doubleH = 2.0f;
        break;
    }

    p_gfx.drawBitmap(pTilesSprite, { coords.x, coords.y, coords.x + cellwidth, coords.y - cellheight * doubleH}, 1.0f, tileCoords);
//    p_gfx.DrawLine(coords.x, coords.y, coords.x + cellwidth, coords.y, borderThickness);
//    p_gfx.DrawLine(coords.x, coords.y, coords.x, coords.y - cellheight, borderThickness);
//    p_gfx.DrawLine(coords.x, coords.y - cellheight, coords.x + cellwidth, coords.y - cellheight, borderThickness);
//    p_gfx.DrawLine(coords.x + cellwidth, coords.y, coords.x + cellwidth, coords.y - cellheight, borderThickness);
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

void GameBoard::BoardCell::setTileType(const tiletype& type){
    tileType = type;
}

D2D1_POINT_2F GameBoard::BoardCell::getCoords() const {
    return coords;
}

D2D1_POINT_2F GameBoard::BoardCell::getSize() const
{
    return {cellwidth, cellheight};
}
