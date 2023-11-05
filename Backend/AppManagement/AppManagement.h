#ifndef CHRONOPLUS_APPMANAGEMENT_H
#define CHRONOPLUS_APPMANAGEMENT_H

#include <windows.h>
#include <list>
#include "../Backend.h"
#include <map>
#include <ctime>

class AppManagement {
    public:
        static void getActualRunnigApps();
        static void activateEventHandler();
};


#endif //CHRONOPLUS_APPMANAGEMENT_H
