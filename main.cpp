#include "main_window.h"
#include "goose_button.h"
#include <QGridLayout>
#include <QPushButton>
#include <QApplication>
#include <QWidget>

//Since it is an early version it's not possible to set a path to save/load files and change their names using GUI.
//To do so you need to change strings set in a code. Search for input.xml and output.xml

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(nullptr);
    w.defaultLookout();
    w.showNormal();
    return a.exec();
}
