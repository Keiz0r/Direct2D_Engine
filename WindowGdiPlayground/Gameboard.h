#pragma once
#include "Backend\Graphics.h"
#include <memory>

class GameBoard {
public:
	GameBoard(Graphics& p_gfx, const int& width, const int& height);
	~GameBoard();
	void Initialize();
	void drawBoardCells(const D2D1_POINT_2F& CameraCoord);
	void fillBoard();
	D2D1_POINT_2F getBoardSize() const;
	void draw(const D2D1_POINT_2F& position);
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
		void setTileType(tiletype&& type);
	private:
		int cellnum;
		tiletype tileType;
		static constexpr float cellwidth = 160.0f;// / 1.41f;	//in pixels
		static constexpr float cellheight = 80.0f;// / 1.41f;
	};
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	D2D1_POINT_2F normalizePositionToTile(const D2D1_POINT_2F& position) const;
	int getCentralTileIndex(const D2D1_POINT_2F& position) const;
	D2D1_POINT_2F toIsometric(const D2D1_POINT_2F& VectorInRegularSpace) const;
	void newDraw(const D2D1_POINT_2F& position);
	void drawGeneratedTile() const;	// debugging or TODO: procedural generation
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pTilesSprite;
	bool initialised = false;
	int boardWidth;
	int boardHeight;
	D2D1_POINT_2F worldCoordinatesSize;
	static constexpr float amountOfspaceInCellx = 50.0f;	//world coords
	static constexpr float amountOfspaceInCelly = 50.0f;
	const D2D1_POINT_2F screenBasisVector = { (BoardCell::cellwidth / 2.0f), (BoardCell::cellheight / 2.0f) };
	int CellsDrawnx = 17;
	int CellsDrawny = 17;
	std::unique_ptr<BoardCell[], std::default_delete<BoardCell[]>> boardcells;
};
