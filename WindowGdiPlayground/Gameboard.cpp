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

    float offsetx = m_pgfx.getScreenSize().width / 2.0f;
    float offsety = m_pgfx.getScreenSize().height / -2.0f;
    float shiftx = BoardCell::cellwidth / 2.0f;    // over /2 because they are isometric so net tile is shifted half x half y
    float shifty = BoardCell::cellheight / 2.0f;

 //   float camToCellCoordsX = ((worldCoordinatesSize.x / 2) + CameraCoord.x);    // camera to cell coords
//    ((worldCoordinatesSize.x  / 2) + CameraCoord.x) / amountOfspaceInCellx  //determine cell�
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
    int centraltile = getCentralTileIndex(position);
    boardcells[centraltile].draw(m_pgfx, m_pTilesSprite, ScreenSpaceDrawCoords);
    boardcells[centraltile].ShowCellNum(m_pgfx, ScreenSpaceDrawCoords);
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

void GameBoard::BoardCell::setTileType(tiletype&& type){
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