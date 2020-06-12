#pragma once
#include "Graphics.h"
#include <unordered_map>

struct Borders {
	float left;
	float right;
};

class GameBoard {
public:
	GameBoard(Graphics& p_gfx);
	~GameBoard();
	void Initialize();
	void drawBoardCells();
	bool isInside(const D2D1_POINT_2F& coord) const;
private:
	class BoardCell {
	public:
		void InitCell(const D2D1_POINT_2F& coords);
		void draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite) const;
		void SetThickness(const float& thickness);
		void ResetThickness();
		void ShowCellNum(const Graphics& p_gfx) const;
		void assignCellNum(const int& num);
		D2D1_POINT_2F getCoords() const;
	private:
		D2D1_POINT_2F coords;
		float borderThickness;
		float defaultThickness = 0.5f;
		int cellnum;
		const float cellwidth = 50.0f;
		const float cellheight = 50.0f;
	};
private:
	void loadSprite(const wchar_t* name, ID2D1Bitmap*& sprite);
private:
	Graphics& m_pgfx;
	ID2D1Bitmap* m_pTilesSprite;
	bool initialised = false;
	static constexpr int boardrows = 16;
	static constexpr int boardcolumns = 9;
	BoardCell boardcells[boardrows * boardcolumns];
	std::unordered_map<int, Borders> bordermap;
};
