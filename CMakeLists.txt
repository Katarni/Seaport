cmake_minimum_required(VERSION 3.23)
project(Seaport)

set(CMAKE_CXX_STANDARD 20)

add_executable(PortManagerInterface main.cpp
        header.h
        PortManagerWin.h
        input-data/GetDataWin.h
        input-data/ShipInput.h
        manager/Ship.cpp
        manager/Ship.h
        manager/Manager.cpp
        manager/Manager.h
        manager/Event.cpp
        manager/Event.h
        manager/includes.h
        manager/ScheduleItem.cpp
        manager/ScheduleItem.h
        ships/DrawableShip.h
        ships/MovingEvent.h
        results/ResultWin.h
        schedule/DrawableScheduleItem.h
        schedule/ScheduleScroll.h
)

add_subdirectory(KatLib)
target_link_libraries(PortManagerInterface KatLib)
