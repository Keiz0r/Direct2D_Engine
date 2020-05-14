#pragma once
#include "Graphics.h"

class GameBoard {
public:
	GameBoard(const Graphics& p_gfx);
	void drawBoardCells();
private:
	class BoardCell {
	public:
		void InitCell(const D2D1_POINT_2F& coords);
		void draw(const Graphics& p_gfx) const;
		void SetThickness(const float& thickness);
		void ResetThickness();
		void ShowCellNum(const Graphics& p_gfx) const;
		void assignCellNum(const int& num);
	private:
		D2D1_POINT_2F coords;
		float borderThickness;
		float defaultThickness = 0.5f;
		int cellnum;
	};
private:
	const Graphics& m_pgfx;
	BoardCell boardcells[16 * 19];
	int boardrows = 16;
	int boardcolumns = 19;
	IDWriteTextFormat* m_pTextFormat;
};
