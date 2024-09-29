//
// Created by Тимур Ахметзянов on 28.09.2024.
//

#include "PortModelWin.h"

PortModelWin::PortModelWin() {
    width_ = 1000;
    height_ = 700;

    setWindowTitle("Port Manager");
    setFixedSize(width_, height_);

    get_data_window_ = new GetDataWin;

    connect(get_data_window_, &GetDataWin::dataLoaded, this, &PortModelWin::parseData);
}

void PortModelWin::open() {
    get_data_window_->show();
}

void PortModelWin::parseData() {
    std::cerr << -1;
}
