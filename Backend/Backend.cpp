#include "Backend.h"

Backend Backend::m_instance = Backend();

Backend::Backend() {
    AppManagement::getActualRunnigApps();
    AppManagement::activateEventHandler();
}

std::map<char*, StructAppInfo> Backend::getOpenedWindows() {
    return Backend::openedWindows;
}

Backend& Backend::getInstance() {
    return m_instance;
}

Backend::~Backend() = default;
