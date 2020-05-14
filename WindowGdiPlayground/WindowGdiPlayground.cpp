#include "mainWindow.h"
#include "Game.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR pCmdLine, _In_ int nCmdShow) {
    //hInstance is something called a "handle to an instance" or "handle to a module." The operating system uses this value to identify the EXE when it is loaded in memory.The instance handle is needed for certain Windows functions—for example, to load icons or bitmaps.
    //hPrevInstance has no meaning. is now always zero.
    //pCmdLine contains the command - line arguments as a Unicode string.
    //nCmdShow is a flag that says whether the main application window will be minimized, maximized, or shown normally.

    // Use HeapSetInformation to specify that the process should
    // terminate if the heap manager detects an error in any heap used
    // by the process.
    // The return value is ignored, because we want to continue running in the
    // unlikely event that HeapSetInformation fails.
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    if (SUCCEEDED(CoInitialize(NULL)))
    {
        {
            mainWindow mainWindow(hInstance, pCmdLine);
            Game gameInstance(mainWindow.getHandle(), mainWindow.kbd);

            while (mainWindow.runMessageLoop()) {
                gameInstance.gameLoop();
            }
        }
        CoUninitialize();
    }

    return 0;
}