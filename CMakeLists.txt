cmake_minimum_required(VERSION 3.23)
project(PortManagerInterface)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(PortManagerInterface main.cpp
        interface-includes.h
        PortModelWin.h
        PortModelWin.cpp
        GetDataWin.cpp
        GetDataWin.h)

target_link_libraries(PortManagerInterface
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
