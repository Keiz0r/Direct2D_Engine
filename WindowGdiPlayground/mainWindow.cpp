#include "mainWindow.h"

mainWindow::mainWindow(HINSTANCE hInst, wchar_t* pArgs)
	:
    hInstance(hInst),
    Args(pArgs)
{
    initialize();
}

mainWindow::~mainWindow(){
    UnregisterClass(mainWindowClassName, hInstance);
}

void mainWindow::initialize(){
    // Register the window class.
    WNDCLASSEX wc = { sizeof(WNDCLASSEX) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = _HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof(LONG_PTR);
    wc.hInstance = hInstance;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    wc.lpszClassName = mainWindowClassName;
    
    RegisterClassEx(&wc);
    
    // Create the window.
    hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        mainWindowClassName,            // Window class
        mainWindowName,                 // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
    
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT,
        1366, 768,
    
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        this        // Additional application data
    );
        ShowWindow(hwnd, SW_SHOWNORMAL);
        UpdateWindow(hwnd);
    
    
}

HWND mainWindow::getHandle(){
    return hwnd;
}

bool mainWindow::runMessageLoop(){
	MSG msg{}; //Current message information struct
	BOOL bRet;  // 0 would be exit message; -1 error message
	while ((bRet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) > 0){
		if (bRet == -1){
            // TODO: handle error message
            return false;
            
		}
		else{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
            if (msg.message == WM_QUIT){
                return false;
            }
		}

	}
    return true;

    
}

LRESULT mainWindow::_HandleMsgSetup(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_NCCREATE) {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        //  extract a pointer to itself (equivalent to "this", but inside a static function)
        mainWindow* const pWnd = reinterpret_cast<mainWindow*>(pCreate->lpCreateParams);
        if (pWnd == NULL) {
            // TODO : error checking
        }
       //  set userdata of window to contain a pointer to itself
       SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
       //   -another option to keep stuff
//           SetProp(hwnd, L"LP_THIS_WINDOW", pWnd);

       //  change default windowproc function to _HandleMsgThunk
       SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&mainWindow::_HandleMsgThunk));
       //   go to special (default) WndProc
        return pWnd->handleMSG(hwnd, message, wParam, lParam);
    }
    // for mesages prior to NCCREATE
    return DefWindowProc(hwnd, message, wParam, lParam);
}

LRESULT mainWindow::_HandleMsgThunk(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //  retrieval of pointer from the window's userdata
    mainWindow* const pWnd = reinterpret_cast<mainWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
//    mainWindow* const pWnd = reinterpret_cast<mainWindow*>(GetProp(hwnd, L"LP_THIS_WINDOW"));
    //  go to special (default) WndProc
    return pWnd->handleMSG(hwnd, message, wParam, lParam);
}
/*
void mainWindow::OnResize(UINT width, UINT height)
{
    if (m_pRenderTarget)
    {
        // Note: This method can fail, but it's okay to ignore the
        // error here, because the error will be returned again
        // the next time EndDraw is called.
        m_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}
*/

 LRESULT mainWindow::handleMSG(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  {
     switch (message) {
     case WM_DESTROY:
 //        RemoveProp(hwnd, L"LP_THIS_WINDOW");
         PostQuitMessage(0);
         break;
     case WM_KEYDOWN:
         if (!(lParam & 0x40000000) || kbd.autorepeatEnabled()) { //  no autorepeat
             kbd.onKeyPressed(static_cast<unsigned char>(wParam));
         }
         break;
     case WM_KEYUP:
         kbd.onKeyReleased(static_cast<unsigned char>(wParam));
         break;
     case WM_CHAR:
         kbd.onChar(static_cast<unsigned char>(wParam));
         break;
     }


     return DefWindowProc(hwnd, message, wParam, lParam);
 }