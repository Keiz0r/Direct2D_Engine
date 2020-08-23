#include "Gameboard.h"

GameBoard::GameBoard(const int& width, const int& height)
{
    boardWidth = width;
    boardHeight = height;
    worldCoordinatesSize = {static_cast<float>(boardWidth) * amountOfspaceInCellx, static_cast<float>(boardHeight) * amountOfspaceInCelly };
    
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
    delete[] brdcells;
}

void GameBoard::fillBoard() {
    //function to use parser from file, which describes said level
    brdcells = new BoardCell[boardWidth * boardHeight];//reinterpret_cast<BoardCell*>(_alloca(sizeof(BoardCell) * boardWidth * boardHeight));
    for (float j = 0.0f; j < static_cast<float>(boardHeight); j += 1.0f) {
        for (float i = 0.0f; i < static_cast<float>(boardWidth); i += 1.0f) {
            unsigned int index = static_cast<int>(i + (j * static_cast<float>(boardWidth)));
            GameBoard::BoardCell::assignCellNum(static_cast<int>(i + (j * static_cast<float>(boardWidth))), &brdcells[index]);
            GameBoard::BoardCell::setTileType(GameBoard::BoardCell::tiletype::Grass, &brdcells[index]);
        }
    }
    //randomize cells
    std::random_device seed;
    std::mt19937 rng(seed());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(to_underlying(GameBoard::BoardCell::tiletype::White), to_underlying(GameBoard::BoardCell::tiletype::Count) - 1);
    for (unsigned int i = 0; i < boardWidth * boardHeight; i++) {
        GameBoard::BoardCell::setTileType(static_cast<GameBoard::BoardCell::tiletype>(distribution(rng)), &brdcells[i]);
    }
}

D2D1_POINT_2F GameBoard::getBoardSize() const {
    return worldCoordinatesSize;
}

D2D1_POINT_2F GameBoard::getWorldBorders_x() const {
    static D2D1_POINT_2F borders_x{(drawnBoardShift.x - worldCoordinatesSize.x / 2.0f), worldCoordinatesSize.x / 2.0f + drawnBoardShift.x - 1.0f};
    return borders_x;
}

D2D1_POINT_2F GameBoard::getWorldBorders_y() const {
    static D2D1_POINT_2F borders_y{ (drawnBoardShift.y - worldCoordinatesSize.y / 2.0f), worldCoordinatesSize.y / 2.0f + drawnBoardShift.y - 1.0f };
    return borders_y;
}

void GameBoard::initialize(const wchar_t* name, Graphics* p_gfx, const D2D1_POINT_2F& isometricCoeffs) {
    if (m_pgfx == nullptr) {
        m_pgfx = p_gfx;
        m_pgfx->loadD2DBitmap(name, 0, m_pTilesSprite);
        isometricCoefficients = isometricCoeffs;
    }
    else {
        Log::putError(L"WARNING: GameBoard reinitialization attempt");
    }
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

unsigned int GameBoard::getCentralTileIndex(const D2D1_POINT_2F& position) const {
    static D2D1_POINT_2F boardCenter = { worldCoordinatesSize.x / 2.0f, worldCoordinatesSize.y / 2.0f }; // coord from border (real 0  of axis)
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

void GameBoard::drawBoard(const unsigned int& center, const D2D1_POINT_2U& drawStartEnd, const D2D1_POINT_2F& centerDrawCoords) const {
    if (m_pTilesSprite != NULL) {
        static D2D1_POINT_2F screenBasisVector{ (BoardCell::cellwidth / 2.0f), (BoardCell::cellheight / 2.0f) };
        unsigned short int amountOfShifts_x = center / boardHeight - drawStartEnd.x / boardHeight;
        unsigned short int amountOfShifts_y = drawStartEnd.x % boardHeight - center % boardHeight;
        D2D1_POINT_2F shiftedCoords{ centerDrawCoords.x + screenBasisVector.x * (amountOfShifts_y - amountOfShifts_x),
            centerDrawCoords.y - screenBasisVector.y * (amountOfShifts_x + amountOfShifts_y) };
        //drawing row top to bottom from leftmost
        int columnDrawingIndex = drawStartEnd.x;
        int columnHeight = drawStartEnd.x % boardHeight - drawStartEnd.y % boardHeight;
        int i = 0;
        int rowsToDraw = drawStartEnd.y / boardHeight - drawStartEnd.x / boardHeight;
        while (i <= rowsToDraw) {
            int counter = 0;
            while (counter <= columnHeight) {
                GameBoard::BoardCell::draw(*m_pgfx, m_pTilesSprite, { shiftedCoords.x + screenBasisVector.x * (i - counter),
                    shiftedCoords.y + screenBasisVector.y * (counter + i) }, &brdcells[columnDrawingIndex]);
                //    GameBoard::BoardCell::ShowCellNum(*m_pgfx, { shiftedCoords.x + screenBasisVector.x * (i - counter), 
                //        shiftedCoords.y + screenBasisVector.y * (counter + i) }, &brdcells[columnDrawingIndex]);
                columnDrawingIndex--;
                counter++;
            }
            i++;
            columnDrawingIndex = drawStartEnd.x + boardHeight * i;
        }
    }
}

D2D1_POINT_2F GameBoard::toIsometric(const D2D1_POINT_2F& VectorInRegularSpace) const {
    D2D1_POINT_2F isometric{(VectorInRegularSpace.x + VectorInRegularSpace.y) * isometricCoefficients.x , (VectorInRegularSpace.x - VectorInRegularSpace.y) * isometricCoefficients.y};
    return isometric;
}

void GameBoard::Draw(const D2D1_POINT_2F& position) {
    D2D1_SIZE_F screensize = m_pgfx->getScreenSize();
//    static const float playerLegOffset = 00.0f / 2.0f;
    static D2D1_POINT_2F baseindent = { (screensize.width / 2.0f) - (BoardCell::cellwidth / 2.0f), (screensize.height / 2.0f) - (BoardCell::cellheight / 2.0f) };// + playerLegOffset}; //centering screen
    //input position is relative to world center -> tile bottom left
    D2D1_POINT_2F normalizedAbsPosition = normalizePositionToTile(position);
    D2D1_POINT_2F indent = { (normalizedAbsPosition.x - amountOfspaceInCellx / 2.0f), normalizedAbsPosition.y - (amountOfspaceInCellx / 2.0f) };
    D2D1_POINT_2F isometricIndent = toIsometric(indent);
    D2D1_POINT_2F ScreenSpaceDrawCoords{ (baseindent.x - isometricIndent.x), (baseindent.y - isometricIndent.y) };
    unsigned int centraltile = getCentralTileIndex(position);
    
    D2D1_POINT_2U drawStartEnd = clampTileDrawingRadius(centraltile);
    drawBoard(centraltile, drawStartEnd, ScreenSpaceDrawCoords);
}

void GameBoard::drawGeneratedTile() const {
    D2D1_SIZE_F screensize = m_pgfx->getScreenSize();
    static float playerLegOffset = 30.0f / 2.0f;
    D2D1_RECT_F testrect{ -25.0f, 25.0f, 25.0f, -25.0f };
    m_pgfx->transformTRSM(playerLegOffset, playerLegOffset, 45.0f, { screensize.width / 2.0f, screensize.height / 2.0f }, 3.2f / 2.0f, 1.6f / 2.0f, false);  //Scales are tied to (Board::cellwidth/amountofspacex)/(2*sqrt(2))
    m_pgfx->DrawRect({ screensize.width / 2.0f + testrect.left, screensize.height / 2.0f - testrect.top, screensize.width / 2.0f + testrect.right,screensize.height / 2.0f - testrect.bottom, }, true, Graphics::D2D_SOLID_COLORS::Black);
    m_pgfx->restoreDefaultDrawingParameters();

}

void GameBoard::BoardCell::draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords, BoardCell* cell) {
    // draws cell unit on screen using screen's coordinates  From Top-left corner
    if (cell->doubleTextureHeight > 1.1f) {
        p_gfx.drawBitmap(pTilesSprite, { screencoords.x, screencoords.y - cellheight, screencoords.x + cellwidth, screencoords.y + cellheight }, 1.0f, cell->TextureCoords);
    }
    else {
        p_gfx.drawBitmap(pTilesSprite, { screencoords.x, screencoords.y, screencoords.x + cellwidth, screencoords.y + cellheight }, 1.0f, cell->TextureCoords);
    }
    
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x + cellwidth, coords.y, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y, coords.x, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x, coords.y - cellheight, coords.x + cellwidth, coords.y - cellheight, borderThickness);
 //   p_gfx.DrawLine(coords.x + cellwidth, coords.y, coords.x + cellwidth, coords.y - cellheight, borderThickness);
}

void GameBoard::BoardCell::ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords, BoardCell* cell) {
    std::wstring name = std::to_wstring(cell->cellnum);
    p_gfx.drawTextBox(name.c_str(), 0, Graphics::D2D_SOLID_COLORS::OrangeRed, { screencoords.x + cellwidth / 2 - 10, screencoords.y, screencoords.x + 150.0f, screencoords.y + 40.0f });
}

void GameBoard::BoardCell::assignCellNum(const int& num, BoardCell* cell) {
    cell->cellnum = num;
}

void GameBoard::BoardCell::setTileType(tiletype type, BoardCell* cell){
    cell->tileType = type;
    switch (type) {
    case GameBoard::BoardCell::tiletype::White:
        cell->TextureCoords = { 0.0f, 0.0f, 159.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Contour_Black:
        cell->TextureCoords = { 163.0f, 0.0f, 319.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::ContourI_Black:
        cell->TextureCoords = { 484.0f, 0.0f, 639.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Black:
        cell->TextureCoords = { 323.0f, 0.0f, 477.0f, 90.0f };
        break;
    case GameBoard::BoardCell::tiletype::Grass:
        cell->TextureCoords = { 0.0f, 100.0f, 159.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water1:
        cell->TextureCoords = { 160.0f, 100.0f, 318.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Water2:
        cell->TextureCoords = { 319.0f, 100.0f, 480.0f, 190.0f };
        break;
    case GameBoard::BoardCell::tiletype::Tree1_DoubleH:
        cell->TextureCoords = { 0.0f, 210.0f, 159.0f, 391.0f };
        cell->doubleTextureHeight = 2.0f;
        break;
    }
}