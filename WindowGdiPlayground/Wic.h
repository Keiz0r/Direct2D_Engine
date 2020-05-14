#pragma once
#include <wincodec.h>
#include <string>

#pragma comment ( lib, "windowscodecs")

class Wic {
public:
    Wic();
    ~Wic();
    IWICFormatConverter* getConvertedBitmap(const wchar_t* filename, const int& frameNum);  // TODO : error checking
private:
    HRESULT initialize();
    HRESULT getFrame(const wchar_t* filename, const int& frameNum);
private:
    IWICImagingFactory* m_pImagingFactory;
    IWICBitmapDecoder* m_pDecoder;
    IWICBitmapFrameDecode* m_pFrame;
    IWICFormatConverter* m_pFormatConverter;
};