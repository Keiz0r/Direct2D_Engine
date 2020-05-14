#pragma once
#include <dwrite.h>
#include <vector>

#pragma comment ( lib, "dwrite" )

class Dwrite {
public:
	Dwrite();
	~Dwrite();
	IDWriteTextFormat* getTextFormat(const int& format) const;
	IDWriteTextLayout* getTextLayout() const;
	HRESULT createTextLayout(const wchar_t* string, IDWriteTextFormat*& textFormat);
private:
	HRESULT initialize();
	HRESULT createTextFormat(const wchar_t* font, const float& fontsize); // TODO : Need enum for brushes and check if same brush exists
private:
	IDWriteFactory* Factory;
	std::vector<IDWriteTextFormat*> m_vTextFormatVector;
	IDWriteTextLayout* m_pTextLayout;
};