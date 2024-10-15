//
// Created by Тимур Ахметзянов on 27.09.2024.
//

#include "PortManagerWin.h"
#include "results/ResultWin.h"

int main() {
    auto get_data_win = new GetDataWin;
    if (!get_data_win->open()) return 0;

    auto port_manager_win = new PortManagerWin(get_data_win);
    delete get_data_win;

    port_manager_win->modeling();

    auto results_win = new ResultWin(port_manager_win->getManager());
    results_win->open();
}
