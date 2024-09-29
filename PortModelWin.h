//
// Created by Тимур Ахметзянов on 28.09.2024.
//

#pragma once

#include "interface-includes.h"
#include "GetDataWin.h"


class PortModelWin : public QMainWindow {
Q_OBJECT

 public:
    PortModelWin();

    void open();

 public slots:
    void parseData();

 private:
    int width_, height_;

    GetDataWin* get_data_window_;
};
