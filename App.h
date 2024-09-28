//
// Created by Тимур Ахметзянов on 28.09.2024.
//

#pragma once

#include "interface-includes.h"


class App : public QMainWindow {
Q_OBJECT

 public:
    App();

 private:
    int width_, height_;
};
