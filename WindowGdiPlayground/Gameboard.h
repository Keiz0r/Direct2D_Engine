#pragma once
#include "Graphics.h"
#include <unordered_map>
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
	void drawBoardCells();
	void fillBoard();
	bool isInside(const D2D1_POINT_2F& coord) const;
private:
	class BoardCell {
	public:
		enum tiletype {
			White, Contour_Black, Black, ContourI_Black, Grass, Water1, Water2, Tree1_DoubleH
		};
		void InitCell(const D2D1_POINT_2F& coords);
		void draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite) const;
		void SetThickness(const float& thickness);
		void ResetThickness();
		void ShowCellNum(const Graphics& p_gfx) const;
		void assignCellNum(const int& num);
		void setTileType(const tiletype& type);
		D2D1_POINT_2F getCoords() const;
		D2D1_POINT_2F getSize() const;
	private:
		D2D1_POINT_2F coords;
		float borderThickness;
		float defaultThickness = 0.5f;
		int cellnum;
		tiletype tileType;
		static constexpr float cellwidth = 40.0f;
		static constexpr float cellheight = 18.0f;
	};
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pTilesSprite;
	bool initialised = false;
	int boardWidth;
	int boardHeight;
	std::unique_ptr<BoardCell[], std::default_delete<BoardCell[]>> boardcells;
	std::unordered_map<int, Borders> bordermap;
};
