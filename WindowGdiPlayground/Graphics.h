#pragma once
#include "windows.h"
#include "Direct2D.h"
#include "Wic.h"
#include "DWrite.h"

#define GAMESPRITE(string) L## "..\\Assets\\Sprites\\" #string

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
};

class Graphics {
public:
    enum D2D_SOLID_COLORS {
        LightSlateGray, CornflowerBlue, OrangeRed, Black, Gray
    };
    Graphics(const HWND &hwnd);
    ~Graphics();
	void beginFrame();
	void endFrame();
	void drawPixel();
    void DrawLine(const float& x1, const float& y1, const  float& x2, const float& y2, const float& thickness) const;
    void DrawRect(const D2D1_RECT_F& rectangle, const bool& filled, const int& color) const;
    void drawTextBox(const wchar_t* string, const int& fontNum, const int& brushNum, const D2D1_RECT_F& startcoords) const; // TODO : ineffective method, go to directdraw docs
    void drawTextLayout(const wchar_t* string, const wchar_t* font, const float& fontsize, IDWriteTextLayout*& textLayout, const int& brushNum, const D2D1_POINT_2F& startcoords) const ;
    void drawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& outRectangle, const float& opacity, const D2D1_RECT_F& bmpOutlineRectangle) const;
    void rotateDrawing( const float& angle, const D2D1_POINT_2F& center) const;
    void scaleDrawing(const float& x, const float& y, const D2D1_POINT_2F& center);
    void restoreDefaultDrawingParameters() const;
    //mirror around 0.0 and translate to x,y, and scale
    void mirrorDrawing(const float& x, const float& y) const;
    void translateDrawing(const float& x, const float& y) const;
    void transformTRSM(const float& Translatex, const float& Translatey, const float& RotationAngle, const D2D1_POINT_2F& ImageCenter, const float& Scalex, const float& Scaley, const bool& mirrored) const;
    HRESULT loadD2DBitmap(const wchar_t* filename, const int& frameNum, ID2D1Bitmap*& pOutBitmap);
private:
    Direct2D d2d;
    Wic Wic;
    Dwrite dwrite;
};