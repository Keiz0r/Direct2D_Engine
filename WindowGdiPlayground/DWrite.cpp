#include "DWrite.h"

template<class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease) {
    if (*ppInterfaceToRelease != NULL) {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}

Dwrite::Dwrite()
	:
	Factory(NULL)
{
    initialize();
    createTextFormat(L"Gabriola", 20.0f);
    createTextFormat(L"Times New Roman", 30.0f);
    createTextFormat(L"Times New Roman", 20.0f);    // for console
}

Dwrite::~Dwrite() {
	SafeRelease(&Factory);
    for (std::vector<IDWriteTextFormat*>::reverse_iterator i = m_vTextFormatVector.rbegin(); i != m_vTextFormatVector.rend(); i++) {
        SafeRelease(&(*i));
    }
    SafeRelease(&m_pTextLayout);
}

IDWriteTextFormat* Dwrite::getTextFormat(const int& format) const
{
    return m_vTextFormatVector[format];
}

IDWriteTextLayout* Dwrite::getTextLayout() const {
    return m_pTextLayout;
}

HRESULT Dwrite::initialize()
{
    HRESULT hr = S_OK;
    hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&Factory));
    return hr;
}

HRESULT Dwrite::createTextFormat(const wchar_t* font, const float& fontsize)
{
    HRESULT hr = S_OK;
    IDWriteTextFormat* Format;
    if (FAILED(hr = Factory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontsize, L"ru-ru", &Format))) {
        return hr;
    }
    m_vTextFormatVector.push_back(Format);
    return hr;
}

HRESULT Dwrite::createTextLayout(const wchar_t* string, IDWriteTextFormat*& textFormat)
{
    HRESULT hr = S_OK;
    hr = Factory->CreateTextLayout(string, wcslen(string), textFormat, 300, 100, &m_pTextLayout);
    if (FAILED(hr)) {

    }
    return hr;
}

