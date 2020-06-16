#include "Graphics.h"

Graphics::Graphics(const HWND& hwnd)
    :
    d2d(hwnd)
{
}

Graphics::~Graphics() {
}

void Graphics::beginFrame(){
    d2d.getRenderTarget()->BeginDraw();
    d2d.getRenderTarget()->Clear(NULL);
}

void Graphics::endFrame(){
    HRESULT hr = d2d.getRenderTarget()->EndDraw();
}

void Graphics::drawPixel(){
}

void Graphics::DrawLine(const float &x1, const float &y1, const  float &x2, const float &y2, const float &thickness) const {
    d2d.getRenderTarget()->DrawLine(
        D2D1::Point2F(static_cast<FLOAT>(x1), static_cast<FLOAT>(y1)),
        D2D1::Point2F(static_cast<FLOAT>(x2), static_cast<FLOAT>(y2)),
        d2d.getSolidColorBrush(2),
        thickness
    );
}

void Graphics::DrawRect(const D2D1_RECT_F &rectangle, const bool &filled, const int& color) const {
    if (filled) {
        d2d.getRenderTarget()->FillRectangle(rectangle, d2d.getSolidColorBrush(color));
    }
    else {
        d2d.getRenderTarget()->DrawRectangle(rectangle, d2d.getSolidColorBrush(color));
    }
}

void Graphics::drawTextBox(const wchar_t* string, const int& fontNum, const int& brushNum, const D2D1_RECT_F &rectCoords) const {
    d2d.getRenderTarget()->DrawText(string, wcslen(string), dwrite.getTextFormat(fontNum), rectCoords, d2d.getSolidColorBrush(brushNum), D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
}

void Graphics::drawTextLayout(const wchar_t* string, const wchar_t* font, const float& fontsize, IDWriteTextLayout*& textLayout, const int& brushNum, const D2D1_POINT_2F& startcoords) const {
    d2d.getRenderTarget()->DrawTextLayout(startcoords, textLayout, d2d.getSolidColorBrush(brushNum), D2D1_DRAW_TEXT_OPTIONS_CLIP);
}

void Graphics::drawBitmap(ID2D1Bitmap* bitmap, const D2D1_RECT_F& outRectangle, const float& opacity, const D2D1_RECT_F& bmpOutlineRectangle) const {
    d2d.getRenderTarget()->DrawBitmap(bitmap, {outRectangle.left, outRectangle.top, outRectangle.right, outRectangle.bottom}, opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, bmpOutlineRectangle);    
}

void Graphics::rotateDrawing(const float& angle, const D2D1_POINT_2F& center) const {
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Rotation(angle, center));
}

void Graphics::scaleDrawing(const float& x, const float& y, const D2D1_POINT_2F& center) {
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Scale(x, y, center));
}

void Graphics::restoreDefaultDrawingParameters() const
{
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Identity());
}

void Graphics::mirrorDrawing(const float& x, const float& y) const {
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F{ -1.0f, 0.0f, 0.0f, 1.0f, x, y });
    // mirrorMatrix * D2D1::Matrix3x2F::Translation(x,y)
}

void Graphics::translateDrawing(const float& x, const float& y) const {
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Translation(x,y));
}

void Graphics::transformTRSM(const float& Translatex, const float& Translatey, const float& RotationAngle, const D2D1_POINT_2F& ImageCenter, const float& Scalex, const float& Scaley, const bool& mirrored) const {
    d2d.getRenderTarget()->SetTransform(D2D1::Matrix3x2F::Translation(Translatex, Translatey) * D2D1::Matrix3x2F::Rotation(RotationAngle, ImageCenter) * D2D1::Matrix3x2F::Scale(Scalex, Scaley, ImageCenter) * D2D1::Matrix3x2F{ (static_cast<int>(!mirrored) * 2) + -1.0f, 0.0f, 0.0f, 1.0f, static_cast<int>(mirrored)* ( ImageCenter.x * 2), 0.0f });
}

HRESULT Graphics::loadD2DBitmap(const wchar_t* filename, const int& frameNum, ID2D1Bitmap*& pOutBitmap) {
    HRESULT hr = d2d.getRenderTarget()->CreateBitmapFromWicBitmap(Wic.getConvertedBitmap(filename, frameNum), &pOutBitmap);
    return hr;
}
