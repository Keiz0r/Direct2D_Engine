#pragma once
#include "Graphics.h"
#include <memory>

struct Borders {
	float left;
	float right;
};

class GameBoard {
public:
	GameBoard(Graphics& p_gfx, const int& width, const int& height);
	~GameBoard();
	void Initialize();
	void drawBoardCells(const D2D1_POINT_2F& CenterCoord);
	void fillBoard();
private:
	class BoardCell {
	public:
		enum class tiletype {
			White, Contour_Black, Black, ContourI_Black, Grass, Water1, Water2, Tree1_DoubleH
		};
		void draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords) const;
		void ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords) const;
		void assignCellNum(const int& num);
		void setTileType(const tiletype& type);
		D2D1_POINT_2F getSize() const;
	private:
		int cellnum;
		tiletype tileType;
		static constexpr float cellwidth = 160.0f;
		static constexpr float cellheight = 95.0f;
	};
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pTilesSprite;
	bool initialised = false;
	int boardWidth;
	int boardHeight;
	D2D1_POINT_2F worldCoordinatesSize;
	static constexpr float amountOfspaceInCellx = 10.0f;
	static constexpr float amountOfspaceInCelly = 10.0f;
	int CellsDrawnx = 17;
	int CellsDrawny = 17;
	std::unique_ptr<BoardCell[], std::default_delete<BoardCell[]>> boardcells;
};
