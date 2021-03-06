#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "windows.h"
#include "Direct2D.h"
#include "Wic.h"
#include "DWrite.h"
#include "Lists/AssetsList.h"
#include <type_traits>

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
};

template <typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

class Graphics {
public:
    enum class D2D_SOLID_COLORS {
        LightSlateGray, CornflowerBlue, OrangeRed, Black, Gray
    };
    Graphics(const HWND &hwnd);
    ~Graphics();
	void beginFrame();
	void endFrame();
	void drawPixel();
    void DrawLine(const float& x1, const float& y1, const  float& x2, const float& y2, const float& thickness) const;
    void DrawRect(const D2D1_RECT_F& rectangle, const bool& filled, const D2D_SOLID_COLORS& color) const;
    void drawTextBox(const wchar_t* string, const int& fontNum, const D2D_SOLID_COLORS& brushColor, const D2D1_RECT_F& startcoords) const; // TODO : ineffective method, go to directdraw docs
    void drawTextLayout(const wchar_t* string, const wchar_t* font, const float& fontsize, IDWriteTextLayout*& textLayout, const int& brushNum, const D2D1_POINT_2F& startcoords) const ;
    void drawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& outRectangle, const float& opacity, const D2D1_RECT_F& bmpOutlineRectangle) const;
    void rotateDrawing( const float& angle, const D2D1_POINT_2F& center) const;
    void scaleDrawing(const float& x, const float& y, const D2D1_POINT_2F& center);
    void restoreDefaultDrawingParameters() const;
    //mirror around 0.0 and translate to x,y, and scale
    void mirrorDrawing(const float& x, const float& y) const;
    void translateDrawing(const float& x, const float& y) const;
    void transformTRSM(const float& Translatex, const float& Translatey, const float& RotationAngle, const D2D1_POINT_2F& ImageCenter, const float& Scalex, const float& Scaley, const bool& mirrored) const;
    void transformSRTM(const float& Translatex, const float& Translatey, const float& RotationAngle, const D2D1_POINT_2F& ImageCenter, const float& Scalex, const float& Scaley, const bool& mirrored) const;
    HRESULT loadD2DBitmap(const wchar_t* filename, const int& frameNum, ID2D1Bitmap*& pOutBitmap);
    D2D1_SIZE_F getScreenSize() const;
private:
    Direct2D d2d;
    Wic Wic;
    Dwrite dwrite;
    D2D1_SIZE_F screensize;
};