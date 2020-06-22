#include "Gameboard.h"

GameBoard::GameBoard(Graphics& p_gfx, const int& width, const int& height)
    :
    m_pgfx(p_gfx),
    m_pTilesSprite(NULL),
    boardWidth(width),
    boardHeight(height)
{
    worldCoordinatesSize = {static_cast<float>(boardWidth) * amountOfspaceInCellx, static_cast<float>(boardHeight) * amountOfspaceInCelly };
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

void GameBoard::drawBoardCells(const D2D1_POINT_2F& CenterCoord) {
    //only positive coords

    //make a translation function between coordinatespaces that returns cellspacedrawcoord
    int temp_clsdrwny = CellsDrawny;
    int temp_clsdrwnx = CellsDrawnx;

    float offsetx = 450.0f;
    float offsety = -150.0f;
    float shiftx = boardcells[0].getSize().x / 2.0f;
    float shifty = boardcells[0].getSize().y / 2.0f;

    int CellSpaceCenterCoord = ((CenterCoord.x / amountOfspaceInCellx) * boardWidth) + CenterCoord.y / amountOfspaceInCelly;
    int CellSpaceDrawCoord = CellSpaceCenterCoord - (CellsDrawny / 2) - ((CellsDrawnx / 2) * boardHeight);

    if (CellSpaceDrawCoord < 0) {
        //  Clipping x < 0
        int cancelledYLines = ((-1 * CellSpaceDrawCoord) / boardHeight) + 1;
        CellsDrawnx -= cancelledYLines;  //  no draw "unexisting" columns
        CellSpaceDrawCoord += cancelledYLines * boardHeight;
        //  shifting to move map edges to center
        offsetx += shiftx * cancelledYLines;
        offsety += shifty * cancelledYLines;
    }

    int cancelledXLines = temp_clsdrwnx / 2 - (CellSpaceCenterCoord % boardHeight);
    if (cancelledXLines > 0) {
        //clipping y < 0
        //don't draw that x line with y coords
        CellsDrawny -= cancelledXLines;  //  no draw "unexisting" rows
        CellSpaceDrawCoord += cancelledXLines;
        //  shifting to move map edges to center
        offsetx -= shiftx * cancelledXLines;
        offsety += shifty * cancelledXLines;
    }

    //  TODO: transition gotta be SMOOTH, so not in entire cell. draw a bit more cells in each direction and adjust with offsets. this way can draw part of cell

    int lastDrawStartColumn = (boardWidth - CellsDrawnx ) * boardHeight;
    if (CellSpaceDrawCoord >= (lastDrawStartColumn + boardHeight)) {
        //  Clipping x > brdW
        CellsDrawnx -= (CellSpaceDrawCoord - lastDrawStartColumn) / boardHeight;
    }

    int lastDrawStartRow = CellSpaceDrawCoord % boardHeight;
    if (lastDrawStartRow > boardHeight - CellsDrawny) {
        //  Clipping y > brdH
        CellsDrawny = boardHeight - lastDrawStartRow;
    }

    // drawing from top left (0,0) in cellspace
    for (int j = 0; j < CellsDrawnx; j++) {
        for (int i = 0; i < CellsDrawny; i++) {
            float x = offsetx - (i * shiftx + 0);
            float y = offsety + (i * shifty - 0);
            D2D1_POINT_2F ScreenSpaceDrawCoords = { x,y };
            
            boardcells[CellSpaceDrawCoord].draw(m_pgfx, m_pTilesSprite, ScreenSpaceDrawCoords);
            boardcells[CellSpaceDrawCoord].ShowCellNum(m_pgfx, ScreenSpaceDrawCoords);
            CellSpaceDrawCoord++;
        }
        CellSpaceDrawCoord += boardHeight - CellsDrawny;

        offsetx += shiftx;
        offsety += shifty;
    }

    //restore initial params
    CellsDrawny = temp_clsdrwny;
    CellsDrawnx = temp_clsdrwnx;
}

void GameBoard::fillBoard() {
    boardcells = std::make_unique<BoardCell[]>(boardWidth * boardHeight);
    for (float j = 0.0f; j < static_cast<float>(boardHeight); j += 1.0f) {
        for (float i = 0.0f; i < static_cast<float>(boardWidth); i += 1.0f) {
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].assignCellNum(static_cast<int>(i + (j * static_cast<float>(boardWidth))));
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].setTileType(GameBoard::BoardCell::tiletype::Grass);
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
    boardcells[48].setTileType(GameBoard::BoardCell::tiletype::Tree1_DoubleH);
    boardcells[63].setTileType(GameBoard::BoardCell::tiletype::Water1);
    boardcells[106].setTileType(GameBoard::BoardCell::tiletype::Water2);
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

void GameBoard::BoardCell::draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords) const {
    D2D1_RECT_F tileCoords;
    float doubleH = 1.0f;
    switch (tileType) {
    case White:
        tileCoords = { 0.0f, 0.0f, 159.0f, 90.0f };
        break;
    case Contour_Black:
        tileCoords = { 163.0f, 0.0f, 319.0f, 90.0f };
        break;
    case ContourI_Black:
        tileCoords = { 484.0f, 0.0f, 639.0f, 90.0f };
        break;
    case Black:
        tileCoords = { 323.0f, 0.0f, 477.0f, 90.0f };
        break;
    case Grass:
        tileCoords = { 0.0f, 100.0f, 159.0f, 190.0f };
        break;
    case Water1:
        tileCoords = { 160.0f, 100.0f, 318.0f, 190.0f };
        break;
    case Water2:
        tileCoords = { 319.0f, 100.0f, 480.0f, 190.0f };
        break;
    case Tree1_DoubleH:
        tileCoords = { 0.0f, 210.0f, 159.0f, 391.0f };
        doubleH = 2.0f;
        break;
    }

    p_gfx.drawBitmap(pTilesSprite, { screencoords.x, screencoords.y, screencoords.x + cellwidth, screencoords.y - cellheight * doubleH}, 1.0f, tileCoords);
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x + cellwidth, coords.y, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y - cellheight, coords.x + cellwidth, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x + cellwidth, coords.y, coords.x + cellwidth, coords.y - cellheight, borderThickness);
}

void GameBoard::BoardCell::ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords) const {
    wchar_t name[4];
    swprintf_s(name, L"%d", cellnum);
    p_gfx.drawTextBox(name, 0, 2, { screencoords.x + cellwidth / 2 - 10, screencoords.y - 85.0f, screencoords.x + 150.0f, screencoords.y - 40.0f });
}

void GameBoard::BoardCell::assignCellNum(const int& num) {
    cellnum = num;
}

void GameBoard::BoardCell::setTileType(const tiletype&& type){
    tileType = type;
}

D2D1_POINT_2F GameBoard::BoardCell::getSize() const
{
    return {cellwidth, cellheight};
}
