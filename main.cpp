//
// Created by Тимур Ахметзянов on 27.09.2024.
//

#include "App.h"

int main(int argc, char** argv) {
    QApplication a(argc, argv);
    const auto app = new App;
    app->show();
    return QApplication::exec();
}
