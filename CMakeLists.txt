cmake_minimum_required(VERSION 3.23)
project(PortManagerInterface)

set(CMAKE_CXX_STANDARD 20)

add_executable(PortManagerInterface main.cpp
        header.h
        PortManagerWin.h
        input-data/GetDataWin.h
        input-data/ShipInput.h
)

add_subdirectory(KatLib)
target_link_libraries(PortManagerInterface KatLib)