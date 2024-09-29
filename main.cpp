//
// Created by Тимур Ахметзянов on 27.09.2024.
//

#include "PortModelWin.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);
    const auto app = new PortModelWin;
    app->open();
    return QApplication::exec();
}
