#include "messageBox.h"

#ifdef _WIN32
#include <windows.h>
#include <winuser.h>

void Kuribrawl::errorPopup(void* winHandle, const char* title, const char* message){
    MessageBoxA((HWND)winHandle, message, title, MB_OK | MB_ICONERROR);
}

#else

void Kuribrawl::errorPopup(void* winHandle, const char* title, const char* message)
{}

#endif

