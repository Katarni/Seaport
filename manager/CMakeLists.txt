cmake_minimum_required(VERSION 3.23)
project(PortManager)

set(CMAKE_CXX_STANDARD 23)

add_executable(PortManager Ship.cpp Ship.h Manager.cpp Manager.h Event.cpp Event.h includes.h)