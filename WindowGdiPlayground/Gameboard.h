#pragma once
#include "Backend\Graphics.h"
#include "Log.h"
#include <memory>
#include <random>

class GameBoard {
public:
	GameBoard(const uint32_t& width, const uint32_t& height);
	~GameBoard();
	void fillBoard();
	D2D1_POINT_2F getBoardSize() const;
	D2D1_POINT_2F getWorldBorders_x() const;
	D2D1_POINT_2F getWorldBorders_y() const;
	void Draw(const D2D1_POINT_2F& position);
	static void initialize(const wchar_t* name, Graphics* p_gfx, const D2D1_POINT_2F& isometricCoeffs);
private:
	class BoardCell {
		friend GameBoard;
	public:
		enum class tiletype {
			White, Contour_Black, Black, ContourI_Black, Grass, Water1, Water2, Count
		};
		static void draw(const Graphics& p_gfx, ID2D1Bitmap* pTilesSprite, const D2D1_POINT_2F& screencoords, BoardCell* cell);
		static void ShowCellNum(const Graphics& p_gfx, const D2D1_POINT_2F& screencoords, BoardCell* cell);
		static void assignCellNum(const uint32_t& num, BoardCell* cell);
		static void setTileType(tiletype type, BoardCell* cell);
	private:
		uint32_t cellnum = 0;
		tiletype tileType = tiletype::White;
		D2D1_RECT_F TextureCoords {0.0f, 0.0f, 0.0f, 0.0f};
		//float_t doubleTextureHeight = 1.0f;
		static constexpr float cellwidth = 160.0f;// / 1.41f;		//in pixels
		static constexpr float cellheight = 80.0f;// / 1.41f;
	};
private:
	D2D1_POINT_2F normalizePositionToTile(const D2D1_POINT_2F& position) const;
	uint32_t getCentralTileIndex(const D2D1_POINT_2F& position) const;
	D2D1_POINT_2U clampTileDrawingRadius(const uint32_t& centertile) const;
	void drawBoard(const uint32_t& center, const D2D1_POINT_2U& drawStartEnd, const D2D1_POINT_2F& centerDrawCoords) const;
	D2D1_POINT_2F toIsometric(const D2D1_POINT_2F& VectorInRegularSpace) const;
	void drawGeneratedTile() const;	// debugging or TODO: procedural generation
	inline static Graphics* m_pgfx = nullptr;
	inline static ID2D1Bitmap* m_pTilesSprite = nullptr;
	inline static uint32_t boardWidth = 0;
	inline static uint32_t boardHeight = 0;
	inline static D2D1_POINT_2F worldCoordinatesSize { 0.0f, 0.0f };
	static constexpr float_t amountOfspaceInCellx = 50.0f;	//world coords
	static constexpr float_t amountOfspaceInCelly = 50.0f;
	inline static uint8_t tileDrawingRadius_x = 9;
	inline static uint8_t tileDrawingRadius_y = 9;
	std::unique_ptr<BoardCell[]> brdcells;
	inline static D2D1_POINT_2F drawnBoardShift { 0.0f, 0.0f };
	inline static D2D1_POINT_2F isometricCoefficients{ 0.0f, 0.0f };
};