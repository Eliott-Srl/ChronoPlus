#include "AppManagement.h"

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);

void AppManagement::addAppToMap(char* title, char* appPath, char* filename) {
    if (std::strcmp(appPath, "") != 0 || std::strcmp(filename, "") != 0) {
        return;
    }

    std::map<char*, StructAppInfo*> openedWindows = Backend::getInstance().openedWindows;

    std::cout << "[ + ] " << title << " : " << filename << std::endl;

    if (openedWindows.find(appPath) != openedWindows.end()) {
        auto infos = openedWindows.at(appPath);
        infos->appCopies++;
    } else {
        auto infos = (StructAppInfo*) malloc(sizeof(StructAppInfo));
        if (infos) {
            infos->appCopies = 1;
            infos->startTime = time(nullptr);
            openedWindows.insert({filename, infos});
        } else {
            std::cout << "It's not squared" << std::endl;
        }
    }
}

void afficheStruct(std::map<char*, StructAppInfo*> openedWindowsMap) {
    for (auto i = openedWindowsMap.end(); i != openedWindowsMap.end(); i++) {
        std::cout << (void*) (*i).first << ": " << (*i).second->startTime << std::endl;
    }
}

// Fonction de rappel pour EnumWindows
BOOL CALLBACK EnumWindowsCallback(HWND hwnd) {
    if (IsWindowVisible(hwnd)) {
        DWORD styles = GetWindowLong(hwnd, GWL_STYLE);
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW) {
            DWORD lpdwProcessId;
            GetWindowThreadProcessId(hwnd, &lpdwProcessId);
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lpdwProcessId);
            char fileName[MAX_PATH];
            DWORD sizeFileName = MAX_PATH;
            GetModuleFileNameEx(hProcess, nullptr, fileName, (DWORD) sizeFileName);

        }
    }
    return TRUE; // Continue l'énumération
}

void AppManagement::getActualRunningApps() {
    EnumWindows((WNDENUMPROC) EnumWindowsCallback, NULL);
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
    GetModuleFileNameEx(hProcess, nullptr, fileName, (DWORD) sizeFileName);

    if (idObject == OBJID_WINDOW && event == EVENT_OBJECT_SHOW) {
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW && styles & WS_VISIBLE) {
            std::cout << "[ + ] ";
            std::cout << title << " : " << fileName << std::endl;
            afficheStruct(Backend::getInstance().getOpenedWindows());
        }
    } else if (idObject == OBJID_WINDOW && event == EVENT_OBJECT_HIDE) {
        if (!(styles & WS_EX_TOOLWINDOW) && styles & WS_EX_APPWINDOW) {
            std::cout << "[ - ] ";
            std::cout << title << " : " << fileName << std::endl;
            afficheStruct(Backend::getInstance().getOpenedWindows());
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
