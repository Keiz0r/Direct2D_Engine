#pragma once
#include "Backend\Graphics.h"
#include <memory>

class GameBoard {
public:
	GameBoard(Graphics& p_gfx, const int& width, const int& height);
	~GameBoard();
	void fillBoard();
	D2D1_POINT_2F getBoardSize() const;
	D2D1_POINT_2F getWorldBorders_x() const;
	D2D1_POINT_2F getWorldBorders_y() const;
	void Draw(const D2D1_POINT_2F& position);
private:
	class BoardCell {
		friend GameBoard;
	public:
		enum class tiletype {
			White, Contour_Black, Black, ContourI_Black, Grass, Water1, Water2, Tree1_DoubleH
		};
		void draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords) const;
		void ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords) const;
		void assignCellNum(const int& num);
		void setTileType(tiletype type);
	private:
		int cellnum;
		tiletype tileType;
		D2D1_RECT_F TextureCoords;
		float doubleTextureHeight = 1.0f;
		static constexpr float cellwidth = 160.0f;// / 1.41f;	//in pixels
		static constexpr float cellheight = 80.0f;// / 1.41f;
	};
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	D2D1_POINT_2F normalizePositionToTile(const D2D1_POINT_2F& position) const;
	unsigned int getCentralTileIndex(const D2D1_POINT_2F& position) const;
	D2D1_POINT_2U clampTileDrawingRadius(const unsigned int& centertile) const;
	void drawBoard(unsigned int& center, D2D1_POINT_2U& drawStartEnd, D2D1_POINT_2F& centerDrawCoords) const;
	D2D1_POINT_2F toIsometric(const D2D1_POINT_2F& VectorInRegularSpace) const;
	void drawGeneratedTile() const;	// debugging or TODO: procedural generation
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pTilesSprite;
	int boardWidth;
	int boardHeight;
	D2D1_POINT_2F worldCoordinatesSize;
	static constexpr float amountOfspaceInCellx = 50.0f;	//world coords
	static constexpr float amountOfspaceInCelly = 50.0f;
	const D2D1_POINT_2F screenBasisVector = { (BoardCell::cellwidth / 2.0f), (BoardCell::cellheight / 2.0f) };
	unsigned int tileDrawingRadius_x = 9;
	unsigned int tileDrawingRadius_y = 9;
	std::unique_ptr<BoardCell[], std::default_delete<BoardCell[]>> boardcells;
	D2D1_POINT_2F drawnBoardShift;
};
