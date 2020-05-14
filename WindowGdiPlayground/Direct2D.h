#pragma once
#include <vector>
#include <d2d1.h>

#pragma comment ( lib, "d2d1" )

class Direct2D {
public:
	Direct2D(const HWND& hwnd);
	~Direct2D();
    ID2D1HwndRenderTarget* getRenderTarget() const;
    ID2D1SolidColorBrush* getSolidColorBrush(const int& color) const;
private:
    HRESULT initialize();   // TODO : check, rendertargethwnd properties. atm it is bound to vsync (60fps)
    HRESULT createBrush(const D2D1::ColorF& color); // TODO : Need enum for brushes and check if same brush exists
private:
    const HWND& m_hwnd;
    ID2D1Factory* m_pFactory;
    ID2D1HwndRenderTarget* m_pRenderTarget;
    std::vector<ID2D1SolidColorBrush*> m_vSolidColorBrushVector;
};