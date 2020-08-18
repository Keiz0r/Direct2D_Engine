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
        loadSprite(GAMESPRITE(SPRITE_WORLD_TILES), m_pTilesSprite);
        initialised = true;
    }
    fillBoard();
}

void GameBoard::drawBoardCells(const D2D1_POINT_2F& CameraCoord) {
    //only positive coords

    //make a translation function between coordinatespaces that returns cellspacedrawcoord
    int temp_clsdrwny = CellsDrawny;
    int temp_clsdrwnx = CellsDrawnx;

    float offsetx = 1366.0f / 2.0f;
    float offsety = 768.0f / -2.0f;
    float shiftx = BoardCell::cellwidth / 2.0f;    // over /2 because they are isometric so net tile is shifted half x half y
    float shifty = BoardCell::cellheight / 2.0f;

 //   float camToCellCoordsX = ((worldCoordinatesSize.x / 2) + CameraCoord.x);    // camera to cell coords
//    ((worldCoordinatesSize.x  / 2) + CameraCoord.x) / amountOfspaceInCellx  //determine cell¹
//    float indent = camToCellCoordsX % amountOfspaceInCellx; //remainder (insidecell)


    int CellSpaceCenterCoord = (CameraCoord.x * boardWidth) + CameraCoord.y;
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
    //function to use parser from file, which describes said level
    boardcells = std::make_unique<BoardCell[]>(boardWidth * boardHeight);
    for (float j = 0.0f; j < static_cast<float>(boardHeight); j += 1.0f) {
        for (float i = 0.0f; i < static_cast<float>(boardWidth); i += 1.0f) {
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].assignCellNum(static_cast<int>(i + (j * static_cast<float>(boardWidth))));
            boardcells[static_cast<int>(i + (j * static_cast<float>(boardWidth)))].setTileType(GameBoard::BoardCell::tiletype::Grass);
        }
    }
    boardcells[48].setTileType(GameBoard::BoardCell::tiletype::Tree1_DoubleH);
    boardcells[63].setTileType(GameBoard::BoardCell::tiletype::Water1);
    boardcells[106].setTileType(GameBoard::BoardCell::tiletype::Water2);
}

D2D1_POINT_2F GameBoard::getBoardSize() const {
    return worldCoordinatesSize;
}

void GameBoard::draw(const D2D1_POINT_2F& position) {
//    centralCellcoords = fucntionofCamera(const D2D1_POINT_2F & CameraCoord);  //just use return value later in next call
//    fromToIndexes = functionThatCalculates(centralCellcoords);
//    drawBoardCells(); proc with draw for loops where indent is take into account.. central cell in the center of the screen +- indent.
//    remember that x and y in world and screen are different!
    newDraw(position);
}

void GameBoard::loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite) {
    m_pgfx.loadD2DBitmap(name, 0, sprite);
}

D2D1_POINT_2F GameBoard::normalizePositionToTile(const D2D1_POINT_2F& position) const {
    //normalizing position to be within one tile, relative to bottom left corner
    D2D1_POINT_2F normalizedAbsPosition = { fmodf((amountOfspaceInCellx / 2.0f) + position.x, amountOfspaceInCellx), fmodf((amountOfspaceInCellx / 2.0f) + position.y, amountOfspaceInCelly) };    //equivalent of % but for float
    //reinterpreting negative values as positive (translation to another tile)
    if (normalizedAbsPosition.x < 0.0f) {
        normalizedAbsPosition.x = amountOfspaceInCellx + normalizedAbsPosition.x;
    }
    if (normalizedAbsPosition.y < 0.0f) {
        normalizedAbsPosition.y = amountOfspaceInCelly + normalizedAbsPosition.y;
    }
    return normalizedAbsPosition;
}

int GameBoard::getCentralTileIndex(const D2D1_POINT_2F& position) const{
    static D2D1_POINT_2F boardCenter = { amountOfspaceInCellx * boardWidth / 2.0f, amountOfspaceInCelly * boardHeight / 2.0f }; // coord from border (real 0  of axis)
    return (static_cast<int>((position.x + boardCenter.x) / amountOfspaceInCellx)) * boardHeight +
        static_cast<int>((position.y + boardCenter.y) / amountOfspaceInCelly);   //cast is floor towards 0;
}

D2D1_POINT_2F GameBoard::toIsometric(const D2D1_POINT_2F& cartesianVector) const {
    D2D1_POINT_2F isometric{ (cartesianVector.x + cartesianVector.y) * (1.6f) , (cartesianVector.x - cartesianVector.y) * (1.0f)};
    //                                                                  (3.2f / 1.41f)                                              (1.6f / 1.41f)
//    D2D1_POINT_2F isometric2 = 
    return isometric;
}

void GameBoard::newDraw(const D2D1_POINT_2F& position) {
    static D2D1_POINT_2F baseindent = { (1366.0f / 2.0f) - screenBasisVector.x, (768.0f / 2.0f) - screenBasisVector.y + 00.0f / 2.0f }; //centering screen  //last part is sonic model height/2 to bind center point to legs
    //                                                                                                                   30.0f / 2.0f  
    //input position is relative to world center -> tile bottom left
    D2D1_POINT_2F normalizedAbsPosition = normalizePositionToTile(position);

    //calculating indentation in world coords

    D2D1_POINT_2F indent = { (normalizedAbsPosition.x - amountOfspaceInCellx / 2.0f), normalizedAbsPosition.y - (amountOfspaceInCellx / 2.0f) };

    D2D1_POINT_2F translatedIndent = toIsometric(indent);
    D2D1_POINT_2F ScreenSpaceDrawCoords{ (baseindent.x - translatedIndent.x), (baseindent.y - translatedIndent.y) };    //rotate vector to match screen coords

    //centralTileindex sees correct. problem in indentation
//    m_pgfx.DrawLine(0.0f, baseindent.y, 1366.0f, baseindent.y, 1.0f);
//    m_pgfx.DrawLine(baseindent.x, 0.0f, baseindent.x, 768.0f, 1.0f);
//    m_pgfx.DrawLine(0.0f, baseindent.y + BoardCell::cellheight, 1366.0f, baseindent.y + BoardCell::cellheight, 1.0f);


    boardcells[getCentralTileIndex(position)].draw(m_pgfx, m_pTilesSprite, ScreenSpaceDrawCoords);
//    boardcells[getCentralTileIndex(position)].draw(m_pgfx, m_pTilesSprite, { (baseindent.x - indent.x), (baseindent.y - indent.y) });
    boardcells[getCentralTileIndex(position)].ShowCellNum(m_pgfx, ScreenSpaceDrawCoords);



    D2D1_RECT_F testrect{ -25.0f, 25.0f, 25.0f, -25.0f };
    D2D1_POINT_2F translatedtestx = toIsometric({-25.0f, 25.0f});
    D2D1_POINT_2F translatedtesty = toIsometric({ 25.0f, -25.0f });
    //origial
//    m_pgfx.DrawRect({ 1366.0f / 2.0f + testrect.left, 768.0f / 2.0f - testrect.top, 1366.0f / 2.0f + testrect.right, 768.0f / 2.0f - testrect.bottom, }, false, Graphics::D2D_SOLID_COLORS::Black);
    //translated
//    m_pgfx.rotateDrawing(45.0f, { 1366.0f / 2.0f, 768.0f / 2.0f });
//    m_pgfx.scaleDrawing( 1.0f, 0.5f, { 1366.0f / 2.0f, 768.0f / 2.0f });
    m_pgfx.transformTRSM(0.0f, 0.0f, 45.0f, { 1366.0f / 2.0f, 768.0f / 2.0f }, 3.2f / 1.41f, 1.6f / 1.41f, false);
    m_pgfx.DrawRect({ 1366.0f / 2.0f + testrect.left, 768.0f / 2.0f - testrect.top, 1366.0f / 2.0f + testrect.right, 768.0f / 2.0f - testrect.bottom, }, false, Graphics::D2D_SOLID_COLORS::Black);
    m_pgfx.restoreDefaultDrawingParameters();
 //   m_pgfx.DrawRect({ 1366.0f / 2.0f + translatedtestx.x, 768.0f / 2.0f - translatedtestx.y, 1366.0f / 2.0f + translatedtesty.x, 768.0f / 2.0f - translatedtesty.y, }, false, Graphics::D2D_SOLID_COLORS::Black);
    //crosshair
    m_pgfx.DrawLine(0.0f, 768.0f/2.0f, 1366.0f, 768.0f / 2.0f ,1.0f);
    m_pgfx.DrawLine(1366.0f/2.0f, 0.0f, 1366.0f / 2.0f, 768.0f, 1.0f);
}

void GameBoard::BoardCell::draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords) const {
    // draws cell unit on screen using screen's coordinates  From Top-left corner
    D2D1_RECT_F tileCoords;
    float doubleH = 1.0f;
    switch (tileType) {
    case GameBoard::BoardCell::tiletype::White:
        tileCoords = { 0.0f, 0.0f, 159.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Contour_Black:
        tileCoords = { 163.0f, 0.0f, 319.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::ContourI_Black:
        tileCoords = { 484.0f, 0.0f, 639.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Black:
        tileCoords = { 323.0f, 0.0f, 477.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Grass:
        tileCoords = { 0.0f, 100.0f, 159.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water1:
        tileCoords = { 160.0f, 100.0f, 318.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water2:
        tileCoords = { 319.0f, 100.0f, 480.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Tree1_DoubleH:
        tileCoords = { 0.0f, 210.0f, 159.0f, 391.0f };
        doubleH = 2.0f;
        break;
    }

    p_gfx.drawBitmap(pTilesSprite, { screencoords.x, screencoords.y, screencoords.x + cellwidth, screencoords.y + cellheight * doubleH}, 1.0f, tileCoords);
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x + cellwidth, coords.y, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y - cellheight, coords.x + cellwidth, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x + cellwidth, coords.y, coords.x + cellwidth, coords.y - cellheight, borderThickness);
}

void GameBoard::BoardCell::ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords) const {
    wchar_t name[4];
    swprintf_s(name, L"%d", cellnum);
    p_gfx.drawTextBox(name, 0, Graphics::D2D_SOLID_COLORS::OrangeRed, { screencoords.x + cellwidth / 2 - 10, screencoords.y - 85.0f, screencoords.x + 150.0f, screencoords.y - 40.0f });
}

void GameBoard::BoardCell::assignCellNum(const int& num) {
    cellnum = num;
}

void GameBoard::BoardCell::setTileType(tiletype&& type){
    tileType = type;
}