#include "AppManagement.h"

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);

// Fonction de rappel pour EnumWindows
BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
    if (IsWindowVisible(hwnd)) {
        DWORD styles = GetWindowLong(hwnd, GWL_STYLE);
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW) {
            DWORD lpdwProcessId;
            GetWindowThreadProcessId(hwnd, &lpdwProcessId);
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lpdwProcessId);
            char fileName[MAX_PATH];
            DWORD sizeFileName = MAX_PATH;
            QueryFullProcessImageName(hProcess, 0, fileName, &sizeFileName);
            ((std::map<char[], time_t>)lParam)->insert({fileName, time(0)});
        }
    }
    return TRUE; // Continue l'énumération
}

void AppManagement::getActualRunnigApps() {
    EnumWindows((WNDENUMPROC) EnumWindowsCallback, &(Backend::getInstance().openedWindows));
}

// The callback function that will be called when a new window is created
void CALLBACK WinEventProc(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    DWORD styles = GetWindowLong(hwnd, GWL_STYLE);
    char title[MAX_PATH];
    GetWindowText(hwnd, title, MAX_PATH);
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lpdwProcessId);
    char fileName[MAX_PATH];
    DWORD sizeFileName = MAX_PATH;
    QueryFullProcessImageName(hProcess, 0, fileName, &sizeFileName);

    if (idObject == OBJID_WINDOW && event == EVENT_OBJECT_SHOW) {
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW && styles & WS_VISIBLE) {
            system("Color 02");
            std::cout << "[ + ] ";
            system("Color 0F");
            std::cout << title << " : " << fileName << std::endl;
        }
    } else if (idObject == OBJID_WINDOW && event == EVENT_OBJECT_HIDE) {
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW) {
            system("Color 04");
            std::cout << "[ - ] ";
            system("Color 0F");
            std::cout << title << " : " << fileName << std::endl;
        }
    }
}

void AppManagement::activateEventHandler() {
    HWINEVENTHOOK hWinEventHook = SetWinEventHook(
            EVENT_OBJECT_SHOW, // eventMin
            EVENT_OBJECT_HIDE, // eventMax
            nullptr, // hmodWinEventProc
            WinEventProc, // lpfnWinEventProc
            0, // idProcess
            0, // idThread
            WINEVENT_OUTOFCONTEXT // dwFlags
    );

    if (!hWinEventHook) {
        std::cerr << "SetWinEventHook failed" << std::endl;
        return;
    }

    // Message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    UnhookWinEvent(hWinEventHook);
}
