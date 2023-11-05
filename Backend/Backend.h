#ifndef CHRONOPLUS_BACKEND_H
#define CHRONOPLUS_BACKEND_H
#pragma once

#include <iostream>
#include <windows.h>
#include <list>
#include "AppManagement/AppManagement.h"
#include <map>
#include <ctime>

class Backend {
    public:
        static Backend& getInstance();
        std::map<char[], time_t> getOpenedWindows();

    private:
        Backend& operator= (const Backend&);
        Backend(const Backend&) {}
        static Backend m_instance;
        Backend();
        ~Backend();

        std::map<char[], time_t> openedWindows;
    friend class AppManagement;
};

#endif //CHRONOPLUS_BACKEND_H
