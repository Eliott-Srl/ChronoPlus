#ifndef CHRONOPLUS_APPMANAGEMENT_H
#define CHRONOPLUS_APPMANAGEMENT_H

#include <windows.h>
#include <list>
#include "../Backend.h"
#include <map>
#include <ctime>
#include "StructAppInfo.h"
#include <psapi.h>
#include <cstring>

class AppManagement {
    public:
        static void getActualRunningApps();
        static void activateEventHandler();
        static void addAppToMap(char* title, char* appPath, char* filename);
};


#endif //CHRONOPLUS_APPMANAGEMENT_H
