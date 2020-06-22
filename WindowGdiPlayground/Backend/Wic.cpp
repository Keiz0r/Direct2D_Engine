#include "Wic.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterface) {
    if (*ppInterface != NULL) {
        (*ppInterface)->Release();
        (*ppInterface) = NULL;
    }
}

Wic::Wic()
    :
    m_pImagingFactory(NULL),
    m_pDecoder(NULL),
    m_pFrame(NULL),
    m_pFormatConverter(NULL)
{
    initialize();
}

Wic::~Wic(){
    SafeRelease(&m_pImagingFactory);
    SafeRelease(&m_pDecoder);
    SafeRelease(&m_pFrame);
    SafeRelease(&m_pFormatConverter);
}

IWICFormatConverter* Wic::getConvertedBitmap(const wchar_t* filename, const int& frameNum) {
    HRESULT hr = S_OK;
    if (m_pFormatConverter != NULL) {
        hr = m_pFormatConverter->Release();
        hr = m_pImagingFactory->CreateFormatConverter(&m_pFormatConverter);
    }
    getFrame(filename, frameNum);
    if (FAILED(hr = m_pFormatConverter->Initialize(m_pFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom))) {
        //outputdebug
    }
    return m_pFormatConverter;
}

HRESULT Wic::initialize() {
    HRESULT hr = S_OK;
    if (FAILED(hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&m_pImagingFactory))) {
        return hr;
    }
    hr = m_pImagingFactory->CreateFormatConverter(&m_pFormatConverter);
    return hr;
}

HRESULT Wic::getFrame(const wchar_t* filename, const int& frameNum) {
    HRESULT hr = S_OK;
    if (FAILED(hr = m_pImagingFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_pDecoder))) {
        return hr;
    }
    hr = m_pDecoder->GetFrame(frameNum, &m_pFrame);
    return hr;
}
