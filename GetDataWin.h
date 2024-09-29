//
// Created by Тимур Ахметзянов on 29.09.2024.
//

#pragma once

#include "interface-includes.h"


class GetDataWin : public QMainWindow {
    Q_OBJECT
 public:
    GetDataWin();

 signals:
    void dataLoaded();

 private:
    int width_, height_;
};

