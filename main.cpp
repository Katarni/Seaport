//
// Created by Тимур Ахметзянов on 27.09.2024.
//

#include "PortManagerWin.h"

int main() {
    auto get_data_win = new GetDataWin;
    get_data_win->open();

    auto port_manager_win = new PortManagerWin(get_data_win);
    delete get_data_win;

    port_manager_win->modeling();
}
