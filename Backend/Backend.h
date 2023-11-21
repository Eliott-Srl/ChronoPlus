#ifndef CHRONOPLUS_BACKEND_H
#define CHRONOPLUS_BACKEND_H
#pragma once

#include <iostream>
#include <windows.h>
#include <list>
#include "AppManagement/AppManagement.h"
#include <map>
#include <ctime>
#include "AppManagement/StructAppInfo.h"

class Backend {
    public:
        static Backend& getInstance();
        std::map<char*, StructAppInfo*> getOpenedWindows();

    private:
        Backend& operator= (const Backend&);
        Backend(const Backend&) {}
        static Backend m_instance;
        Backend();
        ~Backend();

        std::map<char*, StructAppInfo*> openedWindows;
    friend class AppManagement;
};

#endif //CHRONOPLUS_BACKEND_H
