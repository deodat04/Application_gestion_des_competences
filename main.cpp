#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Database mydatabase;
    mydatabase.setFixedSize(800,500);
    mydatabase.setStyleSheet("background-color: sky blue");
    mydatabase.show();
    return a.exec();
}
