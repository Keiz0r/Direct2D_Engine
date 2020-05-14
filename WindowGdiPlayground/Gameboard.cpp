#include "Gameboard.h"

GameBoard::GameBoard(const Graphics& p_gfx)
    :
    m_pgfx(p_gfx)
{
    for (float j = 0.0f; j < float(boardrows); j += 1.0f) {
        for (float i = 0.0f; i < float(boardcolumns); i += 1.0f) {
            boardcells[int(i + (j * float(boardcolumns)))].InitCell({ (j * 25) + (i * 50.0f), 660.0f - (j * 40.0f) });
            boardcells[int(i + (j * float(boardcolumns)))].assignCellNum(int(i + (j * float(boardcolumns))));
        }
    }
    boardcells[19].SetThickness(3.0f);
}

void GameBoard::drawBoardCells() {
    for (int i = 0; i < boardrows * boardcolumns; i++) {
        boardcells[i].draw(m_pgfx);
        boardcells[i].ShowCellNum(m_pgfx);
    }
}

void GameBoard::BoardCell::InitCell(const D2D1_POINT_2F& coordinates) {
    coords = coordinates;
    borderThickness = defaultThickness;
}

void GameBoard::BoardCell::draw(const Graphics& p_gfx) const {
    p_gfx.DrawLine(coords.x, coords.y, coords.x + 50, coords.y, borderThickness);
    p_gfx.DrawLine(coords.x + 25, coords.y - 40, coords.x + 75, coords.y - 40, borderThickness);
    p_gfx.DrawLine(coords.x, coords.y, coords.x + 25, coords.y - 40, borderThickness);
    p_gfx.DrawLine(coords.x + 50, coords.y, coords.x + 75, coords.y - 40, borderThickness);
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
    p_gfx.drawTextBox(name, 0, 1, { coords.x + 25.0f ,coords.y - 45.0f, coords.x + 50.0f ,coords.y });
}

void GameBoard::BoardCell::assignCellNum(const int& num) {
    cellnum = num;
}
