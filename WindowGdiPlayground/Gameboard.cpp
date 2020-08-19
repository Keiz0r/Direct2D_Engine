#include "Gameboard.h"

GameBoard::GameBoard(Graphics& p_gfx, const int& width, const int& height)
    :
    m_pgfx(p_gfx),
    m_pTilesSprite(NULL),
    boardWidth(width),
    boardHeight(height)
{
    worldCoordinatesSize = {static_cast<float>(boardWidth) * amountOfspaceInCellx, static_cast<float>(boardHeight) * amountOfspaceInCelly };

    loadSprite(GAMESPRITE(SPRITE_WORLD_TILES), m_pTilesSprite);
    fillBoard();

    drawnBoardShift = { amountOfspaceInCellx / 2.0f, amountOfspaceInCelly / 2.0f };
    if (boardWidth % 2) {
        drawnBoardShift.x = 0.0f;
    }
    if (boardHeight % 2) {
        drawnBoardShift.y = 0.0f;
    }
}

GameBoard::~GameBoard() {
    SafeRelease(&m_pTilesSprite);
}

void GameBoard::drawBoardCells(const D2D1_POINT_2F& CameraCoord) {
    //only positive coords

    //make a translation function between coordinatespaces that returns cellspacedrawcoord
   int CellsDrawny = 17;
   int CellsDrawnx = 17;

    int temp_clsdrwny = CellsDrawny;
    int temp_clsdrwnx = CellsDrawnx;

    float offsetx = m_pgfx.getScreenSize().width / 2.0f;
    float offsety = m_pgfx.getScreenSize().height / -2.0f;
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

D2D1_POINT_2F GameBoard::getWorldBorders_x() const {
    D2D1_POINT_2F borders_x{(drawnBoardShift.x - worldCoordinatesSize.x / 2.0f), worldCoordinatesSize.x / 2.0f + drawnBoardShift.x - 1.0f};
    return borders_x;
}

D2D1_POINT_2F GameBoard::getWorldBorders_y() const {
    D2D1_POINT_2F borders_y{ (drawnBoardShift.y - worldCoordinatesSize.y / 2.0f), worldCoordinatesSize.y / 2.0f + drawnBoardShift.y - 1.0f };
    return borders_y;
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

unsigned int GameBoard::getCentralTileIndex(const D2D1_POINT_2F& position) const{
    static const D2D1_POINT_2F boardCenter = { worldCoordinatesSize.x / 2.0f, worldCoordinatesSize.y / 2.0f }; // coord from border (real 0  of axis)
    return (static_cast<int>((position.x + boardCenter.x - drawnBoardShift.x) / amountOfspaceInCellx)) * boardHeight +
        static_cast<int>((position.y + boardCenter.y - drawnBoardShift.y) / amountOfspaceInCelly);   //cast is floor towards 0;
    //here I account that 20x20 or any odd number of tiles has center of board in a cross. so in order to compensate do this - amountOfspaceInCellx / 2.0f
}

D2D1_POINT_2U GameBoard::clampTileDrawingRadius(const unsigned int& centertile) const {
    //calculate drawable indexes around central
    D2D1_POINT_2U drawStartEnd { centertile + tileDrawingRadius_y - boardHeight * tileDrawingRadius_x, centertile - tileDrawingRadius_y + boardHeight * tileDrawingRadius_x };
    //start topleft end bottom right (grid)
    unsigned int availableTilesDownwards = centertile % boardHeight;
    unsigned int availableTilesUpwards = boardHeight - availableTilesDownwards - 1;
    unsigned int availableTilesLeftwards = centertile / boardHeight;
    unsigned int availableTilesRightwards = boardWidth - availableTilesLeftwards - 1;
    if (availableTilesDownwards < tileDrawingRadius_y) {
        drawStartEnd.y += tileDrawingRadius_y - availableTilesDownwards;
    }
    if (availableTilesLeftwards < tileDrawingRadius_x) {    //
        drawStartEnd.x += (tileDrawingRadius_x - availableTilesLeftwards) * boardWidth;
    }
    if (availableTilesUpwards < tileDrawingRadius_y) {  
        drawStartEnd.x -= tileDrawingRadius_y - availableTilesUpwards;
    }
    if (availableTilesRightwards < tileDrawingRadius_x) {  
        drawStartEnd.y -= (tileDrawingRadius_x - availableTilesRightwards) * boardWidth;
    }
    return drawStartEnd;
}

D2D1_POINT_2F GameBoard::toIsometric(const D2D1_POINT_2F& VectorInRegularSpace) const {
    static const float scalex = 1.6f; // /1.41f //tied to (Board::cellwidth/amountofspacex)/(2*sqrt(2))
    static const float scaley = 0.8f; // /1.41f
    D2D1_POINT_2F isometric{ (VectorInRegularSpace.x + VectorInRegularSpace.y) * scalex , (VectorInRegularSpace.x - VectorInRegularSpace.y) * scaley};
    return isometric;
}

void GameBoard::newDraw(const D2D1_POINT_2F& position) {
    D2D1_SIZE_F screensize = m_pgfx.getScreenSize();
    static const float playerLegOffset = 30.0f / 2.0f;
    static D2D1_POINT_2F baseindent = { (screensize.width / 2.0f) - screenBasisVector.x, (screensize.height / 2.0f) - screenBasisVector.y + playerLegOffset }; //centering screen
    //input position is relative to world center -> tile bottom left
    D2D1_POINT_2F normalizedAbsPosition = normalizePositionToTile(position);
    D2D1_POINT_2F indent = { (normalizedAbsPosition.x - amountOfspaceInCellx / 2.0f), normalizedAbsPosition.y - (amountOfspaceInCellx / 2.0f) };
    D2D1_POINT_2F isometricIndent = toIsometric(indent);
    D2D1_POINT_2F ScreenSpaceDrawCoords{ (baseindent.x - isometricIndent.x), (baseindent.y - isometricIndent.y) };
    unsigned int centraltile = getCentralTileIndex(position);
    //loop of drawsaround

    boardcells[centraltile].draw(m_pgfx, m_pTilesSprite, ScreenSpaceDrawCoords);
    boardcells[centraltile].ShowCellNum(m_pgfx, ScreenSpaceDrawCoords);

    D2D1_POINT_2U drawStartEnd = clampTileDrawingRadius(centraltile);

//    int shigto = (centraltile - drawStartEnd.x);
//    for (unsigned int i = drawStartEnd.x; i <= drawStartEnd.y; i++) {
//        D2D1_POINT_2F shift {-screenBasisVector.x * shigto, -screenBasisVector.y * shigto };
//        boardcells[i].draw(m_pgfx, m_pTilesSprite, { ScreenSpaceDrawCoords.x + shift.x, ScreenSpaceDrawCoords.y + shift.x });
//        boardcells[i].ShowCellNum(m_pgfx, ScreenSpaceDrawCoords);
//    }
}

void GameBoard::drawGeneratedTile() const {
    D2D1_SIZE_F screensize = m_pgfx.getScreenSize();
    static const float playerLegOffset = 30.0f / 2.0f;
    D2D1_RECT_F testrect{ -25.0f, 25.0f, 25.0f, -25.0f };
    m_pgfx.transformTRSM(playerLegOffset, playerLegOffset, 45.0f, { screensize.width / 2.0f, screensize.height / 2.0f }, 3.2f / 2.0f, 1.6f / 2.0f, false);  //Scales are tied to (Board::cellwidth/amountofspacex)/(2*sqrt(2))
    m_pgfx.DrawRect({ screensize.width / 2.0f + testrect.left, screensize.height / 2.0f - testrect.top, screensize.width / 2.0f + testrect.right,screensize.height / 2.0f - testrect.bottom, }, true, Graphics::D2D_SOLID_COLORS::Black);
    m_pgfx.restoreDefaultDrawingParameters();

}

void GameBoard::BoardCell::draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords) const {
    // draws cell unit on screen using screen's coordinates  From Top-left corner
    p_gfx.drawBitmap(pTilesSprite, { screencoords.x, screencoords.y, screencoords.x + cellwidth, screencoords.y + cellheight * doubleTextureHeight}, 1.0f, TextureCoords);
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

void GameBoard::BoardCell::setTileType(tiletype type){
    tileType = type;
    switch (tileType) {
    case GameBoard::BoardCell::tiletype::White:
        TextureCoords = { 0.0f, 0.0f, 159.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Contour_Black:
        TextureCoords = { 163.0f, 0.0f, 319.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::ContourI_Black:
        TextureCoords = { 484.0f, 0.0f, 639.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Black:
        TextureCoords = { 323.0f, 0.0f, 477.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Grass:
        TextureCoords = { 0.0f, 100.0f, 159.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water1:
        TextureCoords = { 160.0f, 100.0f, 318.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water2:
        TextureCoords = { 319.0f, 100.0f, 480.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Tree1_DoubleH:
        TextureCoords = { 0.0f, 210.0f, 159.0f, 391.0f };
        doubleTextureHeight = 2.0f;
        break;
    }
}