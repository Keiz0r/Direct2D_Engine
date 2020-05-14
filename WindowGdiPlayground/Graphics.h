#pragma once
#include "windows.h"
#include "Direct2D.h"
#include "Wic.h"
#include "DWrite.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
};

class Graphics {
public:
    Graphics(const HWND &hwnd);
    ~Graphics();
	void beginFrame();
	void endFrame();
	void drawPixel();
    void DrawLine(const float& x1, const float& y1, const  float& x2, const float& y2, const float& thickness) const;
    void DrawRect(const D2D1_RECT_F& rectangle, const bool& filled) const;
    void drawTextBox(const wchar_t* string, const int& fontNum, const int& brushNum, const D2D1_RECT_F& startcoords) const; // TODO : ineffective method, go to directdraw docs
    void drawTextLayout(const wchar_t* string, const wchar_t* font, const float& fontsize, IDWriteTextLayout*& textLayout, const int& brushNum, const D2D1_POINT_2F& startcoords) const ;
    void drawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& outRectangle, const float& opacity, const D2D1_RECT_F& bmpOutlineRectangle) const;
    void rotateDrawing( const float& angle, const D2D1_POINT_2F& center) const;
    void restoreDefaultDrawingParameters() const;
    //mirror around 0.0 and translate to x,y
    void mirrorDrawing(const float& x, const float& y) const;
    void translateDrawing(const float& x, const float& y) const;
    HRESULT loadD2DBitmap(const wchar_t* filename, const int& frameNum, ID2D1Bitmap*& pOutBitmap);
private:
    Direct2D d2d;
    Wic Wic;
    Dwrite dwrite;
    D2D1::Matrix3x2F mirrorMatrix{ -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
};