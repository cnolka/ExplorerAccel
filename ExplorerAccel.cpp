#define UNICODE
#define _UNICODE

#include <windows.h>
#include <unordered_set>
#include <string>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")

std::unordered_set<HWND> handledWindows;
UINT WM_SHELLHOOK;
const UINT_PTR CLEANUP_TIMER_ID = 1;
const wchar_t* EXPLORER_CLASS = L"CabinetWClass";

#define TRAY_ICON_ID 1001
#define WM_TRAYICON (WM_USER + 1)
#define IDM_EXIT 1002
#define IDI_MAIN_ICON 101

void AddTrayIcon(HWND hWnd);
void RemoveTrayIcon(HWND hWnd);
void ShowTrayContextMenu(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN_ICON));
            if (hIcon) {
                SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
                SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            }
            
            if (!RegisterShellHookWindow(hWnd)) {
                return -1;
            }
            WM_SHELLHOOK = RegisterWindowMessage(L"SHELLHOOK");
            SetTimer(hWnd, CLEANUP_TIMER_ID, 3600000, nullptr);
            
            AddTrayIcon(hWnd);
            return 0;
        }

        case WM_DESTROY: {
            KillTimer(hWnd, CLEANUP_TIMER_ID);
            DeregisterShellHookWindow(hWnd);
            RemoveTrayIcon(hWnd);
            PostQuitMessage(0);
            return 0;
        }

        case WM_TIMER: {
            if (wParam == CLEANUP_TIMER_ID) {
                for (auto it = handledWindows.begin(); it != handledWindows.end(); ) {
                    if (!IsWindow(*it)) it = handledWindows.erase(it);
                    else ++it;
                }
            }
            return 0;
        }
        
        case WM_TRAYICON: {
            if (lParam == WM_RBUTTONUP) {
                ShowTrayContextMenu(hWnd);
            }
            return 0;
        }
        
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDM_EXIT) {
                DestroyWindow(hWnd);
            }
            return 0;
        }

        default: {
            if (message == WM_SHELLHOOK && wParam == HSHELL_WINDOWCREATED) {
                HWND targetHwnd = reinterpret_cast<HWND>(lParam);
                
                if (!IsWindow(targetHwnd)) return 0;
                
                wchar_t className[256] = {0};
                GetClassName(targetHwnd, className, 256);
                
                if (wcscmp(className, EXPLORER_CLASS) != 0) return 0;
                
                wchar_t title[256] = {0};
                GetWindowText(targetHwnd, title, 256);
                std::wstring titleStr(title);
                
                if (titleStr.find(L"控制面板") != std::wstring::npos || 
                    titleStr.find(L"Control Panel") != std::wstring::npos) {
                    return 0;
                }
                
                if (handledWindows.find(targetHwnd) == handledWindows.end()) {
                    handledWindows.insert(targetHwnd);
                    
                    PostMessage(targetHwnd, WM_KEYDOWN, VK_F11, 0);
                    PostMessage(targetHwnd, WM_KEYUP, VK_F11, 0);
                    PostMessage(targetHwnd, WM_KEYDOWN, VK_F11, 0);
                    PostMessage(targetHwnd, WM_KEYUP, VK_F11, 0);
                }
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
}

void AddTrayIcon(HWND hWnd) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    
    nid.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAIN_ICON));
    if (!nid.hIcon) {
        nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    
    wcscpy_s(nid.szTip, L"ExplorerAccel");
    
    Shell_NotifyIcon(NIM_ADD, &nid);
}

void RemoveTrayIcon(HWND hWnd) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = TRAY_ICON_ID;
    
    Shell_NotifyIcon(NIM_DELETE, &nid);
}

void ShowTrayContextMenu(HWND hWnd) {
    HMENU hMenu = CreatePopupMenu();
    if (hMenu) {
        AppendMenu(hMenu, MF_STRING, IDM_EXIT, L"Exit");
        
        POINT pt;
        GetCursorPos(&pt);
        
        SetForegroundWindow(hWnd);
        TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
        PostMessage(hWnd, WM_NULL, 0, 0);
        
        DestroyMenu(hMenu);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ShellHookWindow";
    
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    if (!wc.hIcon) {
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    }
    
    if (!RegisterClass(&wc)) return 1;

    HWND hWnd = CreateWindow(wc.lpszClassName, nullptr, 0, 0, 0, 0, 0, HWND_MESSAGE, nullptr, hInstance, nullptr);
    if (!hWnd) return 1;

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
