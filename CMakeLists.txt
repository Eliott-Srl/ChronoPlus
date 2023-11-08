cmake_minimum_required(VERSION 3.26)
project(ChronoPlus)

set(CMAKE_CXX_STANDARD 17)

add_executable(ChronoPlus main.cpp
        Backend/Backend.cpp
        Backend/Backend.h
        Backend/AppManagement/AppManagement.cpp
        Backend/AppManagement/AppManagement.h
)
