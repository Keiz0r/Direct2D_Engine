#include "Direct2D.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterface) {
    if (*ppInterface != NULL) {
        (*ppInterface)->Release();
        (*ppInterface) = NULL;
    }
}

Direct2D::Direct2D(const HWND& hwnd)
	:
    m_hwnd(hwnd),
    m_pFactory(NULL),
    m_pRenderTarget(NULL)
{
    initialize();
    createBrush(D2D1::ColorF::LightSlateGray);
    createBrush(D2D1::ColorF::CornflowerBlue);
    createBrush(D2D1::ColorF::OrangeRed);
    createBrush(D2D1::ColorF::Black);
    createBrush(D2D1::ColorF::DarkGray);
}

Direct2D::~Direct2D() {
    SafeRelease(&m_pRenderTarget);
    SafeRelease(&m_pFactory);
    for (std::vector<ID2D1SolidColorBrush*>::reverse_iterator i = m_vpSolidColorBrushVector.rbegin(); i != m_vpSolidColorBrushVector.rend(); i++) {
        SafeRelease(&(*i));
    }
}

HRESULT Direct2D::initialize() {
    HRESULT hr = S_OK;
    if (FAILED( hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory))) {
        return hr;
    }
    RECT rc;
    GetClientRect(m_hwnd, &rc);
    D2D1_SIZE_U activeSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
    hr = m_pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(m_hwnd, activeSize), &m_pRenderTarget);
    return hr;
}

HRESULT Direct2D::createBrush(const D2D1::ColorF& color) {
    HRESULT hr = S_OK;
    ID2D1SolidColorBrush* Brush;
    if (FAILED(hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color), &Brush))){
        return hr;
    }
    m_vpSolidColorBrushVector.push_back(Brush);
    return hr;
}

ID2D1HwndRenderTarget* Direct2D::getRenderTarget() const {
   return m_pRenderTarget;
}

ID2D1SolidColorBrush* Direct2D::getSolidColorBrush(const int& color) const {
    return m_vpSolidColorBrushVector[color];
}
